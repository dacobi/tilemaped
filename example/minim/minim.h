#include <cx16.h>
#include <cbm.h>
//#include <6502.h>

#include "sintable.h"
#include "atantab.h"
#include "colmap.h"
#include "randtab.h"

char randidx;

#define RANDINIT() if(randidx > 99) randidx = 0
#define RANDNEXT() randtab[randidx]; randidx++; if(randidx > 99) randidx = 0

#define	    SPRITE_BLOCKLO(addr)			(addr >> 5)
#define	    SPRITE_BLOCKHI(addr)			(addr >> 13)
#define     SPRITE_MODE_8BPP     		0x80
#define     SPRITE_MODE_4BPP     		0
#define	    SPRITE_32_BY_32		        0xA0
#define	    SPRITE_16_BY_16		        0x50
#define	    SPRITE_8_BY_8		        0x0
#define	    SPRITE_64_BY_64		        0xf0
#define     SPRITE_SIZE				512

#define	    SPRITE_DISABLED		        0
#define	    SPRITE_LAYER_BACKGROUND		0x4
#define	    SPRITE_LAYER_0		        0x8
#define	    SPRITE_LAYER_1		        0xC


#define VRAM_bitmap 0x04000
#define VRAM_palette 	   0x1FA00

#define VRAM_psg  0x1F9C0

#define     SPRITE_REGISTERS(spritenum)    ((spritenum << 3) + 0xfc00)


#define SPRITE_32H     0x80
#define SPRITE_32W     0x20


#define IOINIT 0xFF81
#define LOAD 0xFFD5

#define IRQVec 0x0314

#define VERAcontrol 0x9F25	
#define VERA_addr_low 0x9F20
#define VERA_addr_high 0x9F21
#define VERA_addr_bank 0x9F22

#define VERA_data0        0x9F23

// VRAM ddresses
#define VRAM_spraddr 	   0x1FC00
//#define VRAM_layer0_map    0x00000
//#define VRAM_layer1_map    0x00800
#define VRAM_layer0_map    0x00000
#define VRAM_layer1_map    0x00800
//#define VRAM_tilesbg       0x08800
//#define VRAM_sprites       0x9000

#define VRAM_tilesbg       0x8800
#define VRAM_tiles       0xA800
#define VRAM_sprites         0x10400 //0x12C00
#define VRAM_sprites_hi 0x8
#define VRAM_sprites_lo 0x20
//#define VRAM_textcd          0x12C00
#define VRAM_boom          0x12C00
#define VRAM_boom_hi    0x9
#define VRAM_boom_lo 0x60
#define VRAM_textcd         0x13C00
#define VRAM_texttrk	 0x16400

#define VRAM_intro    0x00000
#define VRAM_texti   0x12C00

#define TEXTISIZEW 512

#define TEXTW_P 0
#define TEXTW_L TEXTISIZEW
#define TEXTW_A (TEXTISIZEW * 2)
#define TEXTW_Y (TEXTISIZEW * 3)
#define TEXTW_E (TEXTISIZEW * 4)
#define TEXTW_R (TEXTISIZEW * 5)
#define TEXTW_1 (TEXTISIZEW * 6)
#define TEXTW_2 (TEXTISIZEW * 7)
#define TEXTW_3 (TEXTISIZEW * 8)
#define TEXTW_4 (TEXTISIZEW * 9)

#define MLAPSCOUNTX 20
#define MLAPSCOUNTY 20
#define MLAPSCOUNTSPACE 28

#define TEXTISIZET 512

#define TEXTT_1 0
#define TEXTT_2 TEXTISIZET
#define TEXTT_3 (TEXTISIZET * 2)
#define TEXTT_4 (TEXTISIZET * 3)
#define TEXTT_5 (TEXTISIZET * 4)
#define TEXTT_L (TEXTISIZET * 5)
#define TEXTT_A (TEXTISIZET * 6)
#define TEXTT_P (TEXTISIZET * 7)
#define TEXTT_S (TEXTISIZET * 8)
#define TEXTT_O (TEXTISIZET * 9)
#define TEXTT_G (TEXTISIZET * 10)


#define TEXTISIZECD 2048
#define TEXTCD_1 0
#define TEXTCD_2 TEXTISIZECD
#define TEXTCD_3 (TEXTISIZECD * 2)
#define TEXTCD_O (TEXTISIZECD * 3)
#define TEXTCD_G (TEXTISIZECD * 4)
#define TEXTCD_4 (TEXTISIZECD * 5)
#define TEXTCD_P (TEXTISIZECD * 6)


#define TEXTISIZE 32
#define TEXTI_A 0
#define TEXTI_I TEXTISIZE
#define TEXTI_J (TEXTISIZE * 2)
#define TEXTI_K (TEXTISIZE * 3)
#define TEXTI_1 (TEXTISIZE * 4)
#define TEXTI_2 (TEXTISIZE * 5)
#define TEXTI_3 (TEXTISIZE * 6)
#define TEXTI_4 (TEXTISIZE * 7)
#define TEXTI_5 (TEXTISIZE * 8)
#define TEXTI_P (TEXTISIZE * 9)
#define TEXTI_L (TEXTISIZE * 10)
#define TEXTI_Y (TEXTISIZE * 11)
#define TEXTI_E (TEXTISIZE * 12)
#define TEXTI_R (TEXTISIZE * 13)
#define TEXTI_B (TEXTISIZE * 14)
#define TEXTI_S (TEXTISIZE * 15)
#define TEXTI_V (TEXTISIZE * 16)
#define TEXTI_T (TEXTISIZE * 17)
#define TEXTI_X (TEXTISIZE * 18)
#define TEXTI_C (TEXTISIZE * 19)
#define TEXTI_6 (TEXTISIZE * 20)
#define TEXTI_7 (TEXTISIZE * 21)
#define TEXTI_8 (TEXTISIZE * 22)
#define TEXTI_9 (TEXTISIZE * 23)

#define MAILEVELPOS 85

#define MAILEVEL1 -10
#define MAILEVEL2 -8
#define MAILEVEL3 0
#define MAILEVEL4 2
#define MAILEVEL5 5

#define CKEY_ENTER 13
#define CKEY_UP 145
#define CKEY_DOWN 17
#define CKEY_LEFT 157
#define CKEY_RIGHT 29

//#define VRAM_sprites       0x8800
//#define VRAM_tiles         0x9E00



//#define VRAM_tiles         0x07000


#define PCM_CTRL 0x9F3B
#define PCM_RATE 0x9F3C
#define PCM_DATA 0x9F3D

#define PSG_BASE 0x1F9C0

#define PCMCTRL (*(volatile unsigned char*)0x9F3B)
#define PCMRATE (*(volatile unsigned char*)0x9F3C)
#define PCMDATA (*(volatile unsigned char*)0x9f3d)

//#define EIN 0x9F26
#define VERA_ISR (*(volatile unsigned char*)0x9F27)

#define  KEY_Q		17
#define  KEY_CTRL	58
#define  KEY_ESC	110

#define  KEY_W		18
#define  KEY_S		32
#define  KEY_D		33
#define  KEY_A		31

#define  KEY_UP		83
#define  KEY_DOWN	84
#define  KEY_LEFT	79
#define  KEY_RIGHT	89
#define  KEY_ENTER	43



#define DIR_0              1
#define DIR_10             2
#define DIR_20             3
#define DIR_30             4
#define DIR_40             5
#define DIR_50             6
#define DIR_60             7
#define DIR_70             8
#define DIR_80             9
#define DIR_90             10
#define DIR_100            11
#define DIR_110            12
#define DIR_120            13
#define DIR_130            14
#define DIR_140            15
#define DIR_150            16
#define DIR_160            17
#define DIR_170            18
#define DIR_180            19
#define DIR_190            20
#define DIR_200            21
#define DIR_210            22
#define DIR_220            23
#define DIR_230            24
#define DIR_240            25
#define DIR_250            26
#define DIR_260            27
#define DIR_270            28
#define DIR_280            29
#define DIR_290 	   30
#define DIR_300            31
#define DIR_310            32
#define DIR_320            33
#define DIR_330            34
#define DIR_340            35
#define DIR_350            36
#define DIR_45             37
#define DIR_135            38
#define DIR_225            39
#define DIR_315            40

//#define SLOW_TIME	   3

#define ROADX 128
#define ROADY 128
#define TILEX 16
#define TILEY 16

#define PLAYERX 16
#define PLAYERY 16

#define JUMPOFF (SPRITE_SIZE*11)

#define JOY_BUT_B 0b10000000
#define JOY_BUT_Y 0b01000000
#define JOY_BUT_A 0b10000000
#define JOY_BUT_X 0b01000000

#define JOY_PAD_L 0b00000010
#define JOY_PAD_R 0b00000001

#define MPLAYERS 3

#define GAME_LEVELS 9
#define MMAXWAYPOINTS 12

unsigned short mBankStack;

#define BINIT() mBankStack = (unsigned short)BANK_RAM + 4096
#define BALLOC(BSIZE) mBankStack; mBankStack += BSIZE

char mNumbers[10] = "0123456789";

#define SETPATHNUM(PATH, NUM, INDEX) PATH[INDEX] = mNumbers[NUM]

char mgamepath[9] = "dat/game";

char mtxintpath[10] = "dat/txint";
char mintropath[10] = "dat/intro";
char mpalipath[9] = "dat/pali";

char mcarspath[9] = "dat/cars";
char mboompath[9] = "dat/boom";
char mtxcdpath[9] = "dat/txcd";

char mtxwnpath[9] = "dat/txwn";
char mwinpath[8] = "dat/win";

char mtxtrpath[9] = "dat/txtr";

char msndpath[9] = "dat/snd1";
#define MSNDPOS 7

char mclmpath[9] = "dat/clm1";
#define MCLMPOS 7

char mmbgpath[9] = "dat/mbg1";
#define MMBGPOS 7

char mwaypath[9] = "dat/way1";
#define MWAYPOS 7

char mtrkpath[9] = "dat/trk1";
#define MTRKPOS 7

char mpalpath[9] = "dat/pal1";
#define MPALPOS 7

char mtlstpath[10] = "dat/tlst1";
#define MTLSTPOS 8

char mtlsbgpath[11] = "dat/tlsbg1";
#define MTLSBGPOS 9

char mwpalpath[10] = "dat/wpal1";
#define MWPALPOS 8

#define CARSIZE 26
#define CARSIZESQR 648

#define MAXCOL 4
//#define MENUDEL 500
#define MENUDEL 2

#define PCMCTRL (*(volatile unsigned char*)0x9F3B)
#define PCMRATE (*(volatile unsigned char*)0x9F3C)
#define PCMDATA (*(volatile unsigned char*)0x9f3d)

extern void install_irq();
extern void remove_irq();
extern void getkeystat();
extern void clearkeystat();
extern void set_keyboard_irq();
extern void rm_keyboard_irq();

#define GETINDEX(CX, CY) (CX >> 4) + ((CY >> 4) << 7)
#define mColmap BANK_RAM

#define MTRACKON 0x1F
#define MTRACKONEDGE 0x2F
#define MTRACKOUTSIDE 0x0

#define MSPEEDMAX 80
#define MSPEEDMAXONEDGE 65
#define MSPEEDMAXOUTSIDE 50

int mScrollX, mScrollY;

unsigned char joyl,joyh,joyok,joynum;

unsigned char keynum;
unsigned char curkey;

unsigned char sample_load;
unsigned char *sample_index;
unsigned char *load_index;

unsigned char * sample_max;

//unsigned char sample_point[4096];
#define sample_point BANK_RAM

unsigned char mHi, mLo, mLoad;

#define MPCMMAX1 252
#define MPCMMAX2 202

#define MPCMCTRL1 0x0c
#define MPCMCTRL2 0x0f

typedef struct PCMAUDIO{

	char mStream;
	char mCtrl;
	int mMaxFile;

	char mFrames;
	int mChunks;
	
	char bLoadSound;
	
	char mFile;
};

struct PCMAUDIO mPCM;

typedef struct Vec2{
	int x;
	int y;
};

typedef struct Vec2C{
	int x;
	int y;
	char c;
};

typedef struct Control {	
	char bIsActive;
	char bIsBot;
	char bIsJoy;
	char mAccP;
	char mRight;
	char mLeft;
	
	char mKeyNum;
 	char mJoyNum;
	
	int mNextWay;
};

typedef struct SThrottle{
	char mChan;
	unsigned int mFreq;
	char mVol;
	char mOn;
	char mRev;
	char mRevTime;	
	int mLength;
	int mLastCount;
};

#define MOUTKILL 80
#define MOUTLOOSE 70

typedef struct Placement{

	char mPlayer;
	unsigned int mPFactor;
};

typedef struct Player {
	int mDir;

	int mVel;
	int mVelX;
	int mVelY;
	int mVelL;
	int mVelR;
	int mMaxX;
	int mMaxP;
	
	char mLevel;
	char mLaps;
	char mPlace;
	char bCheckFinish;
	char bFinished;
	char mPlayer;
	
	char bIsOutside;
	int mOutCount;
	
	char bIsAlive;
	char bIsValid;
	char bIsVisible;
	unsigned short mBoomCount;
	signed char mBoomDelay;

	int pl_cur_dir;
	
	struct Vec2 mPos;
	
	struct Control *mControl;
	
	struct Placement PPlace;
	
	char mLaps;
	
	struct SThrottle mEngine;
};

typedef struct PSprite {
	struct Vec2 mPos;
	char z; 
	char flipx;
	char flipy;
	char colmask;
	char blocklo;
	char blockhi;	
	char mode;
	char dimensions;
	char palette_offset;
};

typedef struct WText {
	char mPOffset[14];
	char mPIndex;
};

typedef struct MLine {
	char mCCount;
	char mSPos;
	struct PSprite *mChar[9];
};

typedef struct SChan{
	char mFreqHi;
	char mFreqLo;	
	char mType;
	char mPuls;	
	char mVol;
	char mClear;
};

typedef struct SBeep{
	char mChan;
	unsigned int mFreq;
	unsigned int mSubFreq;	
	char mVol;
	char mOn;
	int mLength;
	int mLastCount;
};

typedef struct SClick{
	char mChan;
	unsigned int mFreq;
	char mVol;
	char mOn;
	int mFInc;
	int mLength;
	int mLastCount;
};

typedef struct SCrash{
	char mChan;
	unsigned int mFreq;
	char mVol;
	char mOn;
	int mVDec;
	int mLength;
	int mLastCount;
};

typedef struct Menu {   
	
	struct PSprite mMenuLVL;
	struct PSprite mMenuLABS;
	
	struct PSprite mMenuAI;

	struct PSprite mMenuPl1_1;
	struct PSprite mMenuPl1_2;
	
	struct PSprite mMenuPl2_1;
	struct PSprite mMenuPl2_2;

	struct PSprite mMenuPl3_1;
	struct PSprite mMenuPl3_2;

	//struct PSprite mMenuPl4_1;
	//struct PSprite mMenuPl4_2;

	struct PSprite* PMenu[3][2];
	
	struct MLine mMenuLines[8];
	
	char mSelectCol[6];
	char mSelected;
	
	int mNumbers[10];
	
	char mPCtrl[3];
	char mLevel;
	char mLaps;
	char mAISpeed;	
	
	signed char mMenuItem;
	signed char mMenuItemLast;
	signed char mMenuItemUpdate;
	signed char bDoUpdate;
	unsigned int mMenuControl;
	char bShowMenu;
	
	char mMenuLoad1;
	char mMenuLoad2;	
	
	char bRunning;
	char bWinnerRunning;
	
	struct SClick mMClick;
	int mClickLength;	
	int mClickLengthShort;		
};

struct Menu mMenu;
      
//#define MDEBUG 1
      
typedef struct Game {   
   
   char mLevels;
   char mLevel;
   char mLaps;
   char mPlace;
   char mPCount;
   int mFinCount;
   char mWinner;
   
   char bFrameReady;
   
#ifdef MDEBUG   
   char bDebug;   
   struct PSprite mDebug[MMAXWAYPOINTS];
#endif   
   
   signed char mAISpeed;	
   
   unsigned short mCarsAddr;
   unsigned short mBoomAddr;
//   unsigned short mTXTCDAddr;
   unsigned short mTXTLapsAddr;
   
   unsigned short mCarsHi;
   char mCarsLo;

   unsigned short mBoomHi;
   char mBoomLo;
   
//   unsigned short mTXTCDHi;
//   char mTXTCDLo;

   unsigned short mTXTLapsHi;
   char mTXTLapsLo;
   
   struct Player* mLeader;
   
   struct WText mWText;
   struct PSprite mLapsCount[5];

   struct SBeep mCBeep;
   struct SCrash mCrash;
   struct SCrash mBump;   
   int mBeepLength;

   int mFinishLine;
   char mWaypointNum;
   struct Vec2C mWaypoints[MMAXWAYPOINTS];
   
   struct Player Player1;
   struct Player Player2;
   struct Player Player3;
//   struct Player Player4;
   
   struct Player* Players[3];
   
   struct Placement* PPlaces[3];

   struct PSprite PSprite1;
   struct PSprite PSprite2;
   struct PSprite PSprite3;
//   struct PSprite PSprite4;
   
   struct PSprite* PSprites[3];
   
   struct PSprite mCDown1;
   struct PSprite mCDown2;   
   
   struct Control Joy1;
   struct Control Joy2;
   struct Control Joy3;
   struct Control Joy4;

   struct Control KeyWASD;
   struct Control KeyArrow;
   
   struct Control Bot1;
   struct Control Bot2;
   struct Control Bot3;
  // struct Control Bot4;
   struct Control* mBots[3];
   
   struct Control* Joysticks[4];
   
   struct Control* mControls[9];
   struct Control* mPControls[3];
   
   struct Vec2 mViewport;
   
   struct Vec2 StartPos;

   struct Vec2 CarPos1;
   struct Vec2 CarPos2;
   struct Vec2 CarPos3;
//   struct Vec2 CarPos4; 
   
   struct Vec2* mCarPos[3];
   
   struct SChan mChannels[16];
   
   struct Vec2 mOldView;
   
   char bMoveToLead;
   char bViewXDone;
   char bViewYDone;   
   
   char bRunning;
   char bRacing;
   char bGamePaused;
   char bCountDown;
   int bCountDelay;	   
   
   char mCarCol1;
   char mCarCol2;

   int mColDX;
   int mColDY;
   
   char bIRQ;
   
   char bDoCol;
   char bGameRunning;
    
};



struct Game mGame;

void load_game(char clvl);

char get_game_levels();

#ifdef MDEBUG   

void init_debug();

void process_debug();

void load_debug();

#endif

void setTextSprite(struct PSprite* cMenu, int cChar, int mx, int my);

void set_text_sprite(struct PSprite* cMenu, unsigned short cHi, char cLo, char cSize, char cOff, unsigned short cChar, int mx, int my);

void setTrackSprite(struct PSprite* cMenu, int cChar, int mx, int my);

void init_lmenu(char cline, char ccnum, int cchars[], int mspace, struct PSprite* cH1, struct PSprite* cH2, int mx, int my);

void init_wtext(struct WText* wText, int cNR);
   
void reset_vera();

void soundpsg(unsigned char mchan, unsigned char mtype, unsigned char mpuls, unsigned char mvol, unsigned char mHi, unsigned char mLow);

void clearpsg(unsigned char mchan);

void update_sound();

void stop_sound();

void clear_chan(struct SChan* cChan);

void clear_channels();

void clear_control(struct Control* cCon);

void reset_control(struct Control* cCon);

void reset_controls();

void clear_controls();

unsigned char getatan(long x);

int getangle(int x, int y);

void set_player(int pi, struct Player *cPlayer, struct Control *cControl, struct Vec2 *sPos, int isJoy, int isBot);

void set_leader(char nlead);

void calc_player_dfactor(struct Player *cPlayer);

void calc_players_dfactor();

char check_collision_cars(struct Player *cCar1, struct Player *cCar2);

char check_collision();

void solve_collision(int cDX, int cDY, struct Player *cCar1, struct Player *cCar2);

void process_collision();

int getdelta(int car, int point);

unsigned char getkey(unsigned char ckeynum);

char getanykey();

void check_quit();

void clearkey(unsigned char ckeynum);

void process_joystick(int jnum);

void process_keys(char mkeynum);

void process_input();

int getsin(int x);

int getcos(int x);

void calc_way();

void loadVera(char *fname, unsigned int address, int flag);

void loadFile(char *fname, unsigned int address);

void openFile(char *fname, char file);

int rangechk(int dir);

int rangechkcar(int dir);

void apply_physics(struct Player *cPlayer);

void calc_sprite_pos(struct PSprite *cSprite, struct Player *cPlayer);

void set_boom_sprite(struct PSprite *cSprite, struct Player *cPlayer);

void process_sprites();

char getWayState(int wState, int cX, int cY, int vX, int vY);

void process_physics();

void process_bot(struct Player *cBot);

void process_bots();

void swap_placements(char p1, char p2);

void get_live_players();

//void sort_players4();

void sort_players3();

void setPlayerPlace(struct Player *cPlayer);

void killPlayer(struct Player *cPlayer);

void process_player(struct Player *cPlayer);

void process_players();

void calc_viewport();

void clear_sprite(int cs);

void clear_sprites(int cS, int cR);

void load_sprite(struct PSprite *cSprite, int sNum);

void load_psprites();

void setMenuPlayer(int cPlayer, int cM1, int cM2);

void setPlCtrl(int cPlayer, int cIdx);

void load_lmenu(int cLine);

void setMenuOffset(int cLine, int cOffset);

void init_menu();

void init_menulines();

void play_beep(int cbasefq, int clength);

void play_click(int cbasefq, int clength, int cadd);

void play_crash(int cbasefq, int clength, int cdec);

void play_bump(int cbasefq, int clength, int cdec);

void start_engine(struct SThrottle* cEngine, char cchan);

void stop_engine(struct SThrottle* cEngine);

void process_engine(struct SThrottle* cEngine, int mvel);

void process_sound();

//void setCDSprite(struct PSprite* cMenu, int cChar, int mx, int my);

void set_menucd(int cNR);

void render_cdmenu();

void init_game();

unsigned char mauien;

void mscol(void);

void msline(void);

void maflow(void);

void mblank(void);

void load_level();

char getPlItem(int cPl, int nIdx, int cDir);

char process_input_menu();

void render_menu();

void load_menu();

void load_audio(char cstream);

void open_audio();

void close_audio();

void reload_audio();

void init_audio();

void start_audio();

void stop_audio();

void update_audio();

void render_wmenu();

void load_wmenu();

void setup_race();
