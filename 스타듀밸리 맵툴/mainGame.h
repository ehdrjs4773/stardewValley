#pragma once
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ ������ϸ� �̰��� �߰��Ѵ�*/
#include "iniTestScene.h"
#include "soundTestScene.h"
#include "pixelCollisionScene.h"
#include "loadingScene.h"
#include "maptoolScene.h"
#include "testScene.h"
class mainGame : public gameNode
{
public:
	HRESULT init();
	void release();
	void update();
	void render();
};