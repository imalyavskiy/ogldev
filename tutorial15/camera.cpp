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
*/
#include <GL/freeglut.h>

#include "camera.h"

const static float StepScale = 0.1f;

const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight)
{
    m_pos	 = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_up	 = Vector3f(0.0f, 1.0f, 0.0f);

    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_target.Normalize();

    Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
    m_pos    = Pos;

    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;

    m_target = Target;
    m_target.Normalize();
    m_up     = Up;
    m_up.Normalize();

    Init();
}

// TODO: comment
void Camera::Init()
{
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize();
    
    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = 360.0f - ToDegree((float)asin(HTarget.z));
        }
        else
        {
            m_AngleH = 180.0f + ToDegree((float)asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = ToDegree((float)asin(-HTarget.z));
        }
        else
        {
            m_AngleH = 90.0f + ToDegree((float)asin(-HTarget.z));
        }
    }
    
    m_AngleV = -ToDegree((float)asin(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge  = false;
    m_OnRightEdge = false;
    m_mousePos.x  = m_windowWidth / 2;
    m_mousePos.y  = m_windowHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

bool 
Camera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

    case GLUT_KEY_UP:
        {
            m_pos += (m_target * StepScale);
            Ret = true;
        }
        break;

    case GLUT_KEY_DOWN:
        {
            m_pos -= (m_target * StepScale);
            Ret = true;
        }
        break;

    case GLUT_KEY_LEFT:
        {
            Vector3f Left = m_target.Cross(m_up);
            Left.Normalize();
            Left *= StepScale;
            m_pos += Left;
            Ret = true;
        }
        break;

    case GLUT_KEY_RIGHT:
        {
            Vector3f Right = m_up.Cross(m_target);
            Right.Normalize();
            Right *= StepScale;
            m_pos += Right;
            Ret = true;
        }
        break;
    }

    return Ret;
}

const Vector3f& 
Camera::GetPos() const
{
	return m_pos;
}

const Vector3f& 
Camera::GetTarget() const
{
	return m_target;
}

const Vector3f& 
Camera::GetUp() const
{
	return m_up;
}

void Camera::OnMouse(int x, int y)
{
    // mouse speed scale factor
    constexpr float factor = 1 / 20.f;

    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += static_cast<float>(DeltaX) * factor;
    m_AngleV += static_cast<float>(DeltaY) * factor;

//    if (DeltaX == 0) {
//        if (x <= MARGIN) {
//            m_OnLeftEdge = true;
//        }
//        else if (x >= (m_windowWidth - MARGIN)) {
//            m_OnRightEdge = true;
//        }
//    }
//    else {
//        m_OnLeftEdge = false;
//        m_OnRightEdge = false;
//    }
//
//    if (DeltaY == 0) {
//        if (y <= MARGIN) {
//            m_OnUpperEdge = true;
//        }
//        else if (y >= (m_windowHeight - MARGIN)) {
//            m_OnLowerEdge = true;
//        }
//    }
//    else {
//        m_OnUpperEdge = false;
//        m_OnLowerEdge = false;
//    }

    Update();
}

void Camera::OnRender()
{
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
        m_AngleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += 0.1f;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
           m_AngleV += 0.1f;
           ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}

void Camera::Update()
{
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(m_AngleH, Vaxis);
    View.Normalize();

    Vector3f Haxis = Vaxis.Cross(View);
    Haxis.Normalize();
    View.Rotate(m_AngleV, Haxis);
       
    m_target = View;
    m_target.Normalize();

    m_up = m_target.Cross(Haxis);
    m_up.Normalize();
}
