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

#include "t14_camera.h"

namespace t14
{
  const static float StepScale = 0.1f;

  Camera::Camera()
  {
    m_pos    = Vector3f(0.0f, 0.0f, 0.0f);
    m_target = Vector3f(0.0f, 0.0f, 1.0f);
    m_up     = Vector3f(0.0f, 1.0f, 0.0f);
  }

  Camera::Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
  {
    m_pos     = pos;
    m_target  = target.Normalized();
    m_up      = up.Normalized();
  }

  bool
  Camera::OnKeyboard(int key)
  {
    switch (key) {
      case GLUT_KEY_UP: {
          m_pos += (m_target * StepScale);
          return true;
      }
      case GLUT_KEY_DOWN: {
          m_pos -= (m_target * StepScale);
          return true;
      }
      case GLUT_KEY_LEFT: {
          Vector3f left = m_target.Cross(m_up).Normalized();
          left *= StepScale;
          m_pos += left;
          return true;
      }
      case GLUT_KEY_RIGHT: {
          Vector3f right = m_up.Cross(m_target).Normalized();
          right *= StepScale;
          m_pos += right;
          return true;
      }
    }

    return false;
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
}
