#pragma once
#include <EngineCore\Level.h>
// ���� :
class ULevel_Play : public ULevel
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

private:

};

