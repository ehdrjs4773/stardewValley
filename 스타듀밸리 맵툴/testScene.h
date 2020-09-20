#pragma once
#include "gameNode.h"
class testScene :    public gameNode
{
	RECT _rc;
	int check1 = 0;
	int check2 = 0;
	int check3 = 0;

	POINT start;
	POINT end;
	char test[3][256];


public:
	HRESULT init();
	void release();
	void update();
	void render();
};

