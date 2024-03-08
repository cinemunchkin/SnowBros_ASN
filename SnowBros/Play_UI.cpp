#include "Play_UI.h"
#include "Play_Player.h"

#include <EngineCore\EngineResourcesManager.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineBase/EngineTime.h>

#include <conio.h>
#include <vector>

APlay_UI::APlay_UI()
{
	
}

APlay_UI::~APlay_UI()
{
}

void APlay_UI::BeginPlay()
{
	AActor::BeginPlay();
	{
		UIRenderer = CreateImageRenderer(SnowBrosRenderOrder::UI);
		UIRenderer->SetImage("LetterTest_01.png");

		UIRenderer->SetTransform({ {1440/4,20}, {28,128} });
		//1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/
		UIRenderer->CreateAnimation("TotalScore", "LetterTest_01.png", 0,25, 1.0f, true);
	
	}

	StateChange(EUIState::TotalScore);

}


void APlay_UI::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


	if (UEngineInput::IsDown('O'))
	{
		UI_Pause(_DeltaTime);

	}


}

void APlay_UI::UI_Pause(float _DeltaTime)
{

	if (true == UIRenderer->IsActive())
	{
		UIRenderer->SetActive(false);
	}
	else
	{
		UIRenderer->SetActive(true);
	}

}

void APlay_UI::StateChange(EUIState _State)
{
	if (UIState != _State)
	{
		switch (_State)
		{
		case EUIState::TotalScore:
			TotalScoreStart();
			break;
		
		case EUIState::PlayerScore:
			break;
			PlayerScoreStart();
		
		case EUIState::PlayerLife:
			PlayerLifeStart();
			break;

		default:
			break;
		}

	}
}


void APlay_UI::StateUpdate(float _DeltaTime)
{
	switch (UIState)
	{
	case EUIState::TotalScore:
		TotalScore(_DeltaTime);
		break;

	case EUIState::PlayerScore:
		break;
		PlayerScore(_DeltaTime);

	case EUIState::PlayerLife:
		PlayerLife(_DeltaTime);
		break;

	default:
		break;

	}
}
void APlay_UI::TotalScoreStart()
{
	this->SetAnimation("TotalScore");
}

void APlay_UI::PlayerScoreStart()
{
	this->SetAnimation("PlayerScore");
}

void APlay_UI::PlayerLifeStart()
{
	this->SetAnimation("PlayerLife");
}



void APlay_UI::TotalScore(float _DeltaTime)
{

}
void APlay_UI::PlayerScore(float _DeltaTime)
{

}
void APlay_UI::PlayerLife(float _DeltaTime) {

}


void APlay_UI::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	UIRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_UI::GetAnimationFullName(std::string _Name)
{
	return _Name;
}

