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

struct CharaEnemy {
	Vector2 pos;
	int radius;
	unsigned int color;
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

	unsigned int Color = 0;

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

	/*int map2[mapHeight][mapWidth]{
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

	int map3[mapHeight][mapWidth]{
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
	};*/

	Block block[mapHeight][mapWidth]{};

	int bluegroundUp = Novice::LoadTexture("./Resources/bluegroundUp.png");//上が青の地面
	int pinkgroundUp = Novice::LoadTexture("./Resources/pinkgroundUp.png");//上がピンクの地面
	int blueEnemyUp1 = Novice::LoadTexture("./Resources/blueEnemyUp1.png");//青の敵1枚目上
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
	int title = Novice::LoadTexture("./Resources/title.png");//タイトル
	int pinkPlayerUp1 = Novice::LoadTexture("./Resources/pinkPlayerUp1.png");//ピンクプレイヤー１上
	int pinkPlayerUp2 = Novice::LoadTexture("./Resources/pinkPlayerUp2.png");//ピンクプレイヤー２
	int bluePlayerUp1 = Novice::LoadTexture("./Resources/bluePlayerUp1.png");//青プレイヤー１
	int bluePlayerUp2 = Novice::LoadTexture("./Resources/bluePlayerUp2.png");//青プレイヤー２
	int pinkPlayerDown1 = Novice::LoadTexture("./Resources/pinkPlayerDown1.png");//ピンクプレイヤー１下
	int pinkPlayerDown2 = Novice::LoadTexture("./Resources/pinkPlayerDown2.png");//ピンクプレイヤー２
	int bluePlayerDown1 = Novice::LoadTexture("./Resources/bluePlayerDown1.png");//青プレイヤー１
	int bluePlayerDown2 = Novice::LoadTexture("./Resources/bluePlayerDown2.png");//青プレイヤー２
	int gameOver = Novice::LoadTexture("./Resources/gameOver.png");//ゲームオーバー
	int gameClear = Novice::LoadTexture("./Resources/gameClear.png");

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

	//STAGE2
	CharaEnemy blueEnemy1 = {
		{12.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy1 = {
		{12.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy2 = {
		{18.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy2 = {
		{18.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy3 = {
		{240.f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy3 = {
		{24.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy4 = {
		{30.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy4 = {
		{30.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	//STAGE3
	CharaEnemy blueEnemy5 = {
		{12.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy5 = {
		{12.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy6 = {
		{18.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy6 = {
		{18.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy7 = {
		{24.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy7 = {
		{24.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy8 = {
		{30.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy8 = {
		{30.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	//STAGE4
	CharaEnemy blueEnemy9 = {
		{12.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy9 = {
		{12.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy10 = {
		{18.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy10 = {
		{18.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy11 = {
		{24.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy11 = {
		{24.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy blueEnemy12 = {
		{30.0f,8.0f},
		32,
		0xFFFFFFFF,
	};

	CharaEnemy pinkEnemy12 = {
		{30.0f,15.0f},
		32,
		0xFFFFFFFF,
	};

	int pinkEnemyX[4] = { 150,450,750,1050 };
	int pinkEnemyY[4] = { 480,256,480,256 };
	int blueEnemyX[4] = { 300,600,900,1200 };
	int blueEnemyY[4] = { 256,480,256,480 };


	bool UpSideGround = false;
	bool DownSideGround = true;
	bool changeFlag = false;
	int enemyTime = 1;
	int playerTime = 1;
	int Blackinflag = 0;
	int gameOverColor = 0;
	int enemyMoveTime = 0;
	int enemyMove = 2;
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
		
		if (Blackinflag == 1 && Stagecount == 1) {
			gameOverColor += 3;
			if (gameOverColor >= 255) {
				gameOverColor = 255;
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				Scene = STAGE1;
				Blackinflag = 0;
			}
		}
		if (Blackinflag == 1 && Stagecount == 2) {
			gameOverColor += 3;
			if (gameOverColor >= 255) {
				enemyMoveTime = 0;
				gameOverColor = 255;
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				blueEnemy1.pos.x = { 384 };
				blueEnemy2.pos.x = { 576 };
				blueEnemy3.pos.x = { 768 };
				blueEnemy4.pos.x = { 960 };
				pinkEnemy1.pos.x = { 384 };
				pinkEnemy2.pos.x = { 576 };
				pinkEnemy3.pos.x = { 768 };
				pinkEnemy4.pos.x = { 960 };
				Scene = STAGE2;
				Blackinflag = 0;
			}
		}
		if (Blackinflag == 1 && Stagecount == 3) {
			gameOverColor += 3;
			if (gameOverColor >= 255) {
				enemyMoveTime = 0;
				gameOverColor = 255;
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				blueEnemy5.pos.x = { 384 };
				blueEnemy6.pos.x = { 576 };
				blueEnemy7.pos.x = { 768 };
				blueEnemy8.pos.x = { 960 };
				pinkEnemy5.pos.x = { 384 };
				pinkEnemy6.pos.x = { 576 };
				pinkEnemy7.pos.x = { 768 };
				pinkEnemy8.pos.x = { 960 };
				Scene = STAGE3;
				Blackinflag = 0;
			}
		}
		if (Blackinflag == 1 && Stagecount == 4) {
			gameOverColor += 3;
			if (gameOverColor >= 255) {
				gameOverColor = 255;
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				pinkEnemyX[0] = 150;
				pinkEnemyX[1] = 450;
				pinkEnemyX[2] = 750;
				pinkEnemyX[3] = 1050;
				blueEnemyX[0] = 300;
				blueEnemyX[1] = 600;
				blueEnemyX[2] = 900;
				blueEnemyX[3] = 1200;
				Scene = STAGE4;
				Blackinflag = 0;
			}
		}
		if (Blackinflag == 0) {
			gameOverColor -= 3;
			if (gameOverColor <= 0) {
				gameOverColor = 0;
			}
		}

		switch (Scene) {
		case TITLE: //0
			Color = 0;
			stageClear = 0;
			if (Novice::IsTriggerButton(0, kPadButton10)) {
				Scene = STAGE1;
			}
			if (keys[DIK_T] && preKeys[DIK_T] == 0) {
				Scene = STAGE4;
			}
			break;
		case GAMEOVER: //1

			break;
		case STAGE1: //2
			Stagecount = 1;

			enemyTime++;
			playerTime++;
			if (enemyTime >= 81) {
				enemyTime = 0;
			}
			if (playerTime >= 41) {
				playerTime = 0;
			}
			if (Color == 0 && gameOverColor == 0) {
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
								Blackinflag = 1;
								Scene = GAMEOVER;
							}
							if (player->BluePlayer.pos.x * 32 > x * BlockSize && player->BluePlayer.pos.x * 32 < x * BlockSize + 32 &&
								player->BluePlayer.pos.y * 32 + 16 > y * BlockSize && player->BluePlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Blackinflag = 1;
								Scene = GAMEOVER;
							}
						}
					}
					//青の敵との当たり判定
					if (map[y][x] == BLUEENEMY) {
						if (UpSideGround == true) {
							if (player->PinkPlayer.pos.x * 32 + 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 + 32 < x * BlockSize + 32 &&
								player->PinkPlayer.pos.y * 32 + 16 > y * BlockSize && player->PinkPlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Blackinflag = 1;
								Scene = GAMEOVER;
							}
							if (player->PinkPlayer.pos.x * 32 > x * BlockSize && player->PinkPlayer.pos.x * 32 < x * BlockSize + 32 &&
								player->PinkPlayer.pos.y * 32 + 16 > y * BlockSize && player->PinkPlayer.pos.y * 32 + 16 < y * BlockSize + 32) {
								Blackinflag = 1;
								Scene = GAMEOVER;
							}
						}
					}
				}
			}
			}
			if (stageClear == 1) {
				Color = 255;
			}

			if (Color == 255) {
				Scene = STAGE2;
				stageClear = 0;
			}
			//シーン移行
			if (player->BluePlayer.pos.x > 40) {
				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				blueEnemy1.pos.x = { 470 };
				blueEnemy1.pos.y = { 480 };
				pinkEnemy1.pos.x = { 470 };
				pinkEnemy1.pos.y = { 256 };
				blueEnemy2.pos.x = { 662 };
				blueEnemy2.pos.y = { 256 };
				pinkEnemy2.pos.x = { 662 };
				pinkEnemy2.pos.y = { 480 };
				blueEnemy3.pos.x = { 854 };
				blueEnemy3.pos.y = { 480 };
				pinkEnemy3.pos.x = { 854 };
				pinkEnemy3.pos.y = { 256 };
				blueEnemy4.pos.x = { 1046 };
				blueEnemy4.pos.y = { 256 };
				pinkEnemy4.pos.x = { 1046 };
				pinkEnemy4.pos.y = { 480 };

				stageClear = 1;
			}
			break;
		case STAGE2: //3
			Stagecount = 2;
			enemyTime++;
			playerTime++;
			if (enemyTime >= 81) {
				enemyTime = 0;
			}
			if (playerTime >= 41) {
				playerTime = 0;
			}
			if (!stageClear) {
				if (Color != 0) {
					Color -= 3;
				}
			}
			if (Color == 0 && gameOverColor == 0) {
				player->Update();

				blueEnemy1.pos.x--;
				blueEnemy2.pos.x--;
				blueEnemy3.pos.x--;
				blueEnemy4.pos.x--;

				pinkEnemy1.pos.x--;
				pinkEnemy2.pos.x--;
				pinkEnemy3.pos.x--;
				pinkEnemy4.pos.x--;

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

				if (stageClear == 1) {
					Color = 255;
				}

				if (Color == 255) {
					Scene = STAGE3;
					stageClear = 0;
				}
				//シーン移行
				if (player->BluePlayer.pos.x > 40) {
					player->BluePlayer.pos.x = { 1 };
					player->PinkPlayer.pos.x = { 1 };
					enemyTime = 0;
					playerTime = 0;
					stageClear = 1;
					blueEnemy5.pos.x = { 470 };
					blueEnemy5.pos.y = { 480 };
					pinkEnemy5.pos.x = { 470 };
					pinkEnemy5.pos.y = { 256 };
					blueEnemy6.pos.x = { 662 };
					blueEnemy6.pos.y = { 256 };
					pinkEnemy6.pos.x = { 662 };
					pinkEnemy6.pos.y = { 480 };
					blueEnemy7.pos.x = { 854 };
					blueEnemy7.pos.y = { 480 };
					pinkEnemy7.pos.x = { 854 };
					pinkEnemy7.pos.y = { 256 };
					blueEnemy8.pos.x = { 1046 };
					blueEnemy8.pos.y = { 256 };
					pinkEnemy8.pos.x = { 1046 };
					pinkEnemy8.pos.y = { 480 };
					Scene = STAGE3;
				}
			}
			//敵との当たり判定
			if (player->BluePlayer.pos.x * 32 + 32 > pinkEnemy1.pos.x && player->BluePlayer.pos.x * 32 + 32 < pinkEnemy1.pos.x + 32 &&
				player->BluePlayer.pos.y * 32 + 16 > pinkEnemy1.pos.y && player->BluePlayer.pos.y * 32 + 16 < pinkEnemy1.pos.y + 32) {
				Blackinflag = 1;
				Scene = GAMEOVER;
			}
			if (player->BluePlayer.pos.x * 32 + 32 > pinkEnemy3.pos.x && player->BluePlayer.pos.x * 32 + 32 < pinkEnemy3.pos.x + 32 &&
				player->BluePlayer.pos.y * 32 + 16 > pinkEnemy3.pos.y && player->BluePlayer.pos.y * 32 + 16 < pinkEnemy3.pos.y + 32) {
				Blackinflag = 1;
				Scene = GAMEOVER;
			}
			if (player->PinkPlayer.pos.x * 32 + 32 > blueEnemy2.pos.x && player->PinkPlayer.pos.x * 32 + 32 < blueEnemy2.pos.x + 32 &&
				player->PinkPlayer.pos.y * 32 + 16 > blueEnemy2.pos.y && player->PinkPlayer.pos.y * 32 + 16 < blueEnemy2.pos.y + 32) {
				Blackinflag = 1;
				Scene = GAMEOVER;
			}
			if (player->PinkPlayer.pos.x * 32 + 32 > blueEnemy4.pos.x && player->PinkPlayer.pos.x * 32 + 32 < blueEnemy4.pos.x + 32 &&
				player->PinkPlayer.pos.y * 32 + 16 > blueEnemy4.pos.y && player->PinkPlayer.pos.y * 32 + 16 < blueEnemy4.pos.y + 32) {
				Blackinflag = 1;
				Scene = GAMEOVER;
			}

			break;
		case STAGE3: //4
			Stagecount = 3;
			enemyTime++;
			playerTime++;
			if (enemyTime >= 81) {
				enemyTime = 0;
			}
			if (playerTime >= 41) {
				playerTime = 0;
			}
			if (!stageClear) {
				if (Color != 0) {
					Color -= 3;
				}
			}
			if (Color == 0 && gameOverColor == 0) {
				enemyMoveTime++;
				player->Update();
				if (enemyMoveTime >= 0 && enemyMoveTime <= 80) {
					blueEnemy5.pos.x--;
					blueEnemy6.pos.x--;
					blueEnemy7.pos.x--;
					blueEnemy8.pos.x--;

					pinkEnemy5.pos.x--;
					pinkEnemy6.pos.x--;
					pinkEnemy7.pos.x--;
					pinkEnemy8.pos.x--;
				}
				if (enemyMoveTime >= 80) {
					blueEnemy5.pos.x++;
					blueEnemy6.pos.x++;
					blueEnemy7.pos.x++;
					blueEnemy8.pos.x++;

					pinkEnemy5.pos.x++;
					pinkEnemy6.pos.x++;
					pinkEnemy7.pos.x++;
					pinkEnemy8.pos.x++;
				}
				if (enemyMoveTime >= 161) {
					enemyMoveTime = 0;
				}
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
				if (stageClear == 1) {
					Color = 255;
					stageClear = 0;
				}
				//シーン移行
				if (player->BluePlayer.pos.x > 40) {
					player->BluePlayer.pos.x = { 1 };
					player->PinkPlayer.pos.x = { 1 };
					enemyMoveTime = 0;
					Scene = STAGE4;
					stageClear = 1;
				}
			}
			//敵との当たり判定
			if (player->BluePlayer.pos.x * 32 + 32 > pinkEnemy5.pos.x && player->BluePlayer.pos.x * 32 + 32 < pinkEnemy5.pos.x + 32 &&
				player->BluePlayer.pos.y * 32 + 16 > pinkEnemy5.pos.y && player->BluePlayer.pos.y * 32 + 16 < pinkEnemy5.pos.y + 32) {
				enemyMoveTime = 0;
				Blackinflag = 1;
				Scene = GAMEOVER;
			}
			if (player->BluePlayer.pos.x * 32 + 32 > pinkEnemy6.pos.x && player->BluePlayer.pos.x * 32 + 32 < pinkEnemy6.pos.x + 32 &&
				player->BluePlayer.pos.y * 32 + 16 > pinkEnemy6.pos.y && player->BluePlayer.pos.y * 32 + 16 < pinkEnemy6.pos.y + 32) {
				enemyMoveTime = 0;
				Blackinflag = 1;
				Scene = GAMEOVER;
			}
			if (player->PinkPlayer.pos.x * 32 + 32 > blueEnemy7.pos.x && player->PinkPlayer.pos.x * 32 + 32 < blueEnemy7.pos.x + 32 &&
				player->PinkPlayer.pos.y * 32 + 16 > blueEnemy7.pos.y && player->PinkPlayer.pos.y * 32 + 16 < blueEnemy7.pos.y + 32) {
				enemyMoveTime = 0;
				Blackinflag = 1;
				Scene = GAMEOVER;
			}
			if (player->PinkPlayer.pos.x * 32 + 32 > blueEnemy8.pos.x && player->PinkPlayer.pos.x * 32 + 32 < blueEnemy8.pos.x + 32 &&
				player->PinkPlayer.pos.y * 32 + 16 > blueEnemy8.pos.y && player->PinkPlayer.pos.y * 32 + 16 < blueEnemy8.pos.y + 32) {
				enemyMoveTime = 0;
				Blackinflag = 1;
				Scene = GAMEOVER;
			}

			break;
		case STAGE4: //5
			Stagecount = 4;
			enemyTime++;
			playerTime++;
			if (enemyTime >= 81) {
				enemyTime = 0;
			}
			if (playerTime >= 41) {
				playerTime = 0;
			}
			if (!stageClear) {
				if (Color != 0) {
					Color -= 3;
				}
			}
			if (Color == 0 && gameOverColor == 0) {
				enemyMoveTime++;
				player->Update();
				
			if (enemyMoveTime >= 0 && enemyMoveTime <= 60) {
				pinkEnemyX[0]++;
				pinkEnemyX[1]--;
				blueEnemyX[1] -= enemyMove;
				pinkEnemyX[3] -= 4;
			}
			if (enemyMoveTime >= 40) {
				blueEnemyX[2] -= enemyMove;
			}
			if (enemyMoveTime >= 61) {
				pinkEnemyX[0]--;
				pinkEnemyX[1]++;
			}
			if (enemyMoveTime >= 70) {
				blueEnemyX[2]++;
			}
			if (enemyMoveTime >= 80) {
				blueEnemyX[1]++;
				pinkEnemyX[3] += 6;
			}
			if (enemyMoveTime >= 121) {
				enemyMoveTime = 0;
			}
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
				if (stageClear == 1) {
					Color = 255;
					stageClear = 0;
				}
				//シーン移行
				if (player->BluePlayer.pos.x > 40) {
					player->BluePlayer.pos.x = { 1 };
					player->PinkPlayer.pos.x = { 1 };
					Scene = CLEAR;
					stageClear = 1;
				}
			}
			//敵との当たり判定
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (player->BluePlayer.pos.x * 32.0f + 32.0f > pinkEnemyX[i] && player->BluePlayer.pos.x * 32.0f + 32.0f < pinkEnemyX[i] + 32 &&
						player->BluePlayer.pos.y * 32.0f + 16.0f > pinkEnemyY[i] && player->BluePlayer.pos.y * 32.0f + 16.0f < pinkEnemyY[i] + 32) {
						Blackinflag = 1;
						Scene = GAMEOVER;
					}
					if (player->PinkPlayer.pos.x * 32 + 32 > blueEnemyX[i] && player->PinkPlayer.pos.x * 32 + 32 < blueEnemyX[i] + 32 &&
						player->PinkPlayer.pos.y * 32 + 16 > blueEnemyY[i] && player->PinkPlayer.pos.y * 32 + 16 < blueEnemyY[i] + 32) {
						Blackinflag = 1;
						Scene = GAMEOVER;
					}
				}
			}
			
			break;
		case CLEAR: //6
			Stagecount = 0;
			if (Novice::IsTriggerButton(0, kPadButton10)) {
				enemyMoveTime = 0;
				gameOverColor = 255;


				player->BluePlayer.pos.x = { 1 };
				player->PinkPlayer.pos.x = { 1 };
				blueEnemy1.pos.x = { 384 };
				blueEnemy2.pos.x = { 576 };
				blueEnemy3.pos.x = { 768 };
				blueEnemy4.pos.x = { 960 };
				pinkEnemy1.pos.x = { 384 };
				pinkEnemy2.pos.x = { 576 };
				pinkEnemy3.pos.x = { 768 };
				pinkEnemy4.pos.x = { 960 };
				blueEnemy5.pos.x = { 384 };
				blueEnemy6.pos.x = { 576 };
				blueEnemy7.pos.x = { 768 };
				blueEnemy8.pos.x = { 960 };
				pinkEnemy5.pos.x = { 384 };
				pinkEnemy6.pos.x = { 576 };
				pinkEnemy7.pos.x = { 768 };
				pinkEnemy8.pos.x = { 960 };
				pinkEnemyX[0] = 150;
				pinkEnemyX[1] = 450;
				pinkEnemyX[2] = 750;
				pinkEnemyX[3] = 1050;
				blueEnemyX[0] = 300;
				blueEnemyX[1] = 600;
				blueEnemyX[2] = 900;
				blueEnemyX[3] = 1200;
				Scene = TITLE;
			}
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
			Novice::DrawSprite(0, 0, gameOver, 1, 1, 0.0f, WHITE);
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
						if (enemyTime >= 1 && enemyTime <= 10) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp1, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 11 && enemyTime <= 20) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp2, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 21 && enemyTime <= 30) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp3, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 31 && enemyTime <= 40) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp4, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 41 && enemyTime <= 50) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp5678910, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 51 && enemyTime <= 60) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp11, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 61 && enemyTime <= 70) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp12, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 71 && enemyTime <= 80) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, blueEnemyUp13, 1, 1, 0.0f, WHITE);
						}
					}
					if (map[y][x] == PINKENEMY) {
						if (enemyTime >= 1 && enemyTime <= 10) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp1, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 11 && enemyTime <= 20) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp2, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 21 && enemyTime <= 30) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp3, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 31 && enemyTime <= 40) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp4, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 41 && enemyTime <= 50) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp5678910, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 51 && enemyTime <= 60) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp11, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 61 && enemyTime <= 70) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp12, 1, 1, 0.0f, WHITE);
						}
						if (enemyTime >= 71 && enemyTime <= 80) {
							Novice::DrawSprite(x * BlockSize, y * BlockSize, pinkEnemyUp13, 1, 1, 0.0f, WHITE);
						}
					}
				}
			}
			//プレイヤー描画
			if (player->playerUpSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			if (player->playerDownSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			break;
		case STAGE2: //3
			if (UpSideGround == false) {
				Novice::DrawSprite(0, 0, blueHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, pinkgroundUp, 1, 1, 0.0f, WHITE);
			}
			if (DownSideGround == false) {
				Novice::DrawSprite(0, 0, pinkHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruDown, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, bluegroundUp, 1, 1, 0.0f, WHITE);
			}
			//プレイヤー描画
			if (player->playerUpSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			if (player->playerDownSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			//敵の描画
			if (enemyTime >= 1 && enemyTime <= 10) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 11 && enemyTime <= 20) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 21 && enemyTime <= 30) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 31 && enemyTime <= 40) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 41 && enemyTime <= 50) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 51 && enemyTime <= 60) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 61 && enemyTime <= 70) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 71 && enemyTime <= 80) {
				Novice::DrawSprite(int(blueEnemy1.pos.x), int(blueEnemy1.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy2.pos.x), int(blueEnemy2.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy3.pos.x), int(blueEnemy3.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy4.pos.x), int(blueEnemy4.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy1.pos.x), int(pinkEnemy1.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy2.pos.x), int(pinkEnemy2.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy3.pos.x), int(pinkEnemy3.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy4.pos.x), int(pinkEnemy4.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			
			
			break;
		case STAGE3: //4
			if (UpSideGround == false) {
				Novice::DrawSprite(0, 0, blueHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, pinkgroundUp, 1, 1, 0.0f, WHITE);
			}
			if (DownSideGround == false) {
				Novice::DrawSprite(0, 0, pinkHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruDown, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, bluegroundUp, 1, 1, 0.0f, WHITE);
			}
			//プレイヤー描画
			if (player->playerUpSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			if (player->playerDownSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			//敵の描画
			if (enemyTime >= 1 && enemyTime <= 10) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp1, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 11 && enemyTime <= 20) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp2, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 21 && enemyTime <= 30) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp3, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 31 && enemyTime <= 40) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp4, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 41 && enemyTime <= 50) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp5678910, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 51 && enemyTime <= 60) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp11, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 61 && enemyTime <= 70) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp12, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}
			if (enemyTime >= 71 && enemyTime <= 80) {
				Novice::DrawSprite(int(blueEnemy5.pos.x), int(blueEnemy5.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy6.pos.x), int(blueEnemy6.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy7.pos.x), int(blueEnemy7.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(blueEnemy8.pos.x), int(blueEnemy8.pos.y), blueEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy5.pos.x), int(pinkEnemy5.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy6.pos.x), int(pinkEnemy6.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy7.pos.x), int(pinkEnemy7.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(int(pinkEnemy8.pos.x), int(pinkEnemy8.pos.y), pinkEnemyUp13, 1.0, 1.0, 0.0f, 0xFFFFFFFF);
			}

			break;
		case STAGE4: //5
			if (UpSideGround == false) {
				Novice::DrawSprite(0, 0, blueHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, pinkgroundUp, 1, 1, 0.0f, WHITE);
			}
			if (DownSideGround == false) {
				Novice::DrawSprite(0, 0, pinkHaikeiUp, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 0, kurisutaruDown, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(0, 272, bluegroundUp, 1, 1, 0.0f, WHITE);
			}
			//プレイヤー描画
			if (player->playerUpSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			if (player->playerDownSideGround == true) {
				if (playerTime >= 1 && playerTime <= 20) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown1, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp1, 1, 1, 0.0f, WHITE);
				}
				if (playerTime >= 21 && playerTime <= 40) {
					Novice::DrawSprite(
						static_cast<int>(player->PinkPlayer.pos.x * 32),
						static_cast<int>(player->PinkPlayer.pos.y * 32),
						pinkPlayerDown2, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(
						static_cast<int>(player->BluePlayer.pos.x * 32),
						static_cast<int>(player->BluePlayer.pos.y * 32),
						bluePlayerUp2, 1, 1, 0.0f, WHITE);
				}
			}
			//敵の描画
			if (enemyTime >= 1 && enemyTime <= 10) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp1, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp1, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 11 && enemyTime <= 20) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp2, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp2, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 21 && enemyTime <= 30) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp3, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp3, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 31 && enemyTime <= 40) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp4, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp4, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 41 && enemyTime <= 50) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp5678910, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp5678910, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 51 && enemyTime <= 60) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp11, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp11, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 61 && enemyTime <= 70) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp12, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp12, 1, 1, 0.0f, WHITE);
					}
				}
			}
			if (enemyTime >= 71 && enemyTime <= 80) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Novice::DrawSprite(pinkEnemyX[i], pinkEnemyY[i], pinkEnemyUp13, 1, 1, 0.0f, WHITE);
						Novice::DrawSprite(blueEnemyX[j], blueEnemyY[j], blueEnemyUp13, 1, 1, 0.0f, WHITE);
					}
				}
			}
			
			break;
		case CLEAR: //6
			Novice::DrawSprite(0, 0, gameClear, 1, 1, 0.0f, WHITE);
			break;
		}

		Novice::DrawBox(0, 0, 1280, 752, 0.0f, 0x00000000 + Color, kFillModeSolid);
		Novice::DrawBox(0, 0, 1280, 752, 0.0f, 0x00000000 + gameOverColor, kFillModeSolid);

		Novice::ScreenPrintf(100, 100, "Scene = %d", Scene);
		Novice::ScreenPrintf(100, 120, "UpSide = %d", UpSideGround);
		Novice::ScreenPrintf(100, 140, "DownSide = %d", DownSideGround);
		Novice::ScreenPrintf(100, 160, "ChangeFlag = %d", changeFlag);
		Novice::ScreenPrintf(100, 180, "ChangePlayerFlag = %d", player->changePlayerFlag);
		Novice::ScreenPrintf(100, 200, "stageCount = %d", Stagecount);
		Novice::ScreenPrintf(100, 220, "Black = %d", Blackinflag);
		Novice::ScreenPrintf(100, 240, "stageclear = %d", stageClear);

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
