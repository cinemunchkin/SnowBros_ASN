// Playe �� SCORE, LIFE UI

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
		// ���� UI 
		UIRenderer = CreateImageRenderer(SnowBrosRenderOrder::UI);
		UIRenderer->SetImage("LetterTest_01.png");

		UIRenderer->SetTransform({ {360,21}, {48*0.85f,128 * 0.9f} });
		//1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/
		UIRenderer->CreateAnimation("TotalScore", "LetterTest_01.png", 0,25, 1.0f, true);
	}

	{
		// �÷��̾� Life UI 
		// +3���� ����
		// �����ϰ� 1�� �浹�� ������ -1
		// 0�� �Ǹ� GameOver
		UIRenderer = CreateImageRenderer(SnowBrosRenderOrder::UI);
		UIRenderer->SetImage("NumUI_01.png");

		UIRenderer->SetTransform({ {360+22,22}, {40 * 0.9f,48 * 0.75f} });
		//1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/
		UIRenderer->CreateAnimation("TotalScore", "NumUI_01.png", 0, 9, 1.0f, true);

	}

	{
		// ���� UI
		//�ִϸ��̼��� �ƴ϶� �Լ���!!
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
	�̰� �ִϸ��̼����� XXX
	1. ���� 1���� monster destroy �� ������ -> Score �ö�
	
	2. 1�÷��̾� Score + (2�÷��̾� Score) == Total Score

	3. ������ vs �÷��̾� -> ������ destroy �� ������ => score �ö� 
	
	4. Helper�� �ִ� static -> SnowbrosScore
	 1) 1~3 �� ������ +SnowbrosScore
	 2) UI���� string snowbrosscore
	 3) ��µǴ� 0~9�̹��� ��ȭ

	 ����!
	*/


	//std::string ScoreNumIndex = std::to_string(USnowBros_Helper::SnowbrosScore);
	////to_string ; ���ڿ��� ��ȯ
	////int SnowBrosScore = 1000000;
	//std::string _string;
	//for (int i = 0; i < 7; ++i)
	//{
	//	_string = ScoreNumIndex.substr(i + 1, 1);
	//// substr -> �� �ڸ��� ������(ù��° ����), �� ������ŭ(�ι�° ����)  
	////	��ó�� �ϸ�, _String���� ScoreNumIndex�� i+1�� ������ �����ϴ� 1���Ǽ��ڰ� ���� ��
	//	// for�� �����鼭 i��° ���ڵ� ���ǹ����� �ٲ��ֱ�
	//// �׸��� �Ʒ����� _string �ڿ� append ����
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

