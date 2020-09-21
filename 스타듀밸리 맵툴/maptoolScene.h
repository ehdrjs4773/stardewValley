#pragma once
#include "gameNode.h"
#include "tileNode.h"

class maptoolScene : public gameNode
{
private:
	tagTile _tile[TILEY][TILEX]; //인게임화면(왼쪽화면)에 사용되는 타일 총 400개
	std::vector<std::vector<tagTile>> _sampleTile;
	std::vector<tagTile> _sampleTileX;
	tagCurrentTile _currentTile; //현재타일

	RECT _rcSave;		//세이브
	RECT _rcLoad;		//로드
	RECT _rcTerrain;	//지형
	RECT _rcObject;		//오브젝트
	RECT _rcEraser;		//지우개 (오브젝트만 지운다, 지형은 덮어씌운다)
	RECT _rcSpring;
	RECT _rcSummer;
	RECT _rcAutumn;
	RECT _rcWinter;

	RECT horScroll;
	RECT vertScroll;
	RECT sampleHorScroll;
	RECT sampleVertScroll;

	CTRL _ctrlSelect;	//현재 어떤 컨트롤 상태인지

	RECT first, last;
	int first_i, first_j;
	int last_i, last_j;
	bool _click;
	bool _release;

	bool isSelectSeason;
	SEASON _currentSeason;

	int tileX, tileY;
	int sampleTileX, sampleTileY;
	int sampleTileMaxFrameX, sampleTileMaxFrameY;
	POINT scrollLock;
	POINT currentScroll;
	bool isHorLock, isVertLock, isSampleHorLock, isSampleVertLock;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//맵툴세팅
	void maptoolSetup();
	void setMap();
	void setTerrainMap();
	void save();
	void load();

	void setScroll();
	void lockScroll();
	void moveScroll();
	void moveTile();
	void selectSeason();
	void resetSampleTile();
	//지형, 오브젝트 선택하기
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objectSelect(int frameX, int frameY);

};

