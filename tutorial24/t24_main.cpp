#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t24_glut_backend.h"
#include "t24_mainapp.h"

#define WINDOW_WIDTH      1280
#define WINDOW_HEIGHT     1024
#define WINDOW_BPP        32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITTLE     "tutorial 24"

int main(int argc, char** argv)
{
  t24::GLUTBackendInit(argc, argv);

  if (!t24::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITTLE))
    return 1;

  const auto pApp = new t24::MainApp(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  delete pApp;

  return 0;
}
