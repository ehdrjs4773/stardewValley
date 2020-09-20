#include "stdafx.h"
#include "testScene.h"

HRESULT testScene::init()
{
    _rc = RectMake(WINSIZEX / 2, WINSIZEY / 2, 100, 100);
    memset(test, 0, sizeof(test));

    return S_OK;
}

void testScene::release()
{
}

void testScene::update()
{

    if (INPUT->GetKeyDown(VK_LBUTTON))
    {
        check1++;
        start.x = _ptMouse.x;
        start.y = _ptMouse.y;
        end.x = _ptMouse.x + 10;
        end.y = _ptMouse.y + 10;

        sprintf(test[0], "check1 : %d", check1);
    }

    if (INPUT->GetKey(VK_LBUTTON))
    {
        check2++;
        end.x = _ptMouse.x + 10;
        end.y = _ptMouse.y + 10;
        sprintf(test[1], "check2 : %d", check2);
    }
    if (INPUT->GetKeyUp(VK_LBUTTON))
    {
        check3++;
        sprintf(test[2], "check3 : %d", check3);
    }
}

void testScene::render()
{
    Rectangle(getMemDC(), _rc);

    FrameRect(getMemDC(), RectMake(start.x, start.y, end.x - start.x, end.y - start.y),RGB(255,0,0));
    for (int i = 0; i < 3; i++)
    {
        textOut(getMemDC(), 25, 25 + (25 * i), test[i], RGB(255,0,0));
    }
}
