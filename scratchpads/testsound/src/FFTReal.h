/*****************************************************************************
*                                                                            *
*       DIGITAL SIGNAL PROCESSING TOOLS                                      *
*       Version 1.03, 2001/06/15                                             *
*       (c) 1999 - Laurent de Soras                                          *
*                                                                            *
*       FFTReal.h                                                            *
*       Fourier transformation of real number arrays.                        *
*       Portable ISO C++                                                     *
*                                                                            *
*****************************************************************************/

#pragma once

#if defined(_MSC_VER)
#pragma pack(push, 8)
#endif  // _MSC_VER

class  FFTReal
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

  /* Change this typedef to use a different floating point type in your FFTs
   (i.e. float, double or long double). */
  typedef float real;

  explicit FFTReal(const long length);
  virtual ~FFTReal();
  void do_fft(real f[], const real x[]) const; ///< Classical Fast Fourier Transform.
  void do_ifft(const real f[], real x[]) const; ///< Classical Inverse Fast Fourier Transform.
  
  void do_dif(real f[]) const; ///< In-place Simple Decimation in Frequency transform. (FFT with unscrambled source, scrambled results).
  void do_dit(real f[]) const; ///< In-place Simple Decimation in Time transform. (FFT with scrambled sources, unscrambled results).

  void rescale(real x[]) const;

  void MergeComplex( real rComplex[] );
  void SplitComplex( real rComplex[] );

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

  /* Bit-reversed look-up table nested class */
  class BitReversedLUT
  {
  public:
    explicit BitReversedLUT(const int nbr_bits);
    virtual ~BitReversedLUT();
    const long* get_ptr() const
    {
      return(mPtr);
    }
  private:
    long* mPtr;
  };

  /* Trigonometric look-up table nested class */
  class TrigoLUT
  {
  public:
    explicit TrigoLUT(const int nbr_bits);
    virtual ~TrigoLUT();
    const real* get_ptr(const int level) const
    {
      return(mPtr +(1L <<(level - 1)) - 4);
    };
  private:
    real* mPtr;
  };

  const BitReversedLUT mBit_rev_lut;
  const real mSqrt2_2;
  real* mBuffer_ptr;

  const TrigoLUT mTrigo_lut;
  const long mLength;
  const int mNbr_bits;
  real* mpTwidles;

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

  FFTReal(const FFTReal &other);
  const FFTReal& operator =(const FFTReal &other);
  int operator ==(const FFTReal &other);
  int operator !=(const FFTReal &other);
};

#if defined(_MSC_VER)
#pragma pack(pop)
#endif  // _MSC_VER



/*****************************************************************************

  LEGAL

  Source code may be freely used for any purpose, including commercial
  applications. Programs must display in their "About" dialog-box(or
  documentation) a text telling they use these routines by Laurent de Soras.
  Modified source code can be distributed, but modifications must be clearly
  indicated.

  CONTACT

  Laurent de Soras
  92 avenue Albert 1er
  92500 Rueil-Malmaison
  France

  ldesoras@club-internet.fr

*****************************************************************************/



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
