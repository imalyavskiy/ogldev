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

    Tutorial 39 - Silhouette Detection
*/

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

#include "engine_common.h"
#include "ogldev_util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "silhouette_technique.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include <chrono>
#ifdef FREETYPE
#include "freetypeGL.h"
#endif

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

#ifdef FREETYPE
Markup sMarkup = { (char*)"Arial", 64, 1, 0, 0.0, 0.0,
                   {.1,1.0,1.0,.5}, {1,1,1,0},
                   0, {1,0,0,1}, 0, {1,0,0,1},
                   0, {0,0,0,1}, 0, {0,0,0,1} };
#endif

static long long GetCurrentTimeMillis()
{
    const auto duration = std::chrono::system_clock::now().time_since_epoch();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

class Tutorial39 : public ICallbacks
{
public:

    Tutorial39() 
    {
        m_pGameCamera = NULL;
        m_scale = 0.0f;
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.55f;
        m_directionalLight.DiffuseIntensity = 0.9f;
        m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;  
        
        m_frameCount = 0;
        m_fps = 0.0f;
        
        m_boxPos = Vector3f(0.0f, 2.0f, 0.0);
    }

    ~Tutorial39()
    {
        SAFE_DELETE(m_pGameCamera);
    }    

    bool Init()
    {
        Vector3f Pos(0.0f, 2.0f, -7.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
        
        if (!m_silhouetteTech.Init()) {
            printf("Error initializing the silhouette technique\n");
            return false;            
        }
      
        if (!m_LightingTech.Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }
        
        m_LightingTech.Enable();
        
        m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
        m_LightingTech.SetDirectionalLight(m_directionalLight);
        m_LightingTech.SetMatSpecularIntensity(0.0f);
        m_LightingTech.SetMatSpecularPower(0);        

        if (!m_mesh.LoadMesh("../Content/box.obj", true)) {
            printf("Mesh load failed\n");
            return false;            
        }
        
#ifdef FREETYPE
        if (!m_fontRenderer.InitFontRenderer()) {
            return false;
        }
#endif        	
        m_glutTime = glutGet(GLUT_ELAPSED_TIME);
        m_startTime = GetCurrentTimeMillis();
        
        return true;
    }

    void Run()
    {
        GLUTBackendRun(this);
    }
         

    virtual void RenderSceneCB()
    {   
        CalcFPS();
        
        m_scale += 0.01f;
               
        m_pGameCamera->OnRender();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                      
        RenderScene();
              
        RenderFPS();
        
        glutSwapBuffers();
    }

    virtual void IdleCB()
    {
        RenderSceneCB();
    }

    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
            case 'q':
                glutLeaveMainLoop();
                break;
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }
    
    
    virtual void MouseCB(int Button, int State, int x, int y)
    {
    }


private:
           
    void RenderScene()
    {
        // Render the object as-is
        m_LightingTech.Enable();
                                     
        Pipeline p;
        p.SetPerspectiveProj(m_persProjInfo);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());        
        p.WorldPos(m_boxPos);
        m_LightingTech.SetWorldMatrix(p.GetWorldTrans());        
        m_LightingTech.SetWVP(p.GetWVPTrans());        
        m_mesh.Render();
        
        // Render the object's silhouette
        m_silhouetteTech.Enable();
        
        m_silhouetteTech.SetWorldMatrix(p.GetWorldTrans());        
        m_silhouetteTech.SetWVP(p.GetWVPTrans());        
        m_silhouetteTech.SetLightPos(Vector3f(0.0f, 10.0f, 0.0f));
        
        glLineWidth(5.0f);
        
        m_mesh.Render();        
    }        
    
    void CalcFPS()
    {
        m_frameCount++;
        
        int time = glutGet( GLUT_ELAPSED_TIME );

        if (time - m_glutTime > 1000) {
            m_fps = (float)m_frameCount * 1000.0f / (time - m_glutTime);
            m_glutTime = time;
            m_frameCount = 0;
        }
    }
        
    void RenderFPS()
    {
        char text[32];
        ZERO_MEM(text);        
        SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
#ifdef FREETYPE
        m_fontRenderer.RenderText(10, 10, text);        
#endif
    }   
    
    LightingTechnique m_LightingTech;
    SilhouetteTechnique m_silhouetteTech;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
    Vector3f m_boxPos;
    Mesh m_mesh;
    PersProjInfo m_persProjInfo;
#ifdef FREETYPE
    FontRenderer m_fontRenderer;
#endif
    int m_glutTime;
    long long m_startTime;
    int m_frameCount;
    float m_fps;    
};


int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 39")) {
        return 1;
    }
    
    std::srand(GetCurrentProcessId());
    
    Tutorial39* pApp = new Tutorial39();

    if (!pApp->Init()) {
        return 1;
    }
    
    pApp->Run();

    delete pApp;
 
    return 0;
}