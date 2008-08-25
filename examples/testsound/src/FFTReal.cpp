/*****************************************************************************
*                                                                            *
*       DIGITAL SIGNAL PROCESSING TOOLS                                      *
*       Version 1.03, 2001/06/15                                             *
*       (c) 1999 - Laurent de Soras                                          *
*                                                                            *
*       FFTReal.cpp                                                          *
*       Fourier transformation of real number arrays.                        *
*       Portable ISO C++                                                     *
*                                                                            *
*****************************************************************************/

#include "nui.h"

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include  "FFTReal.h"

#include  <cassert>
#include  <cmath>

#include "nglMath.h"

#if defined(_MSC_VER)
#pragma pack(push, 8)
#endif  // _MSC_VER


/*\\\ PUBLIC MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*==========================================================================*/
/*      Name: Constructor                                                   */
/*      Input parameters:                                                   */
/*        - length: length of the array on which we want to do a FFT.       */
/*                  Range: power of 2 only, > 0.                            */
/*      Throws: std::bad_alloc, anything                                    */
/*==========================================================================*/

FFTReal::FFTReal(const long length)
:  mLength(length)
,  mNbr_bits(ToBelow(floor(log((real)length) / log(2.0f) + 0.5f)))
,  mTrigo_lut(ToBelow(floor(log((real)length) / log(2.0f) + 0.5f)))
,  mBit_rev_lut(ToBelow(floor(log((real)length) / log(2.0f) + 0.5f)))
,  mSqrt2_2((real)(sqrt(2.0f) * 0.5f))
{
  assert((1L << mNbr_bits) == length);

  mBuffer_ptr = 0;
  if(mNbr_bits > 2)
  {
    mBuffer_ptr = new real[mLength];
  }

  mpTwidles = new real[mLength*2];
  for (int i = 0; i < mLength; i++)
  {
    mpTwidles[i*2] = (real)sin((M_PI * (real)i)/(real)mLength);
    mpTwidles[i*2+1] = (real)cos((M_PI * (real)i)/(real)mLength);
  }
}



/*==========================================================================*/
/*      Name: Destructor                                                    */
/*==========================================================================*/

FFTReal::~FFTReal(void)
{
  delete[] mBuffer_ptr;
  mBuffer_ptr = 0;
  delete mpTwidles;
}



/*==========================================================================*/
/*      Name: do_fft                                                        */
/*      Description: Compute the FFT of the array.                          */
/*      Input parameters:                                                   */
/*        - x: pointer on the source array(time).                          */
/*      Output parameters:                                                  */
/*        - f: pointer on the destination array(frequencies).              */
/*             f[0...length(x)/2] = real values,                           */
/*             f[length(x)/2+1...length(x)-1] = imaginary values of        */
/*               coefficents 1...length(x)/2-1.                             */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

void FFTReal::do_fft(real f[], const real x[]) const
{
/*______________________________________________
 *
 * General case
 *______________________________________________
 */

  if(mNbr_bits > 2)
  {
    real* sf;
    real* df;

    if(mNbr_bits & 1)
    {
      df = mBuffer_ptr;
      sf = f;
    }
    else
    {
      df = f;
      sf = mBuffer_ptr;
    }

    /* Do the transformation in several pass */
    {
      int pass;
      long nbr_coef;
      long h_nbr_coef;
      long d_nbr_coef;
      long coef_index;

      /* First and second pass at once */
      {
        const long * const bit_rev_lut_ptr = mBit_rev_lut.get_ptr();
        coef_index = 0;
        do
        {
          const long rev_index_0 = bit_rev_lut_ptr[coef_index];
          const long rev_index_1 = bit_rev_lut_ptr[coef_index + 1];
          const long rev_index_2 = bit_rev_lut_ptr[coef_index + 2];
          const long rev_index_3 = bit_rev_lut_ptr[coef_index + 3];

          real* const df2 = df + coef_index;
          df2[1] = x[rev_index_0] - x[rev_index_1];
          df2[3] = x[rev_index_2] - x[rev_index_3];

          const real sf_0 = x[rev_index_0] + x[rev_index_1];
          const real sf_2 = x[rev_index_2] + x[rev_index_3];

          df2[0] = sf_0 + sf_2;
          df2[2] = sf_0 - sf_2;
          
          coef_index += 4;
        }
        while(coef_index < mLength);
      }

      /* Third pass */
      {
        coef_index = 0;
        const real sqrt2_2 = mSqrt2_2;
        do
        {
          real v;

          sf[coef_index] = df[coef_index] + df[coef_index + 4];
          sf[coef_index + 4] = df[coef_index] - df[coef_index + 4];
          sf[coef_index + 2] = df[coef_index + 2];
          sf[coef_index + 6] = df[coef_index + 6];

          v =(df[coef_index + 5] - df[coef_index + 7]) * sqrt2_2;
          sf[coef_index + 1] = df[coef_index + 1] + v;
          sf[coef_index + 3] = df[coef_index + 1] - v;

          v =(df[coef_index + 5] + df[coef_index + 7]) * sqrt2_2;
          sf[coef_index + 5] = v + df[coef_index + 3];
          sf[coef_index + 7] = v - df[coef_index + 3];

          coef_index += 8;
        }
        while(coef_index < mLength);
      }

      /* Next pass */
      for (pass = 3; pass < mNbr_bits; ++pass)
      {
        coef_index = 0;
        nbr_coef = 1 << pass;
        h_nbr_coef = nbr_coef >> 1;
        d_nbr_coef = nbr_coef << 1;
        const real* const cos_ptr = mTrigo_lut.get_ptr(pass);
        do
        {
          long i;
          const real* const sf1r = sf + coef_index;
          const real* const sf2r = sf1r + nbr_coef;
          real* const dfr = df + coef_index;
          real* const dfi = dfr + nbr_coef;

          /* Extreme coefficients are always real*/
          dfr[0] = sf1r[0] + sf2r[0];
          dfi[0] = sf1r[0] - sf2r[0];  // dfr[nbr_coef] =
          dfr[h_nbr_coef] = sf1r[h_nbr_coef];
          dfi[h_nbr_coef] = sf2r[h_nbr_coef];

          /* Others are conjugate complex numbers */
          const real* const sf1i = sf1r + h_nbr_coef;
          const real* const sf2i = sf1i + nbr_coef;
          for (i = 1; i < h_nbr_coef; ++ i)
          {
            const real c = cos_ptr[i];          // cos(i*PI/nbr_coef);
            const real s = cos_ptr[h_nbr_coef - i];  // sin(i*PI/nbr_coef);
            real v;

            v = sf2r[i] * c - sf2i[i] * s;
            dfr[i] = sf1r[i] + v;
            dfi[-i] = sf1r[i] - v;  // dfr[nbr_coef - i] =

            v = sf2r[i] * s + sf2i[i] * c;
            dfi[i] = v + sf1i[i];
            dfi[nbr_coef - i] = v - sf1i[i];
          }

          coef_index += d_nbr_coef;
        }
        while(coef_index < mLength);

        /* Prepare to the next pass */
        {
          real* const temp_ptr = df;
          df = sf;
          sf = temp_ptr;
        }
      }
    }
  }

/*______________________________________________
 *
 * Special cases
 *______________________________________________
 */

  /* 4-point FFT */
  else if(mNbr_bits == 2)
  {
    f[1] = x[0] - x[2];
    f[3] = x[1] - x[3];

    const real b_0 = x[0] + x[2];
    const real b_2 = x[1] + x[3];
    
    f[0] = b_0 + b_2;
    f[2] = b_0 - b_2;
  }

  /* 2-point FFT */
  else if(mNbr_bits == 1)
  {
    f[0] = x[0] + x[1];
    f[1] = x[0] - x[1];
  }

  /* 1-point FFT */
  else
  {
    f[0] = x[0];
  }
}



/*==========================================================================*/
/*      Name: do_ifft                                                       */
/*      Description: Compute the inverse FFT of the array. Notice that      */
/*                   IFFT(FFT(x)) = x * length(x). Data must be          */
/*                   post-scaled.                                           */
/*      Input parameters:                                                   */
/*        - f: pointer on the source array(frequencies).                   */
/*             f[0...length(x)/2] = real values,                           */
/*             f[length(x)/2+1...length(x)] = imaginary values of          */
/*               coefficents 1...length(x)-1.                               */
/*      Output parameters:                                                  */
/*        - x: pointer on the destination array(time).                     */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

void FFTReal::do_ifft(const real f[], real x[]) const
{
/*______________________________________________
 *
 * General case
 *______________________________________________
 */

  if(mNbr_bits > 2)
  {
    real* sf = const_cast <real*>(f);
    real* df;
    real* df_temp;

    if(mNbr_bits & 1)
    {
      df = mBuffer_ptr;
      df_temp = x;
    }
    else
    {
      df = x;
      df_temp = mBuffer_ptr;
    }

    /* Do the transformation in several pass */
    {
      int pass;
      long nbr_coef;
      long h_nbr_coef;
      long d_nbr_coef;
      long coef_index;

      /* First pass */
      for (pass = mNbr_bits - 1; pass >= 3; --pass)
      {
        coef_index = 0;
        nbr_coef = 1 << pass;
        h_nbr_coef = nbr_coef >> 1;
        d_nbr_coef = nbr_coef << 1;
        const real*const cos_ptr = mTrigo_lut.get_ptr(pass);
        do
        {
          long i;
          const real* const sfr = sf + coef_index;
          const real* const sfi = sfr + nbr_coef;
          real* const df1r = df + coef_index;
          real* const df2r = df1r + nbr_coef;

          /* Extreme coefficients are always real*/
          df1r[0] = sfr[0] + sfi[0];    // + sfr[nbr_coef]
          df2r[0] = sfr[0] - sfi[0];    // - sfr[nbr_coef]
          df1r[h_nbr_coef] = sfr[h_nbr_coef] * 2;
          df2r[h_nbr_coef] = sfi[h_nbr_coef] * 2;

          /* Others are conjugate complex numbers */
          real* const df1i = df1r + h_nbr_coef;
          real* const df2i = df1i + nbr_coef;
          for (i = 1; i < h_nbr_coef; ++ i)
          {
            df1r[i] = sfr[i] + sfi[-i];    // + sfr[nbr_coef - i]
            df1i[i] = sfi[i] - sfi[nbr_coef - i];

            const real c = cos_ptr[i];          // cos(i*PI/nbr_coef);
            const real s = cos_ptr[h_nbr_coef - i];  // sin(i*PI/nbr_coef);
            const real vr = sfr[i] - sfi[-i];    // - sfr[nbr_coef - i]
            const real vi = sfi[i] + sfi[nbr_coef - i];

            df2r[i] = vr * c + vi * s;
            df2i[i] = vi * c - vr * s;
          }

          coef_index += d_nbr_coef;
        }
        while(coef_index < mLength);

        /* Prepare to the next pass */
        if(pass < mNbr_bits - 1)
        {
          real* const temp_ptr = df;
          df = sf;
          sf = temp_ptr;
        }
        else
        {
          sf = df;
          df = df_temp;
        }
      }

      /* Antepenultimate pass */
      {
        const real sqrt2_2 = mSqrt2_2;
        coef_index = 0;
        do
        {
          df[coef_index] = sf[coef_index] + sf[coef_index + 4];
          df[coef_index + 4] = sf[coef_index] - sf[coef_index + 4];
          df[coef_index + 2] = sf[coef_index + 2] * 2;
          df[coef_index + 6] = sf[coef_index + 6] * 2;

          df[coef_index + 1] = sf[coef_index + 1] + sf[coef_index + 3];
          df[coef_index + 3] = sf[coef_index + 5] - sf[coef_index + 7];

          const real vr = sf[coef_index + 1] - sf[coef_index + 3];
          const real vi = sf[coef_index + 5] + sf[coef_index + 7];

          df[coef_index + 5] =(vr + vi) * sqrt2_2;
          df[coef_index + 7] =(vi - vr) * sqrt2_2;

          coef_index += 8;
        }
        while(coef_index < mLength);
      }

      /* Penultimate and last pass at once */
      {
        coef_index = 0;
        const long *  bit_rev_lut_ptr = mBit_rev_lut.get_ptr();
        const real* sf2 = df;
        do
        {
          {
            const real b_0 = sf2[0] + sf2[2];
            const real b_2 = sf2[0] - sf2[2];
            const real b_1 = sf2[1] * 2;
            const real b_3 = sf2[3] * 2;

            x[bit_rev_lut_ptr[0]] = b_0 + b_1;
            x[bit_rev_lut_ptr[1]] = b_0 - b_1;
            x[bit_rev_lut_ptr[2]] = b_2 + b_3;
            x[bit_rev_lut_ptr[3]] = b_2 - b_3;
          }
          {
            const real b_0 = sf2[4] + sf2[6];
            const real b_2 = sf2[4] - sf2[6];
            const real b_1 = sf2[5] * 2;
            const real b_3 = sf2[7] * 2;

            x[bit_rev_lut_ptr[4]] = b_0 + b_1;
            x[bit_rev_lut_ptr[5]] = b_0 - b_1;
            x[bit_rev_lut_ptr[6]] = b_2 + b_3;
            x[bit_rev_lut_ptr[7]] = b_2 - b_3;
          }

          sf2 += 8;
          coef_index += 8;
          bit_rev_lut_ptr += 8;
        }
        while(coef_index < mLength);
      }
    }
  }

/*______________________________________________
 *
 * Special cases
 *______________________________________________
 */

  /* 4-point IFFT */
  else if(mNbr_bits == 2)
  {
    const real b_0 = f[0] + f[2];
    const real b_2 = f[0] - f[2];

    x[0] = b_0 + f[1] * 2;
    x[2] = b_0 - f[1] * 2;
    x[1] = b_2 + f[3] * 2;
    x[3] = b_2 - f[3] * 2;
  }

  /* 2-point IFFT */
  else if(mNbr_bits == 1)
  {
    x[0] = f[0] + f[1];
    x[1] = f[0] - f[1];
  }

  /* 1-point IFFT */
  else
  {
    x[0] = f[0];
  }
}

void FFTReal::SplitComplex( real rComplex[] ) 
{
}

void FFTReal::MergeComplex( real rComplex[] ) 
{
}

void FFTReal::do_dif(real f[]) const
{
  /*
{Perform in place DIF of 2^p points (=size of f)}
PROCEDURE DIF(p,VAR f);
LOCAL Bp,Np,Np',P,b,n,BaseE,BaseO,e,o;
BEGIN {DIF}
{initialise pass parameters}
Bp:=1;    {No. of blocks}
Np:=1<<p; {No. of points in each block}    
{perform p passes}
FOR P:=0 TO (p-1) DO
  BEGIN {pass loop}
  Np':=Np>>1; {No. of butterflies}
  BaseE:=0;   {Reset even base index}
  FOR b:=0 TO (Bp-1) DO
    BEGIN {block loop}
    BaseO:=BaseE+Np'; {calc odd base index}
    FOR n:=0 TO (Np'-1) DO
      BEGIN {butterfly loop}
      e:= f[BaseE+n]+f[BaseO+n];
      o:=(f[BaseE+n]-f[BaseO+n])*T(Np,n);
      f[BaseE+n]:=e;
      f[BaseO+n]:=o;
      END; {butterfly loop}
    BaseE:=BaseE+Np; {start of next block}
    END; {block loop}
  {calc parameters for next pass}
  Bp:=Bp<<1; {twice as many blocks}
  Np:=Np>>1; {half as many points in each block}
  END; {pass loop}
END; {DIF}*/
//Perform in place DIF of 2^p points (=size of f)
//  PROCEDURE DIF(p,VAR f);
  int Bp, Np, Npp, P, b, n, BaseE, BaseO;

  //initialise pass parameters
  Bp = 1;    //No. of blocks
  Np = 1 << (mNbr_bits-1); //No. of points in each block

  const real* pT = mpTwidles;
  uint twidlefactor = 2;

  //perform p passes
  for ( P = 0; P < (mNbr_bits-1); P++)
  { //pass loop
    Npp = Np >> 1; //No. of butterflies
    BaseE = 0;   //Reset even base index
    for ( b = 0; b < (Bp); b++)
    { //block loop
      BaseO = BaseE + Npp; //calc odd base index
      for ( n = 0; n < (Npp); n++)
      { //butterfly loop
        const uint ben2 = (BaseE + n) * 2;
        const uint bon2 = (BaseO + n) * 2;

        const real eer = f[ben2];
        const real eei = f[ben2 + 1];
        const real oor = f[bon2];
        const real ooi = f[bon2 + 1];

        const uint n2 = n * twidlefactor;

        const real tr = pT[n2];
        const real ti = pT[n2+1];

        // complex addition
        const real er =  eer + oor;
        const real ei =  eei + ooi;

        // Complex substraction
        const real _or = eer - oor;// T(Np,n);
        const real _oi = eei - ooi;// T(Np,n);

        // Complex multiplication
        const real rr = _or * tr + _oi * ti;
        const real ri = _or * ti + _oi * tr;

        f[ben2]   = er;
        f[ben2+1] = ei;
        f[bon2]   = rr;
        f[bon2+1] = ri;

      } //butterfly loop
      BaseE = BaseE + Np; //start of next block
    } //block loop
    //calc parameters for ( next pass
    twidlefactor *= 2;
    Bp = Bp << 1; //twice as many blocks
    Np = Np >> 1; //half as many points in each block
  } //pass loop
}


void FFTReal::do_dit(real f[]) const
{
/*
{Perform in place DIT of 2^p points (=size of f)
 N.B. The input array f is in bit reversed order! So all the
 'even' input samples are in the 'top' half, all the 'odd'
 input samples are in the 'bottom' half..etc (recursively).
}
PROCEDURE DIT(p,VAR f);
LOCAL Bp,Np,Np',P,b,k,BaseT,BaseB,top,bot;
BEGIN {DIT}
{initialise pass parameters}
Bp:=1<<(p-1);{No. of blocks}
Np:=2;       {No. of points in each block}    
{perform p passes}
FOR P:=0 TO (p-1) DO
  BEGIN {pass loop}
  Np':=Np>>1; {No. of butterflies}
  BaseT:=0;   {Reset even base index}
  FOR b:=0 TO (Bp-1) DO
    BEGIN {block loop}
    BaseB:=BaseT+Np'; {calc odd base index}
    FOR k:=0 TO (Np'-1) DO
      BEGIN {butterfly loop}
        top:=f[BaseT+k];
        bot:=f[BaseB+k]*T(Np,k); {twiddle the odd n results} 
        f[BaseT+k]:= top+bot;    {top    subset}
        f[BaseB+k]:= top-bot;    {bottom subset}
      END; {butterfly loop}
    BaseT:=BaseT+Np; {start of next block}
    END; {block loop}
  {calc parameters for next pass}
  Bp:=Bp>>1; {half as many blocks}
  Np:=Np<<1; {twice as many points in each block}
  END; {pass loop}
END; {DIT}        */

/*Perform in place DIT of 2^p points (=size of f)
 N.B. The input array f is in bit reversed order! So all the
 'even' input samples are in the 'top' half, all the 'odd'
 input samples are in the 'bottom' half..etc (recursively).
*/
//PROCEDURE DIT(p,VAR f);
  int Bp, Np, Npp, P, b, k, BaseT, BaseB, top, bot;
  /*initialise pass parameters*/
  Bp = 1 << (mNbr_bits - 2);/*No. of blocks*/
  Np = 2;       /*No. of points in each block*/    

  const real* pT = mpTwidles;
  int twidlefactor = Bp;

  /*perform p passes*/
  for (P = 0; P < mNbr_bits-1; P++)
  { /*pass loop*/
    Npp = Np >> 1; /*No. of butterflies*/
    BaseT = 0;   /*Reset even base index*/

    for (b = 0; b < Bp; b++)
    { /*block loop*/
      BaseB = BaseT + Npp; /*calc odd base index*/
      
      for (k = 0; k < Npp; k++)
      { /*butterfly loop*/
        const uint btk = 2*(BaseT + k);
        const uint bbk = 2*(BaseB + k);

        const real topr = f[btk];
        const real topi = f[btk+1];

        const real botr = f[bbk];
        const real boti = f[bbk+1];

        const uint k2 = k * twidlefactor;

        const real tr = pT[k2];
        const real ti = pT[k2+1];

        const real tmpr = botr * tr + boti * ti;
        const real tmpi = botr * ti + boti * tr;
//        bot = f[BaseB + k] * T(Np,k); /*twiddle the odd n results*/ 

//        f[BaseT + k] =  top + bot;    /*top    subset*/
//        f[BaseB + k] =  top - bot;    /*bottom subset*/

        f[btk] = topr + tmpr;
        f[btk+1] = topi + tmpi;
        f[bbk] = topr - tmpr;
        f[bbk+1] = topi - tmpi;
      } /*butterfly loop*/

      BaseT = BaseT + Np; /*start of next block*/
    } /*block loop*/

    /*calc parameters for next pass*/
    Bp = Bp >> 1; /*half as many blocks*/
    Np = Np << 1; /*twice as many points in each block*/
    twidlefactor >>= 1;
  } /*pass loop*/
}

/*==========================================================================*/
/*      Name: rescale                                                       */
/*      Description: Scale an array by divide each element by its length.   */
/*                   This function should be called after FFT + IFFT.       */
/*      Input/Output parameters:                                            */
/*        - x: pointer on array to rescale(time or frequency).             */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

void FFTReal::rescale(real x[]) const
{
  const real mul = real(1.0 / mLength);
  long i = mLength - 1;

  do
  {
    x[i] *= mul;
    --i;
  }
  while(i >= 0);
}



/*\\\ NESTED CLASS MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*==========================================================================*/
/*      Name: Constructor                                                   */
/*      Input parameters:                                                   */
/*        - nbr_bits: number of bits of the array on which we want to do a  */
/*                    FFT. Range: > 0                                       */
/*      Throws: std::bad_alloc                                              */
/*==========================================================================*/

FFTReal::BitReversedLUT::BitReversedLUT(const int nbr_bits)
{
  long length;
  long cnt;
  long br_index;
  long bit;

  length = 1L << nbr_bits;
  mPtr = new long[length];

  br_index = 0;
  mPtr[0] = 0;
  for (cnt = 1; cnt < length; ++cnt)
  {
    /* ++br_index(bit reversed) */
    bit = length >> 1;
    while(((br_index ^= bit) & bit) == 0)
    {
      bit >>= 1;
    }

    mPtr[cnt] = br_index;
  }
}



/*==========================================================================*/
/*      Name: Destructor                                                    */
/*==========================================================================*/

FFTReal::BitReversedLUT::~BitReversedLUT(void)
{
  delete[] mPtr;
  mPtr = 0;
}



/*==========================================================================*/
/*      Name: Constructor                                                   */
/*      Input parameters:                                                   */
/*        - nbr_bits: number of bits of the array on which we want to do a  */
/*                    FFT. Range: > 0                                       */
/*      Throws: std::bad_alloc, anything                                    */
/*==========================================================================*/

FFTReal::TrigoLUT::TrigoLUT(const int nbr_bits)
{
  long total_len;

  mPtr = 0;
  if(nbr_bits > 3)
  {
    total_len =(1L <<(nbr_bits - 1)) - 4;
    mPtr = new real[total_len];

    const double  PI = atan(1.0) * 4;
    for (int level = 3; level < nbr_bits; ++level)
    {
      const long level_len = 1L <<(level - 1);
      real* const level_ptr = const_cast<real*>(get_ptr(level));
      const double  mul = PI /(level_len << 1);

      for (long i = 0; i < level_len; ++ i)
      {
        level_ptr[i] =(real) cos(i * mul);
      }
    }
  }
}



/*==========================================================================*/
/*      Name: Destructor                                                    */
/*==========================================================================*/

FFTReal::TrigoLUT::~TrigoLUT(void)
{
  delete[] mPtr;
  mPtr = 0;
}



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
