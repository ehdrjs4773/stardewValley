#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	
	/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
	/*�̰����� ������ ������ �߰��ϰ� ������� �����Ѵ�*/

	/*���߰�*/
	SCENEMANAGER->addScene("INI�׽�Ʈ", new iniTestScene);
	//SCENEMANAGER->addScene("����", new soundTestScene);
	SCENEMANAGER->addScene("�ȼ��浹", new pixelCollisionScene);
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);
	SCENEMANAGER->addScene("����ȭ��", new maptoolScene);
	SCENEMANAGER->addScene("�׽�Ʈ", new testScene);
	/*�����*/
	SCENEMANAGER->loadScene("����ȭ��");
	//SCENEMANAGER->loadScene("�׽�Ʈ");
	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();

	//����Ŵ��� ������Ʈ (�̰� ������ ����Ŵ��� ����� �������� �ʴ´�!!!)
	SOUNDMANAGER->update();
}

//=============================================================
//	## ���� ## render()
//=============================================================
void mainGame::render()
{
	//��� �� ��Ʈ�� (�̰��� ������ �׳� �α�)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	
	//���ڻ� ��� ���ֱ�
	SetBkMode(getMemDC(), TRANSPARENT);

	//���Ŵ��� ����
	SCENEMANAGER->render();
	
//=============================================================
	//������� ������ ȭ��DC�� �׸��� (�̰͵� ������ �׳� �α�)
	this->getBackBuffer()->render(getHDC());
}

