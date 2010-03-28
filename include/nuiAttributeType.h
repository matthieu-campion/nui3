/*
 *  nuiAttributeType.h
 *  nui3
 *
 *  Created by Sebastien Metrot on 3/28/10.
 *  Copyright 2010 libNUI. All rights reserved.
 *
 */

typedef uint64 nuiAttributeType;


template <class Type>
class nuiAttributeTypeTrait
{
public:
  static uint64 mTypeId;
};


uint64 nuiGetNewAttributeUniqueId();

