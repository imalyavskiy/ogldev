#ifndef OGLDEV_ATB_H
#define	OGLDEV_ATB_H

#include "t39_keys.h"
#include "t39_math_3d.h"

class ATB
{
public:
    ATB();
    
    bool Init();
    
    bool KeyboardCB(KEYBOARD_KEY OgldevKey);

    bool PassiveMouseCB(int x, int y);

    void RenderSceneCB();

    bool MouseCB(MOUSE_BUTTON Button, KEYBOARD_KEY_STATE State, int x, int y);
    
};


#endif