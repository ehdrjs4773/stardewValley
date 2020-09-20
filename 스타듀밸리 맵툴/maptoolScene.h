#pragma once
#include "gameNode.h"
#include "tileNode.h"

class maptoolScene : public gameNode
{
private:
	tagTile _tile[TILEY][TILEX]; //�ΰ���ȭ��(����ȭ��)�� ���Ǵ� Ÿ�� �� 400��
	tagTile _sampleTile[SAMPLETILEY][SAMPLETILEX];
	tagCurrentTile _currentTile; //����Ÿ��

	RECT _rcSave;		//���̺�
	RECT _rcLoad;		//�ε�
	RECT _rcTerrain;	//����
	RECT _rcObject;		//������Ʈ
	RECT _rcEraser;		//���찳 (������Ʈ�� �����, ������ ������)

	RECT horScroll;
	RECT vertScroll;
	RECT _sphorScroll;
	RECT _spvertScroll;

	CTRL _ctrlSelect;	//���� � ��Ʈ�� ��������

	RECT first, last;
	int first_i, first_j;
	int last_i, last_j;
	bool _click;
	bool _release;
	int stileX, stileY;
	POINT SpscrollLock;
	POINT SpcurrentScroll;
	bool isSpHorLock, isSpVertLock;

	int tileX, tileY;
	POINT scrollLock;
	POINT currentScroll;
	bool isHorLock, isVertLock;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//��������
	void maptoolSetup();
	void setMap();
	void setTerrainMap();
	void save();
	void load();

	void setScroll();
	void lockScroll();
	void moveScroll();
	void moveTile();
	//����, ������Ʈ �����ϱ�
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objectSelect(int frameX, int frameY);

};

