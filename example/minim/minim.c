#include "minim.h"

void load_game(){
	mGame.mLevels = 2;


   	mGame.mCarsAddr = VRAM_sprites;
   	mGame.mBoomAddr = VRAM_boom;
   	mGame.mTXTCDAddr = VRAM_textcd;
   	mGame.mTXTLapsAddr = VRAM_texttrk;     
   	mGame.mTXTLapsAddrShort = 0xBA0;    
   
   	mGame.mCarsHi = VRAM_sprites_hi;
   	mGame.mCarsLo = VRAM_sprites_lo;

   	mGame.mBoomHi = VRAM_boom_hi;
   	mGame.mBoomLo = VRAM_boom_lo;
   
   	mGame.mTXTCDHi = SPRITE_BLOCKHI(VRAM_textcd);
	mGame.mTXTCDLo = SPRITE_BLOCKLO(VRAM_textcd);

   	mGame.mTXTLapsHi = 0x0B; //SPRITE_BLOCKHI(VRAM_texttrk);
	mGame.mTXTLapsLo = 0xA0; //SPRITE_BLOCKLO(VRAM_texttrk);
}

void set_text_sprite(struct PSprite* cMenu, unsigned short cHi, char cLo, char cSize, char cOff, unsigned short cChar, int mx, int my){

   cMenu->blocklo = cLo + (SPRITE_BLOCKLO(cChar) & 0x00FF);
   cMenu->blockhi = ((cHi + SPRITE_BLOCKLO(cChar)) >> 8) & 0x00fff;
   cMenu->mode = SPRITE_MODE_4BPP;
   cMenu->mPos.x = mx;
   cMenu->mPos.y = my;
   cMenu->z = SPRITE_LAYER_1;
   cMenu->flipx = 0;
   cMenu->flipy = 0;   
   cMenu->colmask = 0;
   cMenu->dimensions = cSize;
   cMenu->palette_offset = cOff;


}

void setTextSprite(struct PSprite* cMenu, int cChar, int mx, int my){

   cMenu->blocklo = SPRITE_BLOCKLO(VRAM_texti + cChar);
   cMenu->blockhi = SPRITE_BLOCKHI(VRAM_texti + cChar);   
   cMenu->mode = SPRITE_MODE_4BPP;
   cMenu->mPos.x = mx;
   cMenu->mPos.y = my;
   cMenu->z = SPRITE_LAYER_1;
   cMenu->flipx = 0;
   cMenu->flipy = 0;   
   cMenu->colmask = 0;
   cMenu->dimensions = SPRITE_8_BY_8;
   cMenu->palette_offset = 15;


}

void setTrackSprite(struct PSprite* cMenu, int cChar, int mx, int my){

   cMenu->blocklo = SPRITE_BLOCKLO(VRAM_texttrk + cChar);
   cMenu->blockhi = SPRITE_BLOCKHI(VRAM_texttrk + cChar);   
   cMenu->mode = SPRITE_MODE_4BPP;
   cMenu->mPos.x = mx;
   cMenu->mPos.y = my;
   cMenu->z = SPRITE_LAYER_1;
   cMenu->flipx = 0;
   cMenu->flipy = 0;   
   cMenu->colmask = 0;
   cMenu->dimensions = SPRITE_32_BY_32;
   cMenu->palette_offset = 0;


}

void init_wtext(struct WText* wText, int cNR){
	int cx, cy, cdelta,pi,pv,pd;
	cy = 50;
	cx = 65;
	cdelta = 27;
	pv = 8;
	pd = 1;
	pi = 0;
	
	wText->mPIndex = 0;

	for(pi = 0; pi < 14; pi++){
	
		wText->mPOffset[pi] = pv;

		if(pd == 1){
			pv++;
			if(pv > 14){
				pd = 0;
			}
		} else {
			pv--;
		}
	}
	
	RAM_BANK = 1;
	BINIT();
	
	for(pi = 0; pi < 7; pi++){
		mMenu.mMenuLines[0].mChar[pi] = (struct PSprite*)BALLOC(sizeof(struct PSprite));
	}
	
	mMenu.mMenuLines[0].mCCount = 7;
		
	setTextSprite(mMenu.mMenuLines[0].mChar[0], TEXTW_P, cx, cy);
	cx += cdelta;
	setTextSprite(mMenu.mMenuLines[0].mChar[1], TEXTW_L, cx, cy);
	cx += cdelta;
	setTextSprite(mMenu.mMenuLines[0].mChar[2], TEXTW_A, cx, cy);
	cx += cdelta;
	setTextSprite(mMenu.mMenuLines[0].mChar[3], TEXTW_Y, cx, cy);
	cx += cdelta;
	setTextSprite(mMenu.mMenuLines[0].mChar[4], TEXTW_E, cx, cy);
	cx += cdelta;
	setTextSprite(mMenu.mMenuLines[0].mChar[5], TEXTW_R, cx, cy);
	cx += cdelta;
	setTextSprite(mMenu.mMenuLines[0].mChar[6], cNR, cx, cy);
	
	for(pi = 0; pi < 7; pi++){
		mMenu.mMenuLines[0].mChar[pi]->dimensions = SPRITE_32_BY_32;
	}
	
	load_lmenu(0);
}

void init_lmenu(char cline, char ccnum, int cchars[], int mspace, struct PSprite* cH1, struct PSprite* cH2, int mx, int my){

	char li;
	int cSpace = 0;
	int cDelta = 8;
	
	if( (cH1 == 0) && (cH2 == 0) ){
		mMenu.mMenuLines[cline].mCCount = ccnum;
	} else if(cH2 == 0){
		mMenu.mMenuLines[cline].mCCount = ccnum + 1;
		mMenu.mMenuLines[cline].mChar[ccnum] = cH1;
	} else {
		mMenu.mMenuLines[cline].mCCount = ccnum + 2;
		mMenu.mMenuLines[cline].mChar[ccnum] = cH1;
		mMenu.mMenuLines[cline].mChar[ccnum + 1] = cH2;
	}
	
	for(li = 0; li < ccnum; li++){
		mMenu.mMenuLines[cline].mChar[li] = (struct PSprite*)BALLOC(sizeof(struct PSprite));
	}
	
	for(li = 0; li < ccnum; li++){		
		setTextSprite(mMenu.mMenuLines[cline].mChar[li], cchars[li], mx + cSpace, my);
		cSpace += cDelta;
		
		if(li == (mspace - 1)){
			cSpace += 4;
		}
	}	
}

void reset_vera(){
	int mVeraRange;

	//VERA.control = 0;
	VERA.address_hi = VERA_INC_1 + 0x1; 
	
    	VERA.address = 0xF9C0;
    	
    	for(mVeraRange = 0; mVeraRange < 0x63F; mVeraRange++){
    		VERA.data0 = 0;
	}
}

void soundpsg(unsigned char mchan, unsigned char mtype, unsigned char mpuls, unsigned char mvol, unsigned char mHi, unsigned char mLow){

	unsigned char psgHi;
	unsigned char psgLow;

	unsigned char psgChan;
	unsigned char psgType;

	unsigned char psgVol;

	if(mvol > 0){
		psgHi = mHi;
		psgLow = mLow;
	
		if(mtype == 0){
			psgType = mpuls & 0x3F;	
		} else {
			psgType = (mtype & 0x3) << 6;
		}

		psgChan = mchan * 4;
		psgVol = mvol | 0xC0;
	} else {
		psgHi = 0;
		psgLow = 0;		
		psgType = 0;
		psgChan = mchan * 4;
		psgVol = 0;
	}
 
	VERA.control = 0;
	VERA.address_hi = VERA_INC_1 + 0x1; 
	
    	VERA.address = PSG_BASE + psgChan; 

    	VERA.data0 = psgLow;
    	VERA.data0 = psgHi;    
    
    	VERA.data0 = psgVol;
    
    	VERA.data0 = psgType;
}

void clearpsg(unsigned char mchan){

    unsigned char mChan;

    mChan = mchan * 4;

    VERA.control = 0;
    VERA.address_hi = VERA_INC_1 + 0x1; 
	
    VERA.address = PSG_BASE + mChan; 

    VERA.data0 = 0;
    VERA.data0 = 0;    
    
    VERA.data0 = 0;
    VERA.data0 = 0;

}

void stop_sound(){
	char ci;
	for(ci = 0; ci < 16; ci++){
		clearpsg(ci);
	}
	
}

void update_sound(){
	int ci;
	for(ci = 0; ci < 16; ci++){
		if(mGame.mChannels[ci].mVol){		
			mGame.mChannels[ci].mClear = 1;
			soundpsg(ci, mGame.mChannels[ci].mType,mGame.mChannels[ci].mPuls, mGame.mChannels[ci].mVol, mGame.mChannels[ci].mFreqHi, mGame.mChannels[ci].mFreqLo);
		} else {
			if(mGame.mChannels[ci].mClear){
				clearpsg(ci);		
				mGame.mChannels[ci].mClear = 0;
			}
			
		}
	}

}

void clear_chan(struct SChan* cChan){
	cChan->mFreqHi = 0;
	cChan->mFreqLo = 0;
	cChan->mType = 0;
	cChan->mPuls = 0;	
	cChan->mVol = 0;
	cChan->mClear = 0;
}

void clear_channels(){
	int ci;
		
	for(ci = 0; ci < 16; ci++){
		clear_chan(&mGame.mChannels[ci]);
	}

	
	
	for(ci = 0; ci < 16; ci++){
		clearpsg(ci);		
	}
	
	mMenu.mMClick.mOn = 0;
	mGame.mCBeep.mOn = 0;
	mGame.mBump.mOn = 0;
	mGame.mCrash.mOn = 0;
	mGame.Player1.mEngine.mOn = 0;
	mGame.Player2.mEngine.mOn = 0;
	mGame.Player3.mEngine.mOn = 0;
}

void clear_control(struct Control* cCon){
	cCon->mAccP = 0;
	cCon->mRight = 0;
	cCon->mLeft = 0;		
}

void reset_control(struct Control* cCon){
	clear_control(cCon);
	cCon->bIsActive = 0;
	cCon->bIsJoy = 0;
	cCon->bIsBot = 0;
	cCon->mNextWay = 1;
}

void reset_controls(){
   int ci;
   
   for(ci = 0; ci < 9; ci++){
   	reset_control(mGame.mControls[ci]);
   }
   
   mGame.Joy1.mJoyNum = 1;
   mGame.Joy2.mJoyNum = 2;
   mGame.Joy3.mJoyNum = 3;
   mGame.Joy4.mJoyNum = 4;

   mGame.KeyArrow.mJoyNum = 0;
   mGame.KeyWASD.mJoyNum = 0;   
   
   mGame.Bot1.mJoyNum = 0;
   mGame.Bot2.mJoyNum = 0;
   mGame.Bot3.mJoyNum = 0;
//   mGame.Bot4.mJoyNum = 0;

   mGame.Joy1.mKeyNum = 0;
   mGame.Joy2.mKeyNum = 0;
   mGame.Joy3.mKeyNum = 0;
   mGame.Joy4.mKeyNum = 0;

   mGame.KeyArrow.mKeyNum = 1;
   mGame.KeyWASD.mKeyNum = 2;   
   
   mGame.Bot1.mKeyNum = 0;
   mGame.Bot2.mKeyNum = 0;
   mGame.Bot3.mKeyNum = 0;
//   mGame.Bot4.mKeyNum = 0;
}

void clear_controls(){
   int ci;
   
   for(ci = 0; ci < 3; ci++){
   
   	if(mGame.mPControls[ci]->bIsActive == 0){
   		clear_control(mGame.mPControls[ci]);
   	}
   }
}

unsigned char getatan(long x){

	unsigned char retval; 

	if(x < 1000){
		retval = atantab[x];
	} else {
	
	if(x < 1100) return 84;
	if((x >= 1100) && (x < 1300)) return 85;
 	if((x >= 1300) && (x < 1700)) return 86;
        if((x >= 1700) && (x < 2300)) return 87;
      	if((x >= 2300) && (x < 3900)) return 88;
   	if((x >= 3900) && (x < 11500)) return 89;
	if(x >= 11500) return 90;
	
	/*	
	84.2894 : 10 - 83.6598 : 9
	84.8056 : 11 - 84.2894 : 10
	85.2364 : 12 - 84.8056 : 11
	85.6013 : 13 - 85.2364 : 12
	86.1859 : 15 - 85.9144 : 14
	86.6335 : 17 - 86.4237 : 16
	87.1376 : 20 - 86.9872 : 19
	87.5104 : 23 - 87.3974 : 22
	88.0251 : 29 - 87.9546 : 28
	88.5312 : 39 - 88.4926 : 38
	89.0122 : 58 - 88.9949 : 57
	89.5018 : 115 - 89.4974 : 114
	*/
	
	}
	
	return retval;	
}

int getangle(int x, int y) {

    int ret;
    long mx,my,myoverx,index;

   if((x == 0) && (y == 0)) return 0;

    if (x == 0) // special cases
        return (y > 0)? 0 : 180;
    else if (y == 0) // special cases
        return (x >= 0)? 90
            : 270;
                       
    mx = x;
    my = y;                 
                           
    if((x > 0) && (y > 0)){ 
    	myoverx = (my*100)/mx; // y*100)/x
    	index = myoverx;
    	ret = 90 - (int)getatan(index);
    } // First
    
    if((x > 0) && (y < 0)){ 
      	myoverx = (-my*100)/mx; // (-y*100)/x)
    	index = myoverx;
    	ret = 90 + (int)getatan(index);
    } // Second
    
    if((x < 0) && (y < 0)){ 
      	myoverx = (-my*100)/-mx; // -y*100)/-x)
    	index = myoverx;
    	ret = 180 + (90 - (int)getatan(index));
    } // Third
    
    if((x < 0) && (y > 0)){ 
       	myoverx = (my*100)/-mx; // (y*100)/-x)
    	index = myoverx;
    	ret = 270 + (int)getatan(index);
    } // Forth


    return ret;
}

void set_player(int pi, struct Player *cPlayer, struct Control *cControl, struct Vec2 *sPos, int isJoy, int isBot){

	cControl->bIsJoy = isJoy;	
	cControl->bIsBot = isBot;
	cControl->bIsActive = 1;
	cControl->mNextWay = 1;
	
	cControl->mAccP = 0;
	cControl->mRight = 0;
	cControl->mLeft = 0;

	cPlayer->mControl = cControl;
	mGame.mPControls[pi] = cControl;
	
	cPlayer->mPos.x = sPos->x;
	cPlayer->mPos.y = sPos->y;
	
	cPlayer->mDir = 0;

	cPlayer->mVel = 0;
	cPlayer->mVelX = 0;
	cPlayer->mVelY = 0;
	cPlayer->mVelL = 0;
	cPlayer->mVelR = 0;
	
		
	if(cControl->bIsBot > 0){
		cPlayer->mMaxX = MSPEEDMAX + mGame.mAISpeed;
	} else {
		cPlayer->mMaxX = MSPEEDMAX;
	}
	
	cPlayer->mMaxP = 50;
	
	cPlayer->mLevel = mGame.mLevel;
	cPlayer->mLaps = mGame.mLaps;
	
	cPlayer->mPlace = 0;
	
	cPlayer->bCheckFinish = 0;
	cPlayer->bFinished = 0;
	cPlayer->mPlayer = pi + 1;
	
	cPlayer->bIsAlive = 1;
	cPlayer->bIsValid = 1;
	cPlayer->bIsVisible = 1;
	cPlayer->mBoomCount = 0;
	cPlayer->mBoomDelay = 0;
	
	cPlayer->bIsOutside = 0;
	
	cPlayer->PPlace.mPlayer = pi;
	cPlayer->PPlace.mPFactor = 0;
	
	cPlayer->pl_cur_dir = DIR_0;		
}

char check_collision_cars(struct Player *cCar1, struct Player *cCar2){
	
	int nDX,nDY,cDX,cDY;
	int colx, coly, colval;
	
	if( (cCar1->bIsAlive == 0) || (cCar2->bIsAlive == 0) ){
		return 0;
	}

	nDX = cCar1->mPos.x - cCar2->mPos.x;
	nDY = cCar1->mPos.y - cCar2->mPos.y;

	cDX = nDX;
	cDY = nDY;
	
	if(nDX < 0){
		nDX = -nDX;
	}
	
	if(nDY < 0){
		nDY = -nDY;
	}


	if( (nDX < CARSIZE) && (nDY < CARSIZE)){
		if(((nDX * nDX) + (nDY * nDY)) < CARSIZESQR){
			
			colval = colmapcar[ (nDX / 2)][ (nDY / 2)];
						
			coly = colval >> 4;
			colx = colval & 0x0f;
			
			if(cDX < 0){
				mGame.mColDX = -colx;				
			} else {
				mGame.mColDX = colx;
			}
			
			if(cDY < 0){
				mGame.mColDY = -coly;				
			} else {
				mGame.mColDY = coly;
			}
			
			return 1;
		}
	}

	return 0;
}

char check_collision(){

	if(check_collision_cars(mGame.Players[0], mGame.Players[1])){
		mGame.mCarCol1 = 0;
		mGame.mCarCol2 = 1;
		return 1;
	}

	if(check_collision_cars(mGame.Players[0], mGame.Players[2])){
		mGame.mCarCol1 = 0;
		mGame.mCarCol2 = 2;
		return 1;
	}
	
	
	/*
	if(check_collision_cars(mGame.Players[0], mGame.Players[3])){
		mGame.mCarCol1 = 0;
		mGame.mCarCol2 = 3;
		return 1;
	}
	
	*/

	if(check_collision_cars(mGame.Players[1], mGame.Players[2])){
		mGame.mCarCol1 = 1;
		mGame.mCarCol2 = 2;
		return 1;
	}

/*
	if(check_collision_cars(mGame.Players[1], mGame.Players[3])){
		mGame.mCarCol1 = 1;
		mGame.mCarCol2 = 3;
		return 1;
	}

	if(check_collision_cars(mGame.Players[2], mGame.Players[3])){
		mGame.mCarCol1 = 2;
		mGame.mCarCol2 = 3;
		return 1;
	}
*/
	return 0;
}

void solve_collision(int cDX, int cDY, struct Player *cCar1, struct Player *cCar2){
	cCar1->mPos.x += cDX;
	cCar1->mPos.y += cDY;
	
	cCar2->mPos.x -= cDX;
	cCar2->mPos.y -= cDY;
}

void process_collision(){

	int colcount = 0;
	char bDidCol = 0;

	if(mGame.bDoCol){	
		while(check_collision() && (colcount < MAXCOL)){
			solve_collision(mGame.mColDX, mGame.mColDY, mGame.Players[mGame.mCarCol1], mGame.Players[mGame.mCarCol2]);
			colcount++;
			bDidCol = 1;
		}
		if(bDidCol){
			play_bump(550, 5, 8);
		}
		mGame.bDoCol = 0;
	
	}
	
}

int getdelta(int car, int point){
	int aa, ab;
	
	if(car == point) return 0;
	
	if(car < point){
		aa = point - car;
		ab = ((360 - point) + car);
	} else {
		aa = car - point;
		ab = ((360 - car) + point);	
	}
		
	if(ab < aa){
	    if(car > point){
	    	return ab;
	    } else {
	    	return -ab;
	    }		    
	} else {
	    if(car < point){
	    	return aa;
	    } else {
	    	return -aa;
	    }		    
	}		
}

void clearkey(unsigned char ckeynum){

    keynum = ckeynum;

    __asm__("ldx _keynum");

   clearkeystat();
}

unsigned char getkey(unsigned char ckeynum){

    unsigned char retval;
    keynum = ckeynum;

    __asm__("ldx _keynum");

   getkeystat();

   retval = curkey;
   
   return retval;

}

void process_joystick(int jnum){

      joynum = jnum;

    __asm__("lda _joynum");
    __asm__("jsr $FF56");
    __asm__("sta _joyl");
    __asm__("stx _joyh");
    __asm__("sty _joyok");

    

	if(joyok == 0){
        	if(joyl & JOY_BUT_B) {
                	mGame.Joysticks[jnum-1]->mAccP = 0;
        	} else {
                	mGame.Joysticks[jnum-1]->mAccP = 1;
        	}
                if(joyh & JOY_BUT_A) {
                        //jumpoff = 0;
			//jumprel = 0;
                } else {
			//if(jumprel == 0){
                           //jumpoff = JUMPOFF;
                           //jumpoff = 0;
			//   audio_2nd = 1;
			 //  audio_2nd_i = 0;
			 //  jumprel = 1;
			//}
                }

		if(joyl & JOY_PAD_L) {
                        mGame.Joysticks[jnum-1]->mLeft = 0;
                } else {
                        mGame.Joysticks[jnum-1]->mLeft = 1;
                }
                if(joyl & JOY_PAD_R) {
                        mGame.Joysticks[jnum-1]->mRight = 0;
                } else {
                        mGame.Joysticks[jnum-1]->mRight = 1;
                }
	}	


	//return 0;
}

void process_keys(char mkeynum){

	if(mkeynum == 1){
		if(getkey(KEY_UP) > 0){
		   mGame.KeyArrow.mAccP = 1;
		} else {
		   mGame.KeyArrow.mAccP = 0;
		}

		if(getkey(KEY_RIGHT) > 0){
		   mGame.KeyArrow.mRight = 1;
		} else {
		   mGame.KeyArrow.mRight = 0;
		}

		if(getkey(KEY_LEFT) > 0){
		   mGame.KeyArrow.mLeft = 1;
		} else {
		   mGame.KeyArrow.mLeft = 0;
		}
	}

	if(mkeynum == 2){
		if(getkey(KEY_W) > 0){
		   mGame.KeyWASD.mAccP = 1;
		   //Keyboard[0]->mAccP = 1;
		} else {
		   mGame.KeyWASD.mAccP = 0;
		   //Keyboard[0]->mAccP = 0;
		}

		if(getkey(KEY_D) > 0){
		   mGame.KeyWASD.mRight = 1;
		   //Keyboard[0]->mRight = 1;
		} else {
		   mGame.KeyWASD.mRight = 0;
		   //Keyboard[0]->mRight = 0;
		}

		if(getkey(KEY_A) > 0){
		   mGame.KeyWASD.mLeft = 1;
		   //Keyboard[0]->mLeft = 1;
		} else {
		   mGame.KeyWASD.mLeft = 0;
		   //Keyboard[0]->mLeft = 0;
		}
	}
}

char getanykey(){
	char ki;
	
	for(ki = 0; ki < 120; ki++){
		if(getkey(ki) > 0){
			return 1;
		}
	}

	return 0;
}

void check_quit(){

   if(getkey(KEY_Q) > 0){
      if(getkey(KEY_CTRL) > 0){
	  mGame.bRacing = 0;
	  mGame.bGamePaused = 0;
	  mGame.bCountDown = 0;
	  mGame.bFrameReady = 0;
      }
   }
}


void process_input(){

   char ci;
   
   for(ci = 0; ci < 3; ci++){
   
   	if(mGame.mPControls[ci]->bIsActive){
   		if(mGame.mPControls[ci]->mKeyNum){
   			process_keys(mGame.mPControls[ci]->mKeyNum);
   		}
   		if(mGame.mPControls[ci]->mJoyNum){
   			process_joystick(mGame.mPControls[ci]->mJoyNum);
   		}
   	}
   }

   check_quit();
   
   if(getkey(KEY_ESC) > 0){
	clearkey(KEY_ESC);
	mGame.bGamePaused = 1;	  
   }
    	
}

int getsin(int x){
   if(x < 0){
        while(x < 0){x += 360;}
   }
   if(x > 359){
        while(x > 359){x -= 360;}
   }

   return sintable[x][0];
}

int getcos(int x){
   if(x < 0){
        while(x < 0){x += 360;}
   }
   if(x > 359){
        while(x > 359){x -= 360;}
   }

   return sintable[x][1];
}

void calc_way(){
 	int wi;
 	int nX, nY;
 	char nC;
 	
 	/*
 	cbm_k_setnam(mwaypath);
	cbm_k_setlfs(3,8,0);
	cbm_k_open();
	
	cbm_k_ckout(3);
	*/
	
	openFile(mwaypath, 3);
	
	wi = cbm_k_acptr();
	
	mGame.mWaypointNum = wi;
			
	for(wi = 0; wi < mGame.mWaypointNum; wi++){
		nX = cbm_k_acptr();
		nY = cbm_k_acptr();
		nC = cbm_k_acptr();				
	
		mGame.mWaypoints[wi].x = (nX * 16) + 8;
		mGame.mWaypoints[wi].y = (nY * 16) + 8;
		mGame.mWaypoints[wi].c = nC;
	}
	
	mGame.mFinishLine = mGame.mWaypoints[0].y;
	
	cbm_k_close(3);

}

void loadVera(char *fname, unsigned int address, int flag){
   cbm_k_setnam(fname);
   cbm_k_setlfs(1,8,0);
   cbm_k_load(flag, address);    
}

void loadFile(char *fname, unsigned int address){
   cbm_k_setnam(fname);
   cbm_k_setlfs(1,8,0);
   cbm_k_load(0, address);    
}

void openFile(char *fname, char file){

 	cbm_k_setnam(fname);
	cbm_k_setlfs(file,8,0);
	cbm_k_open();
	
	cbm_k_ckout(file);
}

int rangechk(int dir){
	int tdir;

	if((dir < 0) || (dir > 359)){
  		VERA.control = 0x80;
		 __asm__("jsr %w",IOINIT);
   	}

	if((dir < 5) || (dir > 354)){return DIR_0;}

	if((dir > 42) && (dir < 48)){return DIR_45;}

        if((dir > 132) && (dir < 138)){return DIR_135;}

        if((dir > 222) && (dir < 228)){return DIR_225;}

        if((dir > 312) && (dir < 318)){return DIR_315;}

	tdir = ( (dir+5) / 10) + 1;
	
	if(tdir > DIR_350){
		return DIR_350;
	}

	if(tdir < 1){
		return DIR_0;
	}
	
	return tdir;
}

int rangechkcar(int dir){
    int tdir = dir;
    
    if(tdir < 0){
      	while(tdir < 0){
      		tdir += 360;
      	}
	return tdir;
    }
     
    if(tdir > 359){
    	while(tdir > 359){
    		tdir -= 360;
    	}
    }

    return tdir;
}

void apply_physics(struct Player *cPlayer){

   int tmpdirx;
   int tmpdiry;
   int tmplx;
   int tmply;
   int tmprx;
   int tmpry;

    if(cPlayer->mControl->mAccP == 1){
        cPlayer->mVel++;
    } else {
        if(cPlayer->mVel > 0) cPlayer->mVel--;
        if(cPlayer->mVelL > 3) cPlayer->mVelL -= 4;
        if(cPlayer->mVelR > 3) cPlayer->mVelR -= 4;
	if(cPlayer->mVelL == 1) cPlayer->mVelL = 0;
        if(cPlayer->mVelR == 1) cPlayer->mVelR = 0;
    }

    if((cPlayer->mControl->mLeft == 1)  && (cPlayer->mVel > 10)){
            cPlayer->mDir = rangechkcar(cPlayer->mDir - 5);
            cPlayer->mVelL++;
           if(cPlayer->mVelR > 0)cPlayer->mVelR--;
    } else {
       if(cPlayer->mVelL > 0) cPlayer->mVelL--;
    }

    if((cPlayer->mControl->mRight == 1) && (cPlayer->mVel > 10)){
            cPlayer->mDir = rangechkcar(cPlayer->mDir + 5);
	    //mDir++;
            cPlayer->mVelR++;
           if(cPlayer->mVelL > 0) cPlayer->mVelL--;
    } else {
       if(cPlayer->mVelR > 0)cPlayer->mVelR--;
    }

//    if(cPlayer->mVel < 0) { cPlayer->mVel = 0;}
    if(cPlayer->mVel > cPlayer->mMaxX){ cPlayer->mVel-=3;} // = cPlayer->mMaxX;
    if(cPlayer->mVel < 0) { cPlayer->mVel = 0;}    
    if(cPlayer->mVel < 15 ) {cPlayer->mVelL = 0; cPlayer->mVelR = 0;}
    
    if(cPlayer->mVelL < 0) {cPlayer->mVelL = 0;}
    if(cPlayer->mVelL > cPlayer->mMaxP) {cPlayer->mVelL = cPlayer->mMaxP;}

    if(cPlayer->mVelR < 0) {cPlayer->mVelR = 0;}
    if(cPlayer->mVelR > cPlayer->mMaxP) {cPlayer->mVelR = cPlayer->mMaxP;}

    tmpdirx = getsin(cPlayer->mDir);
    tmpdiry = getcos(cPlayer->mDir);

    tmplx = getsin(cPlayer->mDir+90);
    tmply = getcos(cPlayer->mDir+90);

    tmprx = getsin(cPlayer->mDir-90);
    tmpry = getcos(cPlayer->mDir-90);

    cPlayer->mVelX = ((tmpdirx * cPlayer->mVel) + (tmplx * cPlayer->mVelL) + (tmprx * cPlayer->mVelR))/2550;
    cPlayer->mVelY = ((tmpdiry * cPlayer->mVel) + (tmply * cPlayer->mVelL) + (tmpry * cPlayer->mVelR))/2550;

    cPlayer->pl_cur_dir = rangechk(rangechkcar(cPlayer->mDir));
//    cPlayer->pl_cur_dir = rangechk(cPlayer->mDir);

    cPlayer->mPos.x += cPlayer->mVelX;
    cPlayer->mPos.y -= cPlayer->mVelY;
    
}

void set_boom_sprite(struct PSprite *cSprite, struct Player *cPlayer){

    if(cPlayer->mBoomCount < 1){
    	if(cPlayer->mBoomDelay < 1){
	    	cPlayer->bIsVisible = 0;
    		clear_sprite(cPlayer->mPlayer);
    	}
    	cPlayer->mBoomDelay--;
    } else {
 	if(cPlayer->mBoomDelay < 1){
 		cPlayer->mBoomCount--;
 		cPlayer->mBoomDelay = 5;
 	}
 	
 	cPlayer->mBoomDelay--;
    	
    	cSprite->blocklo = mGame.mBoomLo + (SPRITE_BLOCKLO(cPlayer->mBoomCount * SPRITE_SIZE) & 0x00ff); // SPRITE_BLOCKLO(VRAM_boom + (cPlayer->mBoomCount * SPRITE_SIZE));
    	cSprite->blockhi = mGame.mBoomHi; // SPRITE_BLOCKHI(VRAM_boom + (cPlayer->mBoomCount * SPRITE_SIZE));    	
    	cSprite->palette_offset = 10;
    	
    	cSprite->mPos.x = (320-16) + ( cPlayer->mPos.x - mGame.mViewport.x);
	cSprite->mPos.y = (240-16) + ( cPlayer->mPos.y - mGame.mViewport.y);
    }

}

void calc_sprite_pos(struct PSprite *cSprite, struct Player *cPlayer){
	
	unsigned short blockoff;
	
	blockoff = 0;
	
	if(cPlayer->pl_cur_dir < DIR_0){
	        mGame.bRacing = 0;   
               	mGame.mWinner = 1;     	
	}

	if(cPlayer->pl_cur_dir > DIR_315){
	        mGame.bRacing = 0;   
               	mGame.mWinner = 2;     	
	}
	
	
	
 	if((cPlayer->pl_cur_dir == DIR_45) || ( cPlayer->pl_cur_dir == DIR_135) || ( cPlayer->pl_cur_dir == DIR_225) || ( cPlayer->pl_cur_dir == DIR_315)){
 	
                blockoff=10*SPRITE_SIZE;
                        
		switch( cPlayer->pl_cur_dir){
		    case DIR_45:
			cSprite->flipx = 0x0;
	                cSprite->flipy = 0x0;
		    break;
                    case DIR_135:
			cSprite->flipx = 0x0;
	                cSprite->flipy = 0x2;
                    break;
                    case DIR_225:
			cSprite->flipx = 0x1;
	                cSprite->flipy = 0x2;
                    break;
                    case DIR_315:
			cSprite->flipx = 0x1;
	                cSprite->flipy = 0x0;
                    break;

		
		}
	} else {


		if(( cPlayer->pl_cur_dir >= DIR_0) && ( cPlayer->pl_cur_dir <= DIR_90)){
		
			cSprite->flipx = 0x0;
        	        cSprite->flipy = 0x0;
        	        
		        //blockoff = ( ( cPlayer->pl_cur_dir - 1 ) * SPRITE_SIZE);
		        
		        switch(cPlayer->pl_cur_dir){
		        	case DIR_0:
		        		blockoff = 0;
		        		break;
		        	case DIR_10:
		        		blockoff = SPRITE_SIZE;
		        		break;
		        	case DIR_20:
		        		blockoff = SPRITE_SIZE * 2;
		        		break;
		        	case DIR_30:
		        		blockoff = SPRITE_SIZE * 3;
		        		break;
		        	case DIR_40:
		        		blockoff = SPRITE_SIZE * 4;
		        		break;
		        	case DIR_50:
		        		blockoff = SPRITE_SIZE * 5;
		        		break;
		        	case DIR_60:
		        		blockoff = SPRITE_SIZE * 6;
		        		break;
		        	case DIR_70:
		        		blockoff = SPRITE_SIZE * 7;
		        		break;
		        	case DIR_80:
		        		blockoff = SPRITE_SIZE * 8;
		        		break;
		        	case DIR_90:
		        		blockoff = SPRITE_SIZE * 9;
		        		break;		        
		        };
		        
		} else

        	if(( cPlayer->pl_cur_dir > DIR_90) && ( cPlayer->pl_cur_dir <= DIR_180)){
        	        
        	        cSprite->flipx = 0x0;
        	        cSprite->flipy = 0x2;
        	        
		        //blockoff = ( ( 9 - ( cPlayer->pl_cur_dir - 10 )) * SPRITE_SIZE);
		        
		       switch(cPlayer->pl_cur_dir){
		        	case DIR_180:
		        		blockoff = 0;
		        		break;
		        	case DIR_170:
		        		blockoff = SPRITE_SIZE;
		        		break;
		        	case DIR_160:
		        		blockoff = SPRITE_SIZE * 2;
		        		break;
		        	case DIR_150:
		        		blockoff = SPRITE_SIZE * 3;
		        		break;
		        	case DIR_140:
		        		blockoff = SPRITE_SIZE * 4;
		        		break;
		        	case DIR_130:
		        		blockoff = SPRITE_SIZE * 5;
		        		break;
		        	case DIR_120:
		        		blockoff = SPRITE_SIZE * 6;
		        		break;
		        	case DIR_110:
		        		blockoff = SPRITE_SIZE * 7;
		        		break;
		        	case DIR_100:
		        		blockoff = SPRITE_SIZE * 8;
		        		break;		        
		        };

	        } else

	        if(( cPlayer->pl_cur_dir > DIR_180) && ( cPlayer->pl_cur_dir <= DIR_270)){

	                cSprite->flipx = 0x1;
	                cSprite->flipy = 0x2;
	                
	                //blockoff = ( (cPlayer->pl_cur_dir-19) * SPRITE_SIZE);
	                
		        switch(cPlayer->pl_cur_dir){
		        	case DIR_190:
		        		blockoff = SPRITE_SIZE;
		        		break;
		        	case DIR_200:
		        		blockoff = SPRITE_SIZE * 2;
		        		break;
		        	case DIR_210:
		        		blockoff = SPRITE_SIZE * 3;
		        		break;
		        	case DIR_220:
		        		blockoff = SPRITE_SIZE * 4;
		        		break;
		        	case DIR_230:
		        		blockoff = SPRITE_SIZE * 5;
		        		break;
		        	case DIR_240:
		        		blockoff = SPRITE_SIZE * 6;
		        		break;
		        	case DIR_250:
		        		blockoff = SPRITE_SIZE * 7;
		        		break;
		        	case DIR_260:
		        		blockoff = SPRITE_SIZE * 8;
		        		break;
		        	case DIR_270:
		        		blockoff = SPRITE_SIZE * 9;
		        		break;		        
		        };
	                

	        } else

	       	if( ( cPlayer->pl_cur_dir > DIR_270) &&  ( cPlayer->pl_cur_dir <= DIR_350) ){
	                
	                cSprite->flipx = 0x1;
	                cSprite->flipy = 0x0;
	                
	                //blockoff = ( (9-( cPlayer->pl_cur_dir-28)) * SPRITE_SIZE);
	                
       		       switch(cPlayer->pl_cur_dir){
		        	case DIR_350:
		        		blockoff = SPRITE_SIZE;
		        		break;
		        	case DIR_340:
		        		blockoff = SPRITE_SIZE * 2;
		        		break;
		        	case DIR_330:
		        		blockoff = SPRITE_SIZE * 3;
		        		break;
		        	case DIR_320:
		        		blockoff = SPRITE_SIZE * 4;
		        		break;
		        	case DIR_310:
		        		blockoff = SPRITE_SIZE * 5;
		        		break;
		        	case DIR_300:
		        		blockoff = SPRITE_SIZE * 6;
		        		break;
		        	case DIR_290:
		        		blockoff = SPRITE_SIZE * 7;
		        		break;
		        	case DIR_280:
		        		blockoff = SPRITE_SIZE * 8;
		        		break;		        
		        };

	        }	        	        	        
        }
        
        if(blockoff > (10 * SPRITE_SIZE)){
        	mGame.bRacing = 0;        	
        	mGame.mWinner = 3;
        }
        
        /*

        if(blockoff < 0){
        	mGame.bRacing = 0;        	
        	mGame.mWinner = 4;
        }
        
        */
	

    cSprite->blocklo = mGame.mCarsLo + (SPRITE_BLOCKLO(blockoff) & 0x00ff); //SPRITE_BLOCKLO(VRAM_sprites+blockoff);
    cSprite->blockhi = mGame.mCarsHi; // SPRITE_BLOCKHI(VRAM_sprites+blockoff);    
    
    cSprite->mPos.x = (320-16) + ( cPlayer->mPos.x - mGame.mViewport.x);
    cSprite->mPos.y = (240-16) + ( cPlayer->mPos.y - mGame.mViewport.y);
    
    if(cPlayer->mPlayer != mGame.mLeader->mPlayer){
    	if( (cSprite->mPos.x < 0) || (cSprite->mPos.x > 608) || (cSprite->mPos.y < 0) || (cSprite->mPos.y > 448)){
    		killPlayer(cPlayer);
    	}    
    }         	
}

void process_sprites(){

	if(mGame.Player1.bIsVisible){
		if(mGame.Player1.bIsAlive){
			calc_sprite_pos(&mGame.PSprite1, &mGame.Player1);
		} else {
			set_boom_sprite(&mGame.PSprite1, &mGame.Player1);		
		}
	}
	
	if(mGame.Player2.bIsVisible){
		if(mGame.Player2.bIsAlive){
			calc_sprite_pos(&mGame.PSprite2, &mGame.Player2);
		} else {
			set_boom_sprite(&mGame.PSprite2, &mGame.Player2);		
		}
		
	}
	
	if(mGame.Player3.bIsVisible){
		if(mGame.Player3.bIsAlive){
			calc_sprite_pos(&mGame.PSprite3, &mGame.Player3);
		} else {
			set_boom_sprite(&mGame.PSprite3, &mGame.Player3);		
		}
		
	}

#ifdef MDEBUG	
	if(mGame.bDebug){
		process_debug();	
	}
#endif	
	
	
	/*
	if(mGame.Player4.bIsVisible){
		if(mGame.Player4.bIsAlive){
			calc_sprite_pos(&mGame.PSprite4, &mGame.Player4);
		} else {
			set_boom_sprite(&mGame.PSprite4, &mGame.Player4);
		}
		
	}*/
}

char getWayState(int wState, int cX, int cY, int vX, int vY){

		switch(wState){
			case 1:
				if(cX > vX){
					return 1;					
				}
				break;
			case 2:
				if(cX < vX){
					return 1;					
				}
				break;
			case 4:
				if(cY > vY){
					return 1;					
				}
				break;
			case 8:
				if(cY < vY){
					return 1;					
				}
				break;
			case 5:
				if(cX > vX){
					return 1;					
				}
				if(cY > vY){
					return 1;					
				}
				break;
			case 10:
				if(cX < vX){
					return 1;					
				}
				if(cY < vY){
					return 1;					
				}
				break;
			case 9:
				if(cX > vX){
					return 1;					
				}
				if(cY < vY){
					return 1;					
				}
				break;
			case 6:
				if(cX < vX){
					return 1;					
				}
				if(cY > vY){
					return 1;					
				}
				break;


			default:
				return 0;
				break;		
		}


	return 0;

}

void process_physics(){

	if(mGame.Player1.bIsAlive){
 		apply_physics(&mGame.Player1);
 	}
 	
	if(mGame.Player2.bIsAlive){
		apply_physics(&mGame.Player2);
	}

	if(mGame.Player3.bIsAlive){
		apply_physics(&mGame.Player3);
	}

/*
	if(mGame.Player4.bIsAlive){
		apply_physics(&mGame.Player4);    
	}*/
}


void process_bot(struct Player *cBot){
	
	int dist, angle, diffa;
	int dx,dy;
	
	cBot->mControl->mAccP = 1;
	cBot->mControl->mLeft = 0;
	cBot->mControl->mRight = 0;
	
	//dx = way_level1[cBot->mControl->mNextWay][0] - (cBot->mPos.x + 320 - 16);
	//dy = way_level1[cBot->mControl->mNextWay][1] - (cBot->mPos.y + 240 - 16);
	
	
	dx = (cBot->mPos.x ) - (mGame.mWaypoints[cBot->mControl->mNextWay].x);// - 320 - 16);
        dy = (cBot->mPos.y ) - (mGame.mWaypoints[cBot->mControl->mNextWay].y);// - 240 - 16) ;

//	dx = (way_level1[cBot->mControl->mNextWay][0]) - (cBot->mPos.x );
//	dy = (way_level1[cBot->mControl->mNextWay][1]) - (cBot->mPos.y );


//	dx = cBot->mPos.x - (way_level1[cBot->mControl->mNextWay][0] - (320 - 16));
//	dy = cBot->mPos.y - (way_level1[cBot->mControl->mNextWay][1] - (320 - 16));


	//dx = 152 - (cBot->mPos.x + 320 - 16);
	//dy = 312 - (cBot->mPos.y + 240 - 16);

	
	//dist = distance(dx,dy);
	


	//angle = getangle(way_level1[cBot->mControl->mNextWay][0] - cBot->mPos.x, way_level1[cBot->mControl->mNextWay][1] - cBot->mPos.y);
	
	angle = getangle(-dx, dy);
		
	//diffa = (cBot->mDir - angle); 
	diffa = getdelta(cBot->mDir,angle);

	if(diffa < -5){
		cBot->mControl->mLeft = 1;
		cBot->mControl->mRight = 0;
		//isdone = 1;
	}

	if(diffa > 5){
		cBot->mControl->mLeft = 0;
		cBot->mControl->mRight = 1;
		//isdone = 1;
	}

	if(dx < 0){
	    dx = -dx;
	}
	
	if(dy < 0){
	    dy = -dy;
	}


	dist = dx+dy;

	if(dist < 65){
//	if((dx < 32) && (dy <32)){
		//isdone = 1;
		if(cBot->mControl->mNextWay == (mGame.mWaypointNum-1)){
			cBot->mControl->mNextWay = 0;
			cBot->bCheckFinish = 1;	
		} else {
			cBot->mControl->mNextWay++;
		}				
	} else {
		if(getWayState(mGame.mWaypoints[cBot->mControl->mNextWay].c, cBot->mPos.x, cBot->mPos.y, mGame.mWaypoints[cBot->mControl->mNextWay].x, mGame.mWaypoints[cBot->mControl->mNextWay].y)){
		if(cBot->mControl->mNextWay == (mGame.mWaypointNum-1)){
			cBot->mControl->mNextWay = 0;
			cBot->bCheckFinish = 1;	
		} else {
			cBot->mControl->mNextWay++;
		}				

		}
	}

}

void process_bots(){

	if(mGame.Player1.mControl->bIsBot > 0){
		process_bot(&mGame.Player1);
	}

	if(mGame.Player2.mControl->bIsBot > 0){
		process_bot(&mGame.Player2);
	}

	if(mGame.Player3.mControl->bIsBot > 0){
		process_bot(&mGame.Player3);
	}

/*
	if(mGame.Player4.mControl->bIsBot > 0){
		process_bot(&mGame.Player4);
	} */


}

void killPlayer(struct Player *cPlayer){

	if(cPlayer->bFinished == 1){
		return;
	}

	if(mGame.mFinCount){
		return;
	}

	cPlayer->bFinished = 1;
	cPlayer->mControl->bIsActive = 0;
	cPlayer->mControl->bIsBot = 0;

	cPlayer->bIsAlive = 0;
	cPlayer->bIsValid = 0;	
	
	//cPlayer->bIsVisible = 0;
	cPlayer->mBoomCount = 8;
	
	mGame.mPCount--;
	
	stop_engine(&cPlayer->mEngine);
	
	play_crash(250, 15, 2);
	
			
}

void setPlayerPlace(struct Player *cPlayer){

	cPlayer->bFinished = 1;
	cPlayer->mControl->bIsActive = 0;

	//cPlayer->bIsVisible = 0; //???
	
	cPlayer->mPlace = mGame.mPlace;
	
	if(mGame.mPlace == 1){
		mGame.mWinner = cPlayer->mPlayer;
		mGame.mFinCount = 120;		
	}
	
	mGame.mPlace++;
	
	//if(mGame.mPlace > mGame.mPCount){
	//	mGame.mFinCount = 120;		
	//}
}

void process_player(struct Player *cPlayer){
	
	int dist;
	int dx,dy,cindex;
	unsigned char cval;
	char mDebug;
	char bDebug;	
//	unsigned char* colmap;
	
	mDebug = 0;
	bDebug = 0;	
	
	if(cPlayer->mControl->bIsBot == 0){
	
		dx = (cPlayer->mPos.x ) - (mGame.mWaypoints[cPlayer->mControl->mNextWay].x);// - 320 - 16);
	        dy = (cPlayer->mPos.y ) - (mGame.mWaypoints[cPlayer->mControl->mNextWay].y);// - 240 - 16) ;

		if(dx < 0){
		    dx = -dx;
		}
	
		if(dy < 0){
		    dy = -dy;
		}


		dist = dx+dy;

		
		if(dist < 65){
			if(cPlayer->mControl->mNextWay == (mGame.mWaypointNum-1)){
				mDebug = cPlayer->mControl->mNextWay;
				bDebug = 1;
				cPlayer->mControl->mNextWay = 0;
				cPlayer->bCheckFinish = 1;

			} else {
				mDebug = cPlayer->mControl->mNextWay;			
				bDebug = 1;				
				cPlayer->mControl->mNextWay++;

			}				
		} else {
			if(getWayState(mGame.mWaypoints[cPlayer->mControl->mNextWay].c, cPlayer->mPos.x, cPlayer->mPos.y, mGame.mWaypoints[cPlayer->mControl->mNextWay].x, mGame.mWaypoints[cPlayer->mControl->mNextWay].y)){
				if(cPlayer->mControl->mNextWay == (mGame.mWaypointNum-1)){
					mDebug = cPlayer->mControl->mNextWay;				
					bDebug = 1;					
					cPlayer->mControl->mNextWay = 0;
					cPlayer->bCheckFinish = 1;

				} else {
					mDebug = cPlayer->mControl->mNextWay;
					bDebug = 1;					
					cPlayer->mControl->mNextWay++;

				}				

			}
		}

	}

#ifdef MDEBUG	
	if(mGame.bDebug){
		if(cPlayer->mPlayer == 1){
			if(bDebug){
				mGame.mDebug[mDebug].palette_offset++;
			}
		}
	}
#endif	
	
	if(cPlayer->bCheckFinish){
		if(cPlayer->mPos.y < mGame.mFinishLine){

			cPlayer->mLaps--;
			
			if(cPlayer->mLaps < mGame.mLaps){
			
				mGame.mLaps--;
				
				if(mGame.mLaps > 0){
				
			   		   set_text_sprite(&mGame.mLapsCount[4], mGame.mTXTLapsAddrShort, mGame.mTXTLapsLo, SPRITE_32_BY_32, 0, (mGame.mLaps - 1) * TEXTISIZET , MLAPSCOUNTX + (MLAPSCOUNTSPACE >> 1) + (MLAPSCOUNTSPACE * 4), MLAPSCOUNTY);

			   		mMenu.bDoUpdate = 4;
			   }
			 }
			
			cPlayer->bCheckFinish = 0;
			
			if(cPlayer->mLaps < 1){
				setPlayerPlace(cPlayer);
				
			}
		}
	
	}
	

		cindex = GETINDEX(cPlayer->mPos.x, cPlayer->mPos.y); //, &cmod);
	
		if(cindex > 8191){
			RAM_BANK = 2;
			cindex -= 8192;
		} else {
			RAM_BANK = 1;		
		}
	
		cval = mColmap[cindex];
		
		if(cval == MTRACKOUTSIDE){
			if(cPlayer->bIsOutside > 0){

				cPlayer->mOutCount--;

				if(cPlayer->mOutCount < 1){						
					killPlayer(cPlayer);								
				} else if(cPlayer->mOutCount < MOUTLOOSE){				
					cPlayer->bIsValid = 0;					
				}
								
			} else {
				cPlayer->bIsOutside = 1;
				cPlayer->mOutCount = MOUTKILL;
				cPlayer->mMaxX = MSPEEDMAXOUTSIDE;
			}			
			cPlayer->mEngine.mRevTime = 0;				
		} else {			
			if(cPlayer->bIsOutside > 0){
				cPlayer->bIsOutside = 0;	
				cPlayer->bIsValid = 1;	
				cPlayer->mOutCount = MOUTKILL;								
			}
			
			if(cval == MTRACKONEDGE){
				cPlayer->mMaxX = MSPEEDMAXONEDGE;
				cPlayer->mEngine.mRevTime = 0;
			} else {
				if(cPlayer->mControl->bIsBot > 0){
					cPlayer->mMaxX = MSPEEDMAX + mGame.mAISpeed;
				} else {
					cPlayer->mMaxX = MSPEEDMAX;
				}						
			}
		}
		
		cPlayer->PPlace.mPFactor = cPlayer->mControl->mNextWay + (((mGame.mWaypointNum + 2) * 2) * (mMenu.mLaps - cPlayer->mLaps));
		if(cPlayer->bCheckFinish){
			cPlayer->PPlace.mPFactor += mGame.mWaypointNum + 1;
		}
}

void set_leader(char nlead){
	int vx, vy;
	if(nlead != mGame.mLeader->mPlayer){
		mGame.bMoveToLead = 16;
		mGame.mLeader = mGame.Players[nlead - 1];	
		vx =  (mGame.mViewport.x - mGame.mLeader->mPos.x);
		vy =  (mGame.mViewport.y - mGame.mLeader->mPos.y);
		
		if(vx < 0 ){vx = -vx;}
		if(vy < 0 ){vx = -vy;}		
		
		if(vx < 2 ){
			mGame.bViewXDone = 1;
		} else {
			mGame.bViewXDone = 0;
			mGame.mOldView.x = vx / 16;
			if(mGame.mOldView.x < 1){
				mGame.mOldView.x = 1;
			}	
		}

		if(vy < 2 ){
			mGame.bViewYDone = 1;
		} else {
			mGame.bViewYDone = 0;
			mGame.mOldView.y = vy / 16;
			if(mGame.mOldView.y < 1){
				mGame.mOldView.y = 1;
			}
		}
		
		if(mGame.bViewXDone && mGame.bViewYDone){
			mGame.bMoveToLead = 0;
		}				
	}
}

void swap_placements(char p1, char p2){

	struct Placement* tmpPlace;
	
	tmpPlace = mGame.PPlaces[p1];
	
	mGame.PPlaces[p1] = mGame.PPlaces[p2];
	mGame.PPlaces[p2] = tmpPlace;
}

void get_live_players(){
	char pi;
	char mi;
	
	for(pi = 0; pi < 3; pi++){
		if(mGame.Players[mGame.PPlaces[pi]->mPlayer]->bIsValid == 0){			
			for(mi = pi; mi < 3; mi++){
				if(mGame.Players[mGame.PPlaces[mi]->mPlayer]->bIsValid){
					swap_placements(pi, mi);
					break;
				}

			}
		}
	}	

}

/*
void sort_players4(){

	if(mGame.PPlaces[0]->mPFactor > mGame.PPlaces[1]->mPFactor){
		swap_placements(0, 1);
	}
	
	if(mGame.PPlaces[2]->mPFactor > mGame.PPlaces[3]->mPFactor){
		swap_placements(2, 3);
	}
	

	if(mGame.PPlaces[0]->mPFactor > mGame.PPlaces[2]->mPFactor){
		swap_placements(0, 2);
	}
	
	if(mGame.PPlaces[3]->mPFactor > mGame.PPlaces[1]->mPFactor){
		swap_placements(3, 1);
	}

}
*/

void sort_players3(){

	if(mGame.PPlaces[0]->mPFactor > mGame.PPlaces[1]->mPFactor){
		swap_placements(0, 1);
	}
	
	if(mGame.PPlaces[2]->mPFactor > mGame.PPlaces[1]->mPFactor){
		swap_placements(2, 1);
	}
	

	if(mGame.PPlaces[0]->mPFactor > mGame.PPlaces[2]->mPFactor){
		swap_placements(0, 2);
	}
}

void calc_player_dfactor(struct Player *cPlayer){

	int dx, dy;
	unsigned int sx,sy;
	
	dx = (cPlayer->mPos.x - mGame.mWaypoints[cPlayer->mControl->mNextWay].x);
	dy = (cPlayer->mPos.y - mGame.mWaypoints[cPlayer->mControl->mNextWay].y);	
	
	if(dx < 0){sx = -dx;} else {sx = dx;}
	if(dy < 0){sy = -dy;} else {sy = dy;}
	
	sx = sx >> 3;
	sy = sy >> 3;
	
	if(sx > 181){sx = 181;}
	if(sy > 181){sy = 181;}	

	cPlayer->PPlace.mPFactor = (sx * sx) + (sy * sy);
}

void calc_players_dfactor(){

	if(mGame.Player1.bIsValid){
		calc_player_dfactor(&mGame.Player1);
	} else {
		mGame.Player1.PPlace.mPFactor = 0xffff;
	}

	if(mGame.Player2.bIsValid){
		calc_player_dfactor(&mGame.Player2);
	} else {
		mGame.Player2.PPlace.mPFactor = 0xffff;
	}

	if(mGame.Player3.bIsValid){
		calc_player_dfactor(&mGame.Player3);
	} else {
		mGame.Player3.PPlace.mPFactor = 0xffff;
	}

/*
	if(mGame.Player4.bIsValid){
		calc_player_dfactor(&mGame.Player4);
	} else {
		mGame.Player4.PPlace.mPFactor = 0xffff;
	}
*/

}


void process_players(){
	char pi, mVCount;

	if(mGame.Player1.bIsAlive){
		process_player(&mGame.Player1);
	}

	if(mGame.Player2.bIsAlive){
		process_player(&mGame.Player2);
	}

	if(mGame.Player3.bIsAlive){
		process_player(&mGame.Player3);
	}

/*
	if(mGame.Player4.bIsAlive){
		process_player(&mGame.Player4);
	}
*/
		
	if(mGame.mPCount == 1){	
		for(pi = 0; pi < 3; pi++){
			if(mGame.Players[pi]->bIsAlive){
				setPlayerPlace(mGame.Players[pi]);
				return;
			}
		}
	}
	
	mVCount = 0;
	
	for(pi = 0; pi < 3; pi++){
		if(mGame.Players[pi]->bIsValid){
			mVCount++;
		}
	}	
		
	switch(mVCount){
	/*
		case 4:
			sort_players4();
			
			if(mGame.PPlaces[0]->mPFactor == mGame.PPlaces[1]->mPFactor){
				calc_players_dfactor();
				sort_players4();
					
				set_leader(mGame.PPlaces[0]->mPlayer + 1);			
			} else {
				set_leader(mGame.PPlaces[1]->mPlayer + 1);
			}
			
			break;*/
		case 3:
			//get_live_players();
			
			sort_players3();
			
			if(mGame.PPlaces[0]->mPFactor == mGame.PPlaces[1]->mPFactor){
			
				calc_players_dfactor();
				sort_players3();			
				
				set_leader(mGame.PPlaces[0]->mPlayer + 1);
			
			} else {
				set_leader(mGame.PPlaces[1]->mPlayer + 1);
			}
			
			break;
		
		case 2:
			get_live_players();
			
			if(mGame.PPlaces[0]->mPFactor == mGame.PPlaces[1]->mPFactor){
				calc_players_dfactor();
				
				if(mGame.PPlaces[0]->mPFactor > mGame.PPlaces[1]->mPFactor){
					set_leader(mGame.PPlaces[1]->mPlayer + 1);
				} else {
					set_leader(mGame.PPlaces[0]->mPlayer + 1);
				}
			
			} else {
				if(mGame.PPlaces[0]->mPFactor > mGame.PPlaces[1]->mPFactor){
					set_leader(mGame.PPlaces[0]->mPlayer + 1);
				} else {
					set_leader(mGame.PPlaces[1]->mPlayer + 1);
				}							
			}
						
			break;	
		case 1:
			for(pi = 0; pi < 3; pi++){
				if(mGame.Players[pi]->bIsValid){
					set_leader(mGame.Players[pi]->mPlayer);
				}
			}	
			break;
		default:
			break;			
	};
	
}

void calc_viewport(){

	int vx, vy;  		
	
	if(mGame.bMoveToLead){
		
		vx = (mGame.mViewport.x - mGame.mLeader->mPos.x);
		vy = (mGame.mViewport.y - mGame.mLeader->mPos.y);						
		
		if(mGame.bViewXDone){
			mGame.mViewport.x = mGame.mLeader->mPos.x;
			vx = 0; 
		}

		if(mGame.bViewYDone){
			mGame.mViewport.y = mGame.mLeader->mPos.y;
			vy = 0; 
		}
		
		if(vx < 0) {vx = -vx;}
		if(vy < 0) {vy = -vy;}
		
		if((vx <= mGame.mOldView.x) && (vy <= mGame.mOldView.y)){
			mGame.mViewport.x = mGame.mLeader->mPos.x;
			mGame.mViewport.y = mGame.mLeader->mPos.y;	
			mGame.bMoveToLead = 0;		
		} else {
			
			if(mGame.mViewport.x != mGame.mLeader->mPos.x){
				if(vx <= mGame.mOldView.x){
					mGame.mOldView.x = vx;
					mGame.bViewXDone = 1;
				}
				if(mGame.mViewport.x < mGame.mLeader->mPos.x){
					mGame.mViewport.x += mGame.mOldView.x;
				} else {
					mGame.mViewport.x -= mGame.mOldView.x;
				}
			}

			if(mGame.mViewport.y != mGame.mLeader->mPos.y){
				if(vy <= mGame.mOldView.y){
					mGame.mOldView.y = vy;
					mGame.bViewYDone = 1;
				}
				if(mGame.mViewport.y < mGame.mLeader->mPos.y){
					mGame.mViewport.y += mGame.mOldView.y;
				} else {
					mGame.mViewport.y -= mGame.mOldView.y;
				}
			}
			
			mGame.bMoveToLead--;			
		}
	} else {		
		mGame.mViewport.x = mGame.mLeader->mPos.x;
		mGame.mViewport.y = mGame.mLeader->mPos.y;	
	}
		

//	mGScroolX = mGame.mViewport.x - (320);
//	mGScroolY = mGame.mViewport.y - (240);
}

void clear_sprite(int cs){
	VERA.address = SPRITE_REGISTERS(cs);
        VERA.address_hi = VERA_INC_1 + 1; // the "+1" is the VRAM high

        VERA.data0 = 0; //cSprite->block & 0xff; // lower VRAM address bits
        VERA.data0 = 0; //cSprite->mode + ((cSprite->block >> 8) & 0x1f);
        VERA.data0 = 0; //cSprite->mPos.x & 0xff;
        VERA.data0 = 0; //cSprite->mPos.x >> 8;
        VERA.data0 = 0; //cSprite->mPos.y & 0xff;
        VERA.data0 = 0; //cSprite->mPos.y >> 8;
        VERA.data0 = 0; //mcol | cSprite->z | cSprite->flipx | cSprite->flipy;                 // leave collision mask and flips alone for now.
        VERA.data0 = 0; //cSprite->dimensions + cSprite->palette_offset;
}

void clear_sprites(int cS, int cR){
	int is;
	
	for(is = cS; is < (cS + cR); is++){
		clear_sprite(is);	
	}
}

void load_sprite(struct PSprite *cSprite, int sNum){

        VERA.address = SPRITE_REGISTERS(sNum);
        VERA.address_hi = VERA_INC_1 + 1; // the "+1" is the VRAM high

        VERA.data0 = cSprite->blocklo;// & 0xff; // lower VRAM address bits
        VERA.data0 = cSprite->mode | cSprite->blockhi; //((cSprite->block >> 8) & 0x0f);
        VERA.data0 = cSprite->mPos.x & 0xff;
        VERA.data0 = cSprite->mPos.x >> 8;
        VERA.data0 = cSprite->mPos.y & 0xff;
        VERA.data0 = cSprite->mPos.y >> 8;
        VERA.data0 = cSprite->colmask | cSprite->z | cSprite->flipx | cSprite->flipy;        
        VERA.data0 = cSprite->dimensions | cSprite->palette_offset;

}

void load_psprites(){
	if(mGame.Player1.bIsVisible){
		load_sprite(&mGame.PSprite1,1);
	}

	if(mGame.Player2.bIsVisible){
		load_sprite(&mGame.PSprite2,2);
	}

	if(mGame.Player3.bIsVisible){
		load_sprite(&mGame.PSprite3,3);
	}

/*
	if(mGame.Player4.bIsVisible){
		load_sprite(&mGame.PSprite4,4);			
	}
	
*/
}

void setMenuPlayer(int cPlayer, int cM1, int cM2){
	mMenu.PMenu[cPlayer][0]->blocklo = SPRITE_BLOCKLO(VRAM_texti + cM1);
	mMenu.PMenu[cPlayer][1]->blocklo = SPRITE_BLOCKLO(VRAM_texti + cM2);
	
	mMenu.PMenu[cPlayer][0]->blockhi = SPRITE_BLOCKHI(VRAM_texti + cM1);
	mMenu.PMenu[cPlayer][1]->blockhi = SPRITE_BLOCKHI(VRAM_texti + cM2);
}

void setPlCtrl(int cPlayer, int cIdx){

	switch(cIdx){
		case 0:
			setMenuPlayer(cPlayer, TEXTI_A, TEXTI_I);
			break;
		case 1:
			setMenuPlayer(cPlayer, TEXTI_J, TEXTI_1);
			break;
		case 2:
			setMenuPlayer(cPlayer, TEXTI_J, TEXTI_2);
			break;
		case 3:
			setMenuPlayer(cPlayer, TEXTI_J, TEXTI_3);
			break;
		case 4:
			setMenuPlayer(cPlayer, TEXTI_J, TEXTI_4);
			break;
		case 5:
			setMenuPlayer(cPlayer, TEXTI_K, TEXTI_1);
			break;
		case 6:
			setMenuPlayer(cPlayer, TEXTI_K, TEXTI_2);
			break;
	};

}

void load_lmenu(int cLine){
  int li = 0;
  int sOff;
  
  sOff = mMenu.mMenuLines[cLine].mSPos;
  
  while(li < mMenu.mMenuLines[cLine].mCCount){
	  load_sprite(mMenu.mMenuLines[cLine].mChar[li], sOff + li);
  	  li++;
  }
}

void setMenuOffset(int cLine, int cOffset){
  int li = 0;
  
  while(li < mMenu.mMenuLines[cLine].mCCount){
	  mMenu.mMenuLines[cLine].mChar[li]->palette_offset = cOffset;
  	  li++;
  }

}


void init_menulines(){

    int mPLine[7] = {TEXTI_P, TEXTI_L, TEXTI_A, TEXTI_Y, TEXTI_E, TEXTI_R, TEXTI_1 };
    int mTLine[5] = {TEXTI_L, TEXTI_E, TEXTI_V, TEXTI_E, TEXTI_L};
    int mLLine[4] = {TEXTI_L, TEXTI_A, TEXTI_P, TEXTI_S};    
    int mSLine[5] = {TEXTI_S, TEXTI_T, TEXTI_A, TEXTI_R, TEXTI_T};    
    int mELine[4] = {TEXTI_E, TEXTI_X, TEXTI_I, TEXTI_T};        
    int mAILine[7] = {TEXTI_A, TEXTI_I, TEXTI_L, TEXTI_E, TEXTI_V, TEXTI_E, TEXTI_L};

    RAM_BANK = 1;
    BINIT();
    
    init_lmenu(0, 7, mPLine, -1, &mMenu.mMenuPl1_1, &mMenu.mMenuPl1_2, 20, 67);
    
    mPLine[6] = TEXTI_2;
    init_lmenu(1, 7, mPLine, -1, &mMenu.mMenuPl2_1, &mMenu.mMenuPl2_2, 20, 77);
    
    mPLine[6] = TEXTI_3;
    init_lmenu(2, 7, mPLine, -1, &mMenu.mMenuPl3_1, &mMenu.mMenuPl3_2, 20, 87);        
    
    init_lmenu(3, 7, mAILine, 2, &mMenu.mMenuAI, 0, 20 , 97);
    
    init_lmenu(4, 5, mTLine, -1, &mMenu.mMenuLVL, 0, 243 , 67);
    
    init_lmenu(5, 4, mLLine, -1, &mMenu.mMenuLABS, 0, 243 , 77);

    init_lmenu(6, 5, mSLine, -1, 0, 0, 243 , 87);

    init_lmenu(7, 4, mELine, -1, 0, 0, 243 , 97);
}


void init_menu(){

    mMenu.bRunning = 0;
    mMenu.bWinnerRunning = 0;
    mPCM.bLoadSound = 0;	

    mMenu.PMenu[0][0] = &mMenu.mMenuPl1_1;
    mMenu.PMenu[0][1] = &mMenu.mMenuPl1_2;

    mMenu.PMenu[1][0] = &mMenu.mMenuPl2_1;
    mMenu.PMenu[1][1] = &mMenu.mMenuPl2_2;

    mMenu.PMenu[2][0] = &mMenu.mMenuPl3_1;
    mMenu.PMenu[2][1] = &mMenu.mMenuPl3_2;

    //mMenu.PMenu[3][0] = &mMenu.mMenuPl4_1;
    //mMenu.PMenu[3][1] = &mMenu.mMenuPl4_2;
        
    setTextSprite(&mMenu.mMenuPl1_1, 0, 80, 67);
    setTextSprite(&mMenu.mMenuPl1_2, 0, 86, 67);

    setTextSprite(&mMenu.mMenuPl2_1, 0, 80, 77);
    setTextSprite(&mMenu.mMenuPl2_2, 0, 86, 77);

    setTextSprite(&mMenu.mMenuPl3_1, 0, 80, 87);
    setTextSprite(&mMenu.mMenuPl3_2, 0, 86, 87);
    
    //setTextSprite(&mMenu.mMenuPl4_1, 0, 80, 97);
    //setTextSprite(&mMenu.mMenuPl4_2, 0, 86, 97);

    setTextSprite(&mMenu.mMenuLVL, TEXTI_1, 287,67);

    setTextSprite(&mMenu.mMenuLABS, TEXTI_5, 279,77);
    
    setTextSprite(&mMenu.mMenuAI, TEXTI_3, MAILEVELPOS, 97);
 
    setMenuPlayer(0, TEXTI_J, TEXTI_1);
    setMenuPlayer(1, TEXTI_A, TEXTI_I);
    setMenuPlayer(2, TEXTI_A, TEXTI_I);
//    setMenuPlayer(3, TEXTI_A, TEXTI_I);
    
    mMenu.mSelectCol[0] = 15;
    mMenu.mSelectCol[1] = 13;
    mMenu.mSelectCol[2] = 12;
    mMenu.mSelectCol[3] = 10;
    mMenu.mSelectCol[4] = 7;
    mMenu.mSelectCol[5] = 6;

    mMenu.mSelected = 0;
    
    mMenu.mMenuItem = 0;
    
    mMenu.mMenuLines[0].mSPos = 1;
    mMenu.mMenuLines[1].mSPos = 10;
    mMenu.mMenuLines[2].mSPos = 19;
    mMenu.mMenuLines[3].mSPos = 28;
    mMenu.mMenuLines[4].mSPos = 37;
    mMenu.mMenuLines[5].mSPos = 43;
    mMenu.mMenuLines[6].mSPos = 48;
    mMenu.mMenuLines[7].mSPos = 53;    
    
    mMenu.mPCtrl[0] = 1;
    mMenu.mPCtrl[1] = 0;
    mMenu.mPCtrl[2] = 0;
//    mMenu.mPCtrl[3] = 0;
    
    mMenu.mNumbers[0] = 0;
    mMenu.mNumbers[1] = TEXTI_1;
    mMenu.mNumbers[2] = TEXTI_2;
    mMenu.mNumbers[3] = TEXTI_3;
    mMenu.mNumbers[4] = TEXTI_4;
    mMenu.mNumbers[5] = TEXTI_5;
    mMenu.mNumbers[6] = 0;
    mMenu.mNumbers[7] = 0;
    mMenu.mNumbers[8] = 0;
    mMenu.mNumbers[9] = 0;
    
    mMenu.mLevel = 1;
    mMenu.mLaps = 5;
    
    mMenu.mAISpeed = 3;
    
    mMenu.mMClick.mChan = 0;
    mMenu.mMClick.mOn = 0;
    mMenu.mClickLength = 5;
    mMenu.mClickLengthShort = 3;
	    
    mMenu.bShowMenu = 1;
}

void process_engine(struct SThrottle* cEngine, int mvel){
	unsigned char mrand;
	int mfreq;
	char fhi, flo;

	if(cEngine->mRevTime == 0){
		mrand = RANDNEXT();
		mrand = mrand >> 2;
		
		if(mvel == 0){
			mrand = mrand >> 2;
		}
		
		cEngine->mRevTime = mrand;
		cEngine->mRev = 0;
	}

	cEngine->mRev++;
	cEngine->mRevTime--;
		
	if(mvel == 0){							
		mfreq = cEngine->mFreq + cEngine->mRev;
	} else {
		mfreq = (cEngine->mFreq + cEngine->mRev) + (mvel << 1);
	}

	fhi = (mfreq & 0xff00) >> 8;
	flo = mfreq & 0x00ff;	
		
	mGame.mChannels[cEngine->mChan].mFreqHi = fhi;
	mGame.mChannels[cEngine->mChan].mFreqLo = flo; 

/*
	if(mvel){
		mfreq -= mvel >> 1;

		fhi = (mfreq & 0xff00) >> 8;
		flo = mfreq & 0x00ff;
	}
*/

	mGame.mChannels[cEngine->mChan + 1].mFreqHi = fhi;
	mGame.mChannels[cEngine->mChan + 1].mFreqLo = flo; 
}

void start_engine(struct SThrottle* cEngine, char cchan){
	cEngine->mVol = 0x6f;
	cEngine->mOn = 1;
	cEngine->mFreq = 200;
	cEngine->mChan = cchan;	
	cEngine->mRev = 0;
	cEngine->mRevTime = 0;	

	mGame.mChannels[cEngine->mChan].mVol = cEngine->mVol;
	mGame.mChannels[cEngine->mChan].mType = 0x2;
	mGame.mChannels[cEngine->mChan].mFreqHi = (cEngine->mFreq & 0xff00) >> 8;
	mGame.mChannels[cEngine->mChan].mFreqLo = cEngine->mFreq & 0x00ff;	
	
	
	mGame.mChannels[cEngine->mChan + 1].mVol = cEngine->mVol;// >> 3;
	mGame.mChannels[cEngine->mChan + 1].mType = 0x0;
	mGame.mChannels[cEngine->mChan + 1].mPuls = 0x30;	
	mGame.mChannels[cEngine->mChan + 1].mFreqHi = (cEngine->mFreq & 0xff00) >> 8;
	mGame.mChannels[cEngine->mChan + 1].mFreqLo = cEngine->mFreq & 0x00ff;			
}

void stop_engine(struct SThrottle* cEngine){
	mGame.mChannels[cEngine->mChan].mVol = 0;
	mGame.mChannels[cEngine->mChan + 1].mVol = 0;
	cEngine->mOn = 0;	
}


void play_beep(int cbasefq, int clength){
	mGame.mCBeep.mVol = 0xff;
	mGame.mCBeep.mOn = 1;
	mGame.mCBeep.mFreq = cbasefq;// (cbasefq & 0xff00) >> 8;
	mGame.mCBeep.mSubFreq = cbasefq>>1; // (cbasefq & 0x00ff);	
	mGame.mCBeep.mLength = clength;
//	mGame.mCBeep.mLastCount = mGame.mFrameCount;
	
	mGame.mChannels[mGame.mCBeep.mChan].mVol = mGame.mCBeep.mVol;
	mGame.mChannels[mGame.mCBeep.mChan].mType = 0x0;			
	mGame.mChannels[mGame.mCBeep.mChan].mPuls = 0x20;			
	mGame.mChannels[mGame.mCBeep.mChan].mFreqHi = (mGame.mCBeep.mFreq & 0xff00) >> 8;
	mGame.mChannels[mGame.mCBeep.mChan].mFreqLo = (mGame.mCBeep.mFreq & 0x00ff);
 		
	mGame.mChannels[mGame.mCBeep.mChan + 1].mVol = mGame.mCBeep.mVol;
	mGame.mChannels[mGame.mCBeep.mChan + 1].mType = 0x0;			
	mGame.mChannels[mGame.mCBeep.mChan + 1].mPuls = 0x30;		
	mGame.mChannels[mGame.mCBeep.mChan + 1].mFreqHi = (mGame.mCBeep.mSubFreq & 0xff00) >> 8;
	mGame.mChannels[mGame.mCBeep.mChan + 1].mFreqLo = (mGame.mCBeep.mSubFreq & 0x00ff);
}



void play_crash(int cbasefq, int clength, int cdec){
	mGame.mCrash.mVol = 0xff;
	mGame.mCrash.mOn = 1;
	mGame.mCrash.mFreq = cbasefq;
	mGame.mCrash.mLength = clength;
	mGame.mCrash.mVDec = cdec;
	mGame.mCrash.mChan = 15;
	
	mGame.mChannels[mGame.mCrash.mChan].mVol = mGame.mCrash.mVol;
	mGame.mChannels[mGame.mCrash.mChan].mType = 0x1;		
	mGame.mChannels[mGame.mCrash.mChan].mFreqHi = (mGame.mCrash.mFreq & 0xff00) >> 8; 
	mGame.mChannels[mGame.mCrash.mChan].mFreqLo = mGame.mCrash.mFreq & 0x00ff;
	
	mGame.mChannels[mGame.mCrash.mChan - 1].mVol = mGame.mCrash.mVol;
	mGame.mChannels[mGame.mCrash.mChan - 1].mType = 0x3;		
	mGame.mChannels[mGame.mCrash.mChan - 1].mFreqHi = ((mGame.mCrash.mFreq << 1) & 0xff00) >> 8; 
	mGame.mChannels[mGame.mCrash.mChan - 1].mFreqLo = (mGame.mCrash.mFreq << 1) & 0x00ff;
}

void play_bump(int cbasefq, int clength, int cdec){
	mGame.mBump.mVol = 0xff;
	mGame.mBump.mOn = 1;
	mGame.mBump.mFreq = cbasefq;
	mGame.mBump.mLength = clength;
	mGame.mBump.mVDec = cdec;
	mGame.mBump.mChan = 13;
	
	mGame.mChannels[mGame.mBump.mChan].mVol = mGame.mBump.mVol;
	mGame.mChannels[mGame.mBump.mChan].mType = 0x1;		
	mGame.mChannels[mGame.mBump.mChan].mFreqHi = (mGame.mBump.mFreq & 0xff00) >> 8; 
	mGame.mChannels[mGame.mBump.mChan].mFreqLo = mGame.mBump.mFreq & 0x00ff;
	
	mGame.mChannels[mGame.mBump.mChan - 1].mVol = mGame.mBump.mVol;
	mGame.mChannels[mGame.mBump.mChan - 1].mType = 0x3;		
	mGame.mChannels[mGame.mBump.mChan - 1].mFreqHi = ((mGame.mBump.mFreq << 1) & 0xff00) >> 8; 
	mGame.mChannels[mGame.mBump.mChan - 1].mFreqLo = (mGame.mBump.mFreq << 1) & 0x00ff;
}


void play_click(int cbasefq, int clength, int cadd){


	mMenu.mMClick.mVol = 0xff;
	mMenu.mMClick.mOn = 1;
	mMenu.mMClick.mFreq = cbasefq; // (cbasefq & 0xff00) >> 8;
//	mMenu.mMClick.mFreqLo = (cbasefq & 0x00ff);	
	mMenu.mMClick.mLength = clength;
	mMenu.mMClick.mFInc = cadd;	
//	mMenu.mMClick.mLastCount = mGame.mFrameCount;
	
	mGame.mChannels[mMenu.mMClick.mChan].mVol = mMenu.mMClick.mVol;
	mGame.mChannels[mMenu.mMClick.mChan].mType = 0x1;		
	mGame.mChannels[mMenu.mMClick.mChan].mFreqHi = (mMenu.mMClick.mFreq & 0xff00) >> 8; 
	mGame.mChannels[mMenu.mMClick.mChan].mFreqLo = mMenu.mMClick.mFreq & 0x00ff;

	mGame.mChannels[mMenu.mMClick.mChan + 1].mVol = mMenu.mMClick.mVol;
	mGame.mChannels[mMenu.mMClick.mChan + 1].mType = 0;
	mGame.mChannels[mMenu.mMClick.mChan + 1].mPuls = 0x25;					
	mGame.mChannels[mMenu.mMClick.mChan + 1].mFreqHi = (mMenu.mMClick.mFreq & 0xff00) >> 8; 
	mGame.mChannels[mMenu.mMClick.mChan + 1].mFreqLo = mMenu.mMClick.mFreq & 0x00ff;

	mGame.mChannels[mMenu.mMClick.mChan + 2].mVol = mMenu.mMClick.mVol;
	mGame.mChannels[mMenu.mMClick.mChan + 2].mType = 0x3;		
	mGame.mChannels[mMenu.mMClick.mChan + 2].mFreqHi = (mMenu.mMClick.mFreq & 0xff00) >> 8; 
	mGame.mChannels[mMenu.mMClick.mChan + 2].mFreqLo = mMenu.mMClick.mFreq & 0x00ff;


/* 		
	mGame.mChannels[mGame.mCBeep.mChan - 1].mVol = mGame.mCBeep.mVol;
	mGame.mChannels[mGame.mCBeep.mChan - 1].mType = 0x3F;		
	mGame.mChannels[mGame.mCBeep.mChan - 1].mFreqHi = mGame.mCBeep.mFreqHi>>1;
	mGame.mChannels[mGame.mCBeep.mChan - 1].mFreqLo = mGame.mCBeep.mFreqLo>>1;	
*/
}

void process_sound(){
	
	if(mMenu.mMClick.mOn){

		mMenu.mMClick.mFreq -= mMenu.mMClick.mFInc;
		mGame.mChannels[mMenu.mMClick.mChan].mFreqHi = (mMenu.mMClick.mFreq & 0xff00) >> 8; 
		mGame.mChannels[mMenu.mMClick.mChan].mFreqLo = mMenu.mMClick.mFreq & 0x00ff;

		mGame.mChannels[mMenu.mMClick.mChan + 1].mFreqHi = (mMenu.mMClick.mFreq & 0xff00) >> 8; 
		mGame.mChannels[mMenu.mMClick.mChan + 1].mFreqLo = mMenu.mMClick.mFreq & 0x00ff;

		mGame.mChannels[mMenu.mMClick.mChan + 2].mFreqHi = (mMenu.mMClick.mFreq & 0xff00) >> 8; 
		mGame.mChannels[mMenu.mMClick.mChan + 2].mFreqLo = mMenu.mMClick.mFreq & 0x00ff;		
		
		mMenu.mMClick.mLength--;
		
		if(mMenu.mMClick.mLength < 1){
			mMenu.mMClick.mOn = 0;
			mMenu.mMClick.mVol = 0;
			mGame.mChannels[mMenu.mMClick.mChan].mVol = 0;
			mGame.mChannels[mMenu.mMClick.mChan + 1].mVol = 0;
			mGame.mChannels[mMenu.mMClick.mChan + 2].mVol = 0;									
		}
	
	
	}

	if(mGame.mCBeep.mOn){
		
		mGame.mCBeep.mLength--;
		
		if(mGame.mCBeep.mLength < 1){
			mGame.mCBeep.mOn = 0;
			mGame.mCBeep.mVol = 0;
			mGame.mChannels[mGame.mCBeep.mChan].mVol = 0;
			mGame.mChannels[mGame.mCBeep.mChan + 1].mVol = 0;			
		}
	}
	
	if(mGame.mCrash.mOn){
		mGame.mCrash.mLength--;
		
		mGame.mChannels[mGame.mCrash.mChan].mVol -= mGame.mCrash.mVDec;
		mGame.mChannels[mGame.mCrash.mChan - 1].mVol -= mGame.mCrash.mVDec;		
		
		if(mGame.mCrash.mLength < 1){
			mGame.mCrash.mOn = 0;
			mGame.mCrash.mVol = 0;
			mGame.mChannels[mGame.mCrash.mChan].mVol = 0;
			mGame.mChannels[mGame.mCrash.mChan - 1].mVol = 0;					
		}
	}
		
	if(mGame.mBump.mOn){
		mGame.mBump.mLength--;
		
		mGame.mChannels[mGame.mBump.mChan].mVol -= mGame.mBump.mVDec;
		mGame.mChannels[mGame.mBump.mChan - 1].mVol -= mGame.mBump.mVDec;		
		
		if(mGame.mBump.mLength < 1){
			mGame.mBump.mOn = 0;
			mGame.mBump.mVol = 0;
			mGame.mChannels[mGame.mBump.mChan].mVol = 0;
			mGame.mChannels[mGame.mBump.mChan - 1].mVol = 0;					
		}
	}
	
	if(mGame.Player1.mEngine.mOn){
		process_engine(&mGame.Player1.mEngine, mGame.Player1.mVel);
	}

	if(mGame.Player2.mEngine.mOn){
		process_engine(&mGame.Player2.mEngine, mGame.Player2.mVel);
	}

	if(mGame.Player3.mEngine.mOn){
		process_engine(&mGame.Player3.mEngine, mGame.Player3.mVel);
	}

	/*
	if(mGame.Player4.mEngine.mOn){
		process_engine(&mGame.Player4.mEngine, mGame.Player4.mVel);
	} */
	

}

void setCDSprite(struct PSprite* cMenu, int cChar, int mx, int my){

   cMenu->blocklo = SPRITE_BLOCKLO(VRAM_textcd + cChar);
   cMenu->blockhi = SPRITE_BLOCKHI(VRAM_textcd + cChar);   
   cMenu->mode = SPRITE_MODE_4BPP;
   cMenu->mPos.x = mx;
   cMenu->mPos.y = my;
   cMenu->z = SPRITE_LAYER_1;
   cMenu->flipx = 0;
   cMenu->flipy = 0;   
   cMenu->colmask = 0;
   cMenu->dimensions = SPRITE_64_BY_64;
   cMenu->palette_offset = 0;


}

void set_menucd(int cNR){

	switch(cNR){
		case 3:
			setCDSprite(&mGame.mCDown1, TEXTCD_3, 320-32, 140);
			break;
		case 2:
			setCDSprite(&mGame.mCDown1, TEXTCD_2, 320-32, 140);
			break;
		case 1:
			setCDSprite(&mGame.mCDown1, TEXTCD_1, 320-32, 140);
			break;
		case 0:
			setCDSprite(&mGame.mCDown1, TEXTCD_G, 320 - 64 + 8, 140);
			setCDSprite(&mGame.mCDown2, TEXTCD_O, 320 - 8, 140);			
			break;
	}
}

void render_cdmenu(){
	int cdelay = 0;
	
	start_engine(&mGame.Player1.mEngine, 2);
	start_engine(&mGame.Player2.mEngine, 4);
	start_engine(&mGame.Player3.mEngine, 6);
//	start_engine(&mGame.Player4.mEngine, 8);		
	
	while((mGame.bCountDown)){
	    		   
		if(cdelay < 1){
			mGame.bCountDown--;
			if(mGame.bCountDown == 0){
				play_beep(2000, mGame.mBeepLength * 3);
				set_menucd(mGame.bCountDown);
				mMenu.bDoUpdate = 2;
				//waitvsync();
				//load_sprite(&mGame.mCDown1, 4);
				//load_sprite(&mGame.mCDown2, 5);
			} else {
				play_beep(1000, mGame.mBeepLength);
				set_menucd(mGame.bCountDown);
				mMenu.bDoUpdate = 1;
				waitvsync();
				//load_sprite(&mGame.mCDown1, 4);
				cdelay = 30000;	
			}

		}
		cdelay--;
	}
	
	
	mGame.bCountDown = 0;
	mGame.bCountDelay = 70;
}

void init_game(){

    mGame.Joysticks[0] = &mGame.Joy1;
    mGame.Joysticks[1] = &mGame.Joy2;    
    mGame.Joysticks[2] = &mGame.Joy3;
    mGame.Joysticks[3] = &mGame.Joy4;
    
    mGame.mBots[0] = &mGame.Bot1;
    mGame.mBots[1] = &mGame.Bot2;
    mGame.mBots[2] = &mGame.Bot3;
//    mGame.mBots[3] = &mGame.Bot4;

    mGame.Players[0] = &mGame.Player1;
    mGame.Players[1] = &mGame.Player2;
    mGame.Players[2] = &mGame.Player3;
//    mGame.Players[3] = &mGame.Player4;
    
    mGame.PSprites[0] = &mGame.PSprite1;
    mGame.PSprites[1] = &mGame.PSprite2;
    mGame.PSprites[2] = &mGame.PSprite3;
//    mGame.PSprites[3] = &mGame.PSprite4;
    
    mGame.mCarPos[0] =  &mGame.CarPos1;
    mGame.mCarPos[1] =  &mGame.CarPos2;
    mGame.mCarPos[2] =  &mGame.CarPos3;
//    mGame.mCarPos[3] =  &mGame.CarPos4;
    
    mGame.mControls[0] = &mGame.Joy1;
    mGame.mControls[1] = &mGame.Joy2;
    mGame.mControls[2] = &mGame.Joy3;
    mGame.mControls[3] = &mGame.Joy4;
    mGame.mControls[4] = &mGame.KeyArrow;
    mGame.mControls[5] = &mGame.KeyWASD;                    
    mGame.mControls[6] = &mGame.Bot1;
    mGame.mControls[7] = &mGame.Bot2;
    mGame.mControls[8] = &mGame.Bot3;
 //   mGame.mControls[9] = &mGame.Bot4;
            
    reset_controls();
    clear_channels();
    
    mauien = 0;   
    PCMRATE = 0;
    
    mGame.mCBeep.mChan = 0;
    mGame.mCBeep.mOn = 0;
    mGame.mBeepLength = 15;
    
    mGame.bDoCol = 0;
    
    mGame.bIRQ = 0;
    
    mGame.bFrameReady = 0;
    
    mGame.bRunning = 1;
    mGame.bRacing = 0;
    mGame.bGamePaused = 0;
    //mGame.bCountDown = 0;
    
    mGame.mWinner = 0;
                
}

void mscol(void){
	mGame.bDoCol = 1;
}

void msline(void){}

void maflow(void){
	
   RAM_BANK = 1;
	
   while(!(PCMCTRL & 0x80)){

	PCMDATA = *sample_index;
	
	sample_index++;
	             
        if(sample_index == sample_max){
	     sample_index = sample_point;
        }
   }

   mPCM.bLoadSound = 12;             
}	

void mblank(void){

	char mL1, mL2;

	if(mMenu.bWinnerRunning){
		if(mMenu.bDoUpdate){			
			RAM_BANK = 1;
			
			load_lmenu(0);
			
			mMenu.mMenuItemUpdate = 0;
			mMenu.bDoUpdate = 0;
		}
	
	}


	if(mMenu.bRunning){
	
		if(mMenu.bDoUpdate){
		
			RAM_BANK = 1;
		   
			mL1 = mMenu.mMenuLoad1;
			mL2 = mMenu.mMenuLoad2;		
	
			load_lmenu(mL1);
			load_lmenu(mL2);	
		
			mMenu.mMenuItemUpdate = 0;	
			mMenu.bDoUpdate = 0;
		}
				
		process_sound();
		update_sound();		
	}


	if(mGame.bRacing){
	
		if(mGame.bCountDown){

			if(mMenu.bDoUpdate == 1){				
				load_sprite(&mGame.mCDown1, 4);
				mMenu.bDoUpdate = 0;
			}									
											
			process_sound();
			update_sound();
			
			check_quit();		
			
		} else if(mGame.bFrameReady){
		
			if(mMenu.bDoUpdate){
				if(mMenu.bDoUpdate == 2){				
					load_sprite(&mGame.mCDown1, 4);
					load_sprite(&mGame.mCDown2, 5);						
					mMenu.bDoUpdate = 0;
				}									
				
				if(mMenu.bDoUpdate == 3){							
					clear_sprite(4);
					clear_sprite(5);
					mMenu.bDoUpdate = 0;
				}
				
				if(mMenu.bDoUpdate == 4){							
				    load_sprite(&mGame.mLapsCount[4], 10);    
				    mMenu.bDoUpdate = 0;
				}
			}
		
			update_sound();
				
			mScrollX = mGame.mViewport.x - (320);
			mScrollY = mGame.mViewport.y - (240);
                       
		        VERA.layer0.hscroll = mScrollX;
        		VERA.layer0.vscroll = mScrollY;

		        VERA.layer1.hscroll = mScrollX;
        		VERA.layer1.vscroll = mScrollY;
				
			load_psprites();
			
			mGame.bFrameReady = 0;				
#ifdef MDEBUG		
			if(mGame.bDebug){
				load_debug();	
			}
#endif
		}
		
	}	
	

	if(mauien){
		RAM_BANK = 1;
		update_audio();
	}
}



void load_level(){

   VERA.control = 0x80;
   VERA.display.video = 0;
   reset_vera();
   VERA.control = 0x0;      
   
   RAM_BANK = 1; 
   
   //SETPATHNUM(mclmpath, mGame.mLevel, MCLMPOS);
   SETPATHNUM(mclmpath, 1, MCLMPOS);
      
   loadFile(mclmpath, (unsigned short)BANK_RAM);
      
   RAM_BANK = 1; 

   SETPATHNUM(mpalpath, mGame.mLevel, MPALPOS);
   
   loadVera(mpalpath, VRAM_palette, 3); 
   
   VERA.display.hscale = 128;
   VERA.display.vscale = 128;


   VERA.layer1.config = 0xA3;
   VERA.layer1.tilebase = (VRAM_tiles >> 9) +3;
   VERA.layer1.mapbase = (VRAM_layer1_map >> 9);

   VERA.layer1.hscroll = 0;
   VERA.layer1.vscroll = 0;

   VERA.layer0.config = 2;
  
   VERA.layer0.tilebase = (VRAM_tilesbg >> 9) +3;
   VERA.layer0.mapbase = (VRAM_layer0_map >> 9);
  
     
   VERA.layer0.hscroll = 0;
   VERA.layer0.vscroll = 0;
   
   //loadVera(mcarspath, VRAM_sprites, 3);
   loadVera(mcarspath, mGame.mCarsAddr, 3);      
   
   
   //loadVera(mboompath, VRAM_boom, 3);
   loadVera(mboompath, mGame.mBoomAddr, 3);
      
     
   SETPATHNUM(mtlstpath, 1, MTLSTPOS);
      
   loadVera(mtlstpath, VRAM_tiles , 2);
   

   SETPATHNUM(mtlsbgpath, mGame.mLevel, MTLSBGPOS);
   loadVera(mtlsbgpath, VRAM_tilesbg , 2);
           
   //loadVera(mtxcdpath, VRAM_textcd, 3);
   loadVera(mtxcdpath, mGame.mTXTCDAddr, 3);      
   
   //loadVera(mtxtrpath, VRAM_texttrk, 3);
   loadVera(mtxtrpath, mGame.mTXTLapsAddr, 3);      
      
   SETPATHNUM(mtrkpath, 1, MTRKPOS);
   loadVera(mtrkpath, VRAM_layer1_map, 2);
    
   SETPATHNUM(mmbgpath, 1, MMBGPOS);    
   loadVera(mmbgpath, VRAM_layer0_map, 2);

   VERA.display.video = 0x71;
   
   load_sprite(&mGame.PSprite1,1);
   load_sprite(&mGame.PSprite2,2);
   load_sprite(&mGame.PSprite3,3);
//   load_sprite(&mGame.PSprite4,4);			

    load_sprite(&mGame.mLapsCount[0], 6);
    load_sprite(&mGame.mLapsCount[1], 7);
    load_sprite(&mGame.mLapsCount[2], 8);
    load_sprite(&mGame.mLapsCount[3], 9);
    load_sprite(&mGame.mLapsCount[4], 10);    
}

char getPlItem(int cPl, int nIdx, int cDir){
	int nItem;
	int pi;
	int used;
	
	nItem = nIdx;
	
	if(nItem < 0){
		nItem = 6;
	}
		
	if(nItem > 6){
		nItem = 0;
	}
	
	if(nItem == 0){
		return 0;
	}

	
	do{
		used = 0;
		for(pi = 0; pi < 4; pi++){
			if(pi != cPl){
				if(nItem == mMenu.mPCtrl[pi]){
					used = 1;
				}
			}
		}
		
		if(used){
			nItem += cDir;
		}
				
		if(nItem < 0){
			nItem = 6;
		}
		
		if(nItem > 6){
			nItem = 0;
		}
		
		if(nItem == 0){
			used = 0;
		}

		
	} while(used);

	return (char)nItem;

}

char process_input_menu(){

	if(mMenu.mMenuControl){
		mMenu.mMenuControl--;
		return 0;
	}
	
	if(getkey(KEY_UP) > 0){
  	   clearkey(KEY_UP);		
	   mMenu.mMenuControl = MENUDEL; 
	   return CKEY_UP;
	}

	if(getkey(KEY_DOWN) > 0){
  	   clearkey(KEY_DOWN);				
	   mMenu.mMenuControl = MENUDEL; 	
	   return CKEY_DOWN;
	}

	if(getkey(KEY_RIGHT) > 0){
  	   clearkey(KEY_RIGHT);		
	   mMenu.mMenuControl = MENUDEL; 	
	   return CKEY_RIGHT;
	}

	if(getkey(KEY_LEFT) > 0){
  	   clearkey(KEY_LEFT);		
	   mMenu.mMenuControl = MENUDEL; 	
	   return CKEY_LEFT;
	}
	
	if(getkey(KEY_ENTER) > 0){
	   mMenu.mMenuControl = MENUDEL; 	
	   return CKEY_ENTER;
	}
		
	return 0;
}

void init_audio(){
	
	
	PCMCTRL = mPCM.mCtrl;
	//PCMDATA = (char)0x0;			
	//mauien = 1;
}

void start_audio(){
	RAM_BANK = 1;
	PCMRATE = 32;
	mauien = 1;				
}

void stop_audio(){
	
	//PCMCTRL=0x80;
	mauien = 0;		
	PCMRATE = 0;	
}

void open_audio(){
		
	SETPATHNUM(msndpath, mPCM.mStream, MSNDPOS);			
	
	/*
	cbm_k_setnam(msndpath);
	cbm_k_setlfs(mPCM.mFile,8,0);
	cbm_k_open();
	
	cbm_k_ckout(mPCM.mFile);
	*/
	
	openFile(msndpath, mPCM.mFile);
}

void close_audio(){
	cbm_k_close(mPCM.mFile);	
}


void reload_audio(){
	
	stop_audio();

	close_audio();

	open_audio();
	
	init_audio();
	
	start_audio();
	
	mPCM.mFrames = 0;
	mPCM.mChunks = 0;
}

void load_audio(char cstream){
	char bi;
	
	mPCM.bLoadSound = 0;	
	
	mPCM.mStream = cstream;
	
	if(mPCM.mStream == 1){
		mPCM.mCtrl = MPCMCTRL1;
		mPCM.mMaxFile = MPCMMAX1;		
	}
	
	if(mPCM.mStream == 2){
		mPCM.mCtrl = MPCMCTRL2;
		mPCM.mMaxFile = MPCMMAX2;		
	}
	
	mPCM.mFile = 2;
	
	open_audio();
		
	PCMCTRL = 0x80;	
	
	for(bi = 0; bi < 16; bi++){	
		__asm__("lda #255");
       		__asm__("ldx #$3d");
		__asm__("ldy #$9f");
		__asm__("sec");
		__asm__("jsr $ff44");		      						       	
	}
	

	while(!(PCMCTRL & 0x80)){
		sample_load = cbm_k_acptr();
        	//__asm__("lda %v",sample_load); // sample_load is already in Reg A
        	__asm__("sta $9f3d");
	}
	
	RAM_BANK = 1;	
		       			
	load_index = sample_point; 
	sample_index = sample_point; 
	sample_max = sample_point + 4096; 

	for(bi = 0; bi < 16; bi++){	
		mLo = (unsigned char)((unsigned short)load_index & 0x00ff);
		mHi = (unsigned char)((unsigned short)load_index >> 8);	

		RAM_BANK = 1;
	
		__asm__("lda #255");
       		__asm__("ldx _mLo");
		__asm__("ldy _mHi");
		__asm__("jsr $ff44");
		      						       	
		load_index += 255;
	}
	
/*
	mLo = (unsigned char)((unsigned short)load_index & 0x00ff);
	mHi = (unsigned char)((unsigned short)load_index >> 8);	

	RAM_BANK = 1;

	//Causes Pop	
	__asm__("lda #16");
	__asm__("ldx _mLo");
	__asm__("ldy _mHi");
	__asm__("jsr $ff44");

*/
	RAM_BANK = 1;

	//bi = 0;
		
	while(load_index < sample_max){
	//while(bi < 16){
		*load_index = cbm_k_acptr();
		load_index++;
	//	bi++;
	}
		
	load_index = sample_point; 
	
	mPCM.mFrames = 0;
	mPCM.mChunks = 0;
}

void update_audio(){

	if(mPCM.bLoadSound){ //bLoadSound is set to 12 after AFLOW interrupt
		
		if(load_index <= (sample_max - 255)){
	
			mLo = (unsigned char)((unsigned short)load_index & 0x00ff);
			mHi = (unsigned char)((unsigned short)load_index >> 8);	

			RAM_BANK = 1;
	
		       	__asm__("lda #255");
       			__asm__("ldx _mLo");
		       	__asm__("ldy _mHi");
		       	__asm__("jsr $ff44");
		       						       	
		       	load_index += 255;
		       	
		       	if(load_index == sample_max){

		       		load_index = sample_point;		       				       		
		       		
		       	} else if( ((unsigned short)sample_max - (unsigned short)load_index) <= 12){

				RAM_BANK = 1;		       			       		

		       		while(load_index < sample_max){
					*load_index = cbm_k_acptr();
					load_index++;
		
				}				

				load_index = sample_point;
		       	}
       	
	       	} else {
       		
       			mLoad = (unsigned char)((unsigned short)sample_max - (unsigned short)load_index);
       	
			mLo = (unsigned char)((unsigned short)load_index & 0x00ff);
			mHi = (unsigned char)((unsigned short)load_index >> 8);	

			RAM_BANK = 1;

		       	__asm__("lda _mLoad");
       			__asm__("ldx _mLo");
		       	__asm__("ldy _mHi");
		       	__asm__("jsr $ff44");
	       	
			load_index = sample_point;
		
			mLoad = (255 - mLoad);
       		
			mLo = (unsigned char)((unsigned short)load_index & 0x00ff);
			mHi = (unsigned char)((unsigned short)load_index >> 8);	

			RAM_BANK = 1;
		
		       	__asm__("lda _mLoad");
       			__asm__("ldx _mLo");
		       	__asm__("ldy _mHi");
		       	__asm__("jsr $ff44");
	       	
		       	load_index += mLoad;
				       		       	
       		}
       	
		mPCM.bLoadSound--;
	
		if(mPCM.bLoadSound == 0){
		
			RAM_BANK = 1;
		
			while(load_index != sample_index){

				*load_index = cbm_k_acptr();
				load_index++;
		
				if(load_index == sample_max){
					load_index = sample_point;
				}		
			}			
		}	       
       	}
       	
       	mPCM.mFrames++;
       	
       	if(mPCM.mFrames == 60){       	
	       	mPCM.mFrames = 0;
		mPCM.mChunks++;
		
		if(mPCM.mChunks == mPCM.mMaxFile){
			reload_audio();
		}
	}
}

void render_menu(){
    int mdelay = 5;
    
    mMenu.mMenuItem = 0;
    mMenu.mMenuItemLast = 0;
    
    mMenu.mMenuItemUpdate = 0;
    mMenu.bDoUpdate = 0;
    
    mMenu.mMenuControl = 0;
            
    mMenu.mMenuLoad1 = 0;
    mMenu.mMenuLoad2 = 0;            
    
    mMenu.bRunning = 1;    
            
    while(mMenu.bShowMenu){
    	
	mdelay--;
    	
    	if(mMenu.mMenuItemUpdate == 0){
    	    	    	
	RAM_BANK = 1;    	    	    	
    	    	    	
    	switch(process_input_menu()){
    		case CKEY_ENTER:
    			if(mMenu.mMenuItem == 6){
    				mMenu.bShowMenu = 0;
    				mGame.bRacing = 1;
    			}
    			if(mMenu.mMenuItem == 7){
	    			mMenu.bShowMenu = 0;
	    			mGame.bRunning = 0;
    			}

    			break;
    		case CKEY_UP:
	    		play_click(2000, mMenu.mClickLength, 500);
	    		setMenuOffset(mMenu.mMenuItem, 15);
	    		mMenu.mMenuItemLast = mMenu.mMenuItem;

    			mMenu.mMenuItem--;
    			    		
    			if(mMenu.mMenuItem < 0){
		    		mMenu.mMenuItem = 7;
    			}
    			mMenu.mMenuItemUpdate |= 2;
    			mdelay = 0;
    			//waitvsync();
    			//load_lmenu(mMenu.mMenuItemLast);
    			break;   		
    		case CKEY_DOWN:
	    		play_click(2000, mMenu.mClickLength, 500);
	    		setMenuOffset(mMenu.mMenuItem, 15);
	    		mMenu.mMenuItemLast = mMenu.mMenuItem;
         		
    			mMenu.mMenuItem++;
    			    	
    			if(mMenu.mMenuItem > 7){
		    		mMenu.mMenuItem = 0;
    			}
    			mMenu.mMenuItemUpdate |= 2;
    			mdelay = 0;
    			//waitvsync();
    			//load_lmenu(mMenu.mMenuItemLast);
    			break;   		
    		case CKEY_LEFT:
    			if(mMenu.mMenuItem < 3){
  				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mPCtrl[mMenu.mMenuItem] = getPlItem(mMenu.mMenuItem, mMenu.mPCtrl[mMenu.mMenuItem] - 1, -1);
    				setPlCtrl(mMenu.mMenuItem, mMenu.mPCtrl[mMenu.mMenuItem]);
    				mMenu.mMenuItemUpdate |= 1;	
    			}

    			if(mMenu.mMenuItem == 3){
    				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mAISpeed--;
    				if(mMenu.mAISpeed < 1){
	    				mMenu.mAISpeed = 5;
    				}
    				setTextSprite(&mMenu.mMenuAI, mMenu.mNumbers[mMenu.mAISpeed], MAILEVELPOS,97);
    				mMenu.mMenuItemUpdate |= 1;	
    			}


    			if(mMenu.mMenuItem == 4){
    				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mLevel--;
    				if(mMenu.mLevel < 1){
	    				mMenu.mLevel = mGame.mLevels;
    				}
    				setTextSprite(&mMenu.mMenuLVL, mMenu.mNumbers[mMenu.mLevel], 286,67);
    				mMenu.mMenuItemUpdate |= 1;	
    			}
   			if(mMenu.mMenuItem == 5){
   				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mLaps--;
    				if(mMenu.mLaps < 1){
	    				mMenu.mLaps = 5;
    				}
    				setTextSprite(&mMenu.mMenuLABS, mMenu.mNumbers[mMenu.mLaps], 278,77);
    				mMenu.mMenuItemUpdate |= 1;	
    			}    	
    			break ;   		
    		case CKEY_RIGHT:
    		    	if(mMenu.mMenuItem < 3){
    		    		play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mPCtrl[mMenu.mMenuItem] = getPlItem(mMenu.mMenuItem, mMenu.mPCtrl[mMenu.mMenuItem] + 1, +1);
    				setPlCtrl(mMenu.mMenuItem, mMenu.mPCtrl[mMenu.mMenuItem]);
    				mMenu.mMenuItemUpdate |= 1;	
    			}
    			
    			if(mMenu.mMenuItem == 3){
    				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mAISpeed++;
    				if(mMenu.mAISpeed > 5){
	    				mMenu.mAISpeed = 1;
    				}
    				setTextSprite(&mMenu.mMenuAI, mMenu.mNumbers[mMenu.mAISpeed], MAILEVELPOS,97);
    				mMenu.mMenuItemUpdate |= 1;	
    			}
    			
    			if(mMenu.mMenuItem == 4){
    				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mLevel++;
    				if(mMenu.mLevel > mGame.mLevels){
	    				mMenu.mLevel = 1;
    				}
    				setTextSprite(&mMenu.mMenuLVL, mMenu.mNumbers[mMenu.mLevel], 286,67);
    				mMenu.mMenuItemUpdate |= 1;	
    			}    		
    			if(mMenu.mMenuItem == 5){
    				play_click(1500, mMenu.mClickLengthShort, 800);
    				mMenu.mLaps++;
    				if(mMenu.mLaps > 5){
	    				mMenu.mLaps = 1;
    				}
    				setTextSprite(&mMenu.mMenuLABS, mMenu.mNumbers[mMenu.mLaps], 278,77);
    				mMenu.mMenuItemUpdate |= 1;	
    			}   
    			break;  		
    		default: 	
	    		break;
    	}
    	    	
    	    	
    	if(mdelay < 1){    
    		mMenu.mSelected++;
    		if(mMenu.mSelected > 5){
    			mMenu.mSelected = 0;
    		}
    		
	        setMenuOffset(mMenu.mMenuItem, mMenu.mSelectCol[mMenu.mSelected]);	        
	        mMenu.mMenuItemUpdate |= 1;
	        mdelay = 5;
      	}    
        

       mMenu.mMenuLoad1 = mMenu.mMenuItemLast;
       mMenu.mMenuLoad2 = mMenu.mMenuItem;
       
       if(mMenu.mMenuItemUpdate){
		mMenu.bDoUpdate = 1;       
       }

       waitvsync();
       
       }
        
    }
    
    mMenu.bShowMenu = 1;
    mMenu.bRunning = 0;
}

void load_menu(){
   int oi;

   VERA.control = 0x80;	
   VERA.display.video = 0;
   reset_vera();
   VERA.control = 0x0;      
   
   init_menulines();
    
   loadVera(mpalipath, VRAM_palette, 3); 

   VERA.layer0.hscroll = 0;
   VERA.layer0.vscroll = 0;

   VERA.layer1.hscroll = 0;
   VERA.layer1.vscroll = 0;

   
   VERA.display.hscale = 64;
   VERA.display.vscale = 64;


   VERA.layer0.config = 0;
   VERA.layer1.config = 7;
  
   loadVera(mintropath, VRAM_intro, 2);
    
   loadVera(mtxintpath, VRAM_texti, 3);
   
   VERA.layer1.tilebase = (VRAM_intro >> 9);// +3;
          
   for(oi = 0; oi < 8; oi++){    
   	setMenuOffset(oi, 15);    	
        load_lmenu(oi);
   }
   
   load_audio(1);
    
   VERA.display.video = 0x61;            
}

void render_wmenu(){
	int wquit;
	int wkey;
	int wi;
	int pi;
	int pdel;

	wquit = 1;
	pdel = 10;
	
        mMenu.bWinnerRunning = 1;
        mMenu.mMenuItemUpdate = 0;
        mMenu.bDoUpdate = 0;
	
	while(wquit){
	
	   pdel--;
		
	   if(mMenu.mMenuItemUpdate == 0){

		pi = mGame.mWText.mPIndex;
		
		RAM_BANK = 1;
		
		//waitvsync();
		//pdel--;
			
		if(pdel < 1){
			for(wi = 0; wi < 7; wi++){
				mMenu.mMenuLines[0].mChar[wi]->palette_offset = mGame.mWText.mPOffset[pi];
			pi++;
			if(pi > 13){
				pi = 0;
			}

			//load_sprite(&mGame.mWText.mChars[wi], wi+ 1);
			}
			pdel = 10;
			mMenu.mMenuItemUpdate = 1;
		}
		
		mGame.mWText.mPIndex++;

		if(mGame.mWText.mPIndex > 13){
			mGame.mWText.mPIndex = 0;
		}
		
		//waitvsync();
		
		wkey = getanykey();
		
		//mMenu.mMenuControl = 0;
		
		if(wkey > 0){
			wquit = 0;
		}
		
		if(mMenu.mMenuItemUpdate){
			mMenu.bDoUpdate = 1;
		}
	
	     }
		
	     waitvsync();
	
	   
	}
	
        mMenu.bWinnerRunning = 0;
	
	   VERA.display.video = 0x0;
	
}

void load_wmenu(){

   int pchar;

   VERA.control = 0x80;	
   VERA.display.video = 0;
   reset_vera();
   VERA.control = 0x0;      
   
   SETPATHNUM(mwpalpath, mGame.mWinner, MWPALPOS);

   loadVera(mwpalpath, VRAM_palette, 3);    
   
   switch(mGame.mWinner){
	case 1:
		pchar = TEXTW_1;  
		break;
	case 2:
		pchar = TEXTW_2; 
		break;
	case 3:
		pchar = TEXTW_3; 
		break;
	default:
		pchar = TEXTW_1; 
		break;
   };
   
   VERA.layer0.hscroll = 0;
   VERA.layer0.vscroll = 0;

   VERA.layer1.hscroll = 0;
   VERA.layer1.vscroll = 0;

   
   VERA.display.hscale = 64;
   VERA.display.vscale = 64;

   VERA.layer0.config = 0;
   VERA.layer1.config = 7;
  
   loadVera(mwinpath, VRAM_intro, 2);
    
   loadVera(mtxwnpath, VRAM_texti, 3);
   
   VERA.layer1.tilebase = (VRAM_intro >> 9);// +3;
   
   load_audio(2);
       	
   init_wtext(&mGame.mWText, pchar);
   
   VERA.display.video = 0x61;		     
}

#ifdef MDEBUG   

void init_debug(){
	char di;

	for(di = 0; di < 12; di++){
		mGame.mDebug[di].blocklo = mGame.mBoomLo; //SPRITE_BLOCKLO(VRAM_sprites);
   		mGame.mDebug[di].blockhi = mGame.mBoomHi; // SPRITE_BLOCKHI(VRAM_sprites);   
		mGame.mDebug[di].mode = SPRITE_MODE_4BPP;
		mGame.mDebug[di].z = SPRITE_LAYER_1;
		mGame.mDebug[di].colmask = 0x0;
		mGame.mDebug[di].dimensions = SPRITE_32_BY_32;
		mGame.mDebug[di].palette_offset = 10;
		mGame.mDebug[di].flipx = 0;
		mGame.mDebug[di].flipy = 0;
	}
	

	//process_debug();

}

void process_debug(){
	char di;
		
	for(di = 0; di < 12; di++){
		mGame.mDebug[di].mPos.x = (320-16) + (mGame.mWaypoints[di].x - mGame.mViewport.x);
		mGame.mDebug[di].mPos.y = (240-16) + (mGame.mWaypoints[di].y - mGame.mViewport.y);
		
		if( (mGame.mDebug[di].mPos.x > 0) && (mGame.mDebug[di].mPos.x < (640)) &&(mGame.mDebug[di].mPos.y > 0) && (mGame.mDebug[di].mPos.y < (480))  ){
			mGame.mDebug[di].z = SPRITE_LAYER_1;
		} else {
			mGame.mDebug[di].z = 0;
		}
		
	}	
}

void load_debug(){
	char di;
		
	for(di = 0; di < 12; di++){
		load_sprite(&mGame.mDebug[di], di+7);			
	}
}

#endif

void setup_race(){
   char cplc, botc;
   
   RANDINIT();

   mGame.PSprite1.blocklo = mGame.mCarsLo; //SPRITE_BLOCKLO(VRAM_sprites);
   mGame.PSprite1.blockhi = mGame.mCarsHi; // SPRITE_BLOCKHI(VRAM_sprites);   
   mGame.PSprite1.mode = SPRITE_MODE_4BPP;
   mGame.PSprite1.mPos.x = 320 - 16;// - 16;//playerX - 16;//* (playerX);
   mGame.PSprite1.mPos.y = 240 - 16;//playerY - 16; // * (playerY);
   mGame.PSprite1.z = SPRITE_LAYER_1;
   mGame.PSprite1.colmask = 0x80;
   mGame.PSprite1.dimensions = SPRITE_32_BY_32;
   mGame.PSprite1.palette_offset = 14;
   mGame.PSprite1.flipx = 0;
   mGame.PSprite1.flipy = 0;

   mGame.PSprite2.blocklo = mGame.mCarsLo; //SPRITE_BLOCKLO(VRAM_sprites);
   mGame.PSprite2.blockhi = mGame.mCarsHi; // SPRITE_BLOCKHI(VRAM_sprites);   
   mGame.PSprite2.mode = SPRITE_MODE_4BPP;
   mGame.PSprite2.mPos.x = 320 - 16 + 32;//playerX - 16;//* (playerX);
   mGame.PSprite2.mPos.y = 240 - 16;//playerY - 16; // * (playerY);
   mGame.PSprite2.z = SPRITE_LAYER_1;
   mGame.PSprite2.colmask = 0x90;
   mGame.PSprite2.dimensions = SPRITE_32_BY_32;
   mGame.PSprite2.palette_offset = 13;
   mGame.PSprite2.flipx = 0;
   mGame.PSprite2.flipy = 0;
   

   mGame.PSprite3.blocklo = mGame.mCarsLo; //SPRITE_BLOCKLO(VRAM_sprites);
   mGame.PSprite3.blockhi = mGame.mCarsHi; // SPRITE_BLOCKHI(VRAM_sprites);   
   mGame.PSprite3.mode = SPRITE_MODE_4BPP;
   mGame.PSprite3.mPos.x = 320 - 16 - 32;//+ 16 + 32;//playerX - 16;//* (playerX);
   mGame.PSprite3.mPos.y = 240 - 16;//playerY - 16; // * (playerY);
   mGame.PSprite3.z = SPRITE_LAYER_1;
   mGame.PSprite3.colmask = 0xA0;
   mGame.PSprite3.dimensions = SPRITE_32_BY_32;
   mGame.PSprite3.palette_offset = 15;
   mGame.PSprite3.flipx = 0;
   mGame.PSprite3.flipy = 0;

/*
   mGame.PSprite4.blocklo = mGame.mCarsLo; //SPRITE_BLOCKLO(VRAM_sprites);
   mGame.PSprite4.blockhi = mGame.mCarsHi; // SPRITE_BLOCKHI(VRAM_sprites);   
   mGame.PSprite4.mode = SPRITE_MODE_4BPP;
   mGame.PSprite4.mPos.x = 320 - 16 - 16 - 32;//playerX - 16;
   mGame.PSprite4.mPos.y = 240 - 16;//playerY - 16; 
   mGame.PSprite4.z = SPRITE_LAYER_1;
   mGame.PSprite4.colmask = 0xC0;
   mGame.PSprite4.dimensions = SPRITE_32_BY_32;
   mGame.PSprite4.palette_offset = 15;
   mGame.PSprite4.flipx = 0;
   mGame.PSprite4.flipy = 0;
*/

   mGame.mLaps = mMenu.mLaps;
   mGame.mLevel = mMenu.mLevel;
   
       set_text_sprite(&mGame.mLapsCount[0], mGame.mTXTLapsAddrShort, mGame.mTXTLapsLo, SPRITE_32_BY_32, 0, TEXTT_L, MLAPSCOUNTX, MLAPSCOUNTY);
    set_text_sprite(&mGame.mLapsCount[1], mGame.mTXTLapsAddrShort, mGame.mTXTLapsLo, SPRITE_32_BY_32, 0, TEXTT_A, MLAPSCOUNTX + MLAPSCOUNTSPACE , MLAPSCOUNTY);
    set_text_sprite(&mGame.mLapsCount[2], mGame.mTXTLapsAddrShort, mGame.mTXTLapsLo, SPRITE_32_BY_32, 0,  TEXTT_P, MLAPSCOUNTX + (MLAPSCOUNTSPACE * 2), MLAPSCOUNTY);
    set_text_sprite(&mGame.mLapsCount[3], mGame.mTXTLapsAddrShort, mGame.mTXTLapsLo, SPRITE_32_BY_32, 0,  TEXTT_S, MLAPSCOUNTX + (MLAPSCOUNTSPACE * 3), MLAPSCOUNTY);
   
   set_text_sprite(&mGame.mLapsCount[4], mGame.mTXTLapsAddrShort, mGame.mTXTLapsLo, SPRITE_32_BY_32, 0, (mGame.mLaps - 1) * TEXTISIZET, MLAPSCOUNTX + (MLAPSCOUNTSPACE >> 1) + (MLAPSCOUNTSPACE * 4), MLAPSCOUNTY);
   
   calc_way();
   
#ifdef MDEBUG      

   mGame.mLevel = 1;
   
   if(mMenu.mLevel == 2){
   	mGame.bDebug = 1;
   	init_debug();
   } else {
      	mGame.bDebug = 0;
   }
   
#endif   
   
   mGame.mPlace = 1;
   mGame.mPCount = 3;
   mGame.mFinCount = 0;
   mGame.mWinner = 0;
   
   mGame.bGamePaused = 0;
   
   switch(mMenu.mAISpeed){
   	case 1:
   		mGame.mAISpeed = MAILEVEL1; //-12;
   		break;
   	case 2:
   		mGame.mAISpeed = MAILEVEL2 ;//-8;
   		break;
   	case 3:
   		mGame.mAISpeed = MAILEVEL3; //0;
   		break;
   	case 4:
   		mGame.mAISpeed = MAILEVEL4; //3;
   		break;
   	case 5:
   		mGame.mAISpeed = MAILEVEL5; //6
   		break;   
   };
      
   mGame.bMoveToLead = 0;
       
   mGame.StartPos.x = mGame.mWaypoints[0].x;// - (320-16);
   mGame.StartPos.y = mGame.mWaypoints[0].y;// - (240-16);

   mGame.CarPos1.x = mGame.StartPos.x;// - 16;
   mGame.CarPos1.y = mGame.StartPos.y;

   mGame.CarPos2.x = mGame.StartPos.x + 32;//16;
   mGame.CarPos2.y = mGame.StartPos.y;

   mGame.CarPos3.x = mGame.StartPos.x - 32;//+ 16 + 32;
   mGame.CarPos3.y = mGame.StartPos.y;

//   mGame.CarPos4.x = mGame.StartPos.x - 16 - 32;
//   mGame.CarPos4.y = mGame.StartPos.y;

   reset_controls();

   botc = 0;

   for(cplc = 0; cplc < 3; cplc++){
	mGame.PPlaces[cplc] = &mGame.Players[cplc]->PPlace;
	switch(mMenu.mPCtrl[cplc]){
		case 0:
		    set_player(cplc, mGame.Players[cplc], mGame.mBots[botc], mGame.mCarPos[cplc], 0, 1);
		    botc++;
		break;
		case 1:
		    set_player(cplc, mGame.Players[cplc], &mGame.Joy1, mGame.mCarPos[cplc], 1, 0);
		break;
		case 2:
		    set_player(cplc, mGame.Players[cplc], &mGame.Joy2, mGame.mCarPos[cplc], 1, 0);
		break;
		case 3:
		    set_player(cplc, mGame.Players[cplc], &mGame.Joy3, mGame.mCarPos[cplc], 1, 0);
		break;
		case 4:
		    set_player(cplc, mGame.Players[cplc], &mGame.Joy4, mGame.mCarPos[cplc], 1, 0);
		break;
		case 5:
		    set_player(cplc, mGame.Players[cplc], &mGame.KeyArrow, mGame.mCarPos[cplc], 0, 0);
		break;
		case 6:
		    set_player(cplc, mGame.Players[cplc], &mGame.KeyWASD, mGame.mCarPos[cplc], 0, 0);
		break;


	};
   }
   
   mGame.mLeader = mGame.Players[0];
   
   mMenu.bDoUpdate = 0;
   
   mGame.bFrameReady = 0;
   
   mGame.bCountDown = 4;
}

void main(void) {

   load_game();
   init_menu(); 
   init_game();  
  
   install_irq();      
   set_keyboard_irq();
   
   VERA.irq_enable = 0;
  
   while(mGame.bRunning){
        	

	load_menu();
				
	init_audio();

	//start_audio();        
	
        VERA.irq_enable = 0x1 | 0x8;        
	
	start_audio();        

   	render_menu();
   	
   	if(mGame.bRunning){
		
		VERA.display.video = 0;
   
		clear_sprites(1,70);
		
	   	stop_audio();   
	   	close_audio();     
	   	
	        VERA.irq_enable = 0;
	        		     	        
		mScrollX = 0;
		mScrollY = 0;

 		setup_race();
 		load_level();
   		 
    		VERA.irq_enable = 0x1 | 0x4;
				
		mGame.bIRQ = 1;
		
	    	calc_viewport(); 
	    	
		process_sprites();
	    	
	    	mScrollX = mGame.mViewport.x - (320);
		mScrollY = mGame.mViewport.y - (240);
	    	
	    	VERA.layer0.hscroll = mScrollX;
        	VERA.layer0.vscroll = mScrollY;

	        VERA.layer1.hscroll = mScrollX;
        	VERA.layer1.vscroll = mScrollY;        	

		render_cdmenu();
  	  }
    
    

	while(mGame.bRacing){
	
		//waitvsync();
		
		if(mGame.bCountDelay){
			mGame.bCountDelay--;
			if(mGame.bCountDelay < 1){
				mMenu.bDoUpdate = 3;
			}
		}
				
   		process_input();

		process_bots();
		process_players();	
		 
		clear_controls();	 

		process_physics();
		
		process_sound();

		process_collision();
		
	    	calc_viewport();    
		
		process_sprites();
		
		if(mGame.bRacing){
			mGame.bFrameReady = 1;
		}
		    
		waitvsync();
		
		while(mGame.bGamePaused){

		   if(mGame.bFrameReady == 0){
		   	stop_sound();
		   }
		   
		   if(getkey(KEY_ENTER) > 0){
       			mGame.bGamePaused = 0;
      			mGame.bRacing = 0;	  
		   }
		   
		   if(getkey(KEY_ESC) > 0){
			clearkey(KEY_ESC);
       			mGame.bGamePaused = 0;	  
		   }
		   
		   check_quit();

		}
           		 		 
		if(mGame.mFinCount){
			mGame.mFinCount--;
 			if(mGame.mFinCount < 1){
 				stop_engine(&mGame.Player1.mEngine);
				stop_engine(&mGame.Player2.mEngine);
				stop_engine(&mGame.Player3.mEngine);
				//stop_engine(&mGame.Player4.mEngine);			

 				mGame.bRacing = 0;
 			}
		}
	}
	
	if(mGame.bIRQ){
	
		waitvsync();
	
		VERA.irq_enable = 0;
		VERA.display.video = 0;

		clear_sprites(1,10);
		
		clear_channels();
		
		mGame.bCountDelay = 0;		
		
	        //VERA.irq_enable = 1;
	        mGame.bIRQ = 0;
	        
	        if(mGame.mWinner){
	        
        	        VERA.irq_enable = 0;
	        		
	        	load_wmenu();
	        	
        		init_audio();
        		//start_audio();	
	        	       		        
       		        VERA.irq_enable = 0x1 | 0x8;        
        		
        		start_audio();	
       		        
       		        render_wmenu();
       		        
        	   	stop_audio();   
        	   	close_audio();  
        	   	   
		        VERA.irq_enable = 0;       		       	       
	        }
        }

   	
   }

   VERA.irq_enable = 0;
   rm_keyboard_irq();  
   remove_irq();      

   VERA.control = 0x80;
   __asm__("jsr %w",IOINIT);
}
