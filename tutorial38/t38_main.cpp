/*

        Copyright 2011 Etay Meiri

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

    Tutorial 38 - Skinning
*/

#include <cmath>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t38_engine_common.h"
#include "t38_app.h"
#include "t38_camera.h"
#include "t38_util.h"
#include "t38_pipeline.h"
#include "t38_camera.h"

#include "t38_skinning_technique.h"
#include "t38_glut_backend.h"
#include "t38_skinned_mesh.h"

using namespace std;

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

// Workaround for tutorials prior to switching to GLFW
int IsGLVersionHigher(int MajorVer, int MinorVer)
{
    return false;
}

class Tutorial38 : public ICallbacks, public OgldevApp
{
public:

    Tutorial38()
    {
        m_pGameCamera = NULL;
        m_pEffect = NULL;
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.55f;
        m_directionalLight.DiffuseIntensity = 0.9f;
        m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;

        m_position = Vector3f(0.0f, 0.0f, 6.0f);
    }

    ~Tutorial38()
    {
        SAFE_DELETE(m_pEffect);
        SAFE_DELETE(m_pGameCamera);
    }

    bool Init()
    {
        Vector3f Pos(0.0f, 3.0f, -1.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        m_pEffect = new SkinningTechnique();

        if (!m_pEffect->Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pEffect->Enable();

        m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
        m_pEffect->SetDirectionalLight(m_directionalLight);
        m_pEffect->SetMatSpecularIntensity(0.0f);
        m_pEffect->SetMatSpecularPower(0);

        if (!m_mesh.LoadMesh("../Content/boblampclean.md5mesh")) {
            printf("Mesh load failed\n");
            return false;
        }

#ifndef WIN32
        //if (!m_fontRenderer.InitFontRenderer()) {
            //return false;
        //}
#endif
        return true;
    }

    void Run()
    {
        GLUTBackendRun(this);
    }


    virtual void RenderSceneCB()
    {
        CalcFPS();

        m_pGameCamera->OnRender();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pEffect->Enable();

        vector<Matrix4f> Transforms;

        float RunningTime = GetRunningTime();

        m_mesh.GetBoneTransforms(RunningTime, Transforms);

        for (uint i = 0 ; i < Transforms.size() ; i++) {
            m_pEffect->SetBoneTransform(i, Transforms[i]);
        }

        m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

        Pipeline p;
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(m_persProjInfo);
        p.Scale(0.1f, 0.1f, 0.1f);

        Vector3f Pos(m_position);
        p.WorldPos(Pos);
        p.Rotate(270.0f, 180.0f, 0.0f);
        m_pEffect->SetWVP(p.GetWVPTrans());
        m_pEffect->SetWorldMatrix(p.GetWorldTrans());

        m_mesh.Render();

        //        RenderFPS();

        glutSwapBuffers();
    }


        virtual void KeyboardCB(KEYBOARD_KEY OgldevKey, KEYBOARD_KEY_STATE State)
        {
                switch (OgldevKey) {
                case OGLDEV_KEY_ESCAPE:
                case OGLDEV_KEY_q:
                        GLUTBackendLeaveMainLoop();
                        break;
                default:
                        m_pGameCamera->OnKeyboard(OgldevKey);
                }
        }


        virtual void PassiveMouseCB(int x, int y)
        {
                m_pGameCamera->OnMouse(x, y);
        }


private:

    SkinningTechnique* m_pEffect;
    Camera* m_pGameCamera;
    DirectionalLight m_directionalLight;
    SkinnedMesh m_mesh;
    Vector3f m_position;
    PersProjInfo m_persProjInfo;
};


int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 38")) {
        return 1;
    }

    SRANDOM;

    Tutorial38* pApp = new Tutorial38();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}
