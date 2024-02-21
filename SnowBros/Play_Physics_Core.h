#pragma once

class APlay_Physics_Core
{
public:
	// constrcuter destructer
	APlay_Physics_Core();
	~APlay_Physics_Core();

	// delete Function
	APlay_Physics_Core(const APlay_Physics_Core& _Other) = delete;
	APlay_Physics_Core(APlay_Physics_Core&& _Other) noexcept = delete;
	APlay_Physics_Core& operator=(const APlay_Physics_Core& _Other) = delete;
	APlay_Physics_Core& operator=(APlay_Physics_Core&& _Other) noexcept = delete;


	void Player_Strobe(float _DestroyTime = 0.0f) override;

protected:

private:

};