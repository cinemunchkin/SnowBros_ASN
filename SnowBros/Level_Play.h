#pragma once
#include <EngineCore\Level.h>
#include <EngineCore\Actor.h>
#include <EnginePlatform\EngineSound.h>

// Ό³Έν :
class ULevel_Play : public ULevel, public AActor
{
public:
	// constrcuter destructer
	ULevel_Play();
	~ULevel_Play();

	// delete Function
	ULevel_Play(const ULevel_Play& _Other) = delete;
	ULevel_Play(ULevel_Play&& _Other) noexcept = delete;
	ULevel_Play& operator=(const ULevel_Play& _Other) = delete;
	ULevel_Play& operator=(ULevel_Play&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;


	UEngineSoundPlayer BGM;

private:



};

