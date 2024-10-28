#ifndef GLUT_BACKEND_H
#define GLUT_BACKEND_H

#include "t18_callbacks.h"
namespace t18
{
  void GLUTBackendInit(int32_t argc, char** argv);

  bool GLUTBackendCreateWindow(uint32_t Width, uint32_t Height, uint32_t bpp, bool isFullScreen, const char* pTitle);

  void GLUTBackendRun(ICallbacks* pCallbacks);
}

#endif /* GLUT_BACKEND_H */
