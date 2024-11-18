#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t16_pipeline.h"
#include "t16_camera.h"
#include "t16_texture.h"


#define VERTICAL_FOV 60.f
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define Z_NEAR_CLIPPING_PLANE 1.f
#define Z_FAR_CLIPPING_PLANE 100.f

namespace t16
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() = default;

    Vertex(Vector3f pos, Vector2f tex)
    {
      m_pos = pos;
      m_tex = tex;
    }
  };
}


GLuint VBO;
GLuint IBO;
GLuint gWVPLocation;
GLuint gSampler;
t16::Texture* pTexture = nullptr;
t16::Camera* pGameCamera = nullptr;

static const char* pVS =
"  #version 330                                                                        \n"\
"                                                                                      \n"\
"  layout (location = 0) in vec3 Position;                                             \n"\
"  layout (location = 1) in vec2 TexCoord;                                             \n"\
"                                                                                      \n"\
"  uniform mat4 gWVP;                                                                  \n"\
"                                                                                      \n"\
"  out vec2 TexCoord0;                                                                 \n"\
"                                                                                      \n"\
"  void main()                                                                         \n"\
"  {                                                                                   \n"\
"      gl_Position = gWVP * vec4(Position, 1.0);                                       \n"\
"      TexCoord0 = TexCoord;                                                           \n"\
"  }                                                                                     ";

static const char* pFS =
"  #version 330                                                                        \n"\
"                                                                                      \n"\
"  in vec2 TexCoord0;                                                                  \n"\
"                                                                                      \n"\
"  out vec4 FragColor;                                                                 \n"\
"                                                                                      \n"\
"  uniform sampler2D gSampler;                                                         \n"\
"                                                                                      \n"\
"  void main()                                                                         \n"\
"  {                                                                                   \n"\
"      FragColor = texture2D(gSampler, TexCoord0.xy);                                  \n"\
"  }                                                                                     ";

static void RenderSceneCB()
{
  pGameCamera->OnRender();

  glClear(GL_COLOR_BUFFER_BIT);

  static float Scale = 0.0f;

  Scale += 0.1f;

  t16::Pipeline pipeline;
  pipeline.Rotate(0.0f, Scale, 0.0f);
  pipeline.WorldPos(0.0f, 0.0f, 3.0f);
  pipeline.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
  pipeline.SetPerspectiveProj(VERTICAL_FOV, WINDOW_WIDTH, WINDOW_HEIGHT, Z_NEAR_CLIPPING_PLANE, Z_FAR_CLIPPING_PLANE);

  glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(pipeline.GetTrans()));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t16::Vertex), nullptr);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(t16::Vertex), reinterpret_cast<const GLvoid*>(12));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  pTexture->ActivateAndBind(GL_TEXTURE0);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glutSwapBuffers();
}


static void SpecialKeyboardCB(int key, int x, int y)
{
  pGameCamera->OnKeyboard(key);
}


static void KeyboardCB(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 0x1b: // Esc
      glutLeaveMainLoop();
      break;
  }
}


static void PassiveMouseCB(int x, int y)
{
  pGameCamera->OnMouse(x, y);
}


static void InitializeGlutCallbacks()
{
  glutDisplayFunc(RenderSceneCB);
  glutIdleFunc(RenderSceneCB);
  glutSpecialFunc(SpecialKeyboardCB);
  glutPassiveMotionFunc(PassiveMouseCB);
  glutKeyboardFunc(KeyboardCB);
}


static void CreateVertexBuffer()
{
  const t16::Vertex vertices[4] = 
  {
      t16::Vertex(t16::Vector3f(-1.0f, -1.0f,  0.57730f), t16::Vector2f(0.0f, 0.0f)),
      t16::Vertex(t16::Vector3f(0.0f, -1.0f, -1.15475f), t16::Vector2f(0.5f, 0.0f)),
      t16::Vertex(t16::Vector3f(1.0f, -1.0f,  0.57730f), t16::Vector2f(1.0f, 0.0f)),
      t16::Vertex(t16::Vector3f(0.0f,  1.0f,  0.00000f), t16::Vector2f(0.5f, 1.0f))
  };

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}


static void CreateIndexBuffer()
{
  const uint32_t indices[] = { 0, 3, 1,  1, 3, 2,  2, 3, 0,  1, 2, 0 };

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


static void AddShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
  const GLuint shaderObj = glCreateShader(shaderType);

  if (shaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", shaderType);
    exit(0);
  }

  const GLchar* p[1];
  p[0] = pShaderText;
  GLint lengths[1];
  lengths[0] = strlen(pShaderText);
  glShaderSource(shaderObj, 1, p, lengths);
  glCompileShader(shaderObj);
  GLint success;
  glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(shaderObj, 1024, nullptr, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
    exit(1);
  }

  glAttachShader(shaderProgram, shaderObj);
}


static void CompileShaders()
{
  const GLuint shaderProgram = glCreateProgram();

  if (shaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }

  AddShader(shaderProgram, pVS, GL_VERTEX_SHADER);
  AddShader(shaderProgram, pFS, GL_FRAGMENT_SHADER);

  GLint success = 0;
  GLchar errorLog[1024] = { 0 };

  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(shaderProgram, sizeof(errorLog), nullptr, errorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
    exit(1);
  }

  glValidateProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, sizeof(errorLog), nullptr, errorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
    exit(1);
  }

  glUseProgram(shaderProgram);

  gWVPLocation = glGetUniformLocation(shaderProgram, "gWVP");
  assert(gWVPLocation != 0xFFFFFFFF);
  gSampler = glGetUniformLocation(shaderProgram, "gSampler");
  assert(gSampler != 0xFFFFFFFF);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Tutorial 16");
  glutGameModeString("1280x1024@32");
  glutEnterGameMode();

  InitializeGlutCallbacks();

  pGameCamera = new t16::Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

  // Must be done after glut is initialized!
  const GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  CreateVertexBuffer();
  CreateIndexBuffer();

  CompileShaders();

  glUniform1i(gSampler, 0); // index of the texture unit to use

  const std::string textureFileName("../Content/test.png");
  pTexture = new t16::Texture(GL_TEXTURE_2D, textureFileName);

  if (!pTexture->Load()) {
    std::cerr << "[ FATAL ] Failed to load image: " << textureFileName << "\n";
    return 1;
  }

  glutMainLoop();

  return 0;
}
