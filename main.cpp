#include <Novice.h>
#include "Player.h"

const char kWindowTitle[] = "並行世界";

enum MapInfo {
	NONE,      // 0
	BLUEENEMY, // 1
	PINKENEMY, // 2
	PORTAL     // 3
};

enum SCENE {
	TITLE,    //0
	GAMEOVER, //1
	STAGE1,   //2
	STAGE2,   //3
	STAGE3,   //4
	STAGE4,   //5
	CLEAR     //6
};

struct Block {
	MapInfo state;    //ブロックの状態
	Vector2 pos;      //座標
	Vector2 size;     //画像の大きさ
	Vector2 imagePos; //読み込む画像の座標
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 752);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player* player = new Player();

	const int mapHeight = 24; //Y軸のブロックの数
	const int mapWidth = 40;  //X軸のブロックの数

	int BlockSize = 32;
	int Scene = TITLE;

	int map[mapHeight][mapWidth]{
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,1,0,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0},

	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},


	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	   {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	Block block[mapHeight][mapWidth]{};

	int blueEnemy = Novice::LoadTexture("./Resources/blueEnemy.png");
	int pinkEnemy = Novice::LoadTexture("./Resources/pinkEnemy.png");
	int pinkground = Novice::LoadTexture("./Resources/pinkground.png");
	int pinkground1 = Novice::LoadTexture("./Resources/pinkground1.png");//反転バージョン
	int blueground = Novice::LoadTexture("./Resources/blueground.png");
	int blueground1 = Novice::LoadTexture("./Resources/blueground1.png");//反転バージョン

	//ブロックの状況
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			//ブロックの座標、サイズ設定
			block[y][x].pos.x = static_cast<float>(x * BlockSize);
			block[y][x].pos.y = static_cast<float>(y * BlockSize);
			block[y][x].size.x = 32;
			block[y][x].size.y = 32;

			//ブロックがある場合
			if (map[y][x] == 1) {
				block[y][x].state = BLUEENEMY;
				block[y][x].imagePos.x = 0;
				block[y][x].imagePos.y = 32;
			}

			if (map[y][x] == 2) {
				block[y][x].state = PINKENEMY;
				block[y][x].imagePos.x = 0;
				block[y][x].imagePos.y = 32;
			}

			//ブロックがない場合
			if (map[y][x] == 0) {
				block[y][x].state = NONE;
				block[y][x].imagePos.x = 32;
				block[y][x].imagePos.y = 0;
			}
		}
	}
	bool UpSideGround = false;
	bool DownSideGround = true;
	bool changeFlag = false;
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (Scene) {
		case TITLE: //0

			if (keys[DIK_SPACE]) {
				Scene = STAGE1;
			}

			break;
		case GAMEOVER: //1

			if (keys[DIK_SPACE]) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				Scene = STAGE1;
			}

			break;
		case STAGE1: //2

			player->Update(keys, preKeys);

			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0 && Scene == STAGE1) {
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
			for (int y = 0; y < mapHeight; y++) {
				//列の繰り返し
				for (int x = 0; x < mapWidth; x++) {
					//配列でブロックが存在している場合は配置する
					if (map[y][x] == BLUEENEMY) {
						if (UpSideGround == true) {

							if (player->BluePlayer.pos.x * 32 + 32 > x * BlockSize && player->BluePlayer.pos.x * 32 + 32 < x * BlockSize) {
								Scene = GAMEOVER;
							}
						}
						if (UpSideGround == false) {

							if (player->PinkPlayer.pos.x * 32 + 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 + 32 < x * BlockSize + 32) {
								Scene = GAMEOVER;
							}

						}
					}
					if (map[y][x] == PINKENEMY) {
						if (DownSideGround == false) {

							if (player->BluePlayer.pos.x * 32 + 32 > x * BlockSize && player->BluePlayer.pos.x * 32 + 32 < x * BlockSize) {
								Scene = GAMEOVER;
							}
						}
						if (DownSideGround == true) {

							if (player->PinkPlayer.pos.x * 32 + 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 + 32 < x * BlockSize + 32) {
								Scene = GAMEOVER;
							}

						}
					}
				}
			}

			break;
		case STAGE2: //3

			break;
		case STAGE3: //4

			break;
		case STAGE4: //5

			break;
		case CLEAR: //6

			break;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (Scene)
		{
		case TITLE:
			break;

		case GAMEOVER: //1

			break;
		case STAGE1: //2

			if (UpSideGround == true) {
				Novice::DrawSprite(0, 272, blueground, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 384, pinkground1, 1, 1, 0.0f, WHITE);
			}
			if (DownSideGround == true) {
				Novice::DrawSprite(0, 272, pinkground, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 384, blueground1, 1, 1, 0.0f, WHITE);
			}
			//行の繰り返し
			for (int y = 0; y < mapHeight; y++) {
				//列の繰り返し
				for (int x = 0; x < mapWidth; x++) {
					//配列でブロックが存在している場合は配置する
					if (map[y][x] == BLUEENEMY) {
						Novice::DrawSpriteRect(x * BlockSize, y * BlockSize, 0, 32, 0, 32, blueEnemy, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
					}
					if (map[y][x] == PINKENEMY) {
						Novice::DrawSpriteRect(x * BlockSize, y * BlockSize, 0, 32, 0, 32, pinkEnemy, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
					}
				}
			}
			player->Draw();

			break;
		case STAGE2: //3

			break;
		case STAGE3: //4

			break;
		case STAGE4: //5

			break;
		case CLEAR: //6

			break;
		}

		Novice::ScreenPrintf(100, 100, "Scene = %d", Scene);
		Novice::ScreenPrintf(100, 120, "UpSide = %d", UpSideGround);
		Novice::ScreenPrintf(100, 140, "DownSide = %d", DownSideGround);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
