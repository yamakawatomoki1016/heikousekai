#include "Player.h"
#include <Novice.h>


void Player::Initialize()
{
}

void Player::Update(char* keys, char* preKeys)
{
	BluePlayer.pos.x += 0.06f;
	PinkPlayer.pos.x += 0.06f;

	if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
		changeFlag = true;
	}

	if (changeFlag == true) {
		if (UpSideGround == true) {
			UpSideGround = false;
			DownSideGround = true;
			changeFlag = false;
		}
		else if (DownSideGround == true) {
			UpSideGround = true;
			DownSideGround = false;
			changeFlag = false;
		}
	}

}

void Player::Draw()
{
	if (UpSideGround == true) {
		Novice::DrawBox(
			static_cast<int>(BluePlayer.pos.x * 32.0f), 
			static_cast<int>(BluePlayer.pos.y * 32.0f + 224.0f), 
			static_cast<int>(BluePlayer.radius), 
			static_cast<int>(BluePlayer.radius), 0.0f, BluePlayer.color, kFillModeSolid);

		Novice::DrawBox(
			static_cast<int>(PinkPlayer.pos.x * 32.0f), 
			static_cast<int>(PinkPlayer.pos.y * 32.0f - 224.0f), 
			static_cast<int>(PinkPlayer.radius), 
			static_cast<int>(PinkPlayer.radius), 0.0f, PinkPlayer.color, kFillModeSolid);
	}
	if (DownSideGround == true) {
		Novice::DrawBox(
			static_cast<int>(BluePlayer.pos.x * 32.0f), 
			static_cast<int>(BluePlayer.pos.y * 32.0f), 
			static_cast<int>(BluePlayer.radius), 
			static_cast<int>(BluePlayer.radius), 0.0f, BluePlayer.color, kFillModeSolid);

		Novice::DrawBox(
			static_cast<int>(PinkPlayer.pos.x * 32.0f),
			static_cast<int>(PinkPlayer.pos.y * 32.0f), 
			static_cast<int>(PinkPlayer.radius), 
			static_cast<int>(PinkPlayer.radius), 0.0f, PinkPlayer.color, kFillModeSolid);
	}
}
