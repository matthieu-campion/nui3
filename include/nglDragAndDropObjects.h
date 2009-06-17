/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglDragAndDropObjects.h
\brief Drag and drop object that contains one or several data types, associated informations and datas, for dnd operations
*/

#ifndef __nglDragAndDropObjects_h__
#define __nglDragAndDropObjects_h__

//#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
//#include "nglDataObjects.h"

#include <map>
#include <list>

enum nglDropEffect
{
  eDropEffectNone=0,
  eDropEffectCopy,
  eDropEffectMove,
  eDropEffectLink,
  eDropEffectScroll
};

class nglDataObject;
class nglImage;

class NGL_API nglDragAndDrop
{
public:
  /** @name Life cycle */
  //@{
  /*!<
    Create a DragnDrop object
    \param pFeedbackImage is an optionnal image which will be displayed under the mouse cursor while dragging.
    \param offsetX is the horizontal offset inside the image that will be drawn. To horizontaly center the image under the mouse cursor use the width of the image divided by 2.
    \param offsetY is the vertical offset inside the image that will be drawn. To verticaly center the image under the mouse cursor use the height of the image divided by 2.
  */
  nglDragAndDrop(nglImage *pFeedbackImage = NULL, uint offsetX = 0, uint offsetY = 0);
  virtual ~nglDragAndDrop();
  //@}

  /** @name User Interfaces */
  //@{
  void AddType(nglDataObject* pObject);
  /*!<
    Add a data type to be involved in a drag and drop operation, and/or not/ its associated data
    \param rMimeType is the mime type string associated to this type \b Caution \b the type has to had been registred in the nglDataTypesRegistry member of nglKernel
    \param pObject is the object containing datas and procedures to share them between platform specific dnd operations and ngl users
  */
  nglDataObject* GetType(const nglString& rMimeType) const;
  /*!<
    Get the DataObject associated to this particular mime type
    \param rMimeType is the mime type string associated to this type \b Caution \b the type has to had been registred in the nglDataTypesRegistry member of nglKernel
    \return the DataObject associated to this particular mime type
  */
  
  bool IsTypeSupported(const nglString& rMimeType) const;
  /*!<
    Check if the type rMimeType has been added
    \param rMimeType is the mime type string associated to this type \b Caution \b the type has to had been registred in the nglDataTypesRegistry member of nglKernel
    \return true if the type has already been added, false otherwise
  */
  

  const std::list<nglDropEffect>& GetSupportedDropEffects() const;
  /*!<
    Accessor to supported drop effects considered by the current drag and drop operation
    \return a reference on the list of supported nglDropEffect
  */
  
  void AddSupportedDropEffect(nglDropEffect effect);
  /*!<
    Add a drop effect to the supported list of a drop operation
    \param effect is the drop effect to be added in the list of supported nglDropEffect
  */

  void SetDesiredDropEffect(nglDropEffect effect);
  /*!<
    Set the effect of a drop when it occurs
    \param effect is the drop effect
  */

  nglDropEffect GetDesiredDropEffect() const;
  /*!<
    Get the effect of a drop when it occurs
    \return is the drop effect
  */

  const std::map<nglString, nglDataObject*>& GetSupportedTypesMap() const;
  
  const nglImage *GetFeedbackImage() const;
  /*!<
    Returns the image used for feedback visualization of the dragging if there is one. Returns NULL if we don't want to show something special
  */

  uint GetFeedbackImageOffsetX() const;
  /*!<
    Returns the X offset in the image used for feedback visualization of the dragging if there is one
  */

  uint GetFeedbackImageOffsetY() const;
  /*!<
    Returns the Y offset in the image used for feedback visualization of the dragging if there is one
  */
  
  int32 GetModifiers() const;
  //@}


protected:
  friend class nglDropTarget;
  int32 mModifiers;
  nglImage *mpFeedbackImage;
  uint32 mOffsetX, mOffsetY;
  std::map<nglString, nglDataObject*>         mObjects;
  std::list<nglDropEffect>                    mSupportedDropEffects;
  nglDropEffect                               mDesiredDropEffect;
};

#endif//__nglDragAndDropObjects_h__
