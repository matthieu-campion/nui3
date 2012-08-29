#include "nui.h"
#include "nuiInit.h"
#include "nuiNativeResource.h"

int main(int argc, const char** argv)
{
  nuiInit(NULL);

  nglString test1 = "bleh!";
  NGL_OUT("Testing NGL_OUT\n");
  NGL_OUT("Native res path: %s\n", nuiGetNativeResourcePath().GetChars());

  nuiUninit();
}
