/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstdint>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t37_util.h"
#include "t37_glut_backend.h"

namespace t37
{

  // Points to the object implementing the ICallbacks interface which was delivered to
  // GLUTBackendRun(). All events are forwarded to this object.
  static ICallbacks* s_pCallbacks = nullptr;

  static void SpecialKeyboardCB(int key, int x, int y)
  {
    s_pCallbacks->SpecialKeyboardCB(key, x, y);
  }


  static void KeyboardCB(unsigned char key, int x, int y)
  {
    s_pCallbacks->KeyboardCB(key, x, y);
  }


  static void PassiveMouseCB(int x, int y)
  {
    s_pCallbacks->PassiveMouseCB(x, y);
  }


  static void RenderSceneCB()
  {
    s_pCallbacks->RenderSceneCB();
  }


  static void IdleCB()
  {
    s_pCallbacks->IdleCB();
  }


  static void MouseCB(int button, int state, int x, int y)
  {
    s_pCallbacks->MouseCB(button, state, x, y);
  }


  static void InitCallbacks()
  {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
    glutMouseFunc(MouseCB);
  }


  void GLUTBackendInit(int argc, char** argv)
  {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  }


  bool GLUTBackendCreateWindow(uint32_t width, uint32_t height, uint32_t bpp, bool isFullScreen, const char* pTitle)
  {
    if (isFullScreen) {
      char ModeString[64] = { 0 };
      SNPRINTF(ModeString, sizeof(ModeString), "%dx%d@%d", width, height, bpp);
      glutGameModeString(ModeString);
      glutEnterGameMode();
    }
    else {
      glutInitWindowSize(width, height);
      glutCreateWindow(pTitle);
    }

    // Must be done after glut is initialized!
    const GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return false;
    }

    return true;
  }

  void GLUTBackendRun(ICallbacks* pCallbacks)
  {
    if (!pCallbacks) {
      fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
      return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    s_pCallbacks = pCallbacks;
    InitCallbacks();
    glutMainLoop();
  }
}