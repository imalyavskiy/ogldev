#ifndef GLUT_BACKEND_H
#define GLUT_BACKEND_H

#include "t19_callbacks.h"
namespace t19
{
  void GLUTBackendInit(int argc, char** argv);

  bool GLUTBackendCreateWindow(unsigned int winWidth, unsigned int winHeight, unsigned int winBPP, bool isFullScreen, const char* pTitle);

  void GLUTBackendRun(ICallbacks* pCallbacks);
}

#endif /* GLUT_BACKEND_H */
