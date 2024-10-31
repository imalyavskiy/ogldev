#include <memory>
#include "t20_glut_backend.h"
#include "t20_mainapp.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_BPP 32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE "tutorial 20"

int main(int argc, char** argv)
{
  t20::GLUTBackendInit(argc, argv);

  if (!t20::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto pApp = std::make_shared<t20::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  return 0;
}
