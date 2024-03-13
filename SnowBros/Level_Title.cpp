#include "Level_Title.h"
#include "Title_Map.h"
#include "EngineCore\EngineCore.h"

ULevel_Title::ULevel_Title() 
{
}

ULevel_Title::~ULevel_Title() 
{
}

void ULevel_Title::BeginPlay()
{
	ULevel::BeginPlay();

	{
		// 여기서 위치가 안정해졌다고 이녀석위치가 0,0인건 아니죠
		ATitle_Map* Logo = SpawnActor<ATitle_Map>();

	}


}
void ULevel_Title::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (UEngineInput::IsAnykeyDown())
	{
		GEngine->ChangeLevel("Play");
	}


	
}
void ULevel_Title::LevelStart(ULevel* _Level)
{
	int a = 0;
}
void ULevel_Title::LevelEnd(ULevel* _Level)
{
	int a = 0;
}