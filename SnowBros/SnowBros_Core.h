#pragma once
#include <EngineCore\EngineCore.h>

// Ό³Έν :
class SnowBros_Core : public EngineCore
{
public:
	// constrcuter destructer
	SnowBros_Core();
	~SnowBros_Core();

	// delete Function
	SnowBros_Core(const SnowBros_Core& _Other) = delete;
	SnowBros_Core(SnowBros_Core&& _Other) noexcept = delete;
	SnowBros_Core& operator=(const SnowBros_Core& _Other) = delete;
	SnowBros_Core& operator=(SnowBros_Core&& _Other) noexcept = delete;

protected:
	void EngineStart();
	void EngineUpdate();
	void EngineEnd();

private:

};