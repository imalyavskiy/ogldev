#ifndef OGLDEV_ATB_H
#define	OGLDEV_ATB_H

#include <AntTweakBar.h>

#include "t44_keys.h"
#include "t44_math_3d.h"

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


extern TwType TW_TYPE_OGLDEV_VECTOR3F;
extern TwType TW_TYPE_OGLDEV_ATTENUATION;

#endif