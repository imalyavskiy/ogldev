#include "engine.h"

Engine::Engine()
{
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.0f;
    m_directionalLight.DiffuseIntensity = 0.75f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
}

Engine::~Engine()
{
}

bool Engine::Init()
{
    Vector3f Pos(0.0f, 0.0f, -3.0f);
    Vector3f Target(0.0f, 0.0f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);
    m_pGameCamera = std::auto_ptr<Camera>(new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up));

    CreateVertexBuffer();

    CreateIndexBuffer();

    CalcNormals();

    BindBuffers();

    m_pEffect = std::auto_ptr<LightingTechnique>(new LightingTechnique());

    if (!m_pEffect->Init())
    {
        printf("Error initializing the lighting technique\n");
        return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetTextureUnit(0);

    m_pTexture = std::auto_ptr<Texture>(new Texture(GL_TEXTURE_2D, "../Content/test.png"));

    if (!m_pTexture->Load()) {
        return false;
    }

    return true;
}

void Engine::Run()
{
    GLUTBackendRun(this);
}

void Engine::RenderSceneCB()
{
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT);

    m_scale += 0.1f;

    Pipeline p;
    p.Rotate(0.0f, m_scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 1.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
    m_pEffect->SetWVP(p.GetWVPTrans());
    const Matrix4f& WorldTransformation = p.GetWorldTrans();
    m_pEffect->SetWorldMatrix(WorldTransformation);
    m_pEffect->SetDirectionalLight(m_directionalLight);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    m_pTexture->Bind(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, sizeof(Vector3ui) * indices_.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glutSwapBuffers();
}

void Engine::IdleCB()
{
    RenderSceneCB();
}

void Engine::SpecialKeyboardCB(int Key, int x, int y)
{
    m_pGameCamera->OnKeyboard(Key);
}


void Engine::KeyboardCB(unsigned char Key, int x, int y)
{
    switch (Key) {
    case 'q':
        glutLeaveMainLoop();
        break;

    case 'a':
        m_directionalLight.AmbientIntensity += 0.05f;
        break;

    case 's':
        m_directionalLight.AmbientIntensity -= 0.05f;
        break;

    case 'z':
        m_directionalLight.DiffuseIntensity += 0.05f;
        break;

    case 'x':
        m_directionalLight.DiffuseIntensity -= 0.05f;
        break;
    }
}


void Engine::PassiveMouseCB(int x, int y)
{
    m_pGameCamera->OnMouse(x, y);
}

void Engine::CreateVertexBuffer() {
#if defined(TETRAHEDRON)
    vertices_ = std::vector<Vertex>{ Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                                     Vertex(Vector3f(0.0f, -1.0f, -1.15475), Vector2f(0.5f, 0.0f)),
                                     Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                                     Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };
#elif defined(TORUS)
    std::vector<Vertex> vertices((size_t)SEGMENTS * SIDES);
    std::vector<VertexParams> params((size_t)SEGMENTS * SIDES);

    // x = (R + r * cos(𝜓)) * cos(𝜑);
    // y = (R + r * cos(𝜓)) * sin(𝜑);
    // z = r * sin(𝜓);
    for (GLuint segment = 0; segment < SEGMENTS; ++segment) {
        for (GLuint side = 0; side < SIDES; ++side) {
            const GLfloat psi = (twoPi / SIDES) * side;
            const GLfloat phi = (twoPi / SEGMENTS) * segment;

            const GLfloat cosPsi = cosf(psi);
            const GLfloat sinPsi = sinf(psi);
            const GLfloat cosPhi = cosf(phi);
            const GLfloat sinPhi = sinf(phi);

            const GLuint index = SIDES * segment + side;

            Vector3f& v = vertices[index].m_pos;
            v.x = (r1 + r2 * cosPsi) * cosPhi;
            v.y = (r1 + r2 * cosPsi) * sinPhi;
            v.z = r2 * sinPsi;

            Vector2f& t = vertices[index].m_tex;
            t.x = phi / twoPi;
            t.y = psi / twoPi;

            const float radperdeg = twoPi / 360.0;

            VertexParams& p = params[index];
            p.psi = psi / radperdeg;
            p.phi = phi / radperdeg;
        }
    }
    vertices_ = vertices;
    params_ = params;
#else
#error Nothing to do.
#endif
}

void Engine::CreateIndexBuffer()
{
#if defined(TETRAHEDRON)
    indices_ = std::vector<Vector3ui>{ { 0, 3, 1 }, { 1, 3, 2 }, { 2, 3, 0 }, { 1, 2, 0 } };
#elif defined(TORUS)
    std::vector<Vector3ui> indices(vertices_.size() * 2);

    for (GLuint segment = 0; segment < SEGMENTS; ++segment) {
        for (GLuint side = 0; side < SIDES; ++side) {
            const GLuint index0 = SIDES * segment + side;
            const GLuint index1 = SIDES * segment + (((side + 1) < SIDES) ? side + 1 : 0);
            const GLuint index2 = SIDES * ((segment + 1) < SEGMENTS ? segment + 1 : 0) + (((side + 1) < SIDES) ? side + 1 : 0);
            const GLuint index3 = SIDES * ((segment + 1) < SEGMENTS ? segment + 1 : 0) + side;

            Vector3ui* pTriangle1 = &indices[index0 * 2];
            pTriangle1->x = index2;
            pTriangle1->y = index1;
            pTriangle1->z = index0;

            Vector3ui* pTriangle2 = &indices[index0 * 2 + 1];
            pTriangle2->x = index3;
            pTriangle2->y = index2;
            pTriangle2->z = index0;
        }
    }
    indices_ = indices;
#else
#error Nothing to do.
#endif
}

void Engine::CalcNormals() {
#if defined(TETRAHEDRON) || defined(TORUS)
    for (unsigned int i = 0; i < indices_.size(); ++i) {
        const GLuint Index0 = indices_[i].x;
        const GLuint Index1 = indices_[i].y;
        const GLuint Index2 = indices_[i].z;
        const Vector3f v1 = vertices_[Index1].m_pos - vertices_[Index0].m_pos;
        const Vector3f v2 = vertices_[Index2].m_pos - vertices_[Index0].m_pos;
        const Vector3f Normal = v1.Cross(v2).Normalize();

        vertices_[Index0].m_normal += Normal;
        vertices_[Index1].m_normal += Normal;
        vertices_[Index2].m_normal += Normal;
    }
#else
#error Nothing to do.
#endif

    for (unsigned int i = 0; i < vertices_.size(); i++) {
        vertices_[i].m_normal.Normalize();
    }
}

void Engine::BindBuffers()
{
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3ui) * indices_.size(), &indices_[0], GL_STATIC_DRAW);
}
