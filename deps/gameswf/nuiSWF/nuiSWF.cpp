/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiSWF.cpp
\brief A widget to display ShockWave Flash movie (implementation).
*/

#include "nui.h"
#include "nuiSWF.h"
#include "gameswf.h"
#include "gameswf_types.h"
#include "gameswf_movie_def.h"
#include "base/tu_file.h"
#include "base/image.h"
#include "base/utility.h"

#include <string.h>


struct render_handler_nui : public gameswf::render_handler
{
  void set_antialiased(bool enable);

  struct fill_style
  {
    enum mode
    {
      INVALID,
      COLOR,
      BITMAP_WRAP,
      BITMAP_CLAMP,
      LINEAR_GRADIENT,
      RADIAL_GRADIENT,
    };
    mode  m_mode;
    gameswf::rgba  m_color;
    const gameswf::bitmap_info*  m_bitmap_info;
    gameswf::matrix  m_bitmap_matrix;
    gameswf::cxform  m_bitmap_color_transform;
    bool  m_has_nonzero_bitmap_additive_color;

    fill_style();
    void  apply(/*const matrix& current_matrix*/) const;

    bool  needs_second_pass() const;
    void  apply_second_pass() const;
    void  cleanup_second_pass() const;

    void  disable();
    void  set_color(gameswf::rgba color);
    void  set_bitmap(const gameswf::bitmap_info* bi, const gameswf::matrix& m, bitmap_wrap_mode wm, const gameswf::cxform& color_transform);
    bool  is_valid() const;
  };

  // Style state.
  enum style_index
  {
    LEFT_STYLE = 0,
    RIGHT_STYLE,
    LINE_STYLE,

    STYLE_COUNT
  };
  fill_style  m_current_styles[STYLE_COUNT];

  gameswf::bitmap_info*  create_bitmap_info_rgb(image::rgb* im);
  gameswf::bitmap_info*  create_bitmap_info_rgba(image::rgba* im);
  gameswf::bitmap_info*  create_bitmap_info_empty();
  gameswf::bitmap_info*  create_bitmap_info_alpha(int w, int h, Uint8* data);
  void  delete_bitmap_info(gameswf::bitmap_info* bi);
  ~render_handler_nui();

  void  begin_display(gameswf::rgba background_color, int viewport_x0, int viewport_y0, int viewport_width, int viewport_height, float x0, float x1, float y0, float y1);
  void  end_display();

  gameswf::matrix  m_current_matrix;
  gameswf::cxform  m_current_cxform;
  void  set_matrix(const gameswf::matrix& m);
  void  set_cxform(const gameswf::cxform& cx);

  void  apply_matrix(const gameswf::matrix& m);
  void  apply_color(const gameswf::rgba& c);

  void  fill_style_disable(int fill_side);
  void  line_style_disable();
  void  fill_style_color(int fill_side, gameswf::rgba color);
  void  line_style_color(gameswf::rgba color);
  void  fill_style_bitmap(int fill_side, const gameswf::bitmap_info* bi, const gameswf::matrix& m, bitmap_wrap_mode wm);
  void  line_style_width(float width);

  void  draw_mesh_strip(const void* coords, int vertex_count);
  void  draw_line_strip(const void* coords, int vertex_count);

  void  draw_bitmap(const gameswf::matrix& m, const gameswf::bitmap_info* bi, const gameswf::rect& coords, const gameswf::rect& uv_coords, gameswf::rgba color);

  void begin_submit_mask();
  void end_submit_mask();
  void disable_mask();

private:
  nuiDrawContext* mpContext;
};  // end struct render_handler_nui


//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

// Error callback for handling gameswf messages.
static void  log_callback(bool error, const char* message)
{
#ifdef _DEBUG_
  if (error)
  {
    NGL_OUT(_T("ERROR from GameSWF: %s\n"), message);
  }
  else
  {
    NGL_OUT(_T("GameSWF> %s\n"), message);
  }
#endif
}

static int _read_func(void* dst, int bytes, void* appdata)
{
  nglIStream* pStream = (nglIStream*)appdata;
  return pStream->ReadUInt8((uint8*)dst, bytes);
}

static int _write_func(const void* src, int bytes, void* appdata)
{
  NGL_ASSERT(0); // We don't support writing right now
  nglOStream* pStream = (nglOStream*)appdata;
  return pStream->WriteUInt8((uint8*)src, bytes);
}

static int _seek_func(int pos, void* appdata)
{
  nglIStream* pStream = (nglIStream*)appdata;
  return pStream->SetPos(pos, eStreamFromStart);
}


static int _seek_to_end_func(void* appdata)
{
  nglIStream* pStream = (nglIStream*)appdata;
  return pStream->SetPos(0, eStreamFromEnd);
}

static int _tell_func(const void* appdata)
{
  nglIStream* pStream = (nglIStream*)appdata;
  return pStream->GetPos();
}

static bool _get_eof_func(void* appdata)
{
  nglIStream* pStream = (nglIStream*)appdata;
  return pStream->Available(1) == 0;
}

static int _close_func(void* appdata)
{
  nglIStream* pStream = (nglIStream*)appdata;
  delete pStream;
  return 0;
}


// Callback function.  This opens files for the gameswf library.
static tu_file*  file_opener(const char* url)
{
  //return new tu_file(url, "rb");
  nglIStream* pIStream = nglPath(url).OpenRead();
  if (!pIStream)
    return NULL;
  // The generic constructor; supply functions for the implementation.
  return new tu_file(
          pIStream,
          _read_func,
          _write_func,
          _seek_func,
          _seek_to_end_func,
          _tell_func,
          _get_eof_func,
          _close_func);
  
}

struct render_handler_nui* create_render_handler_nui();

//class nuiSWF : public nuiWidget
nuiSWF::nuiSWF()
: nuiWidget()
{
  SetObjectClass(_T("nuiSWF"));
  mpPlayer = new gameswf::player();
//  mpRenderHandler = create_render_handler_nui();
  mpRenderHandler = gameswf::create_render_handler_ogl();
  mpMovieInterface = NULL;
  mpMovie = NULL;
  mButtons = 0;
  mFPS = 30.0f;
  mLoop = true;
  mWidth = 0;
  mHeight = 0;
  mFrames = 0;
  mTags = 0;
  mInterceptMouse = false;

  EnableRenderCache(false);

  gameswf::set_render_handler(mpRenderHandler);
  gameswf::register_file_opener_callback(file_opener);
  gameswf::register_log_callback(log_callback);

}

bool nuiSWF::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  mpRenderHandler = gameswf::create_render_handler_ogl();
  mpMovieInterface = NULL;
  mpMovie = NULL;
  mButtons = 0;
  mTags = 0;
  mFPS = 30.0f;
  mLoop = true;
  mInterceptMouse = false;
  mWidth = 0;
  mHeight = 0;
  mFrames = 0;
  mTags = 0;

  EnableRenderCache(false);

  gameswf::set_render_handler(mpRenderHandler);
  gameswf::register_file_opener_callback(file_opener);
  gameswf::register_log_callback(log_callback);

  return true;
}

nuiSWF::~nuiSWF()
{
  delete mpPlayer;
//  
//  if (mpMovie)
//  {
//    mpMovie->drop_ref();
//  }
//  if (mpMovieInterface)
//  {
//    mpMovieInterface->drop_ref();
//  }
  delete mpRenderHandler;
}


bool nuiSWF::Load(const nglPath& rPath)
{
  std::string str(rPath.GetPathName().GetStdString());
  const char* pFlashMovieName = str.c_str();
  int  movie_version = 0;

  StopAutoDraw();

//  gameswf::get_movie_info(pFlashMovieName, &movie_version, &mWidth, &mHeight, &mFPS, &mFrames, &mTags);
//  if (movie_version == 0)
//  {
//    NGL_OUT(_T("error: can't get info about %s\n"), pFlashMovieName);
//    return false;
//  }
//  
//  if (mFPS <= 0.5f)
//  {
//    NGL_OUT(_T("Forcing %f FPS instead of 0\n"), mFPS);
//    mFPS = 12.0f;
//  }
//
//  NGL_OUT(_T("Flash file loaded successfully\nName %s\nVersion %i\nWidth %i\nHeight %i\nFPS %f\n"), pFlashMovieName, movie_version, mWidth, mHeight, mFPS);

  //computeMouseScale(mBounds.extent);

  // Load the actual movie.
  mpMovie = mpPlayer->create_movie(pFlashMovieName);
  if (mpMovie == NULL)
  {
    NGL_OUT(_T("error: can't create a movie from '%s'\n"), pFlashMovieName);
    return false;
  }

  movie_version = mpMovie->get_version();
  mWidth = mpMovie->get_width_pixels();
  mHeight = mpMovie->get_height_pixels();
  mFPS = mpMovie->get_frame_rate();
  mFrames = mpMovie->get_frame_count();
  NGL_OUT(_T("Flash file loaded successfully\nName %s\nVersion %i\nWidth %i\nHeight %i\nFPS %f\n"), pFlashMovieName, movie_version, mWidth, mHeight, mFPS);
  
  mpMovieInterface = mpMovie->create_instance();
  if (mpMovieInterface == NULL)
  {
    NGL_OUT(_T("error: can't create movie instance\n"));
    return false;
  }
  mpMovieInterface->add_ref();
    
  InvalidateLayout();
  return true;
}

nuiRect nuiSWF::CalcIdealSize()
{
  return nuiRect(0, 0, mWidth, mHeight);
}

bool nuiSWF::KeyDown(const nglKeyEvent& rEvent)
{
  return false;
}

bool nuiSWF::KeyUp(const nglKeyEvent& rEvent)
{
  return false;
}

static int GetMouseButtons(nglMouseInfo::Flags Button)
{
  int res;
  if (Button & nglMouseInfo::ButtonLeft)
    res |= 1;
  if (Button & nglMouseInfo::ButtonRight)
    res |= 2;
  if (Button & nglMouseInfo::ButtonMiddle)
    res |= 4;
  if (Button & nglMouseInfo::ButtonWheelUp)
    res |= 8;
  if (Button & nglMouseInfo::ButtonWheelDown)
    res |= 16;

  return res;
}


bool nuiSWF::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mButtons |= GetMouseButtons(Button);
  mpMovieInterface->notify_mouse_state(ToBelow(X), ToBelow(Y), mButtons);
  return mInterceptMouse;
}

bool nuiSWF::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mButtons &= ~GetMouseButtons(Button);
  mpMovieInterface->notify_mouse_state(ToBelow(X), ToBelow(Y), mButtons);
  return mInterceptMouse;
}

bool nuiSWF::MouseMoved (nuiSize X, nuiSize Y)
{
  mpMovieInterface->notify_mouse_state(ToBelow(X), ToBelow(Y), mButtons);
  return true;
}

bool nuiSWF::Draw(nuiDrawContext* pContext)
{
  gameswf::set_render_handler(mpRenderHandler);

  nglTime now;
  if (mLastTime <= 0)
    mLastTime = now;
  double lap = now - mLastTime;
  mLastTime = now;
  /* double advance = lap * mFPS; unused */

  nuiRenderArray* pDummy = new nuiRenderArray(GL_POINTS);
  pContext->DrawArray(pDummy);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  float width, height;
  width = mpMovie->get_width_pixels();
  height = mpMovie->get_height_pixels();
  //glScalef(10/width, 10/height, 0);

  nuiSize x=0,y=0;
  LocalToGlobal(x,y);

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_EDGE_FLAG_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  mpMovieInterface->set_display_viewport(ToBelow(x), ToBelow(y), ToBelow(mRect.GetWidth()), ToBelow(mRect.GetHeight()));
  //mpMovieInterface->set_display_viewport(50, 50, 1, 1);
  mpMovieInterface->set_background_alpha(GetAlpha());
  if (IsPlaying())
    mpMovieInterface->advance((float)lap);               
  mpMovieInterface->display();       

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_EDGE_FLAG_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  // Restore state that gameswf kills
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);

  // See if we should exit.
  if (mLoop == false 
    && (mpMovieInterface->get_current_frame() + 1 == mpMovie->get_frame_count() 
    || mpMovieInterface->get_play_state() == gameswf::character::STOP)
    )
  {
  // We're reached the end of the movie; exit.
    Stop();
  }

  glPopAttrib();
  pContext->SetTexture(NULL);
  return true;
}

void nuiSWF::Play()
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  mpMovieInterface->set_play_state(gameswf::character::PLAY);
  mLastTime = -1;
  StartAutoDraw();
}

void nuiSWF::Loop(bool set)
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
}

void nuiSWF::Stop()
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  StopAutoDraw();
  mpMovieInterface->set_play_state(gameswf::character::STOP);
}

void nuiSWF::SetFrame(int Frame)
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  mpMovieInterface->goto_frame(Frame);
}

void nuiSWF::SetFrame(const nglString& rName)
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  std::string tmp(rName.GetStdString());
  mpMovieInterface->goto_labeled_frame(tmp.c_str());
}

int nuiSWF::GetFrame() const
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  return mpMovieInterface->get_current_frame();
}

int nuiSWF::GetFrameCount() const
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  return mFrames;
}

void nuiSWF::SetFrameRate(float fps)
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  mFPS = fps;
  if (IsPlaying())
  {
    StopAutoDraw();
    StartAutoDraw();
  }
}

bool nuiSWF::IsPlaying() const
{
  NGL_ASSERT(mpMovie && mpMovieInterface);
  return mpMovieInterface->get_play_state() ==  gameswf::character::PLAY;
}










#if 0

////////////
// gameswf_render_handler_nui

// A gameswf::render_handler that uses nuiDrawContext


// bitmap_info_nui declaration
struct bitmap_info_nui : public gameswf::bitmap_info
{
  bitmap_info_nui();
  bitmap_info_nui(int width, int height, Uint8* data);
  bitmap_info_nui(image::rgb* im);
  bitmap_info_nui(image::rgba* im);

  ~bitmap_info_nui()
  {
    if (mpTexture)
      mpTexture->Release();
  }

  nuiTexture* mpTexture;
};


// bitmap_info_nui implementation
void  software_resample(int bytes_per_pixel,int src_width,int src_height,int src_pitch,uint8* src_data,int dst_width,int dst_height);

//struct render_handler_nui : public gameswf::render_handler

render_handler_nui::fill_style::fill_style()
: m_mode(INVALID), m_has_nonzero_bitmap_additive_color(false)
{
}

void  render_handler_nui::fill_style::apply(/*const matrix& current_matrix*/) const
// Push our style into OpenGL.
{
  assert(m_mode != INVALID);

  if (m_mode == COLOR)
  {
    apply_color(m_color);
    glDisable(GL_TEXTURE_2D);
  }
  else if (m_mode == BITMAP_WRAP
    || m_mode == BITMAP_CLAMP)
  {
    assert(m_bitmap_info != NULL);

    apply_color(m_color);

    if (m_bitmap_info == NULL)
    {
      glDisable(GL_TEXTURE_2D);
    }
    else
    {
      // Set up the texture for rendering.

      {
        // Do the modulate part of the color
        // transform in the first pass.  The
        // additive part, if any, needs to
        // happen in a second pass.
        glColor4f(m_bitmap_color_transform.m_[0][0],
          m_bitmap_color_transform.m_[1][0],
          m_bitmap_color_transform.m_[2][0],
          m_bitmap_color_transform.m_[3][0]
          );
      }

      glBindTexture(GL_TEXTURE_2D, m_bitmap_info->m_texture_id);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);

      if (m_mode == BITMAP_CLAMP)
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }
      else
      {
        assert(m_mode == BITMAP_WRAP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      }

      // Set up the bitmap matrix for texgen.

      float  inv_width = 1.0f / m_bitmap_info->m_original_width;
      float  inv_height = 1.0f / m_bitmap_info->m_original_height;

      const gameswf::matrix&  m = m_bitmap_matrix;
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      float  p[4] = { 0, 0, 0, 0 };
      p[0] = m.m_[0][0] * inv_width;
      p[1] = m.m_[0][1] * inv_width;
      p[3] = m.m_[0][2] * inv_width;
      glTexGenfv(GL_S, GL_OBJECT_PLANE, p);

      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      p[0] = m.m_[1][0] * inv_height;
      p[1] = m.m_[1][1] * inv_height;
      p[3] = m.m_[1][2] * inv_height;
      glTexGenfv(GL_T, GL_OBJECT_PLANE, p);
    }
  }
}


bool  render_handler_nui::fill_style::needs_second_pass() const
// Return true if we need to do a second pass to make
// a valid color.  This is for cxforms with additive
// parts; this is the simplest way (that we know of)
// to implement an additive color with stock OpenGL.
{
  if (m_mode == BITMAP_WRAP
    || m_mode == BITMAP_CLAMP)
  {
    return m_has_nonzero_bitmap_additive_color;
  }
  else
  {
    return false;
  }
}

void  render_handler_nui::fill_style::apply_second_pass() const
// Set OpenGL state for a necessary second pass.
{
  assert(needs_second_pass());

  // The additive color also seems to be modulated by the texture. So,
  // maybe we can fake this in one pass using using the mean value of 
  // the colors: c0*t+c1*t = ((c0+c1)/2) * t*2
  // I don't know what the alpha component of the color is for.
  //glDisable(GL_TEXTURE_2D);

  glColor4f(
    m_bitmap_color_transform.m_[0][1] / 255.0f,
    m_bitmap_color_transform.m_[1][1] / 255.0f,
    m_bitmap_color_transform.m_[2][1] / 255.0f,
    m_bitmap_color_transform.m_[3][1] / 255.0f
    );

  glBlendFunc(GL_ONE, GL_ONE);
}

void  render_handler_nui::fill_style::cleanup_second_pass() const
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void  render_handler_nui::fill_style::disable()
{
  m_mode = INVALID;
}

void  render_handler_nui::fill_style::set_color(gameswf::rgba color) 
{
  m_mode = COLOR; 
  m_color = color;
}

void  render_handler_nui::fill_style::set_bitmap(const gameswf::bitmap_info* bi, const gameswf::matrix& m, bitmap_wrap_mode wm, const gameswf::cxform& color_transform)
{
  m_mode = (wm == WRAP_REPEAT) ? BITMAP_WRAP : BITMAP_CLAMP;
  m_color = gameswf::rgba();
  m_bitmap_info = bi;
  m_bitmap_matrix = m;
  m_bitmap_color_transform = color_transform;

  if (m_bitmap_color_transform.m_[0][1] > 1.0f
    || m_bitmap_color_transform.m_[1][1] > 1.0f
    || m_bitmap_color_transform.m_[2][1] > 1.0f
    || m_bitmap_color_transform.m_[3][1] > 1.0f)
  {
    m_has_nonzero_bitmap_additive_color = true;
  }
  else
  {
    m_has_nonzero_bitmap_additive_color = false;
  }
}

bool  render_handler_nui::fill_style::is_valid() const 
{ 
  return m_mode != INVALID;
}

gameswf::bitmap_info*  render_handler_nui::create_bitmap_info_rgb(image::rgb* im)
// Given an image, returns a pointer to a bitmap_info struct
// that can later be passed to fill_styleX_bitmap(), to set a
// bitmap fill style.
{
  return new bitmap_info_nui(im);
}


gameswf::bitmap_info*  render_handler_nui::create_bitmap_info_rgba(image::rgba* im)
// Given an image, returns a pointer to a bitmap_info struct
// that can later be passed to fill_style_bitmap(), to set a
// bitmap fill style.
//
// This version takes an image with an alpha channel.
{
  return new bitmap_info_nui(im);
}


gameswf::bitmap_info*  render_handler_nui::create_bitmap_info_empty()
// Create a placeholder bitmap_info.  Used when
// DO_NOT_LOAD_BITMAPS is set; then later on the host program
// can use movie_definition::get_bitmap_info_count() and
// movie_definition::get_bitmap_info() to stuff precomputed
// textures into these bitmap infos.
{
  return new bitmap_info_nui;
}

gameswf::bitmap_info*  render_handler_nui::create_bitmap_info_alpha(int w, int h, Uint8* data)
// Create a bitmap_info so that it contains an alpha texture
// with the given data (1 byte per texel).
//
// Munges *data (in order to make mipmaps)!!
{
  return new bitmap_info_nui(w, h, data);
}


void  render_handler_nui::delete_bitmap_info(gameswf::bitmap_info* bi)
// Delete the given bitmap info struct.
{
  delete bi;
}


render_handler_nui::~render_handler_nui()
{
}


void  render_handler_nui::begin_display(gameswf::rgba background_color, int viewport_x0, int viewport_y0, int viewport_width, int viewport_height, float x0, float x1, float y0, float y1)
// Set up to render a full frame from a movie and fills the
// background.  Sets up necessary transforms, to scale the
// movie to fit within the given dimensions.  Call
// end_display() when you're done.
//
// The rectangle (viewport_x0, viewport_y0, viewport_x0 +
// viewport_width, viewport_y0 + viewport_height) defines the
// window coordinates taken up by the movie.
//
// The rectangle (x0, y0, x1, y1) defines the pixel
// coordinates of the movie that correspond to the viewport
// bounds.
{
  // Thomas >> - viewport needs to be commented out to work nice with TGE and we manipulate the ortho too
  //glViewport(viewport_x0, viewport_y0, viewport_width, viewport_height);
  /*
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(x0, x1, y1, y0, 0, 1);      
  */
  // << Thomas
  mpContext->PushState();
  mpContext->PushClipping();
  mpContext->EnableBlending(true);
  mpContext->SetBlendFunc(nuiBlendTransp);

//  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  // GL_MODULATE

//  glDisable(GL_TEXTURE_2D);

  // Clear the background, if background color has alpha > 0.
  if (background_color.m_a > 0)
  {
    // Draw a big quad.
/*
    apply_color(background_color);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
*/
    mpContext->SetFillColor(nuiColor(background_color.m_r, background_color.m_g, background_color.m_b, background_color.m_a));
    mpContext->DrawRect(nuiRect(0.0f, 0.0f, x1-x0, y1-y0), eFillShape);
  }
}


void  render_handler_nui::end_display()
// Clean up after rendering a frame.  Client program is still
// responsible for calling glSwapBuffers() or whatever.
{
  mpContext->PopClipping();
  mpContext->PopState();
}


void  render_handler_nui::set_matrix(const gameswf::matrix& m)
// Set the current transform for mesh & line-strip rendering.
{
  m_current_matrix = m;
}


void  render_handler_nui::set_cxform(const gameswf::cxform& cx)
// Set the current color transform for mesh & line-strip rendering.
{
  m_current_cxform = cx;
}

void render_handler_nui::apply_matrix(const gameswf::matrix& m)
// multiply current matrix with opengl matrix
{
  float  mat[16];
  memset(&mat[0], 0, sizeof(mat));
  mat[0] = m.m_[0][0];
  mat[1] = m.m_[1][0];
  mat[4] = m.m_[0][1];
  mat[5] = m.m_[1][1];
  mat[10] = 1;
  mat[12] = m.m_[0][2];
  mat[13] = m.m_[1][2];
  mat[15] = 1;
  //glMultMatrixf(mat);
  mpContext->MultMatrix(nuiMatrix(mat));
}

void render_handler_nui::apply_color(const gameswf::rgba& c)
// Set the given color.
{
  glColor4ub(c.m_r, c.m_g, c.m_b, c.m_a);
}

void  render_handler_nui::fill_style_disable(int fill_side)
// Don't fill on the {0 == left, 1 == right} side of a path.
{
  assert(fill_side >= 0 && fill_side < 2);

  m_current_styles[fill_side].disable();
}


void  render_handler_nui::line_style_disable()
// Don't draw a line on this path.
{
  m_current_styles[LINE_STYLE].disable();
}


void  render_handler_nui::fill_style_color(int fill_side, gameswf::rgba color)
// Set fill style for the left interior of the shape.  If
// enable is false, turn off fill for the left interior.
{
  assert(fill_side >= 0 && fill_side < 2);

  m_current_styles[fill_side].set_color(m_current_cxform.transform(color));
}


void  render_handler_nui::line_style_color(gameswf::rgba color)
// Set the line style of the shape.  If enable is false, turn
// off lines for following curve segments.
{
  m_current_styles[LINE_STYLE].set_color(m_current_cxform.transform(color));
}


void  render_handler_nui::fill_style_bitmap(int fill_side, const gameswf::bitmap_info* bi, const gameswf::matrix& m, bitmap_wrap_mode wm)
{
  assert(fill_side >= 0 && fill_side < 2);
  m_current_styles[fill_side].set_bitmap(bi, m, wm, m_current_cxform);
}

void  render_handler_nui::line_style_width(float width)
{
  // WK: what to do here???
}


void  render_handler_nui::draw_mesh_strip(const void* coords, int vertex_count)
{
  // Set up current style.
  m_current_styles[LEFT_STYLE].apply();

  std::list<nuiShape::CacheElement*> mElements;
  nuiShape::CacheElement element(GL_TRIANGLE_STRIP, vertex_count);
  element.CacheElement())
  mElements.push_back(&element);

  nuiShape shp;
  shp.GetFillCache().push_back()
  mpContext->DrawCachedShape(&mElements, eFillShape);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  apply_matrix(m_current_matrix);

  // Send the tris to OpenGL
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_SHORT, sizeof(Sint16) * 2, coords);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count);

  if (m_current_styles[LEFT_STYLE].needs_second_pass())
  {
    m_current_styles[LEFT_STYLE].apply_second_pass();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count);
    m_current_styles[LEFT_STYLE].cleanup_second_pass();
  }

  glDisableClientState(GL_VERTEX_ARRAY);

  glPopMatrix();
}


void  render_handler_nui::draw_line_strip(const void* coords, int vertex_count)
// Draw the line strip formed by the sequence of points.
{
  // Set up current style.
  m_current_styles[LINE_STYLE].apply();

  nuiShape shp;
  nuiContour* pContour = new nuiContour();
  std::list<nuiPoint> lines;

  for (uint i = 0; i < vertex_count; i++)
    lines.push_back(nuiPoint(coords[i*2], coords[i*2+1]));
  pContour->AddLines(lines);
  shp.AddContour(pContour);

  mpContext->PushMatrix();
  apply_matrix(m_current_matrix);

  if (mMasking)
  {
    mpContext->AddClipShape(shp);
  }
  else
  {
    mpContext->DrawShape(&shp, nuiShapeMode::eStrokeShape);
  }
  mpContext->PopMatrix();

/*
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  apply_matrix(m_current_matrix);

  // Send the line-strip to OpenGL
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_SHORT, sizeof(Sint16) * 2, coords);
  glDrawArrays(GL_LINE_STRIP, 0, vertex_count);
  glDisableClientState(GL_VERTEX_ARRAY);

  glPopMatrix();
*/
}


void  render_handler_nui::draw_bitmap(
                                      const gameswf::matrix& m,
                                      const gameswf::bitmap_info* bi,
                                      const gameswf::rect& coords,
                                      const gameswf::rect& uv_coords,
                                      gameswf::rgba color)
                                      // Draw a rectangle textured with the given bitmap, with the
                                      // given color.   Apply given transform; ignore any currently
                                      // set transforms.
                                      //
                                      // Intended for textured glyph rendering.
{
  assert(bi);
  const bitmap_info_nui* pbi = (bitmap_info_nui*)bi;

  apply_color(color);

  gameswf::point a, b, c, d;
  m.transform(&a, gameswf::point(coords.m_x_min, coords.m_y_min));
  m.transform(&b, gameswf::point(coords.m_x_max, coords.m_y_min));
  m.transform(&c, gameswf::point(coords.m_x_min, coords.m_y_max));
  d.m_x = b.m_x + c.m_x - a.m_x;
  d.m_y = b.m_y + c.m_y - a.m_y;

  mpContext->SetFillColor(nuiColor(color.m_r, color.m_g, color.m_b, color.m_a));
  mpContext->SetCurrentTexture(pbi->mpTexture);
  mpContext->EnableTexturing(true);
  nuiRect src(uv_coords.m_x_min, uv_coords.m_y_min, uv_coords.m_x_max, uv_coords.m_y_max, false);
  mpContext->DrawImageQuad(a.m_x, a.m_y, b.m_x, b.m_y, c.m_x, c.m_y, d.m_x, d.m_y, src);
/*
  glBindTexture(GL_TEXTURE_2D, bi->m_texture_id);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);

  glBegin(GL_TRIANGLE_STRIP);

  glTexCoord2f(uv_coords.m_x_min, uv_coords.m_y_min);
  glVertex2f(a.m_x, a.m_y);

  glTexCoord2f(uv_coords.m_x_max, uv_coords.m_y_min);
  glVertex2f(b.m_x, b.m_y);

  glTexCoord2f(uv_coords.m_x_min, uv_coords.m_y_max);
  glVertex2f(c.m_x, c.m_y);

  glTexCoord2f(uv_coords.m_x_max, uv_coords.m_y_max);
  glVertex2f(d.m_x, d.m_y);

  glEnd();
*/
}

void render_handler_nui::begin_submit_mask()
{
  mMasking = true;
/*
  glEnable(GL_STENCIL_TEST); 
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  glColorMask(0,0,0,0);  // disable framebuffer writes
  glEnable(GL_STENCIL_TEST);  // enable stencil buffer for "marking" the mask
  glStencilFunc(GL_ALWAYS, 1, 1);  // always passes, 1 bit plane, 1 as mask
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // we set the stencil buffer to 1 where we draw any polygon
*/
  // keep if test fails, keep if test passes but buffer test fails
  // replace if test passes 
}

void render_handler_nui::end_submit_mask()
{       
  mMasking = false;
/*
  glColorMask(1,1,1,1);  // enable framebuffer writes
  glStencilFunc(GL_EQUAL, 1, 1);  // we draw only where the stencil is 1 (where the mask was drawn)
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);  // don't change the stencil buffer    
*/
}

void render_handler_nui::disable_mask()
{         
  mMasking = false;

/*
  glDisable(GL_STENCIL_TEST); 
*/
}


// bitmap_info_nui implementation
bitmap_info_nui::bitmap_info_nui()
// Make a placeholder bitmap_info.  Must be filled in later before
// using.
{
  mpTexture = NULL;
}


bitmap_info_nui::bitmap_info_nui(int width, int height, Uint8* data)
// Initialize this bitmap_info to an alpha image
// containing the specified data (1 byte per texel).
//
// !! Munges *data in order to create mipmaps !!
{
  assert(width > 0);
  assert(height > 0);
  assert(data);

  // Create the texture.
  nglImageInfo info(false);
  info.mBitDepth = 8;
  info.mBufferFormat = eImageFormatRaw;
  info.mBytesPerLine = width;
  info.mBytesPerPixel = 1;
  info.mHeight = height;
  info.mWidth = width;
  info.mpBuffer = (char*)data;
  info.mPixelFormat = eImagePixelAlpha;
  mpTexture = nuiTexture::GetTexture(info);
  if (mpTexture)
  {
    mpTexture->SetWrapS(GL_CLAMP_TO_EDGE);
    mpTexture->SetWrapT(GL_CLAMP_TO_EDGE);
    mpTexture->SetMagFilter(GL_LINEAR);
    mpTexture->SetMinFilter(GL_NEAREST);
    mpTexture->SetEnvMode(GL_MODULATE);
  }
}


bitmap_info_nui::bitmap_info_nui(image::rgb* im)
{
  assert(im);

  // Create the texture.
  nglImageInfo info(false);
  info.mBitDepth = 8;
  info.mBufferFormat = eImageFormatRaw;
  info.mBytesPerLine = im->m_width*3 + im->m_pitch;
  info.mBytesPerPixel = 3;
  info.mHeight = im->m_height;
  info.mWidth = im->m_width;
  info.mpBuffer = (char*)im->m_data;
  info.mPixelFormat = eImagePixelRGB;
  mpTexture = nuiTexture::GetTexture(info);
  if (mpTexture)
  {
    mpTexture->SetWrapS(GL_CLAMP_TO_EDGE);
    mpTexture->SetWrapT(GL_CLAMP_TO_EDGE);
    mpTexture->SetMagFilter(GL_LINEAR);
    mpTexture->SetMinFilter(GL_NEAREST);
    mpTexture->SetEnvMode(GL_MODULATE);
  }
}


bitmap_info_nui::bitmap_info_nui(image::rgba* im)
// Version of the constructor that takes an image with alpha.
{
  assert(im);

  // Create the texture.
  nglImageInfo info(false);
  info.mBitDepth = 8;
  info.mBufferFormat = eImageFormatRaw;
  info.mBytesPerLine = im->m_width*4 + im->m_pitch;
  info.mBytesPerPixel = 4;
  info.mHeight = im->m_height;
  info.mWidth = im->m_width;
  info.mpBuffer = (char*)im->m_data;
  info.mPixelFormat = eImagePixelRGBA;
  mpTexture = nuiTexture::GetTexture(info);
  if (mpTexture)
  {
    mpTexture->SetWrapS(GL_CLAMP_TO_EDGE);
    mpTexture->SetWrapT(GL_CLAMP_TO_EDGE);
    mpTexture->SetMagFilter(GL_LINEAR);
    mpTexture->SetMinFilter(GL_NEAREST);
    mpTexture->SetEnvMode(GL_MODULATE);
  }
}

void render_handler_nui::set_antialiased(bool enable)
{
  mpContext->EnableAntialiasing(enable);
}

render_handler_nui* create_render_handler_nui()
{
  return new render_handler_nui();
}

#endif
