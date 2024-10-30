#ifndef GLUT_BACKEND_H
#define GLUT_BACKEND_H

#include "t21_callbacks.h"

namespace t21
{
  void GLUTBackendInit(int argc, char** argv);

  bool GLUTBackendCreateWindow(uint32_t width, uint32_t height, uint32_t bpp, bool isFullScreen, const char* pTitle);

  void GLUTBackendRun(ICallbacks* pCallbacks);
}

#endif /* GLUT_BACKEND_H */
