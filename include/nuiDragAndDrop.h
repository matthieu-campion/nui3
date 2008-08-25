/*
  NUI - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sébastien Métrot

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#define __nuiDragAndDrop_h__ // Disable this include

#ifndef __nuiDragAndDrop_h__
#define __nuiDragAndDrop_h__

#include "nuiContainer.h"
#include <map>

typedef std::map<nglString,nuiObjectPtr, nglString::LessFunctor> nuiDragObjectMap;

class NUI_API nuiDragAndDrop : public nuiSimpleContainer
{
public:
  nuiDragAndDrop(nuiWidget* pSource, nuiSize X, nuiSize Y);
  virtual ~nuiDragAndDrop();

  virtual void SetSource(nuiWidget* pSource);
  virtual nuiWidget* GetSource();
  virtual void SetDestination(nuiWidget* pDest);
  virtual nuiWidget* GetDestination();

  virtual void AddType(const nglString& rType); ///< The drag source object have to call this method to add drag type. Prefered types are MIME compatible.
  virtual bool HasType(const nglString& rType); ///< Returns true if the dragged object supports the given type.

  virtual void SetData(const nglString& rType, nuiObjectPtr pObject); ///< This method must be called by the drag source at last when it has been informed that the drag and drop operation have been confirmed. It should add data for all the supported types. A source *can* add the data when it starts the drag operation, but it is legal to create it at the last time. 
  virtual nuiObjectPtr GetData(const nglString& rType); ///< Returns the data object associated with the dragged object and the given type. This method can only be called from within the nuiWidget::OnObjectDropped method (i.e. after the object have actually been dropped on a widget).

  virtual nuiRect CalcIdealSize();

  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved  (nuiSize X, nuiSize Y);

  virtual bool IsInside(nuiSize X, nuiSize Y);
  virtual bool IsInsideLocal(nuiSize X, nuiSize Y);
  
  virtual bool SetRect(const nuiRect& rRect);

protected:
  nuiWidget* mpSource;
  nuiWidget* mpDestination;
  nuiDragObjectMap mpElements; 

  nuiSize mPosX;
  nuiSize mPosY;
};

#define NUI_MIN_DRAG_TIME (0.1)  // 1/10th of a second seems ok to wait until we start drag & droping.

#endif // __nuiDragAndDrop_h__

