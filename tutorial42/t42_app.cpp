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


#include "t42_app.h"
#include "t42_util.h"

OgldevApp::OgldevApp()
{
    m_frameCount = 0;
    m_frameTime = 0;
    m_fps = 0;

    m_frameTime = m_startTime = GetCurrentTimeMillis();
}



void OgldevApp::CalcFPS()
{
    m_frameCount++;

    long long time = GetCurrentTimeMillis();

    if (time - m_frameTime >= 1000) {
        m_frameTime = time;
        m_fps = m_frameCount;
        m_frameCount = 0;
    }
}

void OgldevApp::RenderFPS()
{
    char text[32];
    ZERO_MEM(text);
    SNPRINTF(text, sizeof(text), "FPS: %d", m_fps);

#ifndef _WIN32
//    m_fontRenderer.RenderText(10, 10, text);
#endif
}

float OgldevApp::GetRunningTime()
{
    float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
    return RunningTime;
}
