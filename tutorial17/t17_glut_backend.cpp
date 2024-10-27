#include <cstdio>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t17_glut_backend.h"
namespace t17
{
  static ICallbacks* s_pCallbacks = nullptr;

  static void SpecialKeyboardCB(int Key, int x, int y) {
    if(s_pCallbacks)
      s_pCallbacks->SpecialKeyboardCB(Key, x, y);
  }

  static void KeyboardCB(unsigned char Key, int x, int y) {
    if (s_pCallbacks)
      s_pCallbacks->KeyboardCB(Key, x, y);
  }

  static void PassiveMouseCB(int x, int y) {
    if (s_pCallbacks)
      s_pCallbacks->PassiveMouseCB(x, y);
  }

  static void RenderSceneCB() {
    if (s_pCallbacks)
      s_pCallbacks->RenderSceneCB();
  }

  static void IdleCB() {
    if (s_pCallbacks)
      s_pCallbacks->IdleCB();
  }

  static void InitCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
  }

  void GLUTBackendInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  }

  bool GLUTBackendCreateWindow(unsigned int width, unsigned int height, unsigned int bpp, bool isFullScreen, const char* pTitle) {
    if (isFullScreen) {
      char ModeString[64] = { 0 };
      snprintf(ModeString, sizeof(ModeString), "%dx%d@%d", width, height, bpp);
      glutGameModeString(ModeString);
      glutEnterGameMode();
    }
    else {
      glutInitWindowSize(width, height);
      glutCreateWindow(pTitle);
    }

    const GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return false;
    }

    return true;
  }

  void GLUTBackendRun(ICallbacks* pCallbacks) {
    if (!pCallbacks) {
      fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
      return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    s_pCallbacks = pCallbacks;
    InitCallbacks();
    glutMainLoop();
  }
}