/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): __USE_LCD   - enable Output on LCD, uncomment #define in code to use
 *  				for demo (NOT for analysis purposes)
 *----------------------------------------------------------------------------
 * Copyright (c) 2008-2011 Keil - An ARM Company.
 * Name: Anita Tino
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H"                       
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000
char text[10];
char direction1[10];
char direction2[10];
struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

uint32_t joystickposition = 0;            /* Unsigned integer (32 bit) used for Joystick Position*/
/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
		uint32_t joystickpositionprevious;

  LED_Init();            /* LED Initialization            */
	KBD_Init();						 /* Joystick Initialization */
	
	#ifdef __USE_LCD 		  	/* IF USE_LCD is defined, Then : */
	
  GLCD_Init();                /* Initialize graphical LCD (if enabled) */

  GLCD_Clear(White);          /* Clear graphical LCD display   */
  GLCD_SetBackColor(Black);
  GLCD_SetTextColor(Yellow); 
	
  GLCD_DisplayString(0, 0, __FI, "COE 718");
  GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	
	GLCD_DisplayString(1, 0, __FI, "ID 500511445");
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	
  GLCD_DisplayString(2, 0, __FI, "Amir S");
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	
	GLCD_DisplayString(3, 0, __FI, "Lab 1 - Joystick");
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	
  GLCD_DisplayString(5, 0, __FI, "Direction =");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);


#endif

  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */
		joystickposition = get_button();  /* Joystick's state */
		while (joystickpositionprevious != joystickposition){
		if (joystickposition == KBD_SELECT){
			
			sprintf(text,"Select"); /* Port1.20*/
			sprintf(direction1," (X)");
			sprintf(direction2,"");
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI); // Clears lines 6-9 
				GLCD_ClearLn(8,__FI);
				GLCD_ClearLn(9,__FI);
			#endif
			LED_Out(16);
		}
		else if (joystickposition == KBD_UP){
			sprintf(text,"UP"); /* Port1.23*/
			sprintf(direction1," ^");
			sprintf(direction2," |");
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_ClearLn(8,__FI);
				GLCD_ClearLn(9,__FI);
			#endif
			LED_Out(1);

		}
		else if (joystickposition == KBD_RIGHT){
			sprintf(text,"RIGHT"); /* Port1.24*/
			sprintf(direction1,"-->");
			sprintf(direction2,"");
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_ClearLn(8,__FI);
				GLCD_ClearLn(9,__FI);
			#endif
			
			LED_Out(2);
		}
		else if (joystickposition == KBD_DOWN){
			sprintf(text,"DOWN"); /* Port1.25*/
			sprintf(direction1," |");
			sprintf(direction2," v ");
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_ClearLn(8,__FI);
				GLCD_ClearLn(9,__FI);
			#endif
			LED_Out(4);
		}
		else if (joystickposition == KBD_LEFT){
			sprintf(text,"LEFT"); /* Port1.26*/
			sprintf(direction1,"<--");
			sprintf(direction2,"");
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_ClearLn(8,__FI);
				GLCD_ClearLn(9,__FI);
			#endif
			LED_Out(8);
		}
		else if (joystickposition == KBD_MASK){
			LED_Out(32);
		}
		joystickpositionprevious = joystickposition;
#ifdef __USE_LCD
		
      GLCD_SetTextColor(Black);
			GLCD_SetBackColor(White);
      GLCD_DisplayString(6,  12, __FI,  (unsigned char *)text);
			GLCD_DisplayString(8,  12, __FI,  (unsigned char *)direction1);
			GLCD_DisplayString(9,  12, __FI,  (unsigned char *)direction2);
#endif
//    }

    /* Print message with Joystick value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;
      printf("Direction : %s \r\n", text);
    }
  }}
}
