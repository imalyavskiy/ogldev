#include <memory>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t22_pipeline.h"
#include "t22_camera.h"
#include "t22_texture.h"
#include "t22_lighting_technique.h"
#include "t22_glut_backend.h"
#include "t22_util.h"
#include "t22_mesh.h"
#include "t22_mainapp.h"

#define WINDOW_WIDTH      1280
#define WINDOW_HEIGHT     1024
#define WINDOW_BPP        32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE      "tutorial 22"

int main(int argc, char** argv)
{
  t22::GLUTBackendInit(argc, argv);

  if (!t22::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto pApp = std::make_shared<t22::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  return 0;
}
