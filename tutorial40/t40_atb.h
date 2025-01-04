#ifndef OGLDEV_ATB_H
#define	OGLDEV_ATB_H

#include "t40_keys.h"
#include "t40_math_3d.h"

class ATB
{
public:
    ATB();
    
    bool Init();
    
    bool KeyboardCB(OGLDEV_KEY OgldevKey);

    bool PassiveMouseCB(int x, int y);

    void RenderSceneCB();

    bool MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y);
    
};


#endif