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
      return KEYBOARD_KEY::F1;
    case GLUT_KEY_F2:
      return KEYBOARD_KEY::F2;
    case GLUT_KEY_F3:
      return KEYBOARD_KEY::F3;
    case GLUT_KEY_F4:
      return KEYBOARD_KEY::F4;
    case GLUT_KEY_F5:
      return KEYBOARD_KEY::F5;
    case GLUT_KEY_F6:
      return KEYBOARD_KEY::F6;
    case GLUT_KEY_F7:
      return KEYBOARD_KEY::F7;
    case GLUT_KEY_F8:
      return KEYBOARD_KEY::F8;
    case GLUT_KEY_F9:
      return KEYBOARD_KEY::F9;
    case GLUT_KEY_F10:
      return KEYBOARD_KEY::F10;
    case GLUT_KEY_F11:
      return KEYBOARD_KEY::F11;
    case GLUT_KEY_F12:
      return KEYBOARD_KEY::F12;
    case GLUT_KEY_LEFT:
      return KEYBOARD_KEY::LEFT;
    case GLUT_KEY_UP:
      return KEYBOARD_KEY::UP;
    case GLUT_KEY_RIGHT:
      return KEYBOARD_KEY::RIGHT;
    case GLUT_KEY_DOWN:
      return KEYBOARD_KEY::DOWN;
    case GLUT_KEY_PAGE_UP:
      return KEYBOARD_KEY::PAGE_UP;
    case GLUT_KEY_PAGE_DOWN:
      return KEYBOARD_KEY::PAGE_DN;
    case GLUT_KEY_HOME:
      return KEYBOARD_KEY::HOME;
    case GLUT_KEY_END:
      return KEYBOARD_KEY::END;
    case GLUT_KEY_INSERT:
      return KEYBOARD_KEY::INSERT;
    case GLUT_KEY_DELETE:
      return KEYBOARD_KEY::DEL;
    default:
      REPORT_ERROR("Unimplemented GLUT key");
      exit(1);
    }

    return KEYBOARD_KEY::UNDEF;
  }


  static MOUSE_BUTTON GLUTMouseToOGLDEVMouse(uint32_t Button)
  {
    switch (Button) {
    case GLUT_LEFT_BUTTON:
      return MOUSE_BUTTON::LEFT;
    case GLUT_RIGHT_BUTTON:
      return MOUSE_BUTTON::RIGHT;
    case GLUT_MIDDLE_BUTTON:
      return MOUSE_BUTTON::MIDDLE;
    default:
      REPORT_ERROR("Unimplemented GLUT mouse button");
    }

    return MOUSE_BUTTON::UNDEF;
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
    KEYBOARD_KEY_STATE OgldevKeyState = (State == GLUT_DOWN) ? KEYBOARD_KEY_STATE::PRESS : KEYBOARD_KEY_STATE::RELEASE;

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