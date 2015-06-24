//Yuna Lee

#include "main.h"


u16 keyHit(u16 key)
{
	return (curr_key & ~prev_key) & key;
}

void getKey()
{
	prev_key = curr_key;
    curr_key = ~REG_KEYINPUT & KEY_MASK;
}


int main()
{
	REG_DISPCNT = MODE3 | BG2_ON;
	
	COIN tmp_coin;
	
	bool started = false;
	bool drawn = false;
	bool titleShown = false;
	
	int timer = 0;
	int timer_2 = 0;
	int fever_timer = 0;
	
	while (1) {
		timer += 1/59.73 * WEIGHT; /** TIMER: 1/59.73s added for each loop **/
		if (timer > 2000000000) timer -= 2000000000; // prevent overflow
		timer_2 += 1/59.73 * WEIGHT;
		if (timer_2 > 2000000000) timer_2 -= 2000000000;
		fever_timer += 1/59.73 * WEIGHT;
		if (fever_timer > 2000000000) fever_timer -= 2000000000;
		getKey();
		if (!started) {
			srand(timer);
			random(0, timer); // shuffles the random generator while waiting
			if (!drawn) {
				timer = 0;
				drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(5,5,5));
				drawImage3(5, 0, MAIN_SCREEN_WIDTH, MAIN_SCREEN_HEIGHT, main_screen_s);
				drawn = true;
				titleShown = true;
			} else {
				// draw/remove 'press select to start'
				if ((timer % 220) == 0) {
					if (titleShown) {
						drawImage3(5, 0, MAIN_SCREEN_WIDTH, MAIN_SCREEN_HEIGHT, main_screen);
					} else {
						drawImage3(5, 0, MAIN_SCREEN_WIDTH, MAIN_SCREEN_HEIGHT, main_screen_s);
					}
					titleShown = !titleShown;
				}
			}
			if (keyHit(KEY_SELECT)) {
				drawn = false;
				titleShown = false;
				initialize();
				drawGameScreen();
				started = true;
				timer = 0;
			}
		} else if (!life) {
			if (timer_2 == 0) {
				// progressively fill in the game screen black
				for (int i = 0; i < SCREEN_HEIGHT; i++) {
					drawRect(i, 0, SCREEN_WIDTH, 1, BLACK);
					sleep(1);
				}
				drawRect(SCREEN_HEIGHT/2+10, 20+95, 22, 7, END); // E
				drawRect(SCREEN_HEIGHT/2+12+10, 20+95, 22, 7, END);
				drawRect(SCREEN_HEIGHT/2+24+10, 20+95, 22, 7, END);
				drawRect(SCREEN_HEIGHT/2+10, 20+95, 7, 24, END);
				drawRect(SCREEN_HEIGHT/2+10, 47+95, 7, 31, END); // N
				drawRect(SCREEN_HEIGHT/2+10+5, 54+95, 6, 15, END);
				drawRect(SCREEN_HEIGHT/2+10+5+7, 54+95+6, 6, 15, END);
				drawRect(SCREEN_HEIGHT/2+10, 54+95+12, 7, 31, END);
				drawRect(SCREEN_HEIGHT/2+10, 78+95, 7, 31, END); // D
				drawRect(SCREEN_HEIGHT/2+10, 85+95, 10, 7, END);
				drawRect(SCREEN_HEIGHT/2+10+24, 85+95, 10, 7, END);
				drawRect(SCREEN_HEIGHT/2+10+7, 95+95, 7, 17, END);
			} else if (timer_2/WEIGHT/5 || keyHit(KEY_SELECT)) {
				started = false;
			}
		} else {
			if (keyHit(KEY_LEFT)) {
				if (apples[0].lane != 0) {
					movePlayer(0, apples[0].lane-1);
				}
			} else if (keyHit(KEY_RIGHT)) {
				if (apples[0].lane != 4) {
					movePlayer(0, apples[0].lane+1);
				}
			} else if (keyHit(KEY_SELECT)) {
				started = false;
			}
			
			if (resurrecting) {
				if (timer_2/WEIGHT/7) {
					apples[0].image = mc;
					resurrecting = false;
				} else {
					if (timer_2/WEIGHT % 2) {
						apples[0].image = mc;
					} else {
						apples[0].image = mc_r;
					}
				}
			}
			
			
			if (prev_velocity != velocity) {
				// fever mode
				fever_timer = 0;
				feverMode = true;
				prev_velocity = velocity;
			}
			
			if (feverMode) {
				if (fever_timer/WEIGHT/5) feverMode = false;
			}
			
			else {
				// create a new apple every 3 seconds < MAX
				if (numOfApples < MAX_APPLE && (timer % (int)(0.1 * WEIGHT) == 0)) {
					APPLE tmp_apple;
					tmp_apple.lane = random(0, MAX_LANE);
					int n = random(2, size(apple_img));
					SIZE s = getSize(n);
					tmp_apple.pt.x = LANE_ORIGIN(tmp_apple.lane, s.width);
					bool added = false;
					if (n == 2 || n == 3) {
						if (occupation[tmp_apple.lane] == 0
							&& getPixel(SCREEN_HEIGHT, LANE_ORIGIN(tmp_apple.lane, 0)) == GRAY
							&& getPixel(SCREEN_HEIGHT+1, LANE_ORIGIN(tmp_apple.lane, 0)) == GRAY) {
							tmp_apple.pt.y = SCREEN_HEIGHT + 30;
							tmp_apple.isMS = true;
							occupation[tmp_apple.lane] = -1;
							close[tmp_apple.lane] = true;
							added = true;
						}
					} else {
						if (occupation[tmp_apple.lane] != -1
							&& getPixel(0, LANE_ORIGIN(tmp_apple.lane, 0)) == GRAY
							&& getPixel(-1, LANE_ORIGIN(tmp_apple.lane, 0)) == GRAY) {
							tmp_apple.pt.y = -s.height - 10;
							tmp_apple.isMS = false;
							occupation[tmp_apple.lane]++;
							added = true;
						}
					}
					if (added) {
						tmp_apple.image = apple_img[n];
						tmp_apple.size = s;
						tmp_apple.flag = true;
						addApple(tmp_apple, apples);
						numOfApples++;
					}
				}
			}
			
			// fever mode
			if (feverMode && numOfCoins < MAX_COIN_FEVER) {
				for (int i = 0; i < MAX_COIN_FEVER; i++) {
					if (numOfCoins >= MAX_COIN_FEVER) break;
					int try = 10;
					do {
						tmp_coin.lane = random(0, MAX_LANE);
						try--;
					} while (getPixel(1, LANE_ORIGIN(tmp_coin.lane, 0)) != GRAY 
							&& getPixel(-COIN_HEIGHT-1, LANE_ORIGIN(tmp_coin.lane, 0)) != GRAY
							&& try);
					tmp_coin.pt.x = LANE_ORIGIN(tmp_coin.lane, COIN_WIDTH);
					tmp_coin.pt.y = -COIN_HEIGHT;
					tmp_coin.flag = true;
					addCoin(tmp_coin, coins);
				}
			}
			
			// create a new coin <MAX
			else if (numOfCoins < MAX_COIN && (timer % (int)(0.3 * WEIGHT) == 0)) {
				int try = 10;
				do {
					tmp_coin.lane = random(0, MAX_LANE);
					try--;
				} while (getPixel(1, LANE_ORIGIN(tmp_coin.lane, 0)) != GRAY
						&& getPixel(-COIN_HEIGHT-1, LANE_ORIGIN(tmp_coin.lane, 0)) != GRAY
						&& try);
				if (getPixel(1, LANE_ORIGIN(tmp_coin.lane, 0)) == GRAY
					&& getPixel(-COIN_HEIGHT-1, LANE_ORIGIN(tmp_coin.lane, 0)) == GRAY) {
					tmp_coin.pt.x = LANE_ORIGIN(tmp_coin.lane, COIN_WIDTH);
					tmp_coin.pt.y = -COIN_HEIGHT;
					tmp_coin.flag = true;
					addCoin(tmp_coin, coins);
				}
			}
			
			// draw lanes ==> optimized drawing
			for (int i = 0; i < size(lanes); i++) {
				drawRect(lanes[i].pt.y, lanes[i].pt.x, 5, velocity, GRAY);
				lanes[i].pt.y += velocity;
				if (lanes[i].pt.y - 20 >= SCREEN_HEIGHT) {
					lanes[i].pt.y = -20;
				}
				drawRect(lanes[i].pt.y + 20 - velocity, lanes[i].pt.x, 5, velocity, WHITE);
			}
			
			// draw coins
			for (int i = 0; i < MAX_COIN; i++) {
				if (coins[i].flag) {
					drawRect(coins[i].pt.y, coins[i].pt.x, COIN_WIDTH, COIN_HEIGHT, GRAY);
					coins[i].pt.y += velocity;
					if (coins[i].pt.y >= SCREEN_HEIGHT) {
						coins[i].flag = false;
						numOfCoins--;
					} else {
						drawImage3(coins[i].pt.y, coins[i].pt.x, COIN_WIDTH, COIN_HEIGHT, coin);
					}
				}
			}
			
			// draw
			for (int i = 0; i < MAX_APPLE; i++) {
				if (apples[i].flag) {
					if (i != 0) {
						drawRect(apples[i].pt.y, apples[i].pt.x, apples[i].size.width, apples[i].size.height, GRAY);
						if (apples[i].isMS) {
							apples[i].pt.y--;
							if (apples[i].pt.y + apples[i].size.height <= 0) {
								apples[i].flag = false;
								numOfApples--;
								occupation[apples[i].lane] = 0;
							} else if (apples[i].pt.y < SCREEN_HEIGHT-1) {
								close[apples[i].lane] = false;
							}
						} else {
							apples[i].pt.y += velocity;
							if (apples[i].pt.y >= SCREEN_HEIGHT) {
								apples[i].flag = false;
								numOfApples--;
								occupation[apples[i].lane]--;
							}
						}
					}
					drawImage3(apples[i].pt.y, apples[i].pt.x, apples[i].size.width, apples[i].size.height, apples[i].image);
				}
			}
			
			// draw item
			if (item.flag) {
				drawRect(item.pt.y, item.pt.x, POWERUP_WIDTH, POWERUP_HEIGHT, GRAY);
				item.pt.y += velocity;
				if (item.pt.y >= SCREEN_HEIGHT) {
					item.flag = false;
				} else {
					drawImage3(item.pt.y, item.pt.x, POWERUP_WIDTH, POWERUP_HEIGHT, powerup);
				}
			} else if (timer % (int)(3 * WEIGHT) == 0) {
				int chance = random(0, 5);
				if (chance == 2) {
					ITEM tmp_item;
					int try = 10;
					do {
						tmp_item.lane = random(0, MAX_LANE);
						try--;
					} while (getPixel(1, LANE_ORIGIN(tmp_item.lane, 0)) != GRAY
							&& getPixel(-POWERUP_HEIGHT-1, LANE_ORIGIN(tmp_item.lane, 0)) != GRAY
							&& try);
					if (getPixel(1, LANE_ORIGIN(tmp_item.lane, 0)) == GRAY
						&& getPixel(-POWERUP_HEIGHT-1, LANE_ORIGIN(tmp_item.lane, 0)) == GRAY) {
						tmp_item.pt.x = LANE_ORIGIN(tmp_item.lane, POWERUP_WIDTH);
						tmp_item.pt.y = -POWERUP_HEIGHT;
						tmp_item.flag = true;
						item = tmp_item;
					}
				}
			}
			
			// draw a warning sign for any incoming microsoft
			for (int i = 0; i < size(close); i++) {
				if (close[i]) {
					drawImage3(SCREEN_HEIGHT-WARNING_HEIGHT-25, LANE_ORIGIN(i, WARNING_WIDTH), WARNING_WIDTH, WARNING_HEIGHT, warning);
				}
			}
			
			// check collision 
			int ax1 = apples[0].pt.x;
			int ay1 = apples[0].pt.y;
			int ax2 = apples[0].pt.x + apples[0].size.width;
			int ay2 = apples[0].pt.y + apples[0].size.height;
			int bx1, by1, bx2, by2;
			int clane = apples[0].lane;
			
			for (int i = 1; i < size(apples); i++) {
				if (resurrecting) break;
				if (!apples[i].flag || apples[i].lane != clane) continue;
				bx1 = apples[i].pt.x;
				by1 = apples[i].pt.y;
				bx2 = bx1 + apples[i].size.width;
				by2 = by1 + apples[i].size.height;
				if (!(by2 < ay1 || ay2 < by1 || bx2 < ax1 || ax2 < bx1)) {
					// crashed
					life--;
					timer_2 = 0;
					int tmp_score = score;
					int tmp_life = life;
					int tmp_velocity = prev_velocity;
					initialize();
					resurrecting = true;
					score = tmp_score;
					life = tmp_life;
					prev_velocity = tmp_velocity;
					drawGameScreen();
					movePlayer(0, 2);
					apples[0].image = mc_r;
					if (!life) timer_2 = -(1/59.73 * WEIGHT);
					break;
				}
			}
			
			for (int i = 0; i < size(coins); i++) {
				if (!coins[i].flag || coins[i].lane != clane) continue;
				bx1 = coins[i].pt.x;
				by1 = coins[i].pt.y;
				bx2 = bx1 + COIN_WIDTH;
				by2 = by1 + COIN_HEIGHT;
				if (!(by2 < ay1 || ay2 < by1 || bx2 < ax1 || ax2 < bx1)) {
					score += 10 * WEIGHT;
					coins[i].flag = false;
					numOfCoins--;
					drawRect(by1, bx1, COIN_WIDTH, COIN_HEIGHT, GRAY);
				}
			}
			
			if (item.flag && item.lane == clane) {
				bx1 = item.pt.x;
				by1 = item.pt.y;
				bx2 = bx1 + POWERUP_WIDTH;
				by2 = by1 + POWERUP_HEIGHT;
				if (!(by2 < ay1 || ay2 < by1 || bx2 < ax1 || ax2 < bx1)) {
					timer_2 = 0;
					resurrecting = true;
					apples[0].image = mc_r;
					item.flag = false;
					drawRect(by1, bx1, POWERUP_WIDTH, POWERUP_HEIGHT, GRAY);
				}
			}
			
			// update the score
			updateScore(score/WEIGHT);
			score += 0.1 * WEIGHT; // score++
			
			// velocity to score
			velocity = score/WEIGHT/500 + 1;
		}
	
		waitForVBlank();
	}
	
	return 0;
}

void initialize()
{
	for (int i = 0; i < size(occupation); i++) {
		occupation[i] = 0;
	}
	
	for (int i = 0; i < size(apples); i++) {
		apples[i].flag = false;
	}
	
	for (int i = 0; i < size(coins); i++) {
		coins[i].flag = false;
	}
	
	for (int i = 0; i < size(close); i++) {
		close[i] = false;
	}
	
	item.flag = false;
	score = 0;
	numOfApples = 0;
	numOfCoins = 0;
	life = MAX_LIFE;
	resurrecting = false;
	feverMode = false;
	velocity = 1;
	prev_velocity = velocity;
}

void drawGameScreen()
{
	// draw initial background
	//drawImage3(0, 0, 20, SCREEN_HEIGHT, grass);
	drawRect(0, 20, 200, SCREEN_HEIGHT, GRAY);
	//drawImage3(0, 220, 20, SCREEN_HEIGHT, grass);
	
	apples[0].lane = 2;
	apples[0].pt.x = LANE_ORIGIN(apples[0].lane, MC_WIDTH);
	apples[0].pt.y = SCREEN_HEIGHT - MC_HEIGHT - 15;
	apples[0].image = mc;
	apples[0].size.width = MC_WIDTH;
	apples[0].size.height = MC_HEIGHT;
	apples[0].flag = true;
	occupation[apples[0].lane] = 1;
	numOfApples++;
	
	// initial lanes
	for (int i = 0; i < size(lanes); i++) {
		lanes[i].lane = i % 4;
		lanes[i].pt.x = lanes[i].lane * 41 + 56;
		lanes[i].pt.y = i / 4 * 40 + 10 - 41;
		drawRect(lanes[i].pt.y, lanes[i].pt.x, 5, 20, WHITE);
	}
	
	if (life) drawImage3(SCREEN_HEIGHT-LIFE_IMG_HEIGHT-5, SCREEN_WIDTH-LIFE_IMG_WIDTH-6, LIFE_IMG_WIDTH, LIFE_IMG_HEIGHT, life_img);
	if (life > 1) drawImage3(SCREEN_HEIGHT-(LIFE_IMG_HEIGHT+5)*2, SCREEN_WIDTH-LIFE_IMG_WIDTH-6, LIFE_IMG_WIDTH, LIFE_IMG_HEIGHT, life_img);
	if (life > 2) drawImage3(SCREEN_HEIGHT-(LIFE_IMG_HEIGHT+5)*3, SCREEN_WIDTH-LIFE_IMG_WIDTH-6, LIFE_IMG_WIDTH, LIFE_IMG_HEIGHT, life_img);
	
	updateScore(score);
}

void addCoin(COIN coin, COIN* coins)
{
	for (int i = 0; i < MAX_COIN; i++) {
		if (!coins[i].flag) {
			coins[i] = coin;
			numOfCoins++;
			break;
		}
	}
}

void addApple(APPLE apple, APPLE* apples)
{
	for (int i = 0; i < MAX_APPLE; i++) {
		if (!apples[i].flag) {
			apples[i] = apple;
			break;
		}
	}
}

void movePlayer(int apple, int lane)
{
	APPLE c = apples[apple];
	occupation[c.lane]--;
	drawRect(c.pt.y, c.pt.x, c.size.width, c.size.height, GRAY);
	apples[apple].pt.x = LANE_ORIGIN(lane, c.size.width);
	apples[apple].lane = lane;
	occupation[c.lane]++;
}

// pauses
void sleep(int interval)
{
	volatile int i;
	for (i = 0; i < interval * 1000; i++);
}

// updates the score
void updateScore(int score)
{
	// draw score label
	drawRect(5, 5, 6, 2, SC); // S
	drawRect(5, 5, 2, 4, SC);
	drawRect(9, 5, 6, 2, SC);
	drawRect(9, 5+4, 2, 4, SC);
	drawRect(13, 5, 6, 2, SC);
	drawRect(5, 5+8, 6, 2, SC); // C
	drawRect(5, 5+8, 2, 8, SC);
	drawRect(13, 5+8, 6, 2, SC);
	drawRect(5, 5+16, 6, 2, SC); // O
	drawRect(5, 5+16, 2, 8, SC);
	drawRect(5, 5+20, 2, 8, SC);
	drawRect(13, 5+16, 6, 2, SC);
	drawRect(5, 5+24, 3, 2, SC); // R
	drawRect(5, 5+24, 2, 10, SC);
	drawRect(5, 5+27, 2, 4, SC);
	drawRect(9, 5+24, 6, 2, SC);
	drawRect(9, 5+28, 2, 6, SC);
	drawRect(5, 5+32, 6, 2, SC); // E
	drawRect(9, 5+32, 6, 2, SC);
	drawRect(13, 5+32, 6, 2, SC);
	drawRect(5, 5+32, 2, 8, SC);
	drawRect(7, 5+40, 2, 2, SC); // :
	drawRect(12, 5+40, 2, 2, SC);
	
	int x = 50, y = 5;
	int n = score;
	
	// remove previous score label
	if (score > 999) {
		drawRect(y, x, 32, 10, GRAY);
	} else if (score > 99) {
		drawRect(y, x, 24, 10, GRAY);
	} else if (score > 9) {
		drawRect(y, x, 16, 10, GRAY);
	} else {
		drawRect(y, x, 8, 10, GRAY);
	}
	
	if (score > 9999) {
//		drawNumber(1000, x, y);
		return;
	}
	
	if (score > 999) {
		n = score / 1000;
		drawNumber(n, x, y);
		score -= n * 1000;
		x += 8;
		if (score < 100) {
			drawNumber(0, x, y);
			x += 8;
			if (score < 10) {
				drawNumber(0, x, y);
				x += 8;
			}
		}
	}
	if (score > 99) {
		n = score / 100;
		drawNumber(n, x, y);
		score -= n * 100;
		x += 8;
		if (score < 10) {
			drawNumber(0, x, y);
			x += 8;
		}
	}
	if (score > 9) {
		n = score / 10;
		drawNumber(n, x, y);
		score -= n * 10;
		x += 8;
	}
	drawNumber(score, x, y);
}

// draws a number n on (x, y)
void drawNumber(int n, int x, int y)
{
	if (n == 0) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y, x, 2, 8, SC);
		drawRect(y, x+4, 2, 8, SC);
		drawRect(y+8, x, 6, 2, SC);
	} else if (n == 1) {
		drawRect(y, x+3, 2, 10, SC);
	} else if (n == 2) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y+4, x, 6, 2, SC);
		drawRect(y+8, x, 6, 2, SC);
		drawRect(y, x+4, 2, 4, SC);
		drawRect(y+4, x, 2, 4, SC);
	} else if (n == 3) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y+4, x, 6, 2, SC);
		drawRect(y+8, x, 6, 2, SC);
		drawRect(y, x+4, 2, 8, SC);
	} else if (n == 4) {
		drawRect(y, x, 2, 4, SC);
		drawRect(y, x+4, 2, 10, SC);
		drawRect(y+4, x, 7, 2, SC);
	} else if (n == 5) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y, x, 2, 4, SC);
		drawRect(y+4, x, 6, 2, SC);
		drawRect(y+4, x+4, 2, 4, SC);
		drawRect(y+8, x, 6, 2, SC);
	} else if (n == 6) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y, x, 2, 10, SC);
		drawRect(y+4, x, 6, 2, SC);
		drawRect(y+4, x+4, 2, 4, SC);
		drawRect(y+8, x, 6, 2, SC);
	} else if (n == 7) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y, x, 2, 4, SC);
		drawRect(y, x+4, 2, 10, SC);
	} else if (n == 8) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y+4, x, 6, 2, SC);
		drawRect(y+8, x, 6, 2, SC);
		drawRect(y, x, 2, 8, SC);
		drawRect(y, x+4, 2, 8, SC);
	} else if (n == 9) {
		drawRect(y, x, 6, 2, SC);
		drawRect(y+4, x, 6, 2, SC);
		drawRect(y+8, x, 6, 2, SC);
		drawRect(y, x, 2, 4, SC);
		drawRect(y, x+4, 2, 8, SC);

	}
}


