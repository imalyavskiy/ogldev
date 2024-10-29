#include <cstdio>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t19_glut_backend.h"
namespace t19
{
  static ICallbacks* s_pCallbacks = NULL;

  static void SpecialKeyboardCB(int Key, int x, int y){
    s_pCallbacks->SpecialKeyboardCB(Key, x, y);
  }

  static void KeyboardCB(unsigned char Key, int x, int y){
    s_pCallbacks->KeyboardCB(Key, x, y);
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

  bool GLUTBackendCreateWindow(unsigned int winWidth, unsigned int winHeight, unsigned int winBPP, bool isFullScreen, const char* pTitle){
    if (isFullScreen) {
      char ModeString[64] = {0};
      snprintf(ModeString, sizeof(ModeString), "%dx%d@%d", winWidth, winHeight, winBPP);
      glutGameModeString(ModeString);
      glutEnterGameMode();
    }
    else {
      glutInitWindowSize(winWidth, winHeight);
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
