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
#define VRAM_tiles         0x08800

#define IOINIT 0xFF81

#define  QUIT_Q		0x51
#define  KEY_W          0x57
#define  KEY_S          0x53
#define  KEY_D          0x44
#define  KEY_A          0x41


void loadVera(char *fname, unsigned int address, int flag){
   cbm_k_setnam(fname);
   cbm_k_setlfs(1,8,0);
   cbm_k_load(flag, address);    
}

void main(void) {

	int mx = 0;
	int my = 0;

   	int i = 0;
   	int isdone = 0;
   	char * loadptr = (char *)0x2000;

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

   	VERA.layer1.config = 0xA3;
   	VERA.layer1.tilebase = (VRAM_tiles >> 9) +3;
   	VERA.layer1.mapbase = (VRAM_layer1_map >> 9);

   	VERA.layer1.hscroll = 0;
   	VERA.layer1.vscroll = 0;

   	VERA.layer0.config = 0x3;
  
   	VERA.layer0.tilebase = (VRAM_tiles >> 9) +3;
   	VERA.layer0.mapbase = (VRAM_layer0_map >> 9);
    
   	VERA.layer0.hscroll = 0;
   	VERA.layer0.vscroll = 0;

   	loadVera("tiles.bin", VRAM_tiles , 2);

   	VERA.control = 0;

   	VERA.address = VRAM_layer1_map;
  	VERA.address_hi = 0x10 | (VRAM_layer1_map >> 16);

	loadVera("map1.bin", VRAM_layer1_map, 2);

  	VERA.address = VRAM_layer0_map;
   	VERA.address_hi = 0x10 | (VRAM_layer0_map >> 16);

        loadVera("map0.bin", VRAM_layer0_map, 2);

   	VERA.display.video = 0x31;

   	while(isdone != 1){
       		
		switch(cbm_k_getin()){
		        case QUIT_Q:
		            	isdone = 1;
	           	break;
		        case KEY_W:
		            	my += -100;
			break;
		        case KEY_S:
		        	my += 120;
            		break;
        		case KEY_D:
            			mx += 160;
            		break;
        		case KEY_A:
            			mx += -160;
            		break;
	    	}

		VERA.layer1.hscroll = mx;
        	VERA.layer1.vscroll = my;

        	VERA.layer0.hscroll = mx;
        	VERA.layer0.vscroll = my;
   	}
 	

   	VERA.control = 0x80;
   	__asm__("jsr %w",IOINIT);
}
