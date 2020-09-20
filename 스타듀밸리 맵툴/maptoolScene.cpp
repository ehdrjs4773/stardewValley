#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init()
{
	//Ÿ�ϸ� �̹��� �ʱ�ȭ
	IMAGEMANAGER->addFrameImage("tilemap", "Images/BMP/��(����).bmp", 400, 1264, SAMPLETILEX, SAMPLETILEY);
	IMAGEMANAGER->addImage("���", "Images/background.bmp", WINSIZEX, WINSIZEY);
	//��������
	this->maptoolSetup();
	this->setScroll();

	first_i = first_j = last_i = last_j = 0;
	_click = false;
	_release = false;

	//����Ÿ�� �ʱ�ȭ (���� = �ܵ�)
	_currentTile.x = 0;
	_currentTile.y = 6;

	tileX = tileY = 0;
	stileX = stileY = 0;

	isSpHorLock = isSpVertLock = false;
	isHorLock = isVertLock = false;

	//�����׸��� �Ӽ����� �����ϱ�
	_ctrlSelect = CTRL_TERRAIN;

	return S_OK;
}

void maptoolScene::release()
{
}

void maptoolScene::update()
{

	setTerrainMap();

	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		lockScroll();
		if (PtInRect(&_rcSave, _ptMouse))
		{
			_ctrlSelect = CTRL_SAVE;
			this->save();
		}
		if (PtInRect(&_rcLoad, _ptMouse))
		{
			_ctrlSelect = CTRL_LOAD;
			this->load();
		}
		if (PtInRect(&_rcTerrain, _ptMouse))
		{
			_ctrlSelect = CTRL_TERRAIN;
		}
		if (PtInRect(&_rcObject, _ptMouse))
		{
			_ctrlSelect = CTRL_OBJECT;
		}
		if (PtInRect(&_rcEraser, _ptMouse))
		{
			_ctrlSelect = CTRL_ERASER;
		}
	}
	if (INPUT->GetKeyUp(VK_LBUTTON))
	{
		isHorLock = isVertLock = isSpHorLock = isSpVertLock =  false;
	}
	if (INPUT->GetKey(VK_LBUTTON))
	{
		if (isHorLock || isVertLock || isSpHorLock || isSpVertLock)
		{
			moveScroll();
		}
		else
		{
			this->setMap();
		}
	}

	moveTile();

	std::cout << tileX << "\t" << tileY << "\t" << horScroll.left - _tile[0][0].rc.left << "\t" << vertScroll.top - _tile[0][0].rc.top << std::endl;
}

void maptoolScene::render()
{
	IMAGEMANAGER->render("���", getMemDC(), 0, 0);
	//Ÿ�ϸ� �̹��� ��üȭ�� ������ܿ� ��ġ�ϱ�
	//IMAGEMANAGER->render("tilemap", getMemDC(), 660, 0);

	//�ΰ��� ȭ�� ������ �׸���
	for (int i = 0; i < DISPLAYX; i++)
	{
		for (int j = 0; j < DISPLAYY; j++)
		{
			IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
				_tile[i + tileY][j + tileX].terrainFrameX, _tile[i + tileY][j + tileX].terrainFrameY);

			//�ΰ��� ȭ�� ������Ʈ �׸���
			if (_tile[i + tileY][j + tileX].obj == OBJ_NONE) continue;
			IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
				_tile[i + tileY][j + tileX].objFrameX, _tile[i + tileY][j + tileX].objFrameY);
		}
	}


	for (int i = 0; i < SPDISPLAYX; i++)
	{
		for (int j = 0; j < SPDISPLAYY; j++)
		{
			IMAGEMANAGER->frameRender("tilemap", getMemDC(), _sampleTile[i][j].rc.left, _sampleTile[i][j].rc.top,
				_sampleTile[i + stileY][j + stileX].terrainFrameX, _sampleTile[i + stileY][j + stileX].terrainFrameY);
		}
	}

	//���� ����ȭ�� �� ������ ����Ÿ�� ��Ʈ �����ֱ�
	if (INPUT->GetToggleKey(VK_F1))
	{
		//for (int i = 0; i < TILEY; i++)
		//{
		//	for (int j = 0; j < TILEX; j++)
		//	{
		//		//Rectangle(getMemDC)
		//		FrameRect(getMemDC(), _tile[i][j].rc, RGB(255, 255, 0));
		//	}
		//}
		for (int i = 0; i < SAMPLETILEY; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				//Rectangle(getMemDC)
				FrameRect(getMemDC(), _sampleTile[i][j].rc, RGB(255, 255, 0));
			}
		}
	}

	//��Ʈ�� ��ư ��Ʈ �����ֱ�
	Rectangle(getMemDC(), _rcSave);
	Rectangle(getMemDC(), _rcLoad);
	Rectangle(getMemDC(), _rcTerrain);
	Rectangle(getMemDC(), _rcObject);
	Rectangle(getMemDC(), _rcEraser);

	textOut(getMemDC(), _rcSave.left + 20, _rcSave.top + 20, "���̺�");
	textOut(getMemDC(), _rcLoad.left + 20, _rcLoad.top + 20, "�ε�");
	textOut(getMemDC(), _rcTerrain.left + 20, _rcTerrain.top + 20, "����");
	textOut(getMemDC(), _rcObject.left + 20, _rcObject.top + 20, "������Ʈ");
	textOut(getMemDC(), _rcEraser.left + 20, _rcEraser.top + 20, "���찳");
	//textOut(getMemDC(), 10, 10, "����ȭ��");

	Rectangle(getMemDC(), horScroll);
	Rectangle(getMemDC(), vertScroll);

	Rectangle(getMemDC(), _sphorScroll);
	Rectangle(getMemDC(), _spvertScroll);

	if(_click) FrameRect(getMemDC(), RectMake(first.left, first.top, last.right - first.left, last.bottom - first.top), RGB(255, 0, 0));
}

void maptoolScene::maptoolSetup()
{
	//������ ����Ÿ�� ��Ʈ �ʱ�ȭ
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTile[i][j].rc = RectMake(660 + j * TILESIZE,50 + i * TILESIZE, TILESIZE, TILESIZE);
			_sampleTile[i][j].terrainFrameX = j;
			_sampleTile[i][j].terrainFrameY = i;
		}
	}

	//���� �ΰ��� ȭ�� ��� �ܵ� �⺻ Ÿ���� �ǵ��� �����ϱ�
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//���� �ΰ��� ȭ�� ��Ʈ �ʱ�ȭ
			_tile[i][j].rc = RectMake(50 + j * TILESIZE, 50 + i * TILESIZE, TILESIZE, TILESIZE);
			_tile[i][j].terrainFrameX = 0;
			_tile[i][j].terrainFrameY = 6;
			_tile[i][j].objFrameX = 0;
			_tile[i][j].objFrameY = 0;
			_tile[i][j].terrain = terrainSelect(_tile[i][j].terrainFrameX, _tile[i][j].terrainFrameY);
			_tile[i][j].obj = OBJ_NONE;
		}
	}

	//��Ʈ�� ��Ʈ ��ġ �ʱ�ȭ
	_rcSave = RectMake(660, 400, 100, 50);
	_rcLoad = RectMake(660 + 100, 400, 100, 50);
	_rcTerrain = RectMake(660, 400 + 100, 100, 50);
	_rcObject = RectMake(660 + 100, 400 + 100, 100, 50);
	_rcEraser = RectMake(660 + 200, 400 + 100, 100, 50);
}

void maptoolScene::setMap()
{
	for (int i = 0; i < SPDISPLAYY; i++)
	{
		for (int j = 0; j < SPDISPLAYY; j++)
		{
			if (PtInRect(&_sampleTile[i][j].rc, _ptMouse))
			{
				_currentTile.x = _sampleTile[i + stileY][j + stileX].terrainFrameX;
				_currentTile.y = _sampleTile[i + stileY][j + stileX].terrainFrameY;
				break;
			}
		}
	}

	//�ΰ���ȭ�� ��Ʈ�� �浹�߳�?
	for (int i = 0; i < DISPLAYY; i++)
	{
		for (int j = 0; j < DISPLAYX; j++)
		{
			if (PtInRect(&_tile[i][j].rc, _ptMouse))
			{
				//FrameRect(getMemDC(), _tiles[i][j].rc, RGB(255, 0, 0));
				//�����ư�� �����̳�?
				/*if (_ctrlSelect == CTRL_TERRAIN)
				{
					_tile[i + tileY][j + tileX].terrainFrameX = _currentTile.x;
					_tile[i + tileY][j + tileX].terrainFrameY = _currentTile.y;
					_tile[i + tileY][j + tileX].terrain = terrainSelect(_currentTile.x, _currentTile.y);
				}*/

				//�����ư�� ������Ʈ��?
				if (_ctrlSelect == CTRL_OBJECT)
				{
					_tile[i + tileY][j + tileX].objFrameX = _currentTile.x;
					_tile[i + tileY][j + tileX].objFrameY = _currentTile.y;
					_tile[i + tileY][j + tileX].obj = objectSelect(_currentTile.x, _currentTile.y);
				}
				//�����ư�� ���찳��?
				if (_ctrlSelect == CTRL_ERASER)
				{
					_tile[i + tileY][j + tileX].objFrameX = 0;
					_tile[i + tileY][j + tileX].objFrameY = 0;
					_tile[i + tileY][j + tileX].obj = OBJ_NONE;
				}
			}
		}
	}
}

void maptoolScene::setTerrainMap()
{
	for (int i = 0; i < DISPLAYY; i++)
	{
		for (int j = 0; j < DISPLAYX; j++)
		{
			if (PtInRect(&_tile[i][j].rc, _ptMouse))
			{
				if (_ctrlSelect == CTRL_TERRAIN)
				{
					if (INPUT->GetKeyDown(VK_LBUTTON))
					{
						_click = true;
						first = _tile[i][j].rc;
						first_i = i;
						first_j = j;

					}
					if (INPUT->GetKey(VK_LBUTTON))
					{
						last = _tile[i][j].rc;
						last_i = i;
						last_j = j;		
					}
					if (INPUT->GetKeyUp(VK_LBUTTON))
					{
						last = _tile[i][j].rc;
						last_i = i;
						last_j = j;
						_click = false;
						_release = true;
					}
				}
			}
		}
	}
	if (_click)
	{
		if (_ptMouse.x >= _tile[0][0].rc.left + TILESIZE * DISPLAYX)
		{
			_click = false;
			_release = true;
		}
	}
	if (_release)
	{
		for (int i = first_i; i <= last_i; i++)
		{
			for (int j = first_j; j <= last_j; j++)
			{
				_tile[i + tileY][j + tileX].terrainFrameX = _currentTile.x;
				_tile[i + tileY][j + tileX].terrainFrameY = _currentTile.y;
				_tile[i + tileY][j + tileX].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
		}
		_release = false;
	}
}

void maptoolScene::save()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, _tile, sizeof(_tile), &write, NULL);
	CloseHandle(file);
}

void maptoolScene::load()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tile, sizeof(_tile), &read, NULL);
	CloseHandle(file);
}

void maptoolScene::setScroll()
{
	horScroll = RectMake(_tile[0][0].rc.left, _tile[DISPLAYY][0].rc.bottom + 10, TILESIZE * DISPLAYX / 2, 10);
	vertScroll = RectMake(_tile[0][DISPLAYX].rc.right + 10, _tile[0][0].rc.top, 10, TILESIZE * DISPLAYY / 2);

	_sphorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[SPDISPLAYY][0].rc.bottom, TILESIZE * SPDISPLAYX / 2, 10);
	_spvertScroll = RectMake(_sampleTile[0][SPDISPLAYX].rc.right, _sampleTile[0][0].rc.top, 10, TILESIZE * SPDISPLAYY / 2);
}

void maptoolScene::lockScroll()
{
	if (PtInRect(&horScroll, _ptMouse))
	{
		scrollLock = _ptMouse;
		currentScroll.x = horScroll.left;
		currentScroll.y = horScroll.top;
		isHorLock = true;
	}
	else if (PtInRect(&vertScroll, _ptMouse))
	{
		scrollLock = _ptMouse;
		currentScroll.x = vertScroll.left;
		currentScroll.y = vertScroll.top;
		isVertLock = true;
	}

	if (PtInRect(&_sphorScroll, _ptMouse))
	{
		scrollLock = _ptMouse;
		currentScroll.x = _sphorScroll.left;
		currentScroll.y = _sphorScroll.top;
		isSpHorLock = true;
	}
	else if (PtInRect(&_spvertScroll, _ptMouse))
	{
		scrollLock = _ptMouse;
		currentScroll.x = _spvertScroll.left;
		currentScroll.y = _spvertScroll.top;
		isSpVertLock = true;
	}
}

void maptoolScene::moveScroll()
{
	if (isHorLock)
	{
		horScroll = RectMake(currentScroll.x + (_ptMouse.x - scrollLock.x), _tile[DISPLAYY][0].rc.bottom + 10, TILESIZE * DISPLAYX / 2, 10);
		if (horScroll.left < _tile[0][0].rc.left)
		{
			horScroll.left = _tile[0][0].rc.left;
			horScroll.right = _tile[0][0].rc.left + TILESIZE * DISPLAYX / 2;
		}
		else if (horScroll.left > _tile[0][0].rc.left + TILESIZE * DISPLAYX / 2)
		{
			horScroll.left = _tile[0][0].rc.left + TILESIZE * DISPLAYX / 2;
			horScroll.right = _tile[0][0].rc.left + TILESIZE * DISPLAYX;
		}
	}
	else if (isVertLock)
	{
		vertScroll = RectMake(_tile[0][DISPLAYX].rc.right + 10, currentScroll.y + (_ptMouse.y - scrollLock.y), 10, TILESIZE * DISPLAYY / 2);
		if (vertScroll.top < _tile[0][0].rc.top)
		{
			vertScroll.top = _tile[0][0].rc.top;
			vertScroll.bottom = _tile[0][0].rc.top + TILESIZE * DISPLAYY / 2;
		}
		else if (vertScroll.top > _tile[0][0].rc.top + TILESIZE * DISPLAYY / 2)
		{
			vertScroll.top = _tile[0][0].rc.top + TILESIZE * DISPLAYY / 2;
			vertScroll.bottom = _tile[0][0].rc.top + TILESIZE * DISPLAYY;
		}
	}

	if (isSpHorLock)
	{
		_sphorScroll = RectMake(currentScroll.x + (_ptMouse.x - scrollLock.x), _sampleTile[SPDISPLAYY][0].rc.bottom, TILESIZE * SPDISPLAYX / 2, 10);
		if (_sphorScroll.left < _sampleTile[0][0].rc.left)
		{
			_sphorScroll.left = _sampleTile[0][0].rc.left;
			_sphorScroll.right = _sampleTile[0][0].rc.left + TILESIZE * SPDISPLAYX / 2;
		}
		else if (_sphorScroll.left > _sampleTile[0][0].rc.left + TILESIZE * SPDISPLAYX / 2)
		{
			_sphorScroll.left = _sampleTile[0][0].rc.left + TILESIZE * SPDISPLAYX / 2;
			_sphorScroll.right = _sampleTile[0][0].rc.left + TILESIZE * SPDISPLAYX;
		}
	}
	else if (isSpVertLock)
	{
		_spvertScroll = RectMake(_sampleTile[0][SPDISPLAYX].rc.right, currentScroll.y + (_ptMouse.y - scrollLock.y), 10, TILESIZE * SPDISPLAYY / 2);
		if (_spvertScroll.top < _sampleTile[0][0].rc.top)
		{
			_spvertScroll.top = _sampleTile[0][0].rc.top;
			_spvertScroll.bottom = _sampleTile[0][0].rc.top + TILESIZE * SPDISPLAYY / 2;
		}
		else if (_spvertScroll.top > _sampleTile[0][0].rc.top + TILESIZE * SPDISPLAYY / 2)
		{
			_spvertScroll.top = _sampleTile[0][0].rc.top + TILESIZE * SPDISPLAYY / 2;
			_spvertScroll.bottom = _sampleTile[0][0].rc.top + TILESIZE * SPDISPLAYY;
		}
	}
}

void maptoolScene::moveTile()
{
	tileX = (float)(horScroll.left - _tile[0][0].rc.left) / (float)((float)(TILESIZE * (float)(DISPLAYX / 2) / (float)(TILEX - DISPLAYX)));
	tileY = (float)(vertScroll.top - _tile[0][0].rc.top) / (float)((float)(TILESIZE * (float)(DISPLAYY / 2) / (float)(TILEX - DISPLAYX)));

	stileX = (float)(_sphorScroll.left - _sampleTile[0][0].rc.left) / (float)((float)(TILESIZE * (float)(SPDISPLAYX / 2) / (float)(SAMPLETILEX - SPDISPLAYX)));
	stileY = (float)(_spvertScroll.top - _sampleTile[0][0].rc.top) / (float)((float)(TILESIZE * (float)(SPDISPLAYY / 2) / (float)(SAMPLETILEY - SPDISPLAYX)));
}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	//�ø�Ʈ
	if (frameX == 1 && frameY == 0)
	{
		return TR_CEMENT;
	}
	//��
	if (frameX == 2 && frameY == 0)
	{
		return TR_GROUND;
	}
	//�ܵ�
	if (frameX == 3 && frameY == 0)
	{
		return TR_GRASS;
	}
	//��
	if (frameX == 4 && frameY == 0)
	{
		return TR_WATER;
	}
	//��Ÿ
	return TR_GROUND;
}

OBJECT maptoolScene::objectSelect(int frameX, int frameY)
{
	return OBJ_BLOCKS;
}
