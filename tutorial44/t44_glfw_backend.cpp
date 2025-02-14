/*

  Copyright 2014 Etay Meiri

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
#include <Windows.h>
#include <GL/glew.h>
//#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "t44_util.h"
#include "t44_glfw_backend.h"

namespace t44
{
  // Points to the object implementing the ICallbacks interface which was delivered to
  // GLUTBackendRun(). All events are forwarded to this object.
  static ICallbacks* s_pCallbacks = nullptr;

  static bool sWithDepth = false;
  static bool sWithStencil = false;
  static GLFWwindow* s_pWindow = nullptr;


  static KEYBOARD_KEY GLFWKeyToOGLDEVKey(uint32_t Key)
  {
    if (Key >= GLFW_KEY_SPACE && Key <= GLFW_KEY_RIGHT_BRACKET) {
      return static_cast<KEYBOARD_KEY>(Key);
    }

    switch (Key) {
    case GLFW_KEY_ESCAPE:
      return KEYBOARD_KEY::ESCAPE;
    case GLFW_KEY_ENTER:
      return KEYBOARD_KEY::ENTER;
    case GLFW_KEY_TAB:
      return KEYBOARD_KEY::TAB;
    case GLFW_KEY_BACKSPACE:
      return KEYBOARD_KEY::BACKSPACE;
    case GLFW_KEY_INSERT:
      return KEYBOARD_KEY::INSERT;
    case GLFW_KEY_DELETE:
      return KEYBOARD_KEY::DEL;
    case GLFW_KEY_RIGHT:
      return KEYBOARD_KEY::RIGHT;
    case GLFW_KEY_LEFT:
      return KEYBOARD_KEY::LEFT;
    case GLFW_KEY_DOWN:
      return KEYBOARD_KEY::DOWN;
    case GLFW_KEY_UP:
      return KEYBOARD_KEY::UP;
    case GLFW_KEY_PAGE_UP:
      return KEYBOARD_KEY::PAGE_UP;
    case GLFW_KEY_PAGE_DOWN:
      return KEYBOARD_KEY::PAGE_DN;
    case GLFW_KEY_HOME:
      return KEYBOARD_KEY::HOME;
    case GLFW_KEY_END:
      return KEYBOARD_KEY::END;
    case GLFW_KEY_F1:
      return KEYBOARD_KEY::F1;
    case GLFW_KEY_F2:
      return KEYBOARD_KEY::F2;
    case GLFW_KEY_F3:
      return KEYBOARD_KEY::F3;
    case GLFW_KEY_F4:
      return KEYBOARD_KEY::F4;
    case GLFW_KEY_F5:
      return KEYBOARD_KEY::F5;
    case GLFW_KEY_F6:
      return KEYBOARD_KEY::F6;
    case GLFW_KEY_F7:
      return KEYBOARD_KEY::F7;
    case GLFW_KEY_F8:
      return KEYBOARD_KEY::F8;
    case GLFW_KEY_F9:
      return KEYBOARD_KEY::F9;
    case GLFW_KEY_F10:
      return KEYBOARD_KEY::F10;
    case GLFW_KEY_F11:
      return KEYBOARD_KEY::F11;
    case GLFW_KEY_F12:
      return KEYBOARD_KEY::F12;
    default:
      REPORT_ERROR("Unimplemented keyboard key");
    }

    return KEYBOARD_KEY::UNDEF;
  }

  static MOUSE_BUTTON GLFWMouseToOGLDEVMouse(uint32_t Button)
  {
    switch (Button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      return MOUSE_BUTTON::LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return MOUSE_BUTTON::RIGHT;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      return MOUSE_BUTTON::MIDDLE;
    default:
      REPORT_ERROR("Unimplemented mouse button");
    }

    return MOUSE_BUTTON::UNDEF;
  }

  static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
  {
    const KEYBOARD_KEY kbKey = GLFWKeyToOGLDEVKey(key);
    const KEYBOARD_KEY_STATE kbKeyState = (action == GLFW_PRESS) ? KEYBOARD_KEY_STATE::PRESS : KEYBOARD_KEY_STATE::RELEASE;
    s_pCallbacks->KeyboardCB(kbKey, kbKeyState);
  }


  static void CursorPosCallback(GLFWwindow* pWindow, double x, double y)
  {
    s_pCallbacks->PassiveMouseCB(static_cast<int>(x), static_cast<int>(y));
  }


  static void MouseButtonCallback(GLFWwindow* pWindow, int Button, int Action, int Mode)
  {
    const MOUSE_BUTTON mouseButton = GLFWMouseToOGLDEVMouse(Button);

    const KEYBOARD_KEY_STATE state = (Action == GLFW_PRESS) ? KEYBOARD_KEY_STATE::PRESS : KEYBOARD_KEY_STATE::RELEASE;

    double x, y;

    glfwGetCursorPos(pWindow, &x, &y);

    s_pCallbacks->MouseCB(mouseButton, state, static_cast<int>(x), static_cast<int>(y));
  }

  static void InitCallbacks()
  {
    glfwSetKeyCallback(s_pWindow, KeyCallback);
    glfwSetCursorPosCallback(s_pWindow, CursorPosCallback);
    glfwSetMouseButtonCallback(s_pWindow, MouseButtonCallback);
  }

  void GLFWErrorCallback(int error, const char* description)
  {
    char msg[1000];
    _snprintf_s(msg, sizeof(msg), "GLFW error %d - %s", error, description);
    MessageBoxA(nullptr, msg, nullptr, 0);

    exit(0);
  }


  void GLFWBackend::Init(int argc, char** argv, bool WithDepth, bool WithStencil)
  {
    sWithDepth = WithDepth;
    sWithStencil = WithStencil;

    glfwSetErrorCallback(GLFWErrorCallback);

    if (glfwInit() != 1) {
      REPORT_ERROR("Error initializing GLFW");
      exit(1);
    }

    int Major, Minor, Rev;

    glfwGetVersion(&Major, &Minor, &Rev);

    printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);
  }


  void GLFWBackend::Terminate()
  {
    glfwDestroyWindow(s_pWindow);
    glfwTerminate();
  }


  bool GLFWBackend::MakeWindow(uint32_t width, uint32_t height, bool isFullScreen, const char* pTitle)
  {
    GLFWmonitor* pMonitor = isFullScreen ? glfwGetPrimaryMonitor() : nullptr;

    s_pWindow = glfwCreateWindow(width, height, pTitle, pMonitor, nullptr);

    if (!s_pWindow) {
      REPORT_ERROR("error creating window");
      exit(1);
    }

    glfwMakeContextCurrent(s_pWindow);

    // Must be done after glfw is initialized!
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      REPORT_ERROR(reinterpret_cast<const char*>(glewGetErrorString(res)));
      exit(1);
    }

    return (s_pWindow != nullptr);
  }

  void GLFWBackend::Run(ICallbacks* pCallbacks)
  {
    if (!pCallbacks) {
      REPORT_ERROR("callbacks not specified");
      exit(1);
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

    while (!glfwWindowShouldClose(s_pWindow)) {
      s_pCallbacks->RenderSceneCB();
      glfwSwapBuffers(s_pWindow);
      glfwPollEvents();
    }
  }


  void GLFWBackend::SwapBuffers()
  {
    // Nothing to do here
  }


  void GLFWBackend::LeaveMainLoop()
  {
    glfwSetWindowShouldClose(s_pWindow, 1);
  }


  void GLFWBackend::SetMousePos(uint32_t x, uint32_t y)
  {
    glfwSetCursorPos(s_pWindow, x, y);
  }
}