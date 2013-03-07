//
//  nuiVertexAttribDesc.h
//  nui3
//
//  Created by Sébastien Métrot on 3/6/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#pragma once


class nuiVertexAttribDesc
{
public:
  nuiVertexAttribDesc();
  nuiVertexAttribDesc(const nglString& rName, GLenum Type, int count, GLuint Location);

  ~nuiVertexAttribDesc();

  nglString mName;
  GLenum mType;
  GLenum mCount;
  GLuint mLocation;

};

