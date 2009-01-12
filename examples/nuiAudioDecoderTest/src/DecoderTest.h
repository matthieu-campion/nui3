/*
 *  DecoderTest.h
 *  nuiAudioDecoderTest
 *
 *  Created by mat on 1/5/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nui.h"
#include "nuiSampleInfo.h"

bool ConvertCompressedAudioFile(nglPath inPath, nglPath outPath);
bool TestSeek(nglPath inPath, nglPath outPath);
void PrintAudioInfos(nuiSampleInfo info);

