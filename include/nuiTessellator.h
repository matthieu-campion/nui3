/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

// nuiTessellator.h

#ifndef __nuiTessellator_h__
#define __nuiTessellator_h__

class nuiRenderObject;
class nuiShape;
class nuiPathGenerator;

class NUI_API nuiTessellator
{
public:
  explicit nuiTessellator(nuiPathGenerator* pPathGenerator);
  explicit nuiTessellator(nuiShape* pShape);
  virtual ~nuiTessellator();

  nuiRenderObject* Generate(float Quality = 0.5f);

  void SetFill(bool Set) { mOutLine = !Set; }
  void SetOutLine(bool Set) { mOutLine = Set; }
  bool GetFill() const { return !mOutLine; }
  bool GetOutLine() const { return mOutLine; }
private:
  nuiPathGenerator* mpPath;
  nuiShape* mpShape;
  bool mOutLine;

  nuiRenderObject* GenerateFromPath(float Quality);
  nuiRenderObject* GenerateFromShape(float Quality);

  static struct GLUtesselator* mpTess;
  static uint32 mRefs;
};

#endif // nuiTessellator
