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

    Tutorial 13 - Camera Space
*/

#include <format>
#include <iostream>
#include <cassert>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t13_pipeline.h"

#define VERTICAL_FOV 60.f
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE "Tutorial 13"
#define Z_NEAR_CLIPPING_PLANE 1.f
#define Z_FAR_CLIPPING_PLANE 100.f

namespace t13
{
  GLuint VBO;

  GLuint IBO;

  GLuint gWVPLocation;

  static const char* pVertexShader =
  "  #version 330                                  \n"\
  "  layout (location = 0) in vec3 Position;       \n"\
  "  uniform mat4 gWVP;                            \n"\
  "  out vec4 Color;                               \n"\
  "  void main()                                   \n"\
  "  {                                             \n"\
  "      gl_Position = gWVP * vec4(Position, 1.0); \n"\
  "      vec3 clamped = clamp(Position, 0.0, 1.0); \n"\
  "      Color = vec4(clamped, 1.0);               \n"\
  "  }                                             \n";

  static const char* pFragmentShader =
  "  #version 330                                  \n"\
  "  in vec4 Color;                                \n"\
  "  out vec4 FragColor;                           \n"\
  "  void main()                                   \n"\
  "  {                                             \n"\
  "      FragColor = Color;                        \n"\
  "  }                                             \n";

  static void RenderSceneCB()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    static float Scale = 0.0f;

    Scale += 0.05f;

    Pipeline pipeline;

    pipeline.Rotate(0.0f, Scale, 0.0f);

    pipeline.WorldPos(0.0f, 0.0f, 0.0f);

    const Vector3f CameraPos(  0.0f,  0.0f, -1.5f );
    const Vector3f CameraTarget(  0.0f,  0.0f,  2.0f );
    const Vector3f CameraUp(  0.0f,  1.0f,  0.0f );
    pipeline.SetCamera(CameraPos, CameraTarget, CameraUp);

    pipeline.SetPerspectiveProj(VERTICAL_FOV, WINDOW_WIDTH, WINDOW_HEIGHT, Z_NEAR_CLIPPING_PLANE, Z_FAR_CLIPPING_PLANE);

    const Matrix4f& wvp = pipeline.GetTrans();

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(&wvp));

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 48 * 3, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
  }

  static void InitializeGlutCallbacks()
  {
    glutDisplayFunc(RenderSceneCB);

    glutIdleFunc(RenderSceneCB);
  }

  static void CreateVertexBuffer()
  {
    const Vector3f Vertices[] =
    {
      /* 0*/	{ -0.2500f, -0.2500f, -0.2500f },	/* 1*/	{ -0.0000f, -0.1001f, -0.1001f },	/* 2*/	{  0.2500f, -0.2500f, -0.2500f },
      /* 3*/	{ -0.1001f, -0.0000f, -0.1001f },	/* 4*/	{  0.0000f,  0.0000f, -0.3247f },	/* 5*/	{  0.1001f, -0.0000f, -0.1001f },
      /* 6*/	{ -0.2500f,  0.2500f, -0.2500f },	/* 7*/	{ -0.0000f,  0.1001f, -0.1001f },	/* 8*/	{  0.2500f,  0.2500f, -0.2500f },
      /* 9*/	{ -0.2500f, -0.2500f,  0.2500f },	/*10*/	{ -0.0000f, -0.1001f,  0.1001f },	/*11*/	{  0.2500f, -0.2500f,  0.2500f },
      /*12*/	{ -0.1001f, -0.0000f,  0.1001f },	/*13*/	{  0.0000f,  0.0000f,  0.3248f },	/*14*/	{  0.1001f, -0.0000f,  0.1001f },
      /*15*/	{ -0.2500f,  0.2500f,  0.2500f },	/*16*/	{ -0.0000f,  0.1001f,  0.1001f },	/*17*/	{  0.2500f,  0.2500f,  0.2500f },
      /*18*/	{ -0.1001f, -0.1001f,  0.0000f },	/*19*/	{  0.0000f, -0.3248f, -0.0000f },	/*20*/	{  0.1001f, -0.1001f,  0.0000f },
      /*21*/	{  0.3248f,  0.0000f, -0.0000f },	/*22*/	{  0.1001f,  0.1001f,  0.0000f },	/*23*/	{  0.0000f,  0.3248f, -0.0000f },
      /*24*/	{ -0.1001f,  0.1001f,  0.0000f },	/*25*/	{ -0.3248f,  0.0000f, -0.0000f },
    };


    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  }

  static void CreateIndexBuffer()
  {
    const unsigned int Indices[] =
    {
      /* 0*/  0,  3,  1,	/* 1*/  1,  5,  2,	/* 2*/  3,  6,  7,	/* 3*/  7,  8,  5,	/* 4*/ 10, 13, 12,	/* 5*/ 10, 14, 13,	/* 6*/ 12, 13, 16,	/* 7*/ 13, 14, 16,
      /* 8*/  0,  1, 18,	/* 9*/  1,  2, 20,	/*10*/ 18, 10,  9,	/*11*/ 20, 11, 10,	/*12*/  2,  5, 20,	/*13*/  5,  8, 22,	/*14*/ 20, 14, 11,	/*15*/ 22, 17, 14,
      /*16*/  8,  7, 22,	/*17*/  7,  6, 24,	/*18*/ 22, 16, 17,	/*19*/ 24, 15, 16,	/*20*/  3, 25, 24,	/*21*/  3, 18, 25,	/*22*/ 24, 25, 12,	/*23*/ 25, 18, 12,
      /*24*/ 12, 15, 24,	/*25*/ 24,  6,  3,	/*26*/  3,  0, 18,	/*27*/ 18,  9, 12,	/*28*/ 18, 19, 10,	/*29*/ 10, 19, 20,	/*30*/ 20, 19,  1,	/*31*/  1, 19, 18,
      /*32*/ 12,  9, 10,	/*33*/ 10, 11, 14,	/*34*/ 14, 17, 16,	/*35*/ 16, 15, 12,	/*36*/ 20, 21, 14,	/*37*/ 14, 21, 22,	/*38*/ 22, 21,  5,	/*39*/  5, 21, 20,
      /*40*/ 22, 23, 16,	/*41*/ 16, 23, 24,	/*42*/ 24, 23,  7,	/*43*/  7, 23, 22,	/*44*/  1,  4,  5,	/*45*/  5,  4,  7,	/*46*/  7,  4,  3,	/*47*/  3,  4,  1,
    };

    glGenBuffers(1, &IBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  }

  static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
  {
    const GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
      std::cerr << std::format("Error creating shader type {}\n", ShaderType);
      exit(0);
    }

    const GLchar* p[] = { pShaderText };
    const GLint Lengths[] = { static_cast<GLint>(strlen(pShaderText)) };

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success = 0;

    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
      GLchar InfoLog[1024];

      glGetShaderInfoLog(ShaderObj, 1024, nullptr, InfoLog);
      std::cerr << std::format("Error compiling shader type {}: '{}'\n", ShaderType, InfoLog);
      exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
  }

  static void CompileShaders()
  {
    const GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
      std::cerr << std::format("Error creating shader program\n");
      exit(1);
    }

    AddShader(ShaderProgram, pVertexShader, GL_VERTEX_SHADER);

    AddShader(ShaderProgram, pFragmentShader, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
      glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
      std::cerr << std::format("Error linking shader program: '{}'\n", ErrorLog);
      exit(1);
    }

    glValidateProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
      glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
      std::cerr << std::format("Invalid shader program: '{}'\n", ErrorLog);
      exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");

    assert(gWVPLocation != 0xFFFFFFFF);
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutInitWindowPosition(100, 100);

  glutCreateWindow(WINDOW_TITLE);

  t13::InitializeGlutCallbacks();

  const GLenum res = glewInit();
  if (res != GLEW_OK) {
    std::cerr << std::format("Error: '{}'\n", reinterpret_cast<const char*>(glewGetErrorString(res)));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f );

  t13::CreateVertexBuffer();

  t13::CreateIndexBuffer();

  t13::CompileShaders();

  glutMainLoop();

  return 0;
}