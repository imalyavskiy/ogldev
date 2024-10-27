#ifndef GLUT_BACKEND_H
#define GLUT_BACKEND_H

#include "t17_callbacks.h"
namespace t17
{
  void GLUTBackendInit(int argc, char** argv);

  bool GLUTBackendCreateWindow(unsigned int width, unsigned int height, unsigned int bpp, bool isFullScreen, const char* pTitle);

  void GLUTBackendRun(ICallbacks* pCallbacks);
}

#endif /* GLUT_BACKEND_H */
