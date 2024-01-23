#pragma once

class SnowBros_Player
{
public:
	// constrcuter destructer
	SnowBros_Player();
	~SnowBros_Player();

	// delete Function
	SnowBros_Player(const SnowBros_Player& _Other) = delete;
	SnowBros_Player(SnowBros_Player&& _Other) noexcept = delete;
	SnowBros_Player& operator=(const SnowBros_Player& _Other) = delete;
	SnowBros_Player& operator=(SnowBros_Player&& _Other) noexcept = delete;

protected:

private:

};