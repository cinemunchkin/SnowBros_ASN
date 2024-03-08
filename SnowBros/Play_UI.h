#pragma once

#include <EngineCore\Actor.h>
#include <EngineBase/Transform.h>
#include "SnowBros_Helper.h"


class APlay_UI : public AActor
{
public:
	APlay_UI();
	~APlay_UI();

	// delete Function
	APlay_UI(const APlay_UI& _Other) = delete;
	APlay_UI(APlay_UI&& _Other) noexcept = delete;
	APlay_UI& operator=(const APlay_UI& _Other) = delete;
	APlay_UI& operator=(APlay_UI&& _Other) noexcept = delete;

	EUIState UIState = EUIState::None;

protected:


	void StateChange(EUIState _State);
	void StateUpdate(float _DeltaTime);
	void TotalScoreStart();
	void PlayerScoreStart();
	void PlayerLifeStart();

	void SetAnimation(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);

	void TotalScore(float _DeltaTime);
	void PlayerScore(float _DeltaTime);
	void PlayerLife(float _DeltaTime);

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void UI_Pause(float DeltaTime);

private:
	
	UImageRenderer* UIRenderer = nullptr;

};