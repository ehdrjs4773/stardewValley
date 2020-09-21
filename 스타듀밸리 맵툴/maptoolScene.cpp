#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init()
{
	//타일맵 이미지 초기화
	IMAGEMANAGER->addFrameImage("outdoorSpring", "Images/BMP/봄(농장).bmp", 400, 1264, 25, 79);
	IMAGEMANAGER->addFrameImage("outdoorWinter", "Images/BMP/outdoor(Winter).bmp", 400, 240, 25, 15);
	IMAGEMANAGER->addImage("배경", "Images/background.bmp", WINSIZEX, WINSIZEY);

	//맵툴세팅
	sampleTileMaxFrameX = IMAGEMANAGER->findImage("outdoorSpring")->getMaxFrameX() + 1;
	sampleTileMaxFrameY = IMAGEMANAGER->findImage("outdoorSpring")->getMaxFrameY() + 1;
	this->maptoolSetup();
	this->setScroll();

	first_i = first_j = last_i = last_j = 0;
	_click = false;
	_release = false;

	//현재타일 초기화 (지형 = 잔디)
	_currentTile.x = 0;
	_currentTile.y = 6;

	_currentSeason = SPRING;

	sampleTileX = 0;
	sampleTileY = 0;


	tileX = tileY = 0;

	isSelectSeason = isHorLock = isVertLock = isSampleHorLock = isSampleVertLock = false;

	//지형그리기 속성으로 시작하기
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
		selectSeason();

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
			isSelectSeason = true;
		}
		if (PtInRect(&_rcObject, _ptMouse))
		{
			_ctrlSelect = CTRL_OBJECT;
			isSelectSeason = true;
		}
		if (PtInRect(&_rcEraser, _ptMouse))
		{
			_ctrlSelect = CTRL_ERASER;
			isSelectSeason = false;
		}
	}
	if (INPUT->GetKeyUp(VK_LBUTTON))
	{
		isHorLock = isVertLock = isSampleHorLock = isSampleVertLock = false;
	}
	if (INPUT->GetKey(VK_LBUTTON))
	{
		if (isHorLock || isVertLock || isSampleHorLock || isSampleVertLock)
		{
			moveScroll();
		}
		else
		{
			this->setMap();
		}
	}
	moveTile();

	std::cout << _sampleTile.size() << "\t" << _sampleTileX.size() << std::endl;
}

void maptoolScene::render()
{
	IMAGEMANAGER->render("배경", getMemDC(), 0, 0);

	//타일맵 이미지 전체화면 우측상단에 배치하기
	switch (_currentSeason)
	{
	case SPRING:
		for (int i = 0; i < SAMPLEDISPLAYY; i++)
		{
			for (int j = 0; j < SAMPLEDISPLAYX; j++)
			{
				if (j < sampleTileMaxFrameX && i < sampleTileMaxFrameY)
				{
					IMAGEMANAGER->frameRender("outdoorSpring", getMemDC(), _sampleTile[i][j].rc.left, _sampleTile[i][j].rc.top,
						_sampleTile[i + sampleTileY][j + sampleTileX].terrainFrameX, _sampleTile[i + sampleTileY][j + sampleTileX].terrainFrameY);
				}
			}
		}
		break;
	case SUMMER:
		break;
	case AUTUMN:
		break;
	case WINTER:
		for (int i = 0; i < SAMPLEDISPLAYY; i++)
		{
			for (int j = 0; j < SAMPLEDISPLAYX; j++)
			{
				if (j < sampleTileMaxFrameX && i < sampleTileMaxFrameY)
				{
					IMAGEMANAGER->frameRender("outdoorWinter", getMemDC(), _sampleTile[i][j].rc.left, _sampleTile[i][j].rc.top,
						_sampleTile[i + sampleTileY][j + sampleTileX].terrainFrameX, _sampleTile[i + sampleTileY][j + sampleTileX].terrainFrameY);
				}
			}
		}
		break;
	}
	
	//인게임 화면 지형을 그린다
	for (int i = 0; i < DISPLAYY; i++)
	{
		for (int j = 0; j < DISPLAYX; j++)
		{
			switch (_tile[i + tileY][j + tileX].season)
			{
			case SPRING:
				IMAGEMANAGER->frameRender("outdoorSpring", getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i + tileY][j + tileX].terrainFrameX, _tile[i + tileY][j + tileX].terrainFrameY);

				//인게임 화면 오브젝트 그린다
				if (_tile[i + tileY][j + tileX].obj == OBJ_NONE) continue;
				IMAGEMANAGER->frameRender("outdoorSpring", getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i + tileY][j + tileX].objFrameX, _tile[i + tileY][j + tileX].objFrameY);
				break;
			case SUMMER:
				break;
			case AUTUMN:
				break;
			case WINTER:
				IMAGEMANAGER->frameRender("outdoorWinter", getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i + tileY][j + tileX].terrainFrameX, _tile[i + tileY][j + tileX].terrainFrameY);

				//인게임 화면 오브젝트 그린다
				if (_tile[i + tileY][j + tileX].obj == OBJ_NONE) continue;
				IMAGEMANAGER->frameRender("outdoorWinter", getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i + tileY][j + tileX].objFrameX, _tile[i + tileY][j + tileX].objFrameY);
				break;
			}
		}
	}

	//왼쪽 게임화면 및 오른쪽 샘플타일 렉트 보여주기
	if (INPUT->GetToggleKey(VK_F1))
	{
		for (int i = 0; i < DISPLAYY; i++)
		{
			for (int j = 0; j < DISPLAYX; j++)
			{
				FrameRect(getMemDC(), _tile[i][j].rc, RGB(255, 255, 0));
			}
		}
		for (int i = 0; i < SAMPLEDISPLAYY; i++)
		{
			for (int j = 0; j < SAMPLEDISPLAYX; j++)
			{
				if (j < sampleTileMaxFrameX && i < sampleTileMaxFrameY)
				{
					FrameRect(getMemDC(), _sampleTile[i][j].rc, RGB(0, 30, 80));
				}
			}
		}
	}

	//컨트롤 버튼 렉트 보여주기
	Rectangle(getMemDC(), _rcSave);
	Rectangle(getMemDC(), _rcLoad);
	Rectangle(getMemDC(), _rcTerrain);
	Rectangle(getMemDC(), _rcObject);
	Rectangle(getMemDC(), _rcEraser);

	textOut(getMemDC(), _rcSave.left + 20, _rcSave.top + 20, "세이브");
	textOut(getMemDC(), _rcLoad.left + 20, _rcLoad.top + 20, "로드");
	textOut(getMemDC(), _rcTerrain.left + 20, _rcTerrain.top + 20, "지형");
	textOut(getMemDC(), _rcObject.left + 20, _rcObject.top + 20, "오브젝트");
	textOut(getMemDC(), _rcEraser.left + 20, _rcEraser.top + 20, "지우개");

	if (isSelectSeason)
	{
		Rectangle(getMemDC(), _rcSpring);
		Rectangle(getMemDC(), _rcSummer);
		Rectangle(getMemDC(), _rcAutumn);
		Rectangle(getMemDC(), _rcWinter);

		textOut(getMemDC(), _rcSpring.left + 10, _rcSpring.top + 5, "봄");
		textOut(getMemDC(), _rcSummer.left + 10, _rcSummer.top + 5, "여름");
		textOut(getMemDC(), _rcAutumn.left + 10, _rcAutumn.top + 5, "가을");
		textOut(getMemDC(), _rcWinter.left + 10, _rcWinter.top + 5, "겨울");
	}

	//textOut(getMemDC(), 10, 10, "맵툴화면");

	Rectangle(getMemDC(), horScroll);
	Rectangle(getMemDC(), vertScroll);
	if (sampleTileMaxFrameX > SAMPLEDISPLAYX) { Rectangle(getMemDC(), sampleHorScroll); }
	if (sampleTileMaxFrameY > SAMPLEDISPLAYY) { Rectangle(getMemDC(), sampleVertScroll); }


	if(_click) FrameRect(getMemDC(), RectMake(first.left, first.top, last.right - first.left, last.bottom - first.top), RGB(255, 0, 0));
}

void maptoolScene::maptoolSetup()
{
	//오른쪽 샘플타일 렉트 초기화
	for (int i = 0; i < sampleTileMaxFrameY; i++)
	{
		_sampleTileX.clear();
		for (int j = 0; j < sampleTileMaxFrameX; j++)
		{
			tagTile temp;
			if (j < SAMPLEDISPLAYX && i < SAMPLEDISPLAYY)
			{
				temp.rc = RectMake(660 + j * TILESIZE, 50 + i * TILESIZE, TILESIZE, TILESIZE);
			}
			temp.terrainFrameX = j;
			temp.terrainFrameY = i;

			_sampleTileX.push_back(temp);
		}
		_sampleTile.push_back(_sampleTileX);
	}

	//왼쪽 인게임 화면 모두 잔디가 기본 타일이 되도록 세팅하기
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//왼쪽 인게임 화면 렉트 초기화
			if (j < DISPLAYX && i < DISPLAYY)
			{
				_tile[i][j].rc = RectMake(50 + j * TILESIZE, 50 + i * TILESIZE, TILESIZE, TILESIZE);
			}
			_tile[i][j].season = SPRING;
			_tile[i][j].terrainFrameX = 0;
			_tile[i][j].terrainFrameY = 6;
			_tile[i][j].objFrameX = 0;
			_tile[i][j].objFrameY = 0;
			_tile[i][j].terrain = terrainSelect(_tile[i][j].terrainFrameX, _tile[i][j].terrainFrameY);
			_tile[i][j].obj = OBJ_NONE;
		}
	}

	//컨트롤 렉트 위치 초기화
	_rcSave = RectMake(660, 400, 100, 50);
	_rcLoad = RectMake(660 + 100, 400, 100, 50);
	_rcTerrain = RectMake(660, 400 + 100, 100, 50);
	_rcObject = RectMake(660 + 100, 400 + 100, 100, 50);
	_rcEraser = RectMake(660 + 200, 400 + 100, 100, 50);

	_rcSpring = RectMake(660 - 100, 450, 50, 30);
	_rcSummer = RectMake(660 - 50, 450, 50, 30);
	_rcAutumn = RectMake(660, 450, 50, 30);
	_rcWinter = RectMake(660 + 50, 450, 50, 30);
}

void maptoolScene::setMap()
{
	for (int i = 0; i < sampleTileMaxFrameY; i++)
	{
		for (int j = 0; j < sampleTileMaxFrameX; j++)
		{
			if (PtInRect(&_sampleTile[i][j].rc, _ptMouse))
			{
				_currentTile.x = _sampleTile[i][j + sampleTileX].terrainFrameX;
				_currentTile.y = _sampleTile[i + sampleTileY][j].terrainFrameY;
				break;
			}
		}
	}

	//인게임화면 렉트와 충돌했냐?
	for (int i = 0; i < DISPLAYY; i++)
	{
		for (int j = 0; j < DISPLAYX; j++)
		{
			if (PtInRect(&_tile[i][j].rc, _ptMouse))
			{
				//FrameRect(getMemDC(), _tiles[i][j].rc, RGB(255, 0, 0));
				//현재버튼이 지형이냐?
				if (_ctrlSelect == CTRL_TERRAIN)
				{
					_tile[i + tileY][j + tileX].terrainFrameX = _currentTile.x;
					_tile[i + tileY][j + tileX].terrainFrameY = _currentTile.y;
					_tile[i + tileY][j + tileX].season = _currentSeason;
					_tile[i + tileY][j + tileX].terrain = terrainSelect(_currentTile.x, _currentTile.y);
				}

				//현재버튼이 오브젝트냐?
				if (_ctrlSelect == CTRL_OBJECT)
				{
					_tile[i + tileY][j + tileX].objFrameX = _currentTile.x;
					_tile[i + tileY][j + tileX].objFrameY = _currentTile.y;
					_tile[i + tileY][j + tileX].season = _currentSeason;
					_tile[i + tileY][j + tileX].obj = objectSelect(_currentTile.x, _currentTile.y);
				}
				//현재버튼이 지우개냐?
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
					if (INPUT->GetKeyDown(VK_RBUTTON))
					{
						_click = true;
						first = _tile[i][j].rc;
						first_i = i;
						first_j = j;

					}
					if (INPUT->GetKey(VK_RBUTTON))
					{
						last = _tile[i][j].rc;
						last_i = i;
						last_j = j;		
					}
					if (INPUT->GetKeyUp(VK_RBUTTON))
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
		if (_ptMouse.x >= _tile[0][DISPLAYX].rc.right)
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
				_tile[i + tileY][j + tileX].season = _currentSeason;
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
	horScroll = RectMake(_tile[0][0].rc.left, _tile[DISPLAYY - 1][0].rc.bottom + 10, TILESIZE * DISPLAYX / 2, 10);
	vertScroll = RectMake(_tile[0][DISPLAYX - 1].rc.right + 10, _tile[0][0].rc.top, 10, TILESIZE * DISPLAYY / 2);

	sampleHorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[SAMPLEDISPLAYY][0].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
	sampleVertScroll = RectMake(_sampleTile[0][SAMPLEDISPLAYX].rc.right + 10, _sampleTile[0][0].rc.top, 10, TILESIZE * SAMPLEDISPLAYY / 2);
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
	else if (PtInRect(&sampleHorScroll, _ptMouse))
	{
		scrollLock = _ptMouse;
		currentScroll.x = sampleHorScroll.left;
		currentScroll.y = sampleHorScroll.top;
		isSampleHorLock = true;
	}
	else if (PtInRect(&sampleVertScroll, _ptMouse))
	{
		scrollLock = _ptMouse;
		currentScroll.x = sampleVertScroll.left;
		currentScroll.y = sampleVertScroll.top;
		isSampleVertLock = true;
	}
}

void maptoolScene::moveScroll()
{
	if (isHorLock)
	{
		horScroll = RectMake(currentScroll.x + (_ptMouse.x - scrollLock.x), _tile[DISPLAYY - 1][0].rc.bottom + 10, TILESIZE * DISPLAYX / 2, 10);
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
		vertScroll = RectMake(_tile[0][DISPLAYX - 1].rc.right + 10, currentScroll.y + (_ptMouse.y - scrollLock.y), 10, TILESIZE * DISPLAYY / 2);
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
	else if (isSampleHorLock)
	{
		sampleHorScroll = RectMake(currentScroll.x + (_ptMouse.x - scrollLock.x), _sampleTile[sampleTileMaxFrameY - 1][0].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
		if (sampleHorScroll.left < _sampleTile[0][0].rc.left)
		{
			sampleHorScroll.left = _sampleTile[0][0].rc.left;
			sampleHorScroll.right = _sampleTile[0][0].rc.left + TILESIZE * SAMPLEDISPLAYX / 2;
		}
		else if (sampleHorScroll.left > _sampleTile[0][0].rc.left + TILESIZE * SAMPLEDISPLAYX / 2)
		{
			sampleHorScroll.left = _sampleTile[0][0].rc.left + TILESIZE * SAMPLEDISPLAYX / 2;
			sampleHorScroll.right = _sampleTile[0][0].rc.left + TILESIZE * SAMPLEDISPLAYX;
		}
	}
	else if (isSampleVertLock)
	{
		sampleVertScroll = RectMake(_sampleTile[0][sampleTileMaxFrameX - 1].rc.right + 10, currentScroll.y + (_ptMouse.y - scrollLock.y), 10, TILESIZE * SAMPLEDISPLAYY / 2);
		if (sampleVertScroll.top < _sampleTile[0][0].rc.top)
		{
			sampleVertScroll.top = _sampleTile[0][0].rc.top;
			sampleVertScroll.bottom = _sampleTile[0][0].rc.top + TILESIZE * SAMPLEDISPLAYY / 2;
		}
		else if (sampleVertScroll.top > _sampleTile[0][0].rc.top + TILESIZE * SAMPLEDISPLAYY / 2)
		{
			sampleVertScroll.top = _sampleTile[0][0].rc.top + TILESIZE * SAMPLEDISPLAYY / 2;
			sampleVertScroll.bottom = _sampleTile[0][0].rc.top + TILESIZE * SAMPLEDISPLAYY;
		}
	}
}

void maptoolScene::moveTile()
{
	tileX = (float)(horScroll.left - _tile[0][0].rc.left) / (float)((float)(TILESIZE * (float)(DISPLAYX / 2) / (float)(TILEX - DISPLAYX)));
	tileY = (float)(vertScroll.top - _tile[0][0].rc.top) / (float)((float)(TILESIZE * (float)(DISPLAYY / 2) / (float)(TILEX - DISPLAYX)));

	if (sampleTileMaxFrameX > SAMPLEDISPLAYX)
	{
		sampleTileX = (float)(sampleHorScroll.left - _sampleTile[0][0].rc.left) / (float)((float)(TILESIZE * (float)(SAMPLEDISPLAYX / 2) / (float)(sampleTileMaxFrameX - SAMPLEDISPLAYX)));
	}
	if (sampleTileMaxFrameY > SAMPLEDISPLAYY)
	{
		sampleTileY = (float)(sampleVertScroll.top - _sampleTile[0][0].rc.top) / (float)((float)(TILESIZE * (float)(SAMPLEDISPLAYY / 2) / (float)(sampleTileMaxFrameY - SAMPLEDISPLAYY)));
	}
}

void maptoolScene::selectSeason()
{
	if (isSelectSeason)
	{
		if (PtInRect(&_rcSpring, _ptMouse))
		{
			_currentSeason = SPRING;
			sampleTileMaxFrameX = IMAGEMANAGER->findImage("outdoorSpring")->getMaxFrameX() + 1;
			sampleTileMaxFrameY = IMAGEMANAGER->findImage("outdoorSpring")->getMaxFrameY() + 1;
			resetSampleTile();
			sampleTileX = sampleTileY = 0;
			if (sampleTileMaxFrameX > SAMPLEDISPLAYX)
			{
				if (sampleTileMaxFrameY > SAMPLEDISPLAYY)
				{
					sampleHorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[SAMPLEDISPLAYY - 1][0].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
				}
				else
				{
					sampleHorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[sampleTileMaxFrameY - 1][0].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
				}
			}
			if (sampleTileMaxFrameY > SAMPLEDISPLAYY)
			{
				if (sampleTileMaxFrameX > SAMPLEDISPLAYX)
				{
					sampleVertScroll = RectMake(_sampleTile[0][SAMPLEDISPLAYX - 1].rc.right + 10, _sampleTile[0][0].rc.top, 10, TILESIZE * SAMPLEDISPLAYY / 2);
				}
				else
				{
					sampleVertScroll = RectMake(_sampleTile[0][sampleTileMaxFrameX - 1].rc.right + 10, _sampleTile[0][0].rc.top, 10, TILESIZE * SAMPLEDISPLAYY / 2);
				}
			}
		}
		if (PtInRect(&_rcSummer, _ptMouse))
		{ 
			_currentSeason = SUMMER;
		}
		if (PtInRect(&_rcAutumn, _ptMouse))
		{
			_currentSeason = AUTUMN;
		}
		if (PtInRect(&_rcWinter, _ptMouse))
		{
			_currentSeason = WINTER;
			sampleTileMaxFrameX = IMAGEMANAGER->findImage("outdoorWinter")->getMaxFrameX() + 1;
			sampleTileMaxFrameY = IMAGEMANAGER->findImage("outdoorWinter")->getMaxFrameY() + 1;
			resetSampleTile();
			sampleTileX = sampleTileY = 0;
			if (sampleTileMaxFrameX > SAMPLEDISPLAYX)
			{
				if (sampleTileMaxFrameY > SAMPLEDISPLAYY)
				{
					sampleHorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[SAMPLEDISPLAYY - 1][0].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
				}
				else
				{
					sampleHorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[sampleTileMaxFrameY - 1][0].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
				}
			}
			if (sampleTileMaxFrameY > SAMPLEDISPLAYY)
			{
				if (sampleTileMaxFrameX > SAMPLEDISPLAYX)
				{
					sampleHorScroll = RectMake(_sampleTile[0][0].rc.left, _sampleTile[0][SAMPLEDISPLAYX - 1].rc.bottom + 10, TILESIZE * SAMPLEDISPLAYX / 2, 10);
				}
				else
				{
					sampleVertScroll = RectMake(_sampleTile[0][sampleTileMaxFrameX - 1].rc.right + 10, _sampleTile[0][0].rc.top, 10, TILESIZE * SAMPLEDISPLAYY / 2);
				}
			}
		}
	}
}

void maptoolScene::resetSampleTile()
{
	_sampleTile.clear();
	for (int i = 0; i < sampleTileMaxFrameY; i++)
	{
		_sampleTileX.clear();
		for (int j = 0; j < sampleTileMaxFrameX; j++)
		{
			tagTile temp;
			if (j < sampleTileMaxFrameX && i < sampleTileMaxFrameY)
			{
				temp.rc = RectMake(660 + j * TILESIZE, 50 + i * TILESIZE, TILESIZE, TILESIZE);
			}
			temp.terrainFrameX = j;
			temp.terrainFrameY = i;

			_sampleTileX.push_back(temp);
		}
		_sampleTile.push_back(_sampleTileX);
	}
}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	//시멘트
	if (frameX == 1 && frameY == 0)
	{
		return TR_CEMENT;
	}
	//땅
	if (frameX == 2 && frameY == 0)
	{
		return TR_GROUND;
	}
	//잔디
	if (frameX == 3 && frameY == 0)
	{
		return TR_GRASS;
	}
	//물
	if (frameX == 4 && frameY == 0)
	{
		return TR_WATER;
	}
	//기타
	return TR_GROUND;
}

OBJECT maptoolScene::objectSelect(int frameX, int frameY)
{
	return OBJ_BLOCKS;
}
