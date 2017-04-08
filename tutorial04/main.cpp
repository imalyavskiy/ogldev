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

    Tutorial 04 - shaders
*/

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"

// ������������� ������� ������ ������
GLuint VBO;

// �������� ��� ���������� �������
static const char* pVS =
// ������������ GLSL ������� 3.3
"#version 330                                                                  \n"\
// ������� �������� �������, ���������� �������� �� 3 ��������� ���� floats, ������� ����� ����� ��� 'Position' ������ �������
// "layout (location = 0)" ������ ����� ����� ��������� �������� � ��������� � ������
"layout (location = 0) in vec3 Position;                                       \n"\
// ����� ����� � ������
"void main()                                                                   \n"\
"{                                                                             \n"\
// �������� ���������� ������� � ���������� ��������� � "vec4 gl_Position"
// "gl_Position" - ��������� ����������, ���������� ��������� ���������� (X, Y, Z � W ����������)
"    gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);  \n"\
"}                                                                             \n";

// �������� ��� ������������ �������
static const char* pFS =
// ������������ GLSL ������ 3.3
"#version 330                                                                  \n"\
// �������� �������� vec4(R,G,B,A)
"out vec4 FragColor;                                                           \n"\
//
"void main()                                                                   \n"\
"{                                                                             \n"\
// ����� ����, � ������ ������ �������
// 0.0 <= R,G,B,A <= 1.0
"    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                     \n"\
"}                                                                             \n";

// ������� ���������� ������ �����
static void RenderSceneCB()
{
	// ������� ����� �����
    glClear(GL_COLOR_BUFFER_BIT);

	// ���������� ������, ������������ � ������, ��������������� ��� ������� ������� � �������� 0 
	// ������� ��������� ���������� ��������� ������� 0
    glEnableVertexAttribArray(0);
	
	// ���������� �����, ��� ����� ������(GL_ARRAY_BUFFER)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// ������� ��������� ��� ������������ ������ ������ ������
    glVertexAttribPointer( 0		// ������ �������� � �������� ������ ������
						 , 3		// ���������� ��������� � ��������
						 , GL_FLOAT // ��� ���������
						 , GL_FALSE // ����� ������������ - ����.
						 , 0		// ��� - ����� ���� ����� ������������ ��������
						 , 0		// �������� ������� �������� � ������ � ������.
						 );

	// ���������� ������ ������ ������
    glDrawArrays( GL_TRIANGLES	// ��� ��������� - ������������(������ 3�� ������ - �����������).
				, 0				// ������ ������� �������� ������ ������.
				, 3				// ���������� ��������� ��� ���������.
				);
	
	// ��������� ������ � ���������� ������� 0
    glDisableVertexAttribArray(0);

	// ������ ������� ������ ������(� ��� �� ���, �� �������� ������� ������������).
    glutSwapBuffers();
}

// ������� ����������� callback'��
static void InitializeGlutCallbacks()
{
	// ������������� callback ������� ���������� ������ �����
    glutDisplayFunc(RenderSceneCB);
}

// ������� �������� ������� ������
static void CreateVertexBuffer()
{
	// ������ �� ��� �������
    Vector3f Vertices[3];

	// ������� ��������� ������
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	// ����������� � OpenGL �������� ������� ������
 	glGenBuffers( 1					// ���������� �������
				, &VBO );			// ���� ������ GLuint'�� ���� �� ����������
	
	// ���������� �����, ��� ����� ������(GL_ARRAY_BUFFER)
	glBindBuffer( GL_ARRAY_BUFFER	// ������������ ����� �����������
				, VBO				// ID ������
				);

	// �������� ������ �� ������� ������ � ������ ��������� OpenGL
	// (�������� ����� ���������� ���������� ������ �� RAM � VRAM)	
	glBufferData( GL_ARRAY_BUFFER	// ��� ������
				, sizeof(Vertices)  // ������ ������� � ���������
				, Vertices          // ����� ������� � ���������
				, GL_STATIC_DRAW    // ������ ������ ��������(���������)
				);
}

// ������� ���������� ������� � ��������� ���������
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// �������� � OpenGL ������� ������ �������
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
		// ���-�� ����� �� ���
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

	const GLchar* p[] = { pShaderText };
    GLint Lengths[]   = { (GLint)strlen(pShaderText) };

	// �������� ������ ������� ���������
    glShaderSource( ShaderObj // �������������
				  , 1		  // ���-�� ��� ���/����� � ������������� ��������
				  , p		  // ��� ��������
				  , Lengths	  // ������ ���� �������
				  ); 

	// �������� ������� ����������
    glCompileShader(ShaderObj);

    GLint success = 0;

	// �������� � ������� ������� �������� - ��������� ����������
    glGetShaderiv( ShaderObj			// ������������� ������� �������
				 , GL_COMPILE_STATUS	// ������������� �������������� ���������
				 , &success				// ���� �������� �������� ���������
				 );

    if (!success) {
		// ���� ���� ������ ���� � �������
        GLchar InfoLog[1024];

		// �������� � ������� ���
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

	// �� ������ �������. 
	// ������� ���������������� ������ � ������ ���������
    glAttachShader(ShaderProgram, ShaderObj);
}

// ������� ������� ��������
static void CompileShaders()
{
	// �������� � OpenGL ������� ������ ���������� ���������
    GLuint ShaderProgram = glCreateProgram();

	// ��������� ��� �� ��� �� �����
    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

	// ��������� � ��������� �������� ��� ���������� ������� 
    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);

	// ��������� � ��������� �������� ��� ������������ ������� 
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

	// ������� ���������
    glLinkProgram(ShaderProgram);

	// ����������� ��������� � ���
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		// ���-�� ����� �� ���
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

	// ��������� ������ �� ��������� ����������� � ������� ���������� ���������.
    glValidateProgram(ShaderProgram);

	// ����������� ��������� � ���
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
		// ���-�� ����� �� ���
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

	// ���������� ��������� � ������������ ���������
    glUseProgram(ShaderProgram);
}

// ���������� ����� �����
int main(int argc, char** argv)
{
	// ������������� GLUT
    glutInit(&argc, argv);
	
	// �������� ����� ������� �����������(GLUT_DOUBLE) � ����� ������ ������(GLUT_RGBA, �� ��������������� ����� �������������).
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	
	// ����� ������ ����
    glutInitWindowSize(1024, 768);
	
	// ����� ������� �������� ������ ���� ������ ����
    glutInitWindowPosition(100, 100);
	
	// ������ ���� � ��������� ������
    glutCreateWindow("Tutorial 04");

	// ������������ callback'�
    InitializeGlutCallbacks();

    // �������������� GLEW.
	// ��������: ���� ��� ������������� ������������ ������ ����� ������������� GLUT.
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

	// ����� ���� ������� ������ �����
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// ������ ����� ������
    CreateVertexBuffer();

	// ����� �������
    CompileShaders();

	// ������� ���������� �������� ����� GLUT
    glutMainLoop();

    return 0;
}


