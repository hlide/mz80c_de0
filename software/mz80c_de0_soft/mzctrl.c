/*
 * MZ-80C on FPGA (Altera DE0 version)
 * MZ control routines
 *
 * (c) Nibbles Lab. 2012
 *
 */

#include "system.h"
#include "io.h"
#include "alt_types.h"
#include <stdio.h>
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "mzctrl.h"

unsigned char buvram[1000],buaram[1000];
unsigned char adconv[256]=
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6b,0x6a,0x2f,0x2a,0x2e,0x2d,
	 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x4f,0x2c,0x51,0x2b,0x57,0x49,
	 0x55,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x52,0x59,0x54,0x50,0x3c,
	 0xf1,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0xe4,0x79,0xe6,0xe3,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0xbd,0x9d,0xb1,0xb5,0xb9,0xb4,0x9e,0xb2,0xb6,0xba,0xbe,0x9f,0xb3,0xb7,0xbb,
	 0xbf,0xa3,0x85,0xa4,0xa5,0xa6,0x94,0x87,0x88,0x9c,0x82,0x98,0x84,0x92,0x90,0x83,
	 0x91,0x81,0x9a,0x97,0x93,0x95,0x89,0xa1,0xaf,0x8b,0x86,0x96,0xa2,0xab,0xaa,0x8a,
	 0x8e,0xb0,0xad,0x8d,0xa7,0xa8,0xa9,0x8f,0x8c,0xae,0xac,0x9b,0xa0,0x99,0xbc,0xb8,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

volatile z80_t z80_status;

/*
 * Push Bottun ISR
 */
static void menu_button(void* context)
{
	volatile z80_t* z80status_pt = (volatile z80_t*)context;

	if((IORD_ALTERA_AVALON_PIO_DATA(INT_BUTTON_BASE)&0x01)==0){
		if((z80status_pt->status&0x01)==0)
			z80status_pt->status|=1;	// Set Flag
		else
			z80status_pt->status&=0xfffffffe;	// Clear Flag
	}

	if((IORD_ALTERA_AVALON_PIO_DATA(INT_BUTTON_BASE)&0x02)==0){
		z80status_pt->status|=2;	// Set Flag
	}

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(INT_BUTTON_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(INT_BUTTON_BASE, 0xf);
}

/*
 * Push Bottun ISR registration
 */
void button_int_regist(void)
{
	alt_ic_isr_register(INT_BUTTON_IRQ_INTERRUPT_CONTROLLER_ID, INT_BUTTON_IRQ, menu_button, (void*)&z80_status, 0x0);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(INT_BUTTON_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(INT_BUTTON_BASE,0xf);
}

/*
 * Release Reset for MZ
 */
void MZ_release(void)
{
	IOWR_ALTERA_AVALON_PIO_DATA(Z80CTRL_BASE,0x03);	// Reset Release
}

/*
 * Bus Request for MZ
 */
void MZ_Brequest(void)
{
	unsigned int i;

	IOWR_ALTERA_AVALON_PIO_DATA(Z80CTRL_BASE,0x02);	// Bus Request
	while((IORD_ALTERA_AVALON_PIO_DATA(Z80STAT_BASE)&0x01)!=0);	// Wait Bus Acknowledge

	// Save and Clear VRAM/ARAM
	for(i=0;i<1000;i++){
		buvram[i]=((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd000))[i];
		((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd000))[i]=0;
		buaram[i]=((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd800))[i];
		((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd800))[i]=0x07;
	}
}

/*
 * Bus Request for MZ(not Clear Screen)
 */
void MZ_Brequest2(void)
{
	unsigned int i;

	IOWR_ALTERA_AVALON_PIO_DATA(Z80CTRL_BASE,0x02);	// Bus Request
	while((IORD_ALTERA_AVALON_PIO_DATA(Z80STAT_BASE)&0x01)!=0);	// Wait Bus Acknowledge

	// Save and Clear VRAM/ARAM
	for(i=0;i<1000;i++){
		buvram[i]=((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd000))[i];
		buaram[i]=((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd800))[i];
	}
}

/*
 * Bus Release for MZ
 */
void MZ_Brelease(void)
{
	unsigned int i;

	// Restore VRAM/ARAM
	for(i=0;i<1000;i++){
		((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd000))[i]=buvram[i];
		((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd800))[i]=buaram[i];
	}

	IOWR_ALTERA_AVALON_PIO_DATA(Z80CTRL_BASE,0x03);	// Bus Release

}

/*
 * Display 1 character to MZ screen
 */
void MZ_disp(unsigned int x, unsigned int y, unsigned char ch)
{
	((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd000))[y*40+x]=adconv[ch];
}

/*
 * Display Message(until NULL) to MZ screen
 */
void MZ_msg(unsigned int x, unsigned int y, char *msg)
{
	while((*msg)!=0){
		MZ_disp(x,y,*msg);
		x++;
		if(x>=40){
			y++;
			x=0;
		}
		msg++;
	}
}

/*
 * Display Message(for numbers) to MZ screen
 */
void MZ_msgx(unsigned int x, unsigned int y, char *msg, unsigned int num)
{
	while((num--)!=0){
		MZ_disp(x,y,*msg);
		x++;
		if(x>=40){
			y++;
			x=0;
		}
		msg++;
	}
}

/*
 * Reverse Display in Rectangle Area
 */
void crev(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned char a;
	unsigned int x,y;

	for(y=y1;y<=y2;y++){
		for(x=x1;x<=x2;x++){
			a=((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd800))[y*40+x];
			((volatile unsigned char*)(INTERNAL_SRAM8_0_BASE+0xd800))[y*40+x]=((a&0xf)<<4)+((a&0xf0)>>4);
		}
	}
}

/*
 * Sharp PWM Pulse Generate
 */
int pulseout(unsigned char c, int num)
{
	int sum=0,stat;

	while(num>0){
		do{
			stat=IORD(CMT_0_BASE, 0);
			if((stat&0x80)==0) return -1;	// tape stop
		}while(stat&0x01);					// still output
		IOWR(CMT_0_BASE, 0, c);		// output 1 pulse
		if(c&0x80) sum++;
		c=c<<1;
		num--;
	}
	return sum;
}

// short 11000bit
int z11000(void)
{
	int p;

	for(p=0;p<1375;p++)	// 11000bit
		if(pulseout(0,8)<0) return -1;
	return 0;
}

// short 20bit
int z20(void)
{
	int p;

	for(p=0;p<4;p++)	// 20bit
		if(pulseout(0,5)<0) return -1;
	return 0;
}

// long 20bit
int o20(void)
{
	int p;

	for(p=0;p<4;p++)	// 20bit
		if(pulseout(0xff,5)<0) return -1;
	return 0;
}

// output checksum
int sumout(unsigned int sum)
{
	if(pulseout(0x80,1)<0) return -1;
	if(pulseout((sum>>8)&0xff,8)<0) return -1;
	if(pulseout(0x80,1)<0) return -1;
	if(pulseout(sum&0xff,8)<0) return -1;
	if(pulseout(0x80,1)<0) return -1;
	return 0;
}
