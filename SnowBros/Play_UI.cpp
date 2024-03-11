// Playe 중 SCORE, LIFE UI

#include "Play_UI.h"
#include "Play_Player.h"
#include "SnowBros_Helper.h"

#include <EngineCore\EngineResourcesManager.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineBase/EngineTime.h>

#include <conio.h>
#include <string>


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
		// 고정 UI 
		UIRenderer = CreateImageRenderer(SnowBrosRenderOrder::UI);
		UIRenderer->SetImage("LetterTest_01.png");

		UIRenderer->SetTransform({ {360,21}, {48*0.85f,128 * 0.9f} });
		//1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/
		UIRenderer->CreateAnimation("TotalScore", "LetterTest_01.png", 0,25, 1.0f, true);
	}

	{
		// 플레이어 Life UI 
		// +3으로 시작
		// 몬스터하고 1번 충돌할 때마다 -1
		// 0이 되면 GameOver
		UIRenderer = CreateImageRenderer(SnowBrosRenderOrder::UI);
		UIRenderer->SetImage("NumUI_01.png");

		UIRenderer->SetTransform({ {360+22,22}, {40 * 0.9f,48 * 0.75f} });
		//1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/
		UIRenderer->CreateAnimation("TotalScore", "NumUI_01.png", 0, 9, 1.0f, true);

	}

	{
		// 점수 UI
		//애니메이션이 아니라 함수로!!
		// ->PlayerScore();
		UIRenderer = CreateImageRenderer(SnowBrosRenderOrder::UI);
		UIRenderer->SetImage("NumUI_01.png");

		UIRenderer->SetTransform({ {360 -24,22}, {40 * 0.9f,48 * 0.75f} });
		//1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/
		UIRenderer->CreateAnimation("TotalScore", "NumUI_01.png", 0, 9, 1.0f, true);

	}

	StateChange(EUIState::TotalScore);

}





void APlay_UI::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	PlayerScore(_DeltaTime);


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
	UIState = _State;

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

	/*
	이건 애니메이션으로 XXX
	1. 몬스터 1마리 monster destroy 될 때마다 -> Score 올라감
	
	2. 1플레이어 Score + (2플레이어 Score) == Total Score

	3. 아이템 vs 플레이어 -> 아이템 destroy 할 때마다 => score 올라감 
	
	4. Helper에 있는 static -> SnowbrosScore
	 1) 1~3 할 때마다 +SnowbrosScore
	 2) UI에서 string snowbrosscore
	 3) 출력되는 0~9이미지 변화

	 아하!
	*/


	//std::string ScoreNumIndex = std::to_string(USnowBros_Helper::SnowbrosScore);
	////to_string ; 문자열로 변환
	////int SnowBrosScore = 1000000;
	//std::string _string;
	//for (int i = 0; i < 7; ++i)
	//{
	//	_string = ScoreNumIndex.substr(i + 1, 1);
	//// substr -> 그 자리의 수부터(첫번째 인자), 그 갯수만큼(두번째 인자)  
	////	위처럼 하면, _String에는 ScoreNumIndex의 i+1의 수부터 시작하는 1개의숫자가 담기는 셈
	//	// for문 돌리면서 i번째 숫자들 조건문으로 바꿔주기
	//// 그리고 아래에서 _string 뒤에 append 해줌
	//	_string.append("_NumUI.png");
	//	ScoreUIArray[i] -> SetImage(_string);
	//}
	//


}


void APlay_UI::PlayerLife(float _DeltaTime) 
{

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

