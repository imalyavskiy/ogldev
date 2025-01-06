/*

	Copyright 2014 Etay Meiri

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


#include "t44_app.h"
#include "t44_util.h"

#ifdef FREETYPE
Markup sMarkup = { (char*)"Arial", 64, 1, 0, 0.0, 0.0,
                   {.1,1.0,1.0,.5}, {1,1,1,0},
                   0, {1,0,0,1}, 0, {1,0,0,1},
                   0, {0,0,0,1}, 0, {0,0,0,1} };
#endif
namespace t44 {
  BaseApp::BaseApp()
#ifdef FREETYPE
    : m_fontRenderer(sMarkup)
#endif
  {
    m_frameTime = m_startTime = GetCurrentTimeMillis();
  }



  void BaseApp::CalcFPS()
  {
    m_frameCount++;

    const int64_t time = GetCurrentTimeMillis();

    if (time - m_frameTime >= 1000) {
      m_frameTime = time;
      m_fps = m_frameCount;
      m_frameCount = 0;
    }
  }

  void BaseApp::RenderFPS()
  {
    char text[32] = {};
    SNPRINTF(text, sizeof(text), "FPS: %d", m_fps);

#ifdef FREETYPE
    m_fontRenderer.RenderText(10, 10, text);
#endif
  }

  float BaseApp::GetRunningTime()
  {
    const float runningTime = static_cast<float>(static_cast<double>(GetCurrentTimeMillis()) - static_cast<double>(m_startTime)) / 1000.0f;
    return runningTime;
  }
}