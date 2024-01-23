#pragma once

class SnowBrosCore
{
public:
	// constrcuter destructer
	SnowBrosCore();
	~SnowBrosCore();

	// delete Function
	SnowBrosCore(const SnowBrosCore& _Other) = delete;
	SnowBrosCore(SnowBrosCore&& _Other) noexcept = delete;
	SnowBrosCore& operator=(const SnowBrosCore& _Other) = delete;
	SnowBrosCore& operator=(SnowBrosCore&& _Other) noexcept = delete;

protected:

private:

};