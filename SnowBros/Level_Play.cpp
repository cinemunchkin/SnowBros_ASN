#include "Level_Play.h"
#include "Play_UI.h"
#include "Play_Map.h"
#include "Ending_Map.h"
#include "Play_Monster.h"
#include "Play_Player.h"
#include "Play_Bullet.h"

#include <EngineCore\EngineCore.h>
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
	
	{
		APlay_UI* UI = SpawnActor<APlay_UI>();
		UI->SetActorLocation({});
		UI->SetName("UI");
	}
	// Map->SwitchDebug();

 
	 {
		 APlay_Player* Player = SpawnActor<APlay_Player>();
		 // 아오 이걸 계속 APlay_Map으로 놨었네
		 Player->SetActorLocation({ 966/2, 300 });
		 Player->SetName("Player");
	 }

	 //{
		// APlay_Monster* Monster1 = SpawnActor<APlay_Monster>();
		// Monster1->SetName("Monster");
		// Monster1->SetActorLocation({ 150, 50 });
	 //}

	 //{
		// APlay_Monster* Monster2 = SpawnActor<APlay_Monster>();
		// Monster2->SetName("Monster");
		// Monster2->SetActorLocation({ 450-150, 50 });
	 //}


	 //{
		// APlay_Monster* Monster3 = SpawnActor<APlay_Monster>();
		// Monster3->SetName("Monster");
		// Monster3->SetActorLocation({ 250, 250 });
	 //}

	 {
		 APlay_Monster* Monster4 = SpawnActor<APlay_Monster>();
		 Monster4->SetName("Monster");
		 Monster4->SetActorLocation({ 450-250, 250 });
	 }

	 
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