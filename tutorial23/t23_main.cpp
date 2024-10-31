#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t23_glut_backend.h"
#include "t23_mainapp.h"

#define WINDOW_WIDTH      1280
#define WINDOW_HEIGHT     1024
#define WINDOW_BPP        32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE      "tutorial 23"

int main(int argc, char** argv)
{
  t23::GLUTBackendInit(argc, argv);

  if (!t23::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  auto pApp = new t23::MainApp(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  delete pApp;

  return 0;
}
