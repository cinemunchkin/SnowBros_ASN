#pragma once
#include <EngineCore\Level.h>

// Ό³Έν :
class ULevel_Title : public ULevel
{
public:
	// constrcuter destructer
	ULevel_Title();
	~ULevel_Title();

	// delete Function
	ULevel_Title(const ULevel_Title& _Other) = delete;
	ULevel_Title(ULevel_Title&& _Other) noexcept = delete;
	ULevel_Title& operator=(const ULevel_Title& _Other) = delete;
	ULevel_Title& operator=(ULevel_Title&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

private:

};

