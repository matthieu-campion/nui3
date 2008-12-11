/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "MainWindow.h"

#include "nuiLabel.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "nuiAudioDevice.h"

#include "FFTReal.h"

const double _log10 = log(10.0);
const double log_5 = log(0.5);
const float logf10 = logf(10.0f);

const double db_mul_const = 20.0 / _log10;
const float db_mul_constf = 20.0f / logf10;

inline double GainToDB(double Gain)
{
  if (Gain == 0.0)
    return (-144.0);
  else
  {
    double  dB;
    
    dB = log(Gain) * db_mul_const;
    
    if (dB < -144.0)
      dB = -144.0;
    return (dB);    // convert factor to dB
  }
}

inline double DBToGain(double dB)
{
  return pow(10.0, dB / 20.0);
}

inline float GainToDB(float Gain)
{
  if (Gain == 0.0f)
    return (-144.0f);
  else
  {
    float  dB;
    
    dB = logf(Gain) * db_mul_constf;
    
    if (dB < -144.0f)
      dB = -144.0f;
    return (dB);    // convert factor to dB
  }
}

inline float DBToGain(float dB)
{
  return powf(10.0f, dB / 20.0f);
}


class AudioMeter : public nuiWidget
{
public:
  enum Mode
  {
    Peak,
    RMS
  };
  
  AudioMeter(const std::vector<std::vector<float> >& rData, Mode mode)
  : mrData(rData), mMode(mode), mLast(0.0f), mPeak(0.0f)
  {
    StartAutoDraw(30);
  }

  virtual ~AudioMeter()
  {
    //...
  }
  
  virtual nuiRect CalcIdealSize()
  {
    return nuiRect(0,0, 32, 256);
  }
  
  bool Draw(nuiDrawContext* pContext)
  {
    float peak = 0;
    float rms = 0;
    
    for (uint32 j = 0; j < mrData.size(); j++)
    {
      for (uint32 i = 0; i < mrData[0].size(); i++)
      {
        float value = mrData[j][i];
        peak = MAX(value, peak);
        peak = MAX(-value, peak);
        rms += value * value;
      }
    }
    rms /= (float)(mrData.size() * mrData[0].size());
    rms = sqrt(rms);
    peak = peak / sqrt(2.0f);

    float value = 0;
    if (mMode == Peak)
      value = peak;
    else if (mMode == RMS)
      value = rms;

    nuiSize h = GetRect().GetHeight();
    nuiSize w = GetRect().GetWidth();

    pContext->EnableBlending(true);
    pContext->SetBlendFunc(nuiBlendTransp);
    {
      nuiColor col(64, 64, 255, 128);
      if (mMode == RMS)
        col = nuiColor(255, 64, 64, 128);

      pContext->SetStrokeColor(col);
      pContext->SetFillColor(col);
      
      nuiSize hh = h - h * value;
      nuiRect r(0.0f, hh, w, h);
      pContext->DrawRect(r, eStrokeAndFillShape);
    }

    if (mLast < value)
    {
      mPeak = MAX(mPeak, value - mLast);
      mPeak = MIN(1.0f, mPeak);
    }

    {
      nuiColor col = nuiColor(64, 255, 64, 128);
      pContext->SetStrokeColor(col);
      pContext->SetFillColor(col);
      nuiSize hh = h - h * mPeak;
      nuiRect r(w / 4, hh, w / 2, h);
      pContext->DrawRect(r, eStrokeAndFillShape);
    }
    
    mLast = value;
    mPeak *= 0.95f;
    
    return true;
  }
  
private:
  const std::vector<std::vector<float> >& mrData;
  Mode mMode;
  float mLast;
  float mPeak;
};


class Oscilloscop : public nuiWidget
{
public:
  Oscilloscop(const std::vector<std::vector<float> >& rData)
  : mrData(rData)
  {
    StartAutoDraw(30);
  }

  virtual ~Oscilloscop()
  {
    //...
  }
  
  virtual nuiRect CalcIdealSize()
  {
    return nuiRect(0,0, mrData.size(), 128);
  }
  
  bool Draw(nuiDrawContext* pContext)
  {
    uint32 s1 = mrData[0].size();
    uint32 s2 = GetRect().GetWidth();
    uint32 count = MIN(s1, s2);
    nuiSize hi = GetRect().GetHeight();
    nuiSize mid = hi / 2;
    for (uint32 j = 0; j < mrData.size(); j++)
    {
      nglString colors[] =
      {
        _T("red"),
        _T("green")
      };
      uint32 colorcount = 2;
      
      nuiRenderArray array(GL_LINE_STRIP);
      nuiColor Color(colors[j % colorcount]);
      array.SetColor(Color);
      for (uint32 i = 0; i < count; i++)
      {
        float value = mrData[j][i];
        array.SetVertex(i, mid + hi * value);
        array.PushVertex();
      }
      
      pContext->DrawArray(array);
    }
    return true;
  }
  
private:
  const std::vector<std::vector<float> >& mrData;
};

class Spectrum : public nuiWidget
{
public:
  Spectrum(const std::vector<std::vector<float> >& rData)
  : mrData(rData), mFFT(rData[0].size())
  {
    StartAutoDraw(30);
    CreateHanningWindow();
//    CreateFlatWindow();

    mData.resize(mrData.size());
    for (uint i = 0; i < mrData.size(); i++)
      mData[i].resize(mrData[i].size());
    
    mMaxDB = 60;
  }
  
  virtual ~Spectrum()
  {
    //...
  }
  
  virtual nuiRect CalcIdealSize()
  {
    return nuiRect(0,0, mrData.size(), 128);
  }
  
  bool Draw(nuiDrawContext* pContext)
  {
    uint32 s1 = mrData[0].size();
    uint32 s2 = GetRect().GetWidth();
    float incr = (float)MIN(s1, s2) / (s1 / 2);
    nuiSize hi = GetRect().GetHeight();
    std::vector<float> fft;

    ApplyWindow(); // this will copy mrData in mData and apply te windowing at the same time
    
    for (uint32 j = 0; j < mrData.size(); j++)
    {
      fft.resize(mData[j].size());
      mFFT.do_fft(&fft[0], &mData[j][0]);
      nglString colors[] =
      {
        _T("red"),
        _T("green")
      };
      uint32 colorcount = 2;
      
      nuiRenderArray array(GL_LINE_STRIP);
      nuiColor Color(colors[j % colorcount]);
      array.SetColor(Color);
      
      for (uint32 i = 0; i < s1 / 2; i++)
      {
        float re = fft[i];
        float im = fft[i + s1/2];
        float value = sqrt(re * re + im * im) ;
        value = GainToDB(value);
        value = (value + mMaxDB) / mMaxDB;
        float x = incr * i;
        array.SetVertex(x, hi - hi * value);
        array.PushVertex();
      }

      pContext->DrawArray(array);
    }
    return true;
  }

private:
  const std::vector<std::vector<float> >& mrData;
  std::vector<std::vector<float> > mData;
  std::vector<float> mWindow;
  FFTReal mFFT;
  float mMaxDB;

  void CreateHanningWindow()
  {
    long n = mrData[0].size();
    mWindow.resize(n);
    float fact = 2.0f * (float)M_PI / (float)n;

    for (long k = 0; k < n; k++)
      mWindow[k] = (.5f - .5f * cosf(k*fact));
  }

  void CreateFlatWindow()
  {
    long n = mrData[0].size();
    mWindow.resize(n);

    for (long k = 0; k < n; k++)
      mWindow[k] = 1.0f;
  }
    
  void ApplyWindow()
  {
    for (uint32 i = 0; i < mrData.size(); i++)
    {
      for (uint32 j = 0; j < mrData[i].size(); j++)
      {
        mData[i][j] = mrData[i][j] * mWindow[j];
      }
    }
  }
};


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(_T("../data"))), mSink(this), mFilter(44100, 1)
{
  SetDebugMode(true);
  mpInAudioDevice = NULL;
  mpOutAudioDevice = NULL;
  mpRingBuffer = NULL;
  mFilter.SetQ(0);
  mFilter.SetFreq(4000);
  mFilter.SetType(BiQuad::LowPass);
  //mFilter.SetType(BiQuad::HighPass);
  //mFilter.SetType(BiQuad::Peak);
}

MainWindow::~MainWindow()
{
  delete mpInAudioDevice;
  delete mpOutAudioDevice;
  delete mpRingBuffer;
}

void MainWindow::OnCreation()
{
  mpInAudioDevice = nuiAudioDeviceManager::Get().GetDefaultInputDevice();
  mpOutAudioDevice = nuiAudioDeviceManager::Get().GetDefaultOutputDevice();
  
  NGL_OUT(_T("Default input : %ls\n"), mpInAudioDevice->GetName().GetChars());
  NGL_OUT(_T("Default output: %ls\n"), mpOutAudioDevice->GetName().GetChars());
  
  std::vector<uint32> InputChannels;
  std::vector<uint32> OutputChannels;
  
  double SampleRate = 44100;
  uint32 BufferSize = 1024;

  mpRingBuffer = new nglRingBuffer(BufferSize * 4, sizeof(float), 1);
  mInput.resize(1);
  mOutput.resize(2);
  
  uint i;
  for (i = 0; i < 1; i++)
    mInput[i].resize(BufferSize);
  for (i = 0; i < 2; i++)
    mOutput[i].resize(BufferSize);
  
  
  InputChannels.push_back(0);
  bool res = mpInAudioDevice->Open(InputChannels, OutputChannels, SampleRate, BufferSize, nuiMakeDelegate(this, &MainWindow::ProcessAudioInput));
  InputChannels.clear();
  OutputChannels.push_back(0);
  OutputChannels.push_back(1);
  //res = mpOutAudioDevice->Open(InputChannels, OutputChannels, SampleRate, BufferSize, nuiMakeDelegate(this, &MainWindow::ProcessAudioOutput));
    
  nuiVBox* pVBox = new nuiVBox();
  AddChild(pVBox);
  pVBox->SetExpand(nuiExpandShrinkAndGrow);

  nuiHBox* pHBox1 = new nuiHBox();
  pHBox1->SetExpand(nuiExpandShrinkAndGrow);
  pVBox->AddCell(pHBox1);
  pVBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  nuiHBox* pHBox2 = new nuiHBox();
  pHBox2->SetExpand(nuiExpandShrinkAndGrow);
  pVBox->AddCell(pHBox2);
  pVBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  
  Oscilloscop* pInOscilloscop = new Oscilloscop(mInput);
  //AddChild(pInOscilloscop);
  //Oscilloscop* pOutOscilloscop = new Oscilloscop(mOutput);
  
  pHBox1->AddCell(pInOscilloscop);
  pHBox1->SetCellExpand(0, nuiExpandShrinkAndGrow);
  //pHBox->AddCell(pOutOscilloscop);

  Spectrum* pInSpectrum = new Spectrum(mInput);
  pHBox2->AddCell(pInSpectrum);
  pHBox2->SetCellExpand(0, nuiExpandShrinkAndGrow);

  nuiHBox* pMetters = new nuiHBox();
  pMetters->SetExpand(nuiExpandShrinkAndGrow);
  pMetters->SetPosition(nuiFill);
  AddChild(pMetters);
  
  AudioMeter* pPeakMeter = new AudioMeter(mInput, AudioMeter::Peak);
  pMetters->AddCell(pPeakMeter);

  AudioMeter* pRMSMeter = new AudioMeter(mInput, AudioMeter::RMS);
  pMetters->AddCell(pRMSMeter);
}

void MainWindow::ProcessAudioInput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames)
{
  uint32 i = 0;
  for (i = 0; i < rInput.size(); i++)
  {
    float* pDest = &mInput[i][0];
    memcpy(pDest, rInput[i], sizeof(float) * SampleFrames);
  }
  float* pDest = &mInput[0][0];
  //mFilter.Process(pDest, pDest, SampleFrames);
  
  for (i = 0; i < rOutput.size(); i++)
  {
    uint j = 0;
    for (j = 0; j < SampleFrames; j++)
    {
      rOutput[i][j] = rInput[i][j]; // Do something with the samples
    }
  }
  
//  for (i = 0; i < rOutput.size(); i++)
//  {
//    memcpy(&mOutput[i][0], rOutput[i], sizeof(float) * SampleFrames);
//  }
}

void MainWindow::ProcessAudioOutput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames)
{
  uint32 i = 0;
  for (i = 0; i < rOutput.size(); i++)
    memset(&rOutput[i][0], 0, sizeof(float) * SampleFrames);
}


void MainWindow::OnClose()
{
  App->Quit();
}

