/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglImagePNGCodec.h"
#include "png.h"


class nglImagePNGCodec : public nglImageCodec
{
public:
  nglImagePNGCodec();
  virtual ~nglImagePNGCodec();
  virtual bool Probe(nglIStream* pIStream);
  virtual bool Feed(nglIStream* pOStream); 
  virtual bool Save(nglOStream* pOStream); 
  virtual float GetCompletion(); 
private:
  png_structp png_ptr;
  png_infop info_ptr;
  png_byte** mpRowPointers;

  int initialize_png_reader();
  /* A code fragment that you call as you receive blocks of data */
  int process_data(png_bytep buffer, png_uint_32 length);
  /* This function is called (as set by png_set_progressive_read_fn() above) when enough data
    has been supplied so all of the header has been read. */
  friend void info_callback(png_structp png_ptr, png_infop info);
  /* This function is called when each row of image data is complete */
  friend void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass);
  friend void end_callback(png_structp png_ptr, png_infop info);

  void InfoCallback(png_structp png_ptr, png_infop info_ptr);

  bool mStop;
};

nglImageCodec* nglImagePNGCodecInfo::CreateInstance()
{
  return new nglImagePNGCodec();
}


nglImagePNGCodec::nglImagePNGCodec()
{
  mpRowPointers = NULL;
  png_ptr = NULL;
  info_ptr = NULL;
  mStop = false;
}

nglImagePNGCodec::~nglImagePNGCodec()
{
  if (mpRowPointers)
    free (mpRowPointers);
  mpRowPointers = NULL;

  if (png_ptr && info_ptr)
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  png_ptr = NULL;
  info_ptr = NULL;
}

bool nglImagePNGCodec::Probe(nglIStream* pIStream)
{
  if (pIStream->Available(4)) // A not so robust png detection routine...
  {
    char buffer[4];
    if (!pIStream->Peek(buffer,1,4))
      return false;
    if (buffer[1]=='P' && 
        buffer[2]=='N' && 
        buffer[3]=='G')
    {
      initialize_png_reader();
      return true;
    }
  }
  return false;
}


/* This function is called (as set by
  png_set_progressive_read_fn() above) when enough data
  has been supplied so all of the header has been
  read.
  */
void info_callback(png_structp png_ptr, png_infop info_ptr) 
{
  /* Do any setup here, including setting any of
    the transformations mentioned in the Reading
    PNG files section.  For now, you _must_ call
    either png_start_read_image() or
    png_read_update_info() after all the
    transformations are set (even if you don't set
    any).  You may start getting rows before
    png_process_data() returns, so this is your
    last chance to prepare for that.
      */
  nglImagePNGCodec* pCodec=(nglImagePNGCodec*)(png_get_progressive_ptr(png_ptr));
  pCodec->InfoCallback(png_ptr, info_ptr);
}

/* This function is called when each row of image
data is complete */
void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) 
{
  nglImagePNGCodec* pCodec = (nglImagePNGCodec*)(png_get_progressive_ptr(png_ptr));

  char* buffer = pCodec->mpImage->GetBuffer();
  uint size = pCodec->mpImage->GetBytesPerLine();
  buffer += row_num * size;
  png_progressive_combine_row(png_ptr, (png_byte*)buffer, new_row);
}

void end_callback(png_structp png_ptr, png_infop info) 
{
/* This function is called after the whole image
  has been read, including any chunks after the
  image (up to and including the IEND).  You
  will usually have the same info chunk as you
  had in the header, although some data may have
  been added to the comments and time fields.

  Most people won't do much here, perhaps setting
  a flag that marks the image as finished.
  */
//  nglImagePNGCodec* pCodec=(nglImagePNGCodec*)(png_get_progressive_ptr(png_ptr));
}

/*  An example code fragment of how you would initialize the progressive reader in your application. */
int nglImagePNGCodec::initialize_png_reader() 
{
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
  if (!png_ptr)
    return -1;
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) 
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return -1;
  }
  
  if (setjmp(png_ptr->jmpbuf)) 
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    return -1;
  }

#ifdef _WIN32_
  double ScreenGamma = 2.2 / 1.0;
#elif (defined _UNIX_)
  double ScreenGamma = 2.2 / 1.7;
#elif (defined _CARBON_)
  double ScreenGamma = 2.2 / 1.45;
#endif //ifdef _WIN32_
  double FileGamma = 1.0 / 2.2;
//  if (png_get_gAMA(png_ptr, info_ptr, &FileGamma))
//    png_set_gamma(png_ptr, ScreenGamma, FileGamma);
//  else
//    png_set_gamma(png_ptr, ScreenGamma, 1.0 / 2.2);
  
  /* This one's new.  You can provide functions to be called when the header info is valid,
    when each row is completed, and when the image is finished.  If you aren't using all functions,
    you can specify NULL parameters.  Even when all three functions are NULL, you need to call
    png_set_progressive_read_fn().  You can use any struct as the user_ptr (cast to a void pointer
    for the function call), and retrieve the pointer from inside the callbacks using the function
  
      png_get_progressive_ptr(png_ptr);
    
        which will return a void pointer, which you have
        to cast appropriately.
    */
  png_set_progressive_read_fn(png_ptr, (void *)this , info_callback, row_callback, end_callback);
  
  return 0;
}

/* A code fragment that you call as you receive blocks of data */
int nglImagePNGCodec::process_data(png_bytep buffer, png_uint_32 length) 
{
  
  if (setjmp(png_jmpbuf( png_ptr ))) 
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    return -1;
  }
  

  /* This one's new also.  Simply give it a chunk
    of data from the file stream (in order, of
    course).  On machines with segmented memory
    models machines, don't give it any more than
    64K.  The library seems to run fine with sizes
    of 4K. Although you can give it much less if
    necessary (I assume you can give it chunks of
    1 byte, I haven't tried less then 256 bytes
    yet).  When this function returns, you may
    want to display any rows that were generated
    in the row callback if you don't already do
    so there.
    */
  png_process_data(png_ptr, info_ptr, buffer, length);
  return 0;
}

void nglImagePNGCodec::InfoCallback(png_structp png_ptr, png_infop info_ptr) 
{
  nglImageInfo imginfo;

 short int color_type = png_get_color_type(png_ptr, info_ptr);

  // expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
  // transparency chunks to full alpha channel; strip 16-bit-per-sample
  // images to 8 bits per sample; and convert grayscale to RGB[A]

  if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_expand(png_ptr);
  if (color_type == PNG_COLOR_TYPE_GRAY && png_get_bit_depth( png_ptr, info_ptr ) < 8)
      png_set_expand(png_ptr);
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      png_set_expand(png_ptr);
//      if (GetBitDepth() == 16)
//          png_set_strip_16(mpPngData);
//  if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
//      png_set_gray_to_rgb(png_ptr);

  if (png_get_interlace_type(png_ptr,info_ptr) != PNG_INTERLACE_NONE)
    png_set_interlace_handling(png_ptr);

  png_read_update_info(png_ptr, info_ptr);
  
  imginfo.mWidth = png_get_image_width( png_ptr, info_ptr );
  imginfo.mHeight = png_get_image_height( png_ptr, info_ptr );
  imginfo.mBufferFormat = eImageFormatRaw;
  if (png_get_channels( png_ptr, info_ptr )==1)
    imginfo.mPixelFormat = eImagePixelLum;
  else if (png_get_channels( png_ptr, info_ptr )==2)
    imginfo.mPixelFormat = eImagePixelLumA;
  else if (png_get_channels( png_ptr, info_ptr )==3)
    imginfo.mPixelFormat = eImagePixelRGB;
  else if (png_get_channels( png_ptr, info_ptr )==4)
    imginfo.mPixelFormat = eImagePixelRGBA;

  imginfo.mBitDepth = png_get_bit_depth( png_ptr, info_ptr ) * png_get_channels( png_ptr, info_ptr );
  imginfo.mBytesPerPixel = imginfo.mBitDepth / 8;
  imginfo.mBytesPerLine = imginfo.mBytesPerPixel * imginfo.mWidth;
  
  if (!SendInfo(imginfo))
  {
    mStop = true;
  }

  mpRowPointers = (png_byte**) malloc(imginfo.mHeight * sizeof(png_byte*));

  uint i;
  for(i = 0; i < imginfo.mHeight; i++)
    mpRowPointers[i] = (png_byte*)mpImage->GetBuffer() 
                     + (imginfo.mHeight - i - 1) * imginfo.mBytesPerPixel * imginfo.mWidth;

  png_start_read_image(png_ptr);
}

bool nglImagePNGCodec::Feed(nglIStream* pIStream)
{
  char buffer[4096];
  nglFileSize size = MIN(pIStream->Available(), 4096);
  
  while (size > 0 && !mStop)
  {
    nglFileSize r = pIStream->Read(buffer,(size_t)size,1);
    process_data((unsigned char*)buffer, (size_t)r);
    size = MIN(pIStream->Available(), 4096);
  }
  return !mStop;
}

#ifdef _CARBON_
void nglWritePNG( png_structp png_ptr, png_bytep data, long unsigned int length )
#else
void nglWritePNG( png_structp png_ptr, png_bytep data, png_size_t length )
#endif
{
  ((nglOStream*)(png_get_io_ptr(png_ptr)))->Write( (png_byte *)data, (long int) length, 1 );
}

void nglFlushPNG( png_structp png_ptr )
{
}


bool nglImagePNGCodec::Save(nglOStream* pOStream)
{
  png_structp png_ptr = png_create_write_struct
                          ( PNG_LIBPNG_VER_STRING, 
                            (png_voidp)NULL, 
                            NULL, 
                            NULL);
  if (!png_ptr)
    return false;
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) 
  {
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    return false;
  }

  if (setjmp(png_ptr->jmpbuf)) 
  {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }

  png_set_write_fn( png_ptr, (void*)pOStream, nglWritePNG, nglFlushPNG);

/* set the zlib compression level */
  png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
  
  /* set other zlib parameters */
  nglImageInfo Info;
  mpImage->GetInfo(Info);

  png_set_IHDR( png_ptr, info_ptr, Info.mWidth, Info.mHeight, Info.mBitDepth/Info.mBytesPerPixel, 
                Info.mBytesPerPixel == 4 ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB, 
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT 
              );

  uint i;
  png_byte **row_pointers;
  row_pointers = (png_byte**) malloc(Info.mHeight * sizeof(png_byte*));

  for(i = 0; i < Info.mHeight; i++)
    row_pointers[i] = (unsigned char*)(Info.mpBuffer + i * Info.mBytesPerLine);

  png_write_info(png_ptr, info_ptr);
  
  
  /* set up the transformations:  for now, just pack low-bit-depth pixels
  * into bytes (one, two or four pixels per byte) */
  
  png_set_packing(png_ptr);
  
  png_write_image (png_ptr, row_pointers);
  png_write_end(png_ptr, info_ptr); // Write the end chunck of the image.
  png_destroy_write_struct( &png_ptr, &info_ptr );
  free(row_pointers);
  return true;
}

float nglImagePNGCodec::GetCompletion()
{
  return 0;
}
