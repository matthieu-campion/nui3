/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

///////////////////////////////////////
//************************************
//Format tags
//************************************
///////////////////////////////////////

///////////////////////////////////////
//Wave
///////////////////////////////////////
///Wave file unknown compression format
#define WAVE_FORMAT_UNKNOWN 0

///Wave file PCM compression format
#define WAVE_FORMAT_PCM 1

///Wave file IEEE float compression format
#define WAVE_FORMAT_IEEE_FLOAT 3

///////////////////////////////////////
//Aiff
#define AIFF_FORMAT_TAG 0

///////////////////////////////////////
//************************************
//Chunks Sizes
//************************************
///////////////////////////////////////

/// Size of a chunk header : chunkID + chunk datas size
#define CHUNK_HEADER_SIZE 8

///////////////////////////////////////
//Wave
///////////////////////////////////////
/// Size of datas in Wave file RIFF chunk
#define RIFF_CHUNK_DATA_SIZE 4 

/// Size of datas in Wave file Fmt chunk when Compression Format = PCM
#define FMT_CHUNK_DATA_SIZE_PCM 16

/// Size of datas in Wave file Fmt chunk when Compression Format = IEEE float
#define FMT_CHUNK_DATA_SIZE_IEEE_FLOAT 18

/// Size of datas in Wave file Fact chunk (only used when Compression Format = IEEE float)
#define FACT_CHUNK_DATA_SIZE 4

///////////////////////////////////////
//Aiff
///////////////////////////////////////
/// Size of datas in Aiff file FORM chunk
#define FORM_CHUNK_DATA_SIZE 4

/// Size of datas in Aiff file COMM chunk
#define COMM_CHUNK_DATA_SIZE 18

///////////////////////////////////////
///////////////////////////////////////


