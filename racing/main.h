//Yuna Lee
#include "images.h"

#define SC WHITE
#define END WHITE

#define MAX_COIN 5
#define MAX_COIN_FEVER 15
#define MAX_APPLE 5
#define MAX_LANE 5
#define MAX_LIFE 3

typedef struct {
	int x, y;
} POINT;

typedef struct {
	int lane;
	POINT pt;
} LANE;

typedef struct {
	int lane;
	POINT pt;
	SIZE size;
	const u16* image;
	bool isMS;
	bool flag;
} APPLE;

typedef struct {
	int lane;
	POINT pt;
	bool flag;
} COIN;

typedef struct {
	int lane;
	POINT pt;
	bool flag;
} ITEM;

typedef struct {
	int lane;
	bool flag;
} WARNING;

void getKey();
u16 keyHit(u16 key);
void initialize();
void drawGameScreen();
void sleep(int interval);
void updateScore(int score);
void drawNumber(int n, int x, int y);
void addCoin(COIN coin, COIN* coins);
void addApple(APPLE apple, APPLE* apples);
void movePlayer(int player, int lane);

int score = 0; // game score
APPLE apples[MAX_APPLE]; // array of apples
LANE lanes[20]; // array of lanes
COIN coins[MAX_COIN_FEVER]; // array of coins
ITEM item;
int occupation[MAX_LANE];
int numOfApples, numOfCoins, life;
bool resurrecting, feverMode;
bool close[MAX_LANE];
int velocity, prev_velocity;
u16 curr_key, prev_key;

