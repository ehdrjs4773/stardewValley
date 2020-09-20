#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{

	cout << "깃허브 테스트용 코드입니다." << endl;
	cout << "이게 제대로 들어와져있으면 됨!" << endl;
	cout << "아ㄴㄴ압ㅈㄷ" << endl;

	gameNode::init();
	
	cout << "dㅏ년ㅇ히게세여" << endl;
	/*앞으로 메인게임은 각각의 씬들만 관리를 한다*/
	/*이곳에서 각각의 씬들을 추가하고 현재씬을 설정한다*/

	/*씬추가*/
	SCENEMANAGER->addScene("INI테스트", new iniTestScene);
	//SCENEMANAGER->addScene("사운드", new soundTestScene);
	SCENEMANAGER->addScene("픽셀충돌", new pixelCollisionScene);
	SCENEMANAGER->addScene("로딩화면", new loadingScene);
	SCENEMANAGER->addScene("맵툴화면", new maptoolScene);
	SCENEMANAGER->addScene("테스트", new testScene);

	cout << "나는 아무 영향이 없다" << endl;
	cout << " 야호~ " << endl;
	/*현재씬*/
	SCENEMANAGER->loadScene("맵툴화면");
	//SCENEMANAGER->loadScene("테스트");
	
	cout << "이쪽저쪽에" << endl;
	cout << " cout을 배치시킨다" << endl;
	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	
	//씬매니져 업데이트
	SCENEMANAGER->update();

	//사운드매니져 업데이트 (이게 없으면 사운드매니져 제대로 동작하지 않는다!!!)
	SOUNDMANAGER->update();
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵 (이것은 렌더에 그냥 두기)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	
	//글자색 배경 없애기
	SetBkMode(getMemDC(), TRANSPARENT);

	//씬매니져 렌더
	SCENEMANAGER->render();
	
//=============================================================
	//백버퍼의 내용을 화면DC에 그린다 (이것도 렌더에 그냥 두기)
	this->getBackBuffer()->render(getHDC());
}

