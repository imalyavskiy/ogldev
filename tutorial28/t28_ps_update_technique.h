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
*/

#ifndef PS_UPDATE_TECHNIQUE_H
#define	PS_UPDATE_TECHNIQUE_H

#include "t28_technique.h"

namespace t28
{
  class PSUpdateTechnique : public Technique
  {
  public:
    PSUpdateTechnique() = default;

    bool Init() override;
    
    void SetParticleLifetime(float Lifetime);
    
    void SetDeltaTimeMillis(float deltaTimeMillis);
    
    void SetTime(int time);

    void SetRandomTextureUnit(unsigned int textureUnit);
    
    void SetLauncherLifetime(float lifetime);
    
    void SetShellLifetime(float lifetime);
    
    void SetSecondaryShellLifetime(float lifetime);
    
  private:
    GLuint m_deltaTimeMillisLocation;
    GLuint m_randomTextureLocation;
    GLuint m_timeLocation;
    GLuint m_launcherLifetimeLocation;
    GLuint m_shellLifetimeLocation;
    GLuint m_secondaryShellLifetimeLocation;
  };
}

#endif	/* PS_UPDATE_TECHNIQUE_H */

