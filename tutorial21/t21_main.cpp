#include <memory>
#include "t21_glut_backend.h"
#include "t21_mainapp.h"

#define WINDOW_WIDTH      1280
#define WINDOW_HEIGHT     1024
#define WINDOW_BPP        32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE      "tutorial 21"

int main(int argc, char** argv)
{
  t21::GLUTBackendInit(argc, argv);

  if (!t21::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto pApp = std::make_shared<t21::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  return 0;
}
