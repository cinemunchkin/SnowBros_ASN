#include "Title_Map.h"

ATitle_Map::ATitle_Map()
{
}

ATitle_Map::~ATitle_Map()
{
}

void ATitle_Map::BeginPlay()
{
	AActor::BeginPlay();

	UImageRenderer* Renderer = CreateImageRenderer();

	Renderer->SetImage("Title.png");
	// 이미지가 나올 위치
	SetActorLocation({ 720 / 2,498 / 2 });
	Renderer->SetTransform({ {0,0}, {1440 / 2,996 / 2} });
	Renderer->SetImageCuttingTransform({ {0,0}, {1440, 996/*720/2,498/2*/} });

	// Renderer->SetImageToScale("Title.png");
}

void ATitle_Map::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	int a = 0;
}