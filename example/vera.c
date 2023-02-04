#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include <joystick.h>
#include <cx16.h>
#include <cbm.h>
#include <6502.h>

// VRAM addresses
#define VRAM_palette 	   0x1FA00
#define VRAM_layer0_map    0x00000
#define VRAM_layer1_map    0x00800
#define VRAM_tiles         0x07000

#define IOINIT 0xFF81

#define  QUIT_Q		0x51

void loadVera(char *fname, unsigned int address, int flag){
   cbm_k_setnam(fname);
   cbm_k_setlfs(1,8,0);
   cbm_k_load(flag, address);    
}

void main(void) {

   	int i = 0;
   	int isdone = 0;
   	char * loadptr = (char *)0x4000;

   	cbm_k_setnam("pal.bin");
   	cbm_k_setlfs(1,8,0);
   	cbm_k_load(0, loadptr);    

   	VERA.address = VRAM_palette;
   	VERA.address_hi = 0x10 | (VRAM_palette >> 16);

   	for(i = 0; i < 512; i+=2){
		VERA.data0 = loadptr[i];
	   	VERA.data0 = loadptr[i+1];
   	}
    
   	VERA.display.video = 0;
   
   	VERA.display.hscale = 128;
   	VERA.display.vscale = 128;

   	VERA.layer1.config = 3;
   	VERA.layer1.tilebase = (VRAM_tiles >> 9) +3;
   	VERA.layer1.mapbase = (VRAM_layer0_map >> 9);

   	VERA.layer1.hscroll = 0;
   	VERA.layer1.vscroll = 0;

   	VERA.layer0.config = 3;
  
   	VERA.layer0.tilebase = (VRAM_tiles >> 9) +3;
   	VERA.layer0.mapbase = (VRAM_layer1_map >> 9);
    
   	VERA.layer0.hscroll = 0;
   	VERA.layer0.vscroll = 0;

   	loadVera("tiles0.bin", VRAM_tiles , 2);
   	loadVera("tiles1.bin", VRAM_tiles+4352,  2);

   	VERA.control = 0;

   	VERA.address = VRAM_layer1_map;
   	VERA.address_hi = 0x10 | (VRAM_layer1_map >> 16);

   	cbm_k_setnam("map0.bin");
   	cbm_k_setlfs(1,8,0);
   	cbm_k_load(0, loadptr);

   	for(i = 0;i < 1024*2; i+=2){
        	VERA.data0 = loadptr[i];
                VERA.data0 = loadptr[i+1];
   	}

   	VERA.address = VRAM_layer0_map;
   	VERA.address_hi = 0x10 | (VRAM_layer0_map >> 16);

   	cbm_k_setnam("map1.bin");
   	cbm_k_setlfs(1,8,0);
   	cbm_k_load(0, loadptr);    

   	for(i = 0;i < 1024*2; i+=2){
		VERA.data0 = loadptr[i] + 17;
        	VERA.data0 = loadptr[i+1];
   	}

   	VERA.display.video = 0x71;

   	while(isdone != 1){

   		switch(cbm_k_getin()){
        	case QUIT_Q:
            		isdone = 1;
            	break;
   		}
 	}

   	VERA.control = 0x80;
   	__asm__("jsr %w",IOINIT);
}
