#pragma once

//게임화면 타일
#define TILESIZE 16
#define TILEX 100
#define TILEY 100
#define DISPLAYX 30
#define DISPLAYY 30
#define TILESIZEX (TILEX * TILESIZE) //640
#define TILESIZEY (TILEY * TILESIZE) //640

//이미지 타일 (화면 우측상단에 붙여놓을 샘플타일)
//#define SAMPLETILEX 25
//#define SAMPLETILEY 80
#define SAMPLEDISPLAYX 20
#define SAMPLEDISPLAYY 20

//맵툴에서 사용할 컨트롤
enum CTRL
{
	CTRL_SAVE, CTRL_LOAD,
	CTRL_TERRAIN, CTRL_OBJECT, CTRL_ERASER
};

enum SEASON
{
	SPRING,
	SUMMER,
	AUTUMN,
	WINTER,
};
//지형
enum TERRAIN
{
	TR_CEMENT, TR_GROUND, TR_GRASS, TR_WATER, TR_END
};

//오브젝트
enum OBJECT
{
	OBJ_BLOCK1, OBJ_BLOCK3, OBJ_BLOCKS,
	OBJ_TANK1, OBJ_TANK2,
	OBJ_NONE
};

//타일구조체
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;
	SEASON season;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};

//이미지 타일 구조체
struct tagSampleTile
{
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
};

//현재타일 구조체
struct tagCurrentTile
{
	int x, y;
};
