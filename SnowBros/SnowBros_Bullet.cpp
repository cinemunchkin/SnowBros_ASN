#include "SnowBros_Bullet.h"
#include <Windows.h>
#include <EngineCore\EngineCore.h>

ASnowBros_Bullet::ASnowBros_Bullet()
{
}

ASnowBros_Bullet::~ASnowBros_Bullet()
{
}


void ASnowBros_Bullet::BeginPlay()
{
	{
		UImageRenderer* Renderer = CreateImageRenderer(-10);
		Renderer->SetImageToScale("CB2.bmp");
	}

	Destroy(2.0f);
}

void ASnowBros_Bullet::Tick(float _DeltaTime)
{
	AddActorLocation(Dir * Speed * _DeltaTime);
}