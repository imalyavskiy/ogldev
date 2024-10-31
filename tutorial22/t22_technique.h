#ifndef TEXHNIQUE_H
#define TEXHNIQUE_H

#include <list>

#include <GL/glew.h>

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

namespace t22
{
  class Technique
  {
  public:
    Technique() = default;

    virtual ~Technique();

    virtual bool Init();

    void Enable();

  protected:
    bool AddShader(GLenum shaderType, const char* pShaderText);

    bool Finalize();

    GLint GetUniformLocation(const char* pUniformName);

  private:
    GLuint m_shaderProg = 0;
    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
  };
}

#endif /* TEXHNIQUE_H */
