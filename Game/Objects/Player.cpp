#include "Player.h"
#include <Novice.h>


void Player::Initialize()
{
}

void Player::Update()
{
	BluePlayer.pos.x += 0.06f;
	PinkPlayer.pos.x += 0.06f;

	if (Novice::IsTriggerButton(0, kPadButton10)) {
		changePlayerFlag = true;
	}

	if (changePlayerFlag == true) {
		if (playerUpSideGround == true) {
			playerUpSideGround = false;
			playerDownSideGround = true;
			changePlayerFlag = false;
		}
		else if (playerDownSideGround == true) {
			playerUpSideGround = true;
			playerDownSideGround = false;
			changePlayerFlag = false;
		}
	}
	if (playerUpSideGround == true) {
		BluePlayer.pos.y = 15;
		PinkPlayer.pos.y = 8;
	}
	if (playerUpSideGround == false) {
		PinkPlayer.pos.y = 15;
		BluePlayer.pos.y = 8;
	}

}
