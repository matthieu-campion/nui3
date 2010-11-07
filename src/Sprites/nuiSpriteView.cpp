/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSpriteView.h"
#include "nuiTexture.h"

// class nuiSpriteFrame
nuiSpriteFrame::nuiSpriteFrame()
: mpTexture(NULL),
  mX(0),
  mY(0)
{
}

nuiSpriteFrame::~nuiSpriteFrame()
{
  if (mpTexture)
    mpTexture->Release();
}

bool nuiSpriteFrame::SetTexture(nuiTexture* pTexture, const nuiRect& rRect)
{
  if (mpTexture)
    mpTexture->Release();
  mpTexture = pTexture;
  mpTexture->Acquire();
  mRect = rRect;
  return true;
}

bool nuiSpriteFrame::SetTexture(const nglPath& rPath, const nuiRect& rRect)
{
  if (mpTexture)
    mpTexture->Release();
  mpTexture = nuiTexture::GetTexture(rPath);
  mRect = rRect;
  return mpTexture != NULL;
}

bool nuiSpriteFrame::SetTexture(const nglPath& rPath)
{
  if (!SetTexture(rPath, nuiRect()))
    return false;
  
  mRect = nuiRect((float)mpTexture->GetWidth(), (float)mpTexture->GetHeight());
  mX = mpTexture->GetWidth() / 2;
  mY = mpTexture->GetHeight() / 2;

}

void nuiSpriteFrame::SetRect(const nuiRect& rRect)
{
  mRect = rRect;
}

void nuiSpriteFrame::SetHandle(float x, float y)
{
  mX = x;
  mY = y;
}

nuiTexture* nuiSpriteFrame::GetTexture() const
{
  return mpTexture;
}

const nuiRect& nuiSpriteFrame::GetRect() const
{
  return mRect;
}

float nuiSpriteFrame::GetHandleX() const
{
  return mX;
}

float nuiSpriteFrame::GetHandleY() const
{
  return mY;
}

//////////////////////////////////////////////////////
// class nuiSpriteAnimation
nuiSpriteAnimation::nuiSpriteAnimation()
: mFPS(10)
{

}

nuiSpriteAnimation::nuiSpriteAnimation(const nglPath& rPath)
: mFPS(10)
{
  std::list<nglPath> children;
  rPath.GetChildren(&children);
  children.sort(nglCompareNaturalPath);
  nuiSpriteAnimation* pAnim1 = new nuiSpriteAnimation();
  std::list<nglPath>::const_iterator it = children.begin();
  std::list<nglPath>::const_iterator end = children.end();
  for (; it != end; it++)
  {
    nuiSpriteFrame* pFrame = new nuiSpriteFrame();
    pFrame->SetTexture(*it);
    AddFrame(pFrame);
  }
  
  SetName(rPath.GetNodeName());
}

nuiSpriteAnimation::~nuiSpriteAnimation()
{  
  for (int32 i = 0; i < mpFrames.size(); i++)
    delete mpFrames[i];
}

int32 nuiSpriteAnimation::GetFrameCount() const
{
  return mpFrames.size();
}

void nuiSpriteAnimation::AddFrame(nuiSpriteFrame* pFrame)
{
  mpFrames.push_back(pFrame);
}

const nuiSpriteFrame* nuiSpriteAnimation::GetFrame(int32 index) const
{
  return mpFrames[index];
}

void nuiSpriteAnimation::SetDuration(float seconds)
{
  mFPS = seconds / (float)GetFrameCount();
}

void nuiSpriteAnimation::SetFPS(float FPS)
{
  mFPS = FPS;
}

float nuiSpriteAnimation::GetDuration() const
{
  return mFPS * GetFrameCount();
}

float nuiSpriteAnimation::GetFPS() const
{
  return mFPS;
}
               
void nuiSpriteAnimation::SetName(const nglString& rName)
{
  mName = rName;
}

const nglString& nuiSpriteAnimation::GetName() const
{
  return mName;
}
               
///////////////////////////////////////////////////////
// class nuiSpriteDef

std::map<nglString, nuiSpriteDef*> nuiSpriteDef::mSpriteMap;

nuiSpriteDef::nuiSpriteDef(const nglString& rSpriteDefName)
{
  Init();
  SetObjectName(rSpriteDefName);
  std::map<nglString, nuiSpriteDef*>::const_iterator it = mSpriteMap.find(rSpriteDefName);
  if (it != mSpriteMap.end())
    it->second->Release();
  mSpriteMap[rSpriteDefName] = this;
}

nuiSpriteDef::nuiSpriteDef(const nglPath& rSpriteDefPath)
{
  Init();
  nglString name(rSpriteDefPath.GetNodeName());
  SetObjectName(name);
  std::map<nglString, nuiSpriteDef*>::const_iterator it = mSpriteMap.find(name);
  if (it != mSpriteMap.end())
    it->second->Release();
  mSpriteMap[name] = this;

  {
    std::list<nglPath> children;
    rSpriteDefPath.GetChildren(&children);
    std::list<nglPath>::const_iterator it = children.begin();
    std::list<nglPath>::const_iterator end = children.end();
    for (; it != end; it++)
    {
      nuiSpriteAnimation* pAnim = new nuiSpriteAnimation(*it);
      if (pAnim->GetFrameCount())
        AddAnimation(pAnim);
      else
        delete pAnim;
    }
  }
}

nuiSpriteDef::~nuiSpriteDef()
{
  for (int32 i = 0; i < mpAnimations.size(); i++)
    delete mpAnimations[i];
}

void nuiSpriteDef::Init()
{
  if (SetObjectClass(_T("nuiSpriteDef")))
  {
    
  }

}
      
void nuiSpriteDef::AddAnimation(nuiSpriteAnimation* pAnim)
{
  mpAnimations.push_back(pAnim);
}

int32 nuiSpriteDef::GetAnimationCount() const
{
  return mpAnimations.size();
}

const nuiSpriteAnimation* nuiSpriteDef::GetAnimation(int32 index) const
{
  return mpAnimations[index];
}

int32 nuiSpriteDef::GetAnimation(const nglString& rName) const
{
  for (int32 i = 0; i < GetAnimationCount(); i++)
  {
    const nuiSpriteAnimation* pAnim = GetAnimation(i);
    if (pAnim->GetName() == rName)
      return i;
  }
  return 0;
}


nuiSpriteDef* nuiSpriteDef::GetSprite(const nglString& rName)
{
  std::map<nglString, nuiSpriteDef*>::const_iterator it = mSpriteMap.find(rName);
  if (it == mSpriteMap.end())
    return NULL;
  
  it->second->Acquire();
  return it->second;
}


/////////////////////////////////////////////////////////
// class nuiSprite
nuiMatrix nuiSprite::mIdentityMatrix;

nuiSprite::nuiSprite(const nglString& rSpriteDefName)
: mpSpriteDef(nuiSpriteDef::GetSprite(rSpriteDefName))
{
  Init();
}

nuiSprite::nuiSprite(nuiSpriteDef* pSpriteDef)
: mpSpriteDef(pSpriteDef)
{
  mpSpriteDef->Acquire();
  Init();
}

nuiSprite::~nuiSprite()
{
  LoadIdentityMatrix();
  if (mpSpriteDef)
    mpSpriteDef->Release();
  for (int32 i = 0; i < mpChildren.size(); i++)
    mpChildren[i]->Release();
}

void nuiSprite::Init()
{
  if (SetObjectClass(_T("nuiSprite")))
  {
    InitAttributes();
  }

  mpParent = NULL;
  mpMatrixNodes = NULL;
  mCurrentAnimation = 0;
  mCurrentFrame = 0;
  mSpeed = 1.0f;
  mScale = 1.0f;
  mScaleX = 1.0f;
  mScaleY = 1.0f;
  
  
  // Init Matrixes:
  mpScale = new nuiMatrixNode_Scale();
  mpPosition = new nuiMatrixNode_Translation();
  mpPivot = new nuiMatrixNode_Pivot();
  AddMatrixNode(mpPosition);
  AddMatrixNode(mpPivot);
  AddMatrixNode(mpScale);
}

void nuiSprite::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Animation")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetCurrentAnimationName),
                nuiMakeDelegate(this, &nuiSprite::_SetAnimation)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Speed")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetSpeed),
                nuiMakeDelegate(this, &nuiSprite::SetSpeed)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("X")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetX),
                nuiMakeDelegate(this, &nuiSprite::SetX)));
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Y")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetY),
                nuiMakeDelegate(this, &nuiSprite::SetY)));

  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Angle")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetAngle),
                nuiMakeDelegate(this, &nuiSprite::SetAngle)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Scale")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetScale),
                nuiMakeDelegate(this, &nuiSprite::SetScale)));
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("ScaleX")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetScaleX),
                nuiMakeDelegate(this, &nuiSprite::SetScaleX)));
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("ScaleY")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiSprite::GetScaleY),
                nuiMakeDelegate(this, &nuiSprite::SetScaleY)));
}


const nuiSpriteDef* nuiSprite::GetDefinition() const
{
  return mpSpriteDef;
}

void nuiSprite::AddMatrixNode(nuiMatrixNode* pNode)
{
  if (!mpMatrixNodes)
    mpMatrixNodes = new std::vector<nuiMatrixNode*>;
  
  pNode->Acquire();
  mpMatrixNodes->push_back(pNode);
}

void nuiSprite::DelMatrixNode(uint32 index)
{
  if (!mpMatrixNodes)
    return;
  
  CheckValid();
  mpMatrixNodes->at(index)->Release();
  mpMatrixNodes->erase(mpMatrixNodes->begin() + index);
  
  DebugRefreshInfo();
}


int32 nuiSprite::GetMatrixNodeCount() const
{
  CheckValid();
  if (!mpMatrixNodes)
    return 0;
  return mpMatrixNodes->size();
}


nuiMatrixNode* nuiSprite::GetMatrixNode(uint32 index) const
{
  CheckValid();
  if (mpMatrixNodes)
    return mpMatrixNodes->at(index);
}


void nuiSprite::LoadIdentityMatrix()
{
  CheckValid();
  
  if (mpMatrixNodes)
  {
    for (uint32 i = 0; i < mpMatrixNodes->size(); i++)
      mpMatrixNodes->at(i)->Release();
    delete mpMatrixNodes;
    mpMatrixNodes = NULL;
  }
  
  DebugRefreshInfo();
}

bool nuiSprite::IsMatrixIdentity() const
{
  nuiMatrix m;
  GetMatrix(m);
  return !mpMatrixNodes || m.IsIdentity();
}

void nuiSprite::GetMatrix(nuiMatrix& rMatrix) const
{
  CheckValid();
  rMatrix.SetIdentity();
  for (uint32 i = 0; i < mpMatrixNodes->size(); i++)
    mpMatrixNodes->at(i)->Apply(rMatrix);
}

nuiMatrix nuiSprite::GetMatrix() const
{
  CheckValid();
  nuiMatrix m;
  GetMatrix(m);
  return m;
}

void nuiSprite::AddChild(nuiSprite* pChild)
{
  pChild->Acquire();
  nuiSprite* pParent = pChild->GetParent();
  pParent->DelChild(pChild);
  mpChildren.push_back(pChild);
}

void nuiSprite::DelChild(nuiSprite* pChild)
{
  for (int32 i = 0; i < mpChildren.size(); i++)
  {
    if (mpChildren[i] == pChild)
    {
      mpChildren.erase(mpChildren.begin() + i);
      pChild->Release();
      return;
    }
  }
}

void nuiSprite::SetParent(nuiSprite* pParent)
{
  mpParent = pParent;
}

nuiSprite* nuiSprite::GetParent() const
{
  return mpParent;
}


void nuiSprite::Draw(nuiDrawContext* pContext)
{
  nuiMatrix m;
  GetMatrix(m);
  pContext->PushMatrix();
  pContext->MultMatrix(m);
  
  const nuiSpriteAnimation* pAnim = mpSpriteDef->GetAnimation(mCurrentAnimation);
  const nuiSpriteFrame* pFrame = pAnim->GetFrame(ToBelow(mCurrentFrame));
  nuiRect src(pFrame->GetRect());
  nuiRect dst(src);
  dst.Move(-pFrame->GetHandleX(), -pFrame->GetHandleY());

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetFillColor(nuiColor(255, 255, 255));
  pContext->SetTexture(pFrame->GetTexture());
  pContext->DrawImage(dst, src);
  
  pContext->PopMatrix();

  for (int32 i = 0; i < mpChildren.size(); i++)
    mpChildren[i]->Draw(pContext);
}

void nuiSprite::SetAnimation(const nglString& rAnimationName)
{
  SetAnimation(mpSpriteDef->GetAnimation(rAnimationName));
}

void nuiSprite::SetAnimation(int32 index)
{
  NGL_ASSERT(index < mpSpriteDef->GetAnimationCount());
  mCurrentAnimation = index;
}

void nuiSprite::_SetAnimation(const nglString& rAnimationName)
{
  SetAnimation(rAnimationName);
}



const nglString& nuiSprite::GetCurrentAnimationName() const
{
  const nuiSpriteAnimation* pAnim = mpSpriteDef->GetAnimation(mCurrentAnimation);
  if (!pAnim)
    return nglString::Null;
  return pAnim->GetName();
}


void nuiSprite::Animate(float passedtime)
{
  const nuiSpriteAnimation* pAnim = mpSpriteDef->GetAnimation(mCurrentAnimation);
  float fps = pAnim->GetFPS();
  NGL_ASSERT(fps != 0);
  mCurrentFrame += passedtime * mSpeed * fps;
  float count = pAnim->GetFrameCount();
  while (mCurrentFrame >= count)
    mCurrentFrame -= count;
  
  for (int32 i = 0; i < mpChildren.size(); i++)
    mpChildren[i]->Animate(passedtime);
}

float nuiSprite::GetSpeed() const
{
  return mSpeed;
}

void nuiSprite::SetSpeed(float speed)
{
  mSpeed = speed;
}

void nuiSprite::SetPosition(float X, float Y)
{
  mpPosition->Set(X, Y, 0.0f);
}

void nuiSprite::SetAngle(float angle)
{
  mpPivot->SetAngle(angle);
}

void nuiSprite::SetX(float X)
{
  mpPosition->SetX(X);
}

void nuiSprite::SetY(float Y)
{
  mpPosition->SetY(Y);
}

float nuiSprite::GetX() const
{
  return mpPosition->GetX();
}

float nuiSprite::GetY() const
{
  return mpPosition->GetY();
}

float nuiSprite::GetAngle() const
{
  return mpPivot->GetAngle();
}



float nuiSprite::GetScaleX() const
{
  return mpScale->GetX();
}

float nuiSprite::GetScaleY() const
{
  return mpScale->GetY();
}

float nuiSprite::GetScale() const
{
  return mpScale->GetScale();
}

void nuiSprite::SetScaleX(float value)
{
  mpScale->SetX(value);
}

void nuiSprite::SetScaleY(float value)
{
  mpScale->SetY(value);
}

void nuiSprite::SetScale(float value)
{
  mpScale->SetScale(value);
}



/////////////////////////////////////////////
// class nuiSpriteView : public nuiSimpleContainer
nuiSpriteView::nuiSpriteView()
{
  mLastTime = 0;
  if (SetObjectClass(_T("nuiSpriteView")))
  {
    // Init attributes
  }
  
  StartAutoDraw();
}

nuiSpriteView::~nuiSpriteView()
{
  for (int32 i = 0; i < mpChildren.size(); i++)
    mpSprites[i]->Release();
}

void nuiSpriteView::AddSprite(nuiSprite* pSprite)
{
  mpSprites.push_back(pSprite);
}

void nuiSpriteView::DelSprite(nuiSprite* pSprite)
{
  for (int32 i = 0; i < mpChildren.size(); i++)
  {
    if (mpSprites[i] == pSprite)
    {
      mpSprites[i]->Release();
      mpSprites.erase(mpSprites.begin() + i);
      return;
    }
  }
}
  
nuiRect nuiSpriteView::CalcIdealRect()
{
  return nuiRect(320, 200);
}

bool nuiSpriteView::Draw(nuiDrawContext* pContext)
{
  nglTime now;
  double t = 0;
  if (mLastTime != 0)
    t = now - mLastTime;
  mLastTime = now;

  for (int32 i = 0; i < mpSprites.size(); i++)
  {
    mpSprites[i]->Animate(t);
    mpSprites[i]->Draw(pContext);
    
  }

  return true;
}



