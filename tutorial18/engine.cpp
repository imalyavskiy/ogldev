#include "engine.h"

Main::Main()
{
    m_pGameCamera = NULL;
    m_pTexture = NULL;
    m_pEffect = NULL;
    m_scale = 0.0f;
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.0f;
    m_directionalLight.DiffuseIntensity = 0.75f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
}

Main::~Main()
{
    delete m_pEffect;
    delete m_pGameCamera;
    delete m_pTexture;
}

bool Main::Init()
{
    Vector3f Pos(0.0f, 0.0f, -3.0f);
    Vector3f Target(0.0f, 0.0f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);
    m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

    indices_ = std::vector<Vector3ui>{ { 0, 3, 1 }, { 1, 3, 2 }, { 2, 3, 0 }, { 1, 2, 0 } };

    CreateIndexBuffer(indices_);

    CreateVertexBuffer(indices_);

    m_pEffect = new LightingTechnique();

    if (!m_pEffect->Init())
    {
        printf("Error initializing the lighting technique\n");
        return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetTextureUnit(0);

    m_pTexture = new Texture(GL_TEXTURE_2D, "../Content/test.png");

    if (!m_pTexture->Load()) {
        return false;
    }

    return true;
}

void Main::Run()
{
    GLUTBackendRun(this);
}

void Main::RenderSceneCB()
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
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glutSwapBuffers();
}

void Main::IdleCB()
{
    RenderSceneCB();
}

void Main::SpecialKeyboardCB(int Key, int x, int y)
{
    m_pGameCamera->OnKeyboard(Key);
}


void Main::KeyboardCB(unsigned char Key, int x, int y)
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


void Main::PassiveMouseCB(int x, int y)
{
    m_pGameCamera->OnMouse(x, y);
}

void Main::CalcNormals(const std::vector<Vector3ui>& indices, std::vector<Vertex>& vertices) {
    for (unsigned int i = 0; i < indices.size(); ++i) {
        const GLuint Index0 = indices[i].x;
        const GLuint Index1 = indices[i].y;
        const GLuint Index2 = indices[i].z;
        const Vector3f v1 = vertices[Index1].m_pos - vertices[Index0].m_pos;
        const Vector3f v2 = vertices[Index2].m_pos - vertices[Index0].m_pos;
        const Vector3f Normal = v1.Cross(v2).Normalize();

        vertices[Index0].m_normal += Normal;
        vertices[Index1].m_normal += Normal;
        vertices[Index2].m_normal += Normal;
    }

    for (unsigned int i = 0; i < vertices.size(); i++) {
        vertices[i].m_normal.Normalize();
    }
}


void Main::CreateVertexBuffer(const std::vector<Vector3ui> indices)
{
    vertices_ = std::vector<Vertex>{ Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                                        Vertex(Vector3f(0.0f, -1.0f, -1.15475), Vector2f(0.5f, 0.0f)),
                                        Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                                        Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };

    CalcNormals(indices, vertices_);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);
}

void Main::CreateIndexBuffer(const std::vector<Vector3ui>& indices)
{
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3ui) * indices.size(), &indices[0], GL_STATIC_DRAW);
}