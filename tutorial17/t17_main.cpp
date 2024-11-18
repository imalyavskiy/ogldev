#include "t17_glut_backend.h"
#include "t17_main_app.h"

#define VERTICAL_FOV 60.f
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_BPP 32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE "Tutorial 17"
#define Z_NEAR_CLIPPING_PLANE 1.f
#define Z_FAR_CLIPPING_PLANE 100.f

int main(int argc, char** argv)
{
  t17::GLUTBackendInit(argc, argv);

  if (!t17::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto app = std::make_shared<t17::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT, VERTICAL_FOV, Z_NEAR_CLIPPING_PLANE, Z_FAR_CLIPPING_PLANE);

  if (!app->Init())
    return 1;

  app->Run();

  return 0;
}
