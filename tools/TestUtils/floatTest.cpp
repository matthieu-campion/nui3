#include "nui3/include/nui.h"
#include "nui3/include/nglMath.h"

typedef int32 (*Fastmathfunc)(double);
typedef double (*Classicmathfunc)(double);

void performTests(uint64 numTests, const char* fastFuncName, Fastmathfunc fastfunc, const char* classicFuncName, Classicmathfunc classicfunc, uint8 verbosity)
{
  printf("Testing %s against %s.\n", fastFuncName, classicFuncName);
  
  srandomdev();
  double x;
  int64 intx;
  int32 fasty, classicy;
  double scale = 1.0/double(1 << 16);
  int fails = 0;
  for (int i=0; i<numTests; i++)
  {
    double intg1 = random()*scale;
    double intg2 = random()*scale;
    double intg3 = random()*scale;
    double frac = random();
    
    x = double(intg1) - double(intg2) + double(intg3) + double(frac) / RAND_MAX;
    fasty = fastfunc(x);
    classicy = (int32)classicfunc(x);
    
    if (fasty == classicy)
    {
      if (verbosity > 1)
        printf("Test succeeded:\n\t%s(%.15g) = %d\n\t%s(%.15g) = %d\n", fastFuncName, x, fasty, classicFuncName, x, classicy);
    }
    else
    {
      if (verbosity > 0)
        printf("Test failed:\n\t%s(%.15g) = %d\n\t%s(%.15g) = %d\n", fastFuncName, x, fasty, classicFuncName, x, classicy);
      fails++;
    }
  }
  
  printf("%d tests failed.\n\n", fails);
}

double classicToZero(double x)
{
  return double(int32(x));
}

void printUsage()
{
  printf("usage: floatTest [-q | -v | -vv] [-h] [<n>]\n");
  printf("\t-q : quiet mode. Only report number of failed tests.\n");
  printf("\t-v : verbose mode (default). Report each failed test individually.\n");
  printf("\t-vv: very verbose mode. Report each test.\n");
  printf("\t-h : display this help message.\n");
  printf("\t<n>: number of tests to perform (default is 100000)\n");
}

int main(int argc, char** argv)
{
  uint8 verbosity = 1;
  uint64 numOfTests = 100000;
  if (argc > 1)
  {
    for (uint8 i = 1; i < argc; i++)
    {
      if (strncmp(argv[i], "-q", 2) == 0)
      {
        verbosity = 0;
      }
      else if (strncmp(argv[i], "-v", 2) == 0)
      {
        if (strncmp(argv[i], "-vv", 3) == 0)
        {
          verbosity = 2;
        }
        else
        {
          verbosity = 1;
        }
      }
      else if (strncmp(argv[i], "-h", 2) == 0)
      {
        printUsage();
        exit(0);
      }
      else if (strtoll(argv[i], NULL, 10) > 0)
      {
        numOfTests = strtoll(argv[i], NULL, 10);
      }
      else
      {
        printUsage();
        exit(0);
      }
    }
  }
  
  printf("%d\n", verbosity);
    
  // Banker's Rounding Test
  performTests(numOfTests, "ToNearest", ToNearest, "round", round, verbosity);
  
  // To-Zero Rounding Test
  performTests(numOfTests, "ToZero", ToZero, "int32", classicToZero, verbosity);
  
  // Floor Test
  performTests(numOfTests, "ToBelow", ToBelow, "floor", floor, verbosity);
  
  // Ceiling Test
  performTests(numOfTests, "ToAbove", ToAbove, "ceil", ceil, verbosity);
  return 0;
}