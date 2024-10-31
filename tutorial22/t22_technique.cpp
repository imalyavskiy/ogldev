#include <cstdio>
#include <cstring>

#include "t22_technique.h"

namespace t22
{
  Technique::~Technique(){
    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it){
      glDeleteShader(*it);
    }

    if (m_shaderProg != 0){
      glDeleteProgram(m_shaderProg);
      m_shaderProg = 0;
    }
  }

  bool Technique::Init(){
    m_shaderProg = glCreateProgram();

    if (m_shaderProg == 0){
      fprintf(stderr, "Error creating shader program\n");
      return false;
    }

    return true;
  }

  bool Technique::AddShader(GLenum shaderType, const char* pShaderText){
    const GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0){
      fprintf(stderr, "Error creating shader type %d\n", shaderType);
      return false;
    }

    m_shaderObjList.push_back(shaderObj);

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(shaderObj, 1, p, Lengths);

    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success){
      GLchar InfoLog[1024];
      glGetShaderInfoLog(shaderObj, 1024, nullptr, InfoLog);
      fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
      return false;
    }

    glAttachShader(m_shaderProg, shaderObj);

    return true;
  }

  bool Technique::Finalize(){
    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
    if (Success == 0){
      glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), nullptr, ErrorLog);
      fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
      return false;
    }

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (Success == 0){
      glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), nullptr, ErrorLog);
      fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
      return false;
    }

    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it){
      glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return true;
  }

  void Technique::Enable(){
    glUseProgram(m_shaderProg);
  }

  GLint Technique::GetUniformLocation(const char* pUniformName){
    const GLint location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (location == 0xFFFFFFFF){
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return location;
  }
}
