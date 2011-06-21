  #include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


#include "nui.h"
#include "nuiInit.h"

#include "nuiAndroidBridge.h"

#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

#include "nuiWaveReader.h"

#include "mpg123.h"

nuiAndroidBridge* gpBridge = NULL;

nglIStream* gpStream = NULL;
nuiWaveReader* gpReader = NULL;
mpg123_handle* gpDecoder = NULL;

// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
  static nglTime last;
  nglTime now;
  double elapsed = now.GetValue() - last.GetValue();
  last = now;
  LOGI("audio callback  elapsed %lf (%lf samples)", elapsed, elapsed * 44100);
  
  int sampleframes = 1024;
  int channels = 1;
  int bytes = sampleframes * channels * sizeof(short);
  uint8* pBuffer = new uint8[bytes];
  
  if (gpReader)
  {
    // WAV
    gpReader->ReadIN((void*)pBuffer, sampleframes, eSampleInt16);
  }
  else if (gpStream)
  {
    // MP3
    
  }
  else
  {
    // SYNTH
    float freq = 440;
    int period = ToBelow(44100.f / freq);
    int semiperiod = period / 2;
    static int signalDone = 0;
    static float sign = 1;
    int frames = sampleframes;
    int done = 0;
    
    while (frames)
    {
      int todo = MIN(frames, semiperiod - signalDone);
      for (int i = 0; i < todo; i++)
      {
        short val = 20000 * sign;
        for (int c = 0; c < channels; c++)
        {
          pBuffer[(i + done) * channels + c] = val;
        }
      }
      
      frames -= todo;
      done += todo;
      signalDone += todo;
      if (signalDone >= semiperiod)
      {
        sign *= -1;
        signalDone = 0;
      }
    }
  }
  
  
  SLresult result;
  // enqueue another buffer
  result = (*bq)->Enqueue(bq, pBuffer, bytes);
  if (result == SL_RESULT_SUCCESS)
    LOGI("Enqueue OK");
  else
    LOGI("Enqueue ERROR");
  
}


/**
  * Our saved state data.
  */
struct saved_state 
{
  float angle;
  int32_t x;
  int32_t y;
};

/**
  * Shared state for our app.
  */
struct engine
{
  struct android_app* app;
  
  ASensorManager* sensorManager;
  const ASensor* accelerometerSensor;
  ASensorEventQueue* sensorEventQueue;
  
  int animating;
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  int32_t width;
  int32_t height;
  struct saved_state state;
};

/**
  * Initialize an EGL context for the current display.
  */
static int engine_init_display(struct engine* engine) 
{
  // initialize OpenGL ES and EGL
  
  /*
    * Here specify the attributes of the desired configuration.
    * Below, we select an EGLConfig with at least 8 bits per color
    * component compatible with on-screen windows
    */
  const EGLint attribs[] = 
  {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_NONE
  };
  EGLint w, h, dummy, format;
  EGLint numConfigs;
  EGLConfig config;
  EGLSurface surface;
  EGLContext context;
  
  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  
  eglInitialize(display, 0, 0);
  
  /* Here, the application chooses the configuration it desires. In this
    * sample, we have a very simplified selection process, where we pick
    * the first EGLConfig that matches our criteria */
  eglChooseConfig(display, attribs, &config, 1, &numConfigs);
  
  /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
    * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
    * As soon as we picked a EGLConfig, we can safely reconfigure the
    * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
  
  ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
  
  surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
  context = eglCreateContext(display, config, NULL, NULL);
  
  if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
  {
    LOGI("Unable to eglMakeCurrent");
    return -1;
  }
  
  eglQuerySurface(display, surface, EGL_WIDTH, &w);
  eglQuerySurface(display, surface, EGL_HEIGHT, &h);
  
  engine->display = display;
  engine->context = context;
  engine->surface = surface;
  engine->width = w;
  engine->height = h;
  engine->state.angle = 0;
  
  // Initialize GL state.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);
  
  return 0;
}

/**
  * Just the current frame in the display.
  */
static void engine_draw_frame(struct engine* engine) 
{ 
  if (engine->display == NULL) 
  {
    // No display.
    return;
  }
  
  // Just fill the screen with a color.
  glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
               ((float)engine->state.y)/engine->height, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  
  
  LOGI("android bridge: display");
  gpBridge->Display();
  LOGI("android bridge: display OK");
  
  eglSwapBuffers(engine->display, engine->surface);
}

/**
  * Tear down the EGL context currently associated with the display.
  */
static void engine_term_display(struct engine* engine) 
{
  if (engine->display != EGL_NO_DISPLAY) 
  {
    eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (engine->context != EGL_NO_CONTEXT) 
    {
      eglDestroyContext(engine->display, engine->context);
    }
    if (engine->surface != EGL_NO_SURFACE) 
    {
      eglDestroySurface(engine->display, engine->surface);
    }
    eglTerminate(engine->display);
  }
  engine->animating = 0;
  engine->display = EGL_NO_DISPLAY;
  engine->context = EGL_NO_CONTEXT;
  engine->surface = EGL_NO_SURFACE;
  
  
  
  // Exit the application
  // First destroy the NUI bridge / widget tree:
  LOGI("delete android bridge");
  delete gpBridge;
  LOGI("delete android bridge OK");
  
  // Shutdown the basic NUI services:
  LOGI("nuiUninit");
  nuiUninit();
  LOGI("nuiUninit OK");
}

/**
  * Process the next input event.
  */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
  struct engine* engine = (struct engine*)app->userData;
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
  {
    int x = AMotionEvent_getX(event, 0);
    int y = AMotionEvent_getY(event, 0);
    if ((AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction( event )) == AMOTION_EVENT_ACTION_DOWN)
    {
      nuiAndroidBridge::androidMouse(0, 0, x, y);
    }
    else if ((AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction( event )) == AMOTION_EVENT_ACTION_UP)
    {
      nuiAndroidBridge::androidMouse(0, 1, x, y);      
    }
    else if ((AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction( event )) == AMOTION_EVENT_ACTION_MOVE)
    {
      nuiAndroidBridge::androidMotion(x, y);
    }

    
    engine->animating = 1;
    engine->state.x = AMotionEvent_getX(event, 0);
    engine->state.y = AMotionEvent_getY(event, 0);
    return 1;
  }
  return 0;
}

/**
  * Process the next main command.
  */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
  struct engine* engine = (struct engine*)app->userData;
  switch (cmd) {
    case APP_CMD_SAVE_STATE:
      // The system has asked us to save our current state.  Do so.
      engine->app->savedState = malloc(sizeof(struct saved_state));
      *((struct saved_state*)engine->app->savedState) = engine->state;
      engine->app->savedStateSize = sizeof(struct saved_state);
      break;
    case APP_CMD_INIT_WINDOW:
      // The window is being shown, get it ready.
      if (engine->app->window != NULL) 
      {
        engine_init_display(engine);

        nuiAndroidBridge::androidResize(ANativeWindow_getWidth(app->window), ANativeWindow_getHeight(app->window));
        nuiButton* pButton = new nuiButton("Prout!");
//        pButton->SetPosition(nuiCenter);
        gpBridge->AddChild(pButton);

        engine_draw_frame(engine);
        engine->animating = 1;
        
        
        
        
        // AUDIO tests
        
//        nglPath path("/data/mat/rock.wav");
//        gpStream = path.OpenRead();
//        if (gpStream)
//        {
//          gpReader = new nuiWaveReader(*gpStream);    
//          nuiSampleInfo info;
//          gpReader->GetInfo(info);
//          LOGI("bits per sample: %d", info.GetBitsPerSample());
//        }
//        else
//        {
//          LOGI("stream not open");
//        }

        
        nglPath path("/data/mat/rock.mp3");
        gpStream = path.OpenRead();
        if (!gpStream)
        {
          LOGI("mp3 stream not open");
        }

        //
        LOGI("init mpg123");
        int mpgRes = mpg123_init();
        if (mpgRes == MPG123_OK)
          LOGI("init mpg123 OK");
        else
          LOGI("init mpg123 error %d", mpgRes);
        
        LOGI("new decoder");
        gpDecoder = mpg123_new(NULL, &mpgRes);
        if (!gpDecoder)
          LOGI("new decoder error %d", mpgRes);
        else 
          LOGI("new decoder OK");
        
        LOGI("open decoder");
        mpgRes = mpg123_open_feed(gpDecoder);
        if (mpgRes == MPG123_OK)
          LOGI("open decoder OK");
        else
          LOGI("open decoder error %d", mpgRes);
        
        
        SLObjectItf engineObject = NULL;
        SLEngineItf engineEngine;
        SLObjectItf outputMixObject = NULL;
        SLObjectItf bqPlayerObject = NULL;
        SLPlayItf bqPlayerPlay;
        SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
        SLresult result;
        
        // create engine
        result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
        if (result == SL_RESULT_SUCCESS)
          LOGI("create audio engine OK");
        else if (result == SL_RESULT_PARAMETER_INVALID)
          LOGI("create audio engine ERROR: parameter invalid");
        else if (result == SL_RESULT_MEMORY_FAILURE)
          LOGI("create audio engine ERROR: memory failure");
        else if (result == SL_RESULT_FEATURE_UNSUPPORTED)
          LOGI("create audio engine ERROR: feature unsupported");
        else if (result == SL_RESULT_RESOURCE_ERROR)
          LOGI("create audio engine ERROR: resource error");
        else
          LOGI("create audio engine ERROR: unknown error");
        
        
        result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
        if (result == SL_RESULT_SUCCESS)
          LOGI("realize audio engine OK");
        else
          LOGI("realize audio engine ERROR");
        
        
        result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
        if (result == SL_RESULT_SUCCESS)
          LOGI("get engine interface OK");
        else
          LOGI("get engine interface ERROR");
        
        
        
        result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL);
        if (result == SL_RESULT_SUCCESS)
          LOGI("create output mix OK");
        else
          LOGI("create output mix ERROR");
        
        result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
        if (result == SL_RESULT_SUCCESS)
          LOGI("realize output mix OK");
        else
          LOGI("realize output mix ERROR");
        
        // configure audio source
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
        SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16, SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
        SLDataSource audioSrc = {&loc_bufq, &format_pcm};
        
        // configure audio sink
        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
        SLDataSink audioSnk = {&loc_outmix, NULL};
        
        // create audio player
        const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
        const SLboolean req[1] = {SL_BOOLEAN_TRUE};
        result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 1, ids, req);
        if (result == SL_RESULT_SUCCESS)
          LOGI("create player OK");
        else
          LOGI("create player ERROR");
        
        result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
        if (result == SL_RESULT_SUCCESS)
          LOGI("realize player OK");
        else
          LOGI("realize player ERROR");
        
        // get the play interface
        result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
        if (result == SL_RESULT_SUCCESS)
          LOGI("get play interface OK");
        else
          LOGI("get play interface ERROR");
        
        // get the buffer queue interface
        result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
        if (result == SL_RESULT_SUCCESS)
          LOGI("get buffer queue interface OK");
        else
          LOGI("get buffer queue interface ERROR");
        
        // register callback on the buffer queue
        result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
        if (result == SL_RESULT_SUCCESS)
          LOGI("register callback OK");
        else
          LOGI("register callback ERROR");
        
        // set the player's state to playing
        result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
        if (result == SL_RESULT_SUCCESS)
          LOGI("play OK");
        else
          LOGI("play ERROR");
//        
//        
//        LOGI("first fill");
//        bqPlayerCallback(bqPlayerBufferQueue, NULL);
//        LOGI("first fill done");

//        SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *)"file://data/mat/rock.mp3"};
//        SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
//        SLDataSource audioSrc = {&loc_uri, &format_mime};
//        
//        // configure audio sink
//        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
//        SLDataSink audioSnk = {&loc_outmix, NULL};
//        
//        // create audio player
//        const SLInterfaceID ids[1] = {SL_IID_SEEK};
//        const SLboolean req[1] = {SL_BOOLEAN_TRUE};
//        result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 1, ids, req);
//        // note that an invalid URI is not detected here, but during prepare/prefetch on Android,
//        // or possibly during Realize on other platforms
//        if (result == SL_RESULT_SUCCESS)
//          LOGI("audio player OK");
//        else
//          LOGI("audio player ERROR");
//        
//        // realize the player
//        result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
//        
//        // get the play interface
//        result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
//        if (result == SL_RESULT_SUCCESS)
//          LOGI("get play interface OK");
//        else
//          LOGI("get play interface ERROR");
//        
//        // set the player's state to playing
//        result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
//        if (result == SL_RESULT_SUCCESS)
//          LOGI("play OK");
//        else
//          LOGI("play ERROR");
        
        //
        
        
        
      }
      break;
    case APP_CMD_TERM_WINDOW:
      // The window is being hidden or closed, clean it up.
      engine_term_display(engine);
      break;
    case APP_CMD_GAINED_FOCUS:
      // When our app gains focus, we start monitoring the accelerometer.
      if (engine->accelerometerSensor != NULL) 
      {
        ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                       engine->accelerometerSensor);
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                       engine->accelerometerSensor, (1000L/60)*1000);
      }
      break;
    case APP_CMD_LOST_FOCUS:
      // When our app loses focus, we stop monitoring the accelerometer.
      // This is to avoid consuming battery while not being used.
      if (engine->accelerometerSensor != NULL) 
      {
        ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                        engine->accelerometerSensor);
      }
      // Also stop animating.
      engine->animating = 1;
      engine_draw_frame(engine);
      break;
  }
}

/**
  * This is the main entry point of a native application that is using
  * android_native_app_glue.  It runs in its own thread, with its own
  * event loop for receiving input events and doing other things.
  */
void android_main(struct android_app* state) 
{ 
  LOGI("nuiInit");
  nuiInit(NULL);
  LOGI("nuiInit OK");
  
  // Create the NUI bridge which also serves as the main window/widget tree:
  LOGI("create Android Bridge");
  gpBridge = new nuiAndroidBridge();
  LOGI("create Android Bridge OK");
  
  struct engine engine;
  
  // Make sure glue isn't stripped.
  app_dummy();
  
  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;
  
  // Prepare to monitor accelerometer
  engine.sensorManager = ASensorManager_getInstance();
  engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
                                                               ASENSOR_TYPE_ACCELEROMETER);
  engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
                                                            state->looper, LOOPER_ID_USER, NULL, NULL);
  
  if (state->savedState != NULL) 
  {
    // We are starting with a previous saved state; restore from it.
    engine.state = *(struct saved_state*)state->savedState;
  }
  
  // loop waiting for stuff to do.

  
  while (1) 
  {
    // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;
    
    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                                  (void**)&source)) >= 0) 
    {
      
      // Process this event.
      if (source != NULL) 
      {
        source->process(state, source);
      }
      
      // If a sensor has data, process it now.
      if (ident == LOOPER_ID_USER) 
      {
        if (engine.accelerometerSensor != NULL) 
        {
          ASensorEvent event;
          while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                                             &event, 1) > 0) 
          {
//            LOGI("accelerometer: x=%f y=%f z=%f",
//                 event.acceleration.x, event.acceleration.y,
//                 event.acceleration.z);
          }
        }
      }
      
      // Check if we are exiting.
      if (state->destroyRequested != 0) 
      {
        engine_term_display(&engine);
        return;
      }
    }
    
    if (engine.animating) 
    {
      // Done with events; draw next animation frame.
      engine.state.angle += .01f;
      if (engine.state.angle > 1) 
      {
        engine.state.angle = 0;
      }
      
      // Drawing is throttled to the screen update rate, so there
      // is no need to do timing here.
      engine_draw_frame(&engine);
    }
  }
}