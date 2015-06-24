//Yuna Lee

#include <stdlib.h>

typedef unsigned short u16;
typedef unsigned int u32;

extern u16 *videoBuffer;

typedef struct DMA_REC
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMA_REC;

#define REG_DISPCNT *(u16*) 0x4000000
#define RGB(r,g,b) ((b)<<10|(g)<<5 |(r))
#define MODE3 3
#define BG2_ON (1 << 10)

#define KEY_A        0x0001		
#define KEY_B        0x0002		
#define KEY_SELECT   0x0004		
#define KEY_START    0x0008		
#define KEY_RIGHT    0x0010	
#define KEY_LEFT     0x0020	
#define KEY_UP       0x0040	
#define KEY_DOWN     0x0080	\
#define KEY_R        0x0100		
#define KEY_L        0x0200		
#define KEY_MASK     0x03FF		

#define REG_KEYINPUT *(u16*) 0x4000130

// Colors
#define WHITE RGB(31,31,31)
#define BLACK RGB(0,0,0)
#define RED RGB(31,0,0)
#define GREEN RGB(0,31,0)
#define BLUE RGB(0,0,31)
#define YELLOW (GREEN|RED)
#define CYAN (GREEN|BLUE)
#define PINK (RED|BLUE)
#define GRAY RGB(20,20,20)

#define REG_DMA ((volatile DMA_REC*)0x040000B0)
#define DMA_ON (1 << 31)
#define DMA_SRC_INC (0 << 23)
#define DMA_SRC_DEC (1 << 23)
#define DMA_SRC_FIXED (2 << 23)
#define DMA_DST_INC (0 << 21)
#define DMA_DST_DEC (1 << 21)
#define DMA_DST_FIXED (2 << 21)
#define DMA_DST_RELOAD (3 << 21)

#define REG_VCOUNT *(volatile u16*)0x04000006

#define REG_TM2D ((volatile u16*)0x0)
#define TM_FREQ_1024 3
#define TM_ENABLE (1 << 7)
#define TM_CASCADE (1 << 2)

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define WEIGHT 1000

#define size(arr) (sizeof(arr) == 0 ? 0 : (sizeof(arr)/sizeof(arr[0])))

#define LANE_WIDTH 36
#define LANE_ORIGIN(lane, width) (20+LANE_WIDTH/2-(width)/2+(lane)*(LANE_WIDTH+5))

typedef enum {
	false, true
} bool;

typedef struct {
	int width, height;
} SIZE;

void setPixel(int r, int c, u16 color);
void drawRect(int r, int c, int width, int height, u16 color);
void removeRect(int r, int c, int width, int height);
void drawImage3(int r, int c, int width, int height, const u16* image);
u16 getPixel(int r, int c);
void waitForVBlank();
int random(int start, int end);

