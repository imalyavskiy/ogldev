#include "t18_main_app.h"
#include "t18_glut_backend.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_BPP    32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE "Tutorial 18"


int main(int argc, char** argv)
{
  t18::GLUTBackendInit(argc, argv);

  if (!t18::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto pApp =
    std::make_shared<t18::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  return 0;
}
