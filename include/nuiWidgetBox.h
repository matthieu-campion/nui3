/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWidgetBox_h__
#define __nuiWidgetBox_h__

#include "nuiContainer.h"

/** A nuiWidgetBox is a widget container that packs its children horizontally or vertically.
*/

class NUI_API nuiWidgetBox : public nuiSimpleContainer
{
public:

  nuiWidgetBox(nuiOrientation orientation = nuiVertical);
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiWidgetBox();

  virtual void InitProperties();

  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect); ///< This method asks the object to recalculate its layout and to force using the given bounding rectangle. It returns false in case of error.

  virtual void SetOrientation(nuiOrientation orientation);
  virtual nuiOrientation SetOrientation();
  virtual void SetProportional(bool Proportional); ///< if Proportional is true all the children will have a final size proportional to their ideal size, if it is false they will all have the same size.
  virtual bool GetProportional(); ///< Return true if the children use a size proportional to their ideal size or false if they have all the same size.
  virtual void SetFill(bool Fill); ///< if Fill is true and the size given to this widget is respected by its container it will fill this place with its children. If it is false it will stack the children from the top to the bottom respecting their ideal size.
  virtual bool GetFill(); ///< Return true if the children try to fill the WidgetBox's area (see nuiWidgetBox::SetFill()).

  // Other atributes:
  virtual nuiSize GetBorderSize(); ///< Retreive the current border around the area of the enclosed widget.
  virtual void SetBorderSize(nuiSize BorderSize); ///< Set the current border around the area of the enclosed widget.

  // Class attributes:
  static nuiSize GetDefaultBorderSize(); ///< Retreive the default border around the area of the enclosed widget of every button that will be created.
  static void SetDefaultBorderSize(nuiSize BorderSize); ///< Set the default border around the area of the enclosed widget of every button that will be created.
protected:
  nuiOrientation mOrientation; /// mHorizontal is true if the containers' layout if horizontal, false if vertical.
  std::list<nuiRect> mPositions;
  float mBorderSize;
  bool mProportional;
  bool mFill;
    
  static nuiSize mDefaultBorderSize;
};

#endif // __nuiWidgetBox_h__
