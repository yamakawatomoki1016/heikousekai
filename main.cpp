#include <Novice.h>
#include "Player.h"

const char kWindowTitle[] = "並行世界";

enum MapInfo {
	NONE,      // 0
	BLUEENEMY, // 1
	PINKENEMY, // 2
	PORTAL,    // 3
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
	int Stagecount = 0;

	bool stageClear = 0;

	unsigned int Color = 0x00000000;

	int map[mapHeight][mapWidth]{
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},

	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},


	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	   {0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	int map2[mapHeight][mapWidth]{
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},

	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},


	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	   {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
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

	int bluegroundUp = Novice::LoadTexture("./Resources/bluegroundUp.png");//上が青の地面
	int pinkgroundUp = Novice::LoadTexture("./Resources/pinkgroundUp.png");//上がピンクの地面
	int blueEnemyUp1 = Novice::LoadTexture("./Resources/blueEnemyUp1.png");//青の敵1枚目
	int blueEnemyUp2 = Novice::LoadTexture("./Resources/blueEnemyUp2.png");//青の敵2枚目
	int blueEnemyUp3 = Novice::LoadTexture("./Resources/blueEnemyUp3.png");//青の敵3枚目
	int blueEnemyUp4 = Novice::LoadTexture("./Resources/blueEnemyUp4.png");//青の敵4枚目
	int blueEnemyUp5678910 = Novice::LoadTexture("./Resources/blueEnemyUp5678910.png");//青の敵45678910枚目
	int blueEnemyUp11 = Novice::LoadTexture("./Resources/blueEnemyUp11.png");//青の敵11枚目
	int blueEnemyUp12 = Novice::LoadTexture("./Resources/blueEnemyUp12.png");//青の敵12枚目
	int blueEnemyUp13 = Novice::LoadTexture("./Resources/blueEnemyUp13.png");//青の敵13枚目
	int pinkEnemyUp1 = Novice::LoadTexture("./Resources/pinkEnemyUp1.png");//ピンクの敵1枚目
	int pinkEnemyUp2 = Novice::LoadTexture("./Resources/pinkEnemyUp2.png");//ピンクの敵2枚目
	int pinkEnemyUp3 = Novice::LoadTexture("./Resources/pinkEnemyUp3.png");//ピンクの敵3枚目
	int pinkEnemyUp4 = Novice::LoadTexture("./Resources/pinkEnemyUp4.png");//ピンクの敵4枚目
	int pinkEnemyUp5678910 = Novice::LoadTexture("./Resources/pinkEnemyUp5678910.png");//ピンクの敵45678910枚目
	int pinkEnemyUp11 = Novice::LoadTexture("./Resources/pinkEnemyUp11.png");//ピンクの敵11枚目
	int pinkEnemyUp12 = Novice::LoadTexture("./Resources/pinkEnemyUp12.png");//ピンクの敵12枚目
	int pinkEnemyUp13 = Novice::LoadTexture("./Resources/pinkEnemyUp13.png");//ピンクの敵13枚目
	int blueHaikeiUp = Novice::LoadTexture("./Resources/blueHaikeiUp.png");//青の背景上
	int pinkHaikeiUp = Novice::LoadTexture("./Resources/pinkHaikeiUp.png");//ピンクの背景上
	int kurisutaruUp = Novice::LoadTexture("./Resources/kurisutaruUp.png");//クリスタル上
	int kurisutaruDown = Novice::LoadTexture("./Resources/kurisutaruDown.png");//クリスタル下
	int title = Novice::LoadTexture("./Resources/title.png");

	//ブロックの状況
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			//ブロックの座標、サイズ設定
			block[y][x].pos.x = static_cast<float>(x * BlockSize);
			block[y][x].pos.y = static_cast<float>(y * BlockSize);
			block[y][x].size.x = 32;
			block[y][x].size.y = 32;

			//青い敵
			if (map[y][x] == 1) {
				block[y][x].state = BLUEENEMY;
				block[y][x].imagePos.x = 0;
				block[y][x].imagePos.y = 32;
			}

			//ピンクの敵
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
	int EnemyTime = 1;
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
		if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0) {
			player->BluePlayer.pos.x = { 1 };
			player->PinkPlayer.pos.x = { 1 };
			Scene = Stagecount;
		}

		switch (Scene) {
		case TITLE: //0

			if (Novice::IsTriggerButton(0, kPadButton10)) {
				Scene = STAGE1;
			}

			break;
		case GAMEOVER: //1

			if (Novice::IsTriggerButton(0, kPadButton10) && Stagecount == 1) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				Scene = STAGE1;
			}
			if (Novice::IsTriggerButton(0, kPadButton10) && Stagecount == 2) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				Scene = STAGE2;
			}
			if (Novice::IsTriggerButton(0, kPadButton10) && Stagecount == 3) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				Scene = STAGE3;
			}
			if (Novice::IsTriggerButton(0, kPadButton10) && Stagecount == 4) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				Scene = STAGE4;
			}


			break;
		case STAGE1: //2
			Stagecount = 1;

			EnemyTime++;
			if (EnemyTime >= 81) {
				EnemyTime = 0;
			}

			player->Update();

			//地面を入れ替える
			if (Novice::IsTriggerButton(0, kPadButton10)) {
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

			//ピンクの敵との当たり判定
			for (int y = 0; y < mapHeight; y++) {
				for (int x = 0; x < mapWidth; x++) {
					if (map[y][x] == PINKENEMY) {
						if (UpSideGround == false) {
							if (player->BluePlayer.pos.x * 32 + 32 > x * BlockSize && player->BluePlayer.pos.x * 32 + 32 < x * BlockSize + 32 &&
								player->BluePlayer.pos.y * 32 + 16 > y * BlockSize && player->BluePlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Scene = GAMEOVER;
							}
							if (player->BluePlayer.pos.x * 32 > x * BlockSize && player->BluePlayer.pos.x * 32 < x * BlockSize + 32 &&
								player->BluePlayer.pos.y * 32 + 16 > y * BlockSize && player->BluePlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Scene = GAMEOVER;
							}
						}
					}
					//青の敵との当たり判定
					if (map[y][x] == BLUEENEMY) {
						if (UpSideGround == true) {
							if (player->PinkPlayer.pos.x * 32 + 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 + 32 < x * BlockSize + 32 &&
								player->PinkPlayer.pos.y * 32 + 16 > y * BlockSize && player->PinkPlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Scene = GAMEOVER;
							}
							if (player->PinkPlayer.pos.x * 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 < x * BlockSize + 32 &&
								player->PinkPlayer.pos.y * 32 + 16 > y * BlockSize && player->PinkPlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Scene = GAMEOVER;
							}
						}
					}
				}
			}

			if (stageClear == 1) {
				Color = 0x000000FF;
			}

			if (Color == 0x000000FF) {
				Scene = STAGE2;
				stageClear = 0;
			}
			//シーン移行
			if (player->BluePlayer.pos.x > 40) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				stageClear = 1;
			}
			break;
		case STAGE2: //3
			Stagecount = 2;
			if (!stageClear) {
				if (Color != 0x00000000) {
					Color -= 0x00000003;
				}
			}
			
			if (Color == 0x00000000) {
				player->Update();

				//地面を入れ替える
				if (Novice::IsTriggerButton(0, kPadButton10)) {
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

				//ピンクの敵との当たり判定
				for (int y = 0; y < mapHeight; y++) {
					for (int x = 0; x < mapWidth; x++) {
						if (map2[y][x] == PINKENEMY) {
							if (UpSideGround == false) {
								if (player->BluePlayer.pos.x * 32 + 32 > x * BlockSize && player->BluePlayer.pos.x * 32 + 32 < x * BlockSize + 32 &&
									player->BluePlayer.pos.y * 32 + 16 > y * BlockSize && player->BluePlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
									Scene = GAMEOVER;
								}
								if (player->BluePlayer.pos.x * 32 > x * BlockSize && player->BluePlayer.pos.x * 32 < x * BlockSize + 32 &&
									player->BluePlayer.pos.y * 32 + 16 > y * BlockSize && player->BluePlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
									Scene = GAMEOVER;
								}
							}
						}
						//青の敵との当たり判定
						if (map2[y][x] == BLUEENEMY) {
							if (UpSideGround == true) {
								if (player->PinkPlayer.pos.x * 32 + 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 + 32 < x * BlockSize + 32 &&
									player->PinkPlayer.pos.y * 32 + 16 > y * BlockSize && player->PinkPlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
									Scene = GAMEOVER;
								}
								if (player->PinkPlayer.pos.x * 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 < x * BlockSize + 32 &&
									player->PinkPlayer.pos.y * 32 + 16 > y * BlockSize && player->PinkPlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
									Scene = GAMEOVER;
								}
							}
						}
					}
				}

				//シーン移行
				if (player->BluePlayer.pos.x > 40) {
					player->BluePlayer.pos.x = { 1 };
					player->PinkPlayer.pos.x = { 1 };
					Scene = STAGE3;
				}
			}
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
			Novice::DrawSprite(0, 0, title, 1, 1, 0.0f, WHITE);
			break;

		case GAMEOVER: //1

			break;
		case STAGE1: //2

			if (UpSideGround == true) {
				Novice::DrawSprite(0, 0, pinkHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruDown, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, bluegroundUp, 1, 1, 0.0f, WHITE);
			}
			if (DownSideGround == true) {
				Novice::DrawSprite(0, 0, blueHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, pinkgroundUp, 1, 1, 0.0f, WHITE);
			}
			for (int y = 0; y < mapHeight; y++) {
				for (int x = 0; x < mapWidth; x++) {
					if (map[y][x] == BLUEENEMY) {
						if (EnemyTime >= 1 && EnemyTime <= 10) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp1, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 11 && EnemyTime <= 20) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp2, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 21 && EnemyTime <= 30) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp3, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 31 && EnemyTime <= 40) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp4, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 41 && EnemyTime <= 50) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp5678910, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 51 && EnemyTime <= 60) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp11, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 61 && EnemyTime <= 70) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp12, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 71 && EnemyTime <= 80) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp13, 1, 1, 0.0f, WHITE);
						}
					}
					if (map[y][x] == PINKENEMY) {
						if (EnemyTime >= 1 && EnemyTime <= 10) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp1, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 11 && EnemyTime <= 20) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp2, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 21 && EnemyTime <= 30) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp3, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 31 && EnemyTime <= 40) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp4, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 41 && EnemyTime <= 50) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp5678910, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 51 && EnemyTime <= 60) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp11, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 61 && EnemyTime <= 70) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp12, 1, 1, 0.0f, WHITE);
						}
						if (EnemyTime >= 71 && EnemyTime <= 80) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp13, 1, 1, 0.0f, WHITE);
						}
					}
				}
			}
			player->Draw();

			break;
		case STAGE2: //3
			if (UpSideGround == false) {
				Novice::DrawSprite(0, 272, pinkgroundUp, 1, 1, 0.0f, WHITE);
			}
			if (DownSideGround == false) {
				Novice::DrawSprite(0, 272, bluegroundUp, 1, 1, 0.0f, WHITE);
			}
			for (int y = 0; y < mapHeight; y++) {
				for (int x = 0; x < mapWidth; x++) {
					if (map2[y][x] == BLUEENEMY) {
						Novice::DrawSpriteRect(x * BlockSize, y * BlockSize, 0, 32, 0, 32, blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
					}
					if (map2[y][x] == PINKENEMY) {
						Novice::DrawSpriteRect(x * BlockSize, y * BlockSize, 0, 32, 0, 32, blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
					}
				}
			}
			player->Draw();
			break;
		case STAGE3: //4

			break;
		case STAGE4: //5

			break;
		case CLEAR: //6

			break;
		}

		Novice::DrawBox(0, 0, 1280, 752, 0.0f, 0x00000000 + Color, kFillModeSolid);

		Novice::ScreenPrintf(100, 100, "Scene = %d", Scene);
		Novice::ScreenPrintf(100, 120, "UpSide = %d", UpSideGround);
		Novice::ScreenPrintf(100, 140, "DownSide = %d", DownSideGround);
		Novice::ScreenPrintf(100, 160, "ChangeFlag = %d", changeFlag);
		Novice::ScreenPrintf(100, 180, "ChangePlayerFlag = %d", player->changePlayerFlag);
		Novice::ScreenPrintf(100, 200, "stageCount = %d", Stagecount);

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
