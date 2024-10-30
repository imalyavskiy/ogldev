#include <cstdint>
#include <cstdio>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t21_glut_backend.h"


namespace t21
{
  static ICallbacks* s_pCallbacks = nullptr;

  static void SpecialKeyboardCB(int key, int x, int y){
    s_pCallbacks->SpecialKeyboardCB(key, x, y);
  }

  static void KeyboardCB(unsigned char key, int x, int y){
    s_pCallbacks->KeyboardCB(key, x, y);
  }

  static void PassiveMouseCB(int x, int y){
    s_pCallbacks->PassiveMouseCB(x, y);
  }

  static void RenderSceneCB(){
    s_pCallbacks->RenderSceneCB();
  }

  static void IdleCB(){
    s_pCallbacks->IdleCB();
  }

  static void InitCallbacks(){
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
  }

  void GLUTBackendInit(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  }

  bool GLUTBackendCreateWindow(uint32_t width, uint32_t height, uint32_t bpp, bool isFullScreen, const char* pTitle){
    if (isFullScreen){
      char ModeString[64] = {0};
      snprintf(ModeString, sizeof(ModeString), "%dx%d@%d", width, height, bpp);
      glutGameModeString(ModeString);
      glutEnterGameMode();
    }
    else {
      glutInitWindowSize(width, height);
      glutCreateWindow(pTitle);
    }

    GLenum res = glewInit();
    if (res != GLEW_OK){
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return false;
    }

    return true;
  }

  void GLUTBackendRun(ICallbacks* pCallbacks){
    if (!pCallbacks){
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
