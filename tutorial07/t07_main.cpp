/*

  Copyright 2010 Etay Meiri

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

    Tutorial 07 - Rotation transformation
*/

#include <format>
#include <iostream>
#include <cassert>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t07_math_3d.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE  "Totorial 07"

namespace t07
{
  GLuint VBO;

  GLint gWorldLocation;

  static const char* pVertexShader =
  "  #version 330                                  \n"\
  "  layout (location = 0) in vec3 Position;       \n"\
  "  uniform mat4 gWorld;                          \n"\
  "  void main()                                   \n"\
  "  {                                             \n"\
  "    gl_Position = gWorld * vec4(Position, 1.0); \n"\
  "  }                                             \n";

  static const char* pFragmentShader =
  "  #version 330                                  \n"\
  "  out vec4 FragColor;                           \n"\
  "  void main()                                   \n"\
  "  {                                             \n"\
  "    FragColor = vec4(1.0, 0.0, 0.0, 1.0);       \n"\
  "  }                                             \n";

  static void RenderSceneCB()
  {
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr GLuint vertex_attribute_index = 0;
    // This value correlates to value of Position attribute mentioned in vertex
    // shader

    static float scaleValue = 0.0f;

    scaleValue += 0.005f;

    Matrix4f world;

    // |        Rotate in XOY plane      |       Rotate in XOZ plane       |        Rotate in YOZ plane      |
    // |
    // |  cos(a)  sin(a)    0       0    |  cos(a)    0     sin(a)    0    |    1       0       0       0    |
    // | -sin(a)  cos(a)    0       0    |    0       1       0       0    |    0     cos(a)  sin(a)    0    |
    // |    0       0       1       0    | -sin(a)    0     cos(a)    0    |    0    -sin(a)  cos(a)    0    |
    // |    0       0       0       1    |    0       0       0       1    |    0       0       0       1    |

    // Rotating in XOY plane (around Z axis)
    world.m[0][0] = cosf(scaleValue); world.m[0][1] = -sinf(scaleValue); world.m[0][2] = 0.0f; world.m[0][3] = 0.0f;  //  cos(a) -sin(a)    0       0
    world.m[1][0] = sinf(scaleValue); world.m[1][1] = cosf(scaleValue);  world.m[1][2] = 0.0f; world.m[1][3] = 0.0f;  //  sin(a)  cos(a)    0       0
    world.m[2][0] = 0.0f;             world.m[2][1] = 0.0f;              world.m[2][2] = 1.0f; world.m[2][3] = 0.0f;  //    0       0       1       0
    world.m[3][0] = 0.0f;             world.m[3][1] = 0.0f;              world.m[3][2] = 0.0f; world.m[3][3] = 1.0f;  //    0       0       0       1

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &world.m[0][0]);

    glEnableVertexAttribArray(vertex_attribute_index);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(vertex_attribute_index);

    glutSwapBuffers();
  }

  static void InitializeGlutCallbacks()
  {
    glutDisplayFunc(RenderSceneCB);

    glutIdleFunc(RenderSceneCB);
  }

  static void CreateVertexBuffer()
  {
    Vector3f vertices[3]{
      { -1.0f, -1.0f,  0.0f },
      {  1.0f, -1.0f,  0.0f },
      {  0.0f,  1.0f,  0.0f }
    };

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  }

  static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
  {
    const GLuint shaderObj = glCreateShader(ShaderType);

    if (shaderObj == 0) {
      std::cerr << std::format("Error creating shader type {}\n", ShaderType);
      exit(0);
    }

    const GLchar* translationUnits[] = { pShaderText };
    const GLint   translationUnitLengths[] = { static_cast<GLint>(strlen(pShaderText)) };
    const GLint   translationUnitsNumber = sizeof(translationUnits) / sizeof(translationUnits[0]);
    glShaderSource(shaderObj, translationUnitsNumber, translationUnits, translationUnitLengths);

    glCompileShader(shaderObj);

    GLint success = 0;

    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
      GLchar InfoLog[1024];

      glGetShaderInfoLog(shaderObj, 1024, nullptr, InfoLog);
      std::cerr << std::format("Error compiling shader type {}: '{}'\n", ShaderType, InfoLog);
      exit(1);
    }

    glAttachShader(ShaderProgram, shaderObj);
  }

  static void CompileShaders()
  {
    const GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0) {
      std::cerr << std::format("Error creating shader program\n");
      exit(1);
    }

    AddShader(shaderProgram, pVertexShader, GL_VERTEX_SHADER);

    AddShader(shaderProgram, pFragmentShader, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
      glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
      std::cerr << std::format("Error linking shader program: '{}'\n", ErrorLog);
      exit(1);
    }

    glValidateProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
      glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
      std::cerr << std::format("Invalid shader program: '{}'\n", ErrorLog);
      exit(1);
    }

    glUseProgram(shaderProgram);

    gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");

    assert(static_cast<GLuint>(gWorldLocation) != 0xFFFFFFFF);
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutInitWindowPosition(100, 100);

  glutCreateWindow(WINDOW_TITLE);

  t07::InitializeGlutCallbacks();

  GLenum res = glewInit();
  if (res != GLEW_OK) {
    std::cerr << std::format("Error: '{}'\n", reinterpret_cast<const char*>(glewGetErrorString(res)));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  t07::CreateVertexBuffer();

  t07::CompileShaders();

  glutMainLoop();

  return 0;
}