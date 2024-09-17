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

#include <cstdio>
#include <cstdint>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "math_3d.h"

GLuint VBO;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr GLuint vertex_attribute_index = 0;

    glEnableVertexAttribArray(vertex_attribute_index);
      // IM: not using shaders and thus vertex buffer must be inked as vertex attribute with 0 index
      // This limitation is not yet clear.

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
      // IM: Binding our VertexBufferObject to a specific target once more. This is not required at the
      // moment but is in case of multiple buffers, I suppose.

    constexpr GLuint num_components = 3;
    constexpr GLenum component_type = GL_FLOAT;
    constexpr GLboolean normalized = GL_FALSE; // not yet clear
    constexpr GLsizei stride = 0; // an offset between two adjacent component items with the buffer. This
                                  // is useful in case of packeted buffers where e.g. coordinates interlaced
                                  // with normals and/or texture coords
    glVertexAttribPointer(vertex_attribute_index, num_components, component_type, normalized, stride, nullptr);
      // IM: Instructing the convey how to treat the buffer.

    constexpr GLenum draw_mode = GL_POINTS; // individual vertexes
    constexpr GLuint starting_index = 0;    // starting from index 0
    constexpr GLuint num_points = 1;        // just single vertex
    glDrawArrays( draw_mode, starting_index, num_components);
      // IM: The drawing itself. The ordered drawing.
      // All the above is just a set up.

    glDisableVertexAttribArray(vertex_attribute_index);
      // IM: Turning vertex attribute index off.

    glutSwapBuffers();
      // IM: Swapping buffers. We've initialized the system with double buffering, the are BG and FG. This means - draw in the BG buffer,
      // show the FG buffer and swap them when BG is fulfilled.
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
    const Vector3f Vertices[] = { Vector3f(0.0f, 0.0f, 0.0f) };
      // IM: Creating our own data buffer

    glGenBuffers( 1, &VBO );
      // IM: Get new buffer object

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
      // IM: Bind the buffer object to a specific target. This is somehow related with shaders.

    glBufferData( GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW );
      // IM: Linking together the Vertex Buffer Object and our data buffer. Last parameter means that the data buffer is of constant size and data.
      // Note: Note there are no VBO in the params but just the binding target (GL_ARRAY_BUFFER) that was used to bind to VBO.
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA ); // double buffering, rgba color space

    glutInitWindowSize(1024, 768);

    glutInitWindowPosition(100, 100);

    glutCreateWindow("Tutorial 02");

    InitializeGlutCallbacks();

    const GLenum result = glewInit();
    if (result != GLEW_OK) {
        fprintf(stderr, "Error: \"%s\"\n", glewGetErrorString(result));
        return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // opaque black

    CreateVertexBuffer();

    glutMainLoop();

    return 0;
}
