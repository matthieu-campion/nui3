/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

// JPEG CODEC
// based on libjpeg
// implemented by jerome blondon (jerome.blondon@wanadoo.fr)
// !TODO : better load method

#include "nui.h"

#ifdef HAVE_LIBJPEG

#ifdef _WIN32_ // Because of name collision in between the _WIN32_ platform SDK and jpeglib
#define XMD_H
#undef FAR
#endif // _WIN32_

extern "C"
{
#include "jpeglib.h"
}


#include "nglBitmapTools.h"
#include <setjmp.h>

struct my_error_mgr 
{
  struct jpeg_error_mgr pub;  /* "public" fields */
  jmp_buf setjmp_buffer;  /* for return to caller */
};

// function prototypes
void jpeg_ostream_dest (j_compress_ptr cinfo, nglOStream * outStream);
void jpeg_istream_src (j_decompress_ptr cinfo, nglIStream* inStream);



class nglImageJPEGCodec : public nglImageCodec
{
public:
  nglImageJPEGCodec();
  virtual ~nglImageJPEGCodec();
  virtual bool Init(nglImage* pImage); 

  virtual bool Probe(nglIStream* pIStream);
  virtual bool Feed(nglIStream* pOStream); 
  virtual bool Save(nglOStream* pOStream); 
  virtual float GetCompletion(); 
private:
  bool ReadHeader(nglIStream* pIStream);
  bool ReadData();
  
  enum jpegState
  {
    jpegError = 0,
    jpegReadHeader,
    jpegReadData
  };
  jpegState mState;
  
  uint mLine;
  uint mLineSize;
  struct jpeg_decompress_struct mCinfo;
  struct my_error_mgr mJerr;
  char* mpBuffer;
private:
};



typedef struct my_error_mgr * my_error_ptr;
void my_error_exit (j_common_ptr cinfo);


nglImageCodec* nglImageJPEGCodecInfo::CreateInstance()
{
  return new nglImageJPEGCodec();
}


nglImageJPEGCodec::nglImageJPEGCodec()
{
  mpBuffer = NULL;
  mLine = 0;
  mState = jpegReadHeader;
}

nglImageJPEGCodec::~nglImageJPEGCodec()
{
  mpBuffer = NULL;
  mLine = 0;
  mState = jpegReadHeader;
}

bool nglImageJPEGCodec::Init(nglImage* pImage)
{
  mpImage = pImage;
  return true;
}

bool nglImageJPEGCodec::Probe(nglIStream* pIStream)
{
  if (pIStream->Available(10)) // A not so robust png detection routine...
  {
    char buffer[10];
    if (!pIStream->Peek(buffer,1,10))
      return false;
    if (buffer[6]=='J' && 
        buffer[7]=='F' && 
        buffer[8]=='I' && 
        buffer[9]=='F')
    {
      return true;
    }
  }
  return false;
}

bool nglImageJPEGCodec::ReadHeader(nglIStream* pIStream)
{

  mCinfo.err = jpeg_std_error(&mJerr.pub);
  mJerr.pub.error_exit = my_error_exit;
  if (setjmp(mJerr.setjmp_buffer)) 
  {
    jpeg_destroy_decompress(&mCinfo);
    return false;
  }
  jpeg_create_decompress(&mCinfo);
  mCinfo.do_block_smoothing = TRUE;
  mCinfo.do_fancy_upsampling = TRUE;
  
  jpeg_istream_src(&mCinfo, pIStream);
  jpeg_read_header(&mCinfo, TRUE);
  jpeg_start_decompress(&mCinfo);

  nglImageInfo info;
  info.mBitDepth = mCinfo.output_components * 8;
  info.mPixelFormat = eImagePixelRGB;
  if (mCinfo.output_components == 1)
    info.mPixelFormat = eImagePixelLum;
  else if (mCinfo.output_components == 4)
    info.mPixelFormat = eImagePixelRGBA;
  info.mBufferFormat = eImageFormatRaw;
  info.mBytesPerPixel = mCinfo.output_components;
  info.mBytesPerLine = info.mBytesPerPixel * mCinfo.output_width;
  info.mHeight = mCinfo.output_height;        
  info.mWidth = mCinfo.output_width;
  mLineSize = info.mBytesPerLine;

  return SendInfo(info);
}

bool nglImageJPEGCodec::ReadData()
{
  unsigned char *buffer;
  int row_stride;    /* physical row width in output buffer */
  nglImageInfo info;
  mpImage->GetInfo(info);

  row_stride = mCinfo.output_width * mCinfo.output_components;
  buffer = new unsigned char[row_stride];
  
  while (mCinfo.output_scanline < mCinfo.output_height)  // !warning! can't work with progressive loading
  {
    jpeg_read_scanlines(&mCinfo, &buffer, 1);
    memcpy((char *)mpBuffer, (const char*)buffer, row_stride);
    mLine++;
    mpBuffer += mLineSize;
  }  
  
  mpBuffer -= mLineSize * (mpImage->GetHeight());
  delete []buffer;
  if(mCinfo.output_scanline >= mCinfo.output_height)
  {
    jpeg_finish_decompress(&mCinfo);
    jpeg_destroy_decompress(&mCinfo); 
  }  
  return true;
}


bool nglImageJPEGCodec::Feed(nglIStream* pIStream)
{
  if(ReadHeader(pIStream))
  {
    mpBuffer = mpImage->GetBuffer(); // + mLineSize*(mpImage->GetHeight()-1);
    ReadData();
    if (!SendData((float)mLine/(float)mpImage->GetHeight()))
      return false;
  }
  return (pIStream->GetState()==eStreamWait) || (pIStream->GetState()==eStreamReady);   
}

bool nglImageJPEGCodec::Save(nglOStream* pOStream)
{
  // this code is taken from example.c in libjpeg
  // with minor modifications
  // the key to save here is that i have defined a
  // new function, jpeg_ostream_dest(), that
  // acts like jpeg_std_dest() but uses nglOStream
  // instead of FILE*

  // first thing to do : convert image into 
  // a 24 bits rgb image
  nglImageInfo Info;
  mpImage->GetInfo(Info);
  char *pConvertedBuffer = new char[Info.mWidth*Info.mHeight*3];
  nglCopyImage(pConvertedBuffer, Info.mWidth, Info.mHeight, 24, mpImage->GetBuffer(), Info.mWidth, Info.mHeight, Info.mBitDepth, true, false);
  unsigned char *image_buffer = (unsigned char *) pConvertedBuffer;

  // then libjpeg can start working..

  int quality = 100; // best quality
  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * (see the second half of this file for an example).  But here we just
   * take the easy way out and use the standard error handler, which will
   * print a message on stderr and call exit() if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  JSAMPROW row_pointer[1];  /* pointer to JSAMPLE row[s] */
  int32 row_stride;  /* physical row width in image buffer */

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
   * step fails.  (Unlikely, but it could happen if you are out of memory.)
   * This routine fills in the contents of struct jerr, and returns jerr's
   * address which we place into the link field in cinfo.
   */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  jpeg_ostream_dest(&cinfo, pOStream);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = Info.mWidth;  /* image width and height, in pixels */
  cinfo.image_height = Info.mHeight;
  cinfo.input_components = 3;  /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB;  /* colorspace of input image */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  row_stride = Info.mWidth * 3;  /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height)
  {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    const int32 index = cinfo.image_height - cinfo.next_scanline - 1;
    row_pointer[0] = & image_buffer[index * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  delete []pConvertedBuffer;
  /* And we're done! */

  return true;
}

float nglImageJPEGCodec::GetCompletion()
{
  return 0;
}


////////////////////////////////////////////////////////////////////////////////////////
// the following code is based on jdatadst.c, modified by jerome blondon in order
// to work with nglOStream
// original comments are not removed
////////////////////////////////////////////////////////////////////////////////////////

/* Expanded data destination object for stdio output */
typedef struct 
{
  struct jpeg_destination_mgr pub; /* public fields */

  nglOStream *outStream;  /* target stream */
  JOCTET * buffer;  /* start of buffer */
} ngl_destination_mgr;

typedef ngl_destination_mgr * ngl_dest_ptr;

#define OUTPUT_BUF_SIZE  4096  /* choose an efficiently fwrite'able size */


/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */

void init_destination (j_compress_ptr cinfo)
{
  ngl_dest_ptr dest = (ngl_dest_ptr) cinfo->dest;

  /* Allocate the output buffer --- it will be released when done with image */
  dest->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
       OUTPUT_BUF_SIZE * sizeof(JOCTET));

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}


/*
 * Empty the output buffer --- called whenever buffer fills up.
 *
 * In typical applications, this should write the entire output buffer
 * (ignoring the current state of next_output_byte & free_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been dumped.
 *
 * In applications that need to be able to suspend compression due to output
 * overrun, a FALSE return indicates that the buffer cannot be emptied now.
 * In this situation, the compressor will return to its caller (possibly with
 * an indication that it has not accepted all the supplied scanlines).  The
 * application should resume compression after it has made more room in the
 * output buffer.  Note that there are substantial restrictions on the use of
 * suspension --- see the documentation.
 *
 * When suspending, the compressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_output_byte & free_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point will be regenerated after resumption, so do not
 * write it out when emptying the buffer externally.
 */

boolean empty_output_buffer (j_compress_ptr cinfo)
{
  ngl_dest_ptr dest = (ngl_dest_ptr) cinfo->dest;

  if ( dest->outStream->Write(dest->buffer, OUTPUT_BUF_SIZE, 1) !=
      (size_t) OUTPUT_BUF_SIZE)
      {
        NGL_LOG(_T("image"), NGL_LOG_ERROR, _T("error: JERR_FILE_WRITE"));
        return false;
      }

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

  return TRUE;
}


/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

void term_destination (j_compress_ptr cinfo)
{
  ngl_dest_ptr dest = (ngl_dest_ptr) cinfo->dest;
  size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

  /* Write any data remaining in the buffer */
  if (datacount > 0) {
    if ((size_t)(dest->outStream->Write(dest->buffer, datacount, 1)) != datacount)
    {
      NGL_LOG(_T("image"), NGL_LOG_ERROR, _T("error: JERR_FILE_WRITE"));
    }
  }
}


/*
 * Prepare for output to an ngl stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing compression.
 */

void jpeg_ostream_dest (j_compress_ptr cinfo, nglOStream * outStream)
{
  ngl_dest_ptr dest;

  /* The destination object is made permanent so that multiple JPEG images
   * can be written to the same file without re-executing jpeg_stdio_dest.
   * This makes it dangerous to use this manager and a different destination
   * manager serially with the same JPEG object, because their private object
   * sizes may be different.  Caveat programmer.
   */
  if (cinfo->dest == NULL) /* first time for this JPEG object? */
  {
    cinfo->dest = (struct jpeg_destination_mgr *) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(ngl_destination_mgr));
  }

  dest = (ngl_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->outStream = outStream;
}

////////////////////////////////////////////////////////////////////////////////////////
// end code based on jdatadst.c
////////////////////////////////////////////////////////////////////////////////////////





/*
 * Here's the routine that will replace the standard error_exit method:
 */

void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  
  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);
  
  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

///////////////////// SOURCE MANAGER:
/* Expanded data source object for stdio input */

typedef struct 
{
  struct jpeg_source_mgr pub;  /* public fields */
  
  nglIStream * inStream;    /* source stream */
  JOCTET * buffer;    /* start of buffer */
  boolean start_of_file;  /* have we gotten any data yet? */
} ngl_source_mgr;

typedef ngl_source_mgr * ngl_src_ptr;

#define INPUT_BUF_SIZE  4096  /* choose an efficiently fread'able size */


/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

void init_source (j_decompress_ptr cinfo)
{
  ngl_src_ptr src = (ngl_src_ptr) cinfo->src;
  
  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->start_of_file = TRUE;
}


/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

boolean fill_input_buffer (j_decompress_ptr cinfo)
{
  ngl_src_ptr src = (ngl_src_ptr) cinfo->src;
  size_t nbytes;
  
  nbytes = src->inStream->Read(src->buffer, INPUT_BUF_SIZE, 1);
  
  if (nbytes <= 0) 
  {
    if (src->start_of_file)  /* Treat empty input file as fatal error */
      NGL_LOG(_T("image"), NGL_LOG_ERROR, _T("error: JERR_INPUT_EMPTY"));
    NGL_LOG(_T("image"), NGL_LOG_WARNING, _T("warning: JWRN_JPEG_EOF"));
    /* Insert a fake EOI marker */
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  }
  
  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;
  src->start_of_file = FALSE;
  
  return TRUE;
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  ngl_src_ptr src = (ngl_src_ptr) cinfo->src;
  
  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) 
  {
    while (num_bytes > (long) src->pub.bytes_in_buffer) 
    {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */
 
void term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}


/*
 * Prepare for input from a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing decompression.
 */

void jpeg_istream_src (j_decompress_ptr cinfo, nglIStream* inStream)
{
  ngl_src_ptr src;
  
  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) 
  {  /* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
          sizeof(ngl_source_mgr));
    src = (ngl_src_ptr) cinfo->src;
    src->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
          INPUT_BUF_SIZE * sizeof(JOCTET));
  }
  
  src = (ngl_src_ptr) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->inStream = inStream;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}

#endif // HAVE_LIBJPEG
