#pragma once

class Play_Actor_Core
{
public:
	// constrcuter destructer
	Play_Actor_Core();
	~Play_Actor_Core();

	// delete Function
	Play_Actor_Core(const Play_Actor_Core& _Other) = delete;
	Play_Actor_Core(Play_Actor_Core&& _Other) noexcept = delete;
	Play_Actor_Core& operator=(const Play_Actor_Core& _Other) = delete;
	Play_Actor_Core& operator=(Play_Actor_Core&& _Other) noexcept = delete;

protected:

private:

};