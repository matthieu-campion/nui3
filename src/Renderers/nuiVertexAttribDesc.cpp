//
//  nuiVertexAttribDesc.cpp
//  nui3
//
//  Created by Sébastien Métrot on 3/6/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#include "nui.h"

nuiVertexAttribDesc::nuiVertexAttribDesc()
{

}

nuiVertexAttribDesc::nuiVertexAttribDesc(const nglString& rName, GLenum Type, int count, GLuint Location)
: mName(rName), mType(Type), mCount(count), mLocation(Location)
{

}

nuiVertexAttribDesc::~nuiVertexAttribDesc()
{

}



