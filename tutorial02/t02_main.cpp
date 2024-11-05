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

    Tutorial 02 - Hello dot!
*/

#include <iostream>
#include <format>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t02_math_3d.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE  "Tutorial 02"

namespace t02
{
  GLuint VBO;

  static void RenderSceneCB()
  {
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr GLuint vertex_attribute_index = 0;

    glEnableVertexAttribArray(vertex_attribute_index);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    constexpr GLuint num_components = 3;
    constexpr GLenum component_type = GL_FLOAT;
    constexpr GLboolean normalized = GL_FALSE;
    constexpr GLsizei stride = 0;

    glVertexAttribPointer(vertex_attribute_index, num_components, component_type, normalized, stride, nullptr);

    constexpr GLenum draw_mode = GL_POINTS;
    constexpr GLuint starting_index = 0;
    constexpr GLuint num_points = 1;
    glDrawArrays(draw_mode, starting_index, num_points);

    glDisableVertexAttribArray(vertex_attribute_index);

    glutSwapBuffers();
  }

  static void InitializeGlutCallbacks()
  {
    glutDisplayFunc(RenderSceneCB);
  }

  static void CreateVertexBuffer()
  {
    const Vector3f Vertices[] = { Vector3f(0.0f, 0.0f, 0.0f) };

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutInitWindowPosition(100, 100);

  glutCreateWindow(WINDOW_TITLE);

  t02::InitializeGlutCallbacks();

  const GLenum result = glewInit();
  if (result != GLEW_OK) {
    std::cerr << std::format("Error: \"{}\"\n", reinterpret_cast<const char*>(glewGetErrorString(result)));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  t02::CreateVertexBuffer();

  glutMainLoop();

  return 0;
}
