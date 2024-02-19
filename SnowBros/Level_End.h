#pragma once
#include <EngineCore\Level.h>
#include <EngineCore\Actor.h>


class ULevel_End : public ULevel, public AActor
{
public:
	// constrcuter destructer
	ULevel_End();
	~ULevel_End();

	// delete Function
	ULevel_End(const ULevel_End& _Other) = delete;
	ULevel_End(ULevel_End&& _Other) noexcept = delete;
	ULevel_End& operator=(const ULevel_End& _Other) = delete;
	ULevel_End& operator=(ULevel_End&& _Other) noexcept = delete;

protected:

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;


private:

};