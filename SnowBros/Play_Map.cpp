#include "Play_Map.h"
#include "SnowBros_Helper.h"
#include <EngineCore\EngineResourcesManager.h>

APlay_Map::APlay_Map()
{
}

APlay_Map::~APlay_Map()
{
}

void APlay_Map::SetMapImage(std::string_view _MapImageName)
{
	Renderer->SetImage(_MapImageName);
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void APlay_Map::SetColMapImage(std::string_view _MapImageName)
{
	ColRenderer->SetImage(_MapImageName);
	UWindowImage* Image = ColRenderer->GetImage();
	USnowBros_Helper::ColMapImage = Image;
	FVector ImageScale = Image->GetScale();
	ColRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void APlay_Map::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);



	if (UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}
}


void APlay_Map::SwitchDebug()
{
	if (true == Renderer->IsActive())
	{
		Renderer->SetActive(false);
		ColRenderer->SetActive(true);
	}
	else
	{
		Renderer->SetActive(true);
		ColRenderer->SetActive(false);
	}

}
void APlay_Map::BeginPlay()
{
	// 여기서 상수로 고정시키면.
	Renderer = CreateImageRenderer(SnowBrosRenderOrder::Map);
	ColRenderer = CreateImageRenderer(SnowBrosRenderOrder::Map);
	ColRenderer->SetActive(false);
	// Renderer->SetImageCuttingTransform({ 0,0 }, { });
}