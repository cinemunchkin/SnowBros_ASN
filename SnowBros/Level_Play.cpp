#include "Level_Play.h"
#include "Play_Player.h"
#include "Play_Map.h"
#include <EngineCore\EngineCore.h>
#include "Play_Monster.h"

ULevel_Play::ULevel_Play()
{
}

ULevel_Play::~ULevel_Play()
{
}

void ULevel_Play::BeginPlay()
{
	ULevel::BeginPlay();

	APlay_Map* Map = SpawnActor<APlay_Map>();
	Map->SetMapImage("SnowBros_Lv_1.png");
	Map->SetColMapImage("SnowBros_Lv_1_Col.png");
	
	// Map->SwitchDebug();

	//APlay_UIMap* UIMap = SpawnActor<APlay_UIMap>();
	//UIMap->SetMapImage("SnowBros_UI.png");
 
	 {
		 APlay_Player* Player = SpawnActor<APlay_Player>(); // 아오 이걸 계속 APlay_Map으로 놨었네
		 Player->SetActorLocation({ 100, 300 });
		 Player->SetName("Player");
	 }

	 {
		 APlay_Monster* Monster = SpawnActor<APlay_Monster>();
		 Monster->SetName("Monster");
		 Monster->SetActorLocation({ 100, 150 });
	 }



			// 몬스터등장, 플레이어 등장 구현

}

void ULevel_Play::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (UEngineInput::IsDown('E'))
	{
		GEngine->ChangeLevel("End");
	
		

	}

	

}

void ULevel_Play::LevelStart(ULevel* _Level)
{
	// 여기서 리소스를 로드하고
	// 액터도 여기서 만들고
	int a = 0;
}
void ULevel_Play::LevelEnd(ULevel* _Level)
{
	// 리소스도 날려요.
	// 액터를 삭제한다.
	int a = 0;
}