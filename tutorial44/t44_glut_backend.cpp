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

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t44_util.h"
#include "t44_glut_backend.h"

namespace t44
{
  // Points to the object implementing the ICallbacks interface which was delivered to
  // GLUTBackendRun(). All events are forwarded to this object.
  static ICallbacks* s_pCallbacks = nullptr;

  static bool sWithDepth = false;
  static bool sWithStencil = false;

  KEYBOARD_KEY GLUTKeyToOGLDEVKey(uint32_t Key)
  {
    switch (Key) {
    case GLUT_KEY_F1:
      return KB_KEY_F1;
    case GLUT_KEY_F2:
      return KB_KEY_F2;
    case GLUT_KEY_F3:
      return KB_KEY_F3;
    case GLUT_KEY_F4:
      return KB_KEY_F4;
    case GLUT_KEY_F5:
      return KB_KEY_F5;
    case GLUT_KEY_F6:
      return KB_KEY_F6;
    case GLUT_KEY_F7:
      return KB_KEY_F7;
    case GLUT_KEY_F8:
      return KB_KEY_F8;
    case GLUT_KEY_F9:
      return KB_KEY_F9;
    case GLUT_KEY_F10:
      return KB_KEY_F10;
    case GLUT_KEY_F11:
      return KB_KEY_F11;
    case GLUT_KEY_F12:
      return KB_KEY_F12;
    case GLUT_KEY_LEFT:
      return KB_KEY_LEFT;
    case GLUT_KEY_UP:
      return KB_KEY_UP;
    case GLUT_KEY_RIGHT:
      return KB_KEY_RIGHT;
    case GLUT_KEY_DOWN:
      return KB_KEY_DOWN;
    case GLUT_KEY_PAGE_UP:
      return KB_KEY_PAGE_UP;
    case GLUT_KEY_PAGE_DOWN:
      return KB_KEY_PAGE_DOWN;
    case GLUT_KEY_HOME:
      return KB_KEY_HOME;
    case GLUT_KEY_END:
      return KB_KEY_END;
    case GLUT_KEY_INSERT:
      return KB_KEY_INSERT;
    case GLUT_KEY_DELETE:
      return KB_KEY_DELETE;
    default:
      REPORT_ERROR("Unimplemented GLUT key");
      exit(1);
    }

    return KB_KEY_UNDEFINED;
  }


  static MOUSE_BUTTON GLUTMouseToOGLDEVMouse(uint32_t Button)
  {
    switch (Button) {
    case GLUT_LEFT_BUTTON:
      return MOUSE_BUTTON_LEFT;
    case GLUT_RIGHT_BUTTON:
      return MOUSE_BUTTON_RIGHT;
    case GLUT_MIDDLE_BUTTON:
      return MOUSE_BUTTON_MIDDLE;
    default:
      REPORT_ERROR("Unimplemented GLUT mouse button");
    }

    return MOUSE_UNDEFINED;
  }


  static void SpecialKeyboardCB(int Key, int x, int y)
  {
    KEYBOARD_KEY OgldevKey = GLUTKeyToOGLDEVKey(Key);
    s_pCallbacks->KeyboardCB(OgldevKey);
  }


  static void KeyboardCB(unsigned char Key, int x, int y)
  {
    if (((Key >= '0') && (Key <= '9')) || ((Key >= 'A') && (Key <= 'Z')) || ((Key >= 'a') && (Key <= 'z')))
    {
      KEYBOARD_KEY OgldevKey = static_cast<KEYBOARD_KEY>(Key);
      s_pCallbacks->KeyboardCB(OgldevKey);
    }
    else {
      REPORT_ERROR("Unimplemented GLUT key");
    }
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
    s_pCallbacks->RenderSceneCB();
  }


  static void MouseCB(int Button, int State, int x, int y)
  {
    MOUSE_BUTTON OgldevMouse = GLUTMouseToOGLDEVMouse(Button);
    KEYBOARD_KEY_STATE OgldevKeyState = (State == GLUT_DOWN) ? KB_KEY_STATE_PRESS : KB_KEY_STATE_RELEASE;

    s_pCallbacks->MouseCB(OgldevMouse, OgldevKeyState, x, y);
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


  void GLUTBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil)
  {
    sWithDepth = WithDepth;
    sWithStencil = WithStencil;

    glutInit(&argc, argv);

    uint32_t DisplayMode = GLUT_DOUBLE | GLUT_RGBA;

    if (WithDepth) {
      DisplayMode |= GLUT_DEPTH;
    }

    if (WithStencil) {
      DisplayMode |= GLUT_STENCIL;
    }

    glutInitDisplayMode(DisplayMode);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  }


  bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, bool isFullScreen, const char* pTitle)
  {
    if (isFullScreen) {
      char ModeString[64] = { 0 };
      int bpp = 32;
      SNPRINTF(ModeString, sizeof(ModeString), "%dx%d@%d", Width, Height, bpp);
      glutGameModeString(ModeString);
      glutEnterGameMode();
    }
    else {
      glutInitWindowSize(Width, Height);
      glutCreateWindow(pTitle);
    }

    // Must be done after glut is initialized!
    GLenum res = glewInit();
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

    if (sWithDepth) {
      glEnable(GL_DEPTH_TEST);
    }

    s_pCallbacks = pCallbacks;
    InitCallbacks();
    glutMainLoop();
  }


  void GLUTBackendSwapBuffers()
  {
    glutSwapBuffers();
  }

  void GLUTBackendLeaveMainLoop()
  {
    glutLeaveMainLoop();
  }
}