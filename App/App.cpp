#include <Windows.h>
#include <EngineCore\EngineCore.h>
#include <EnginePlatform\EngineWindow.h>
#include <SnowBros\SnowBros_Core.h>

//#pragma comment(lib, "fmod_vc.lib")

// 옛날 엔진들이 유저들에게 엔진을 기동을 쉽게하기 위해서 사용한 방식.

// 사용자가 하고 싶을일을 어떻게 어디에 정의하게 만들거냐?
// 내가 윈도우 크기를 변경하고 싶어.

// ENGINESTART(UContentExCore);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPWSTR    lpCmdLine, 
	_In_ int       nCmdShow) 

{ 
  // D2R 사용해서 단위각도 활용한 함수 만들기

	LeakCheck; 
	USnowBros_Core NewUserCore = USnowBros_Core();
	NewUserCore.EngineStart(hInstance);
}