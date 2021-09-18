/**
 * AIIT STM32F4 Template
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

volatile int32_t SysTickCnt=0;
volatile int cnt = 0;

// function declarations
static int GetUserButtonPressed ();
static int GetTouchState (int* xCoord, int* yCoord);



void SysTick_Handler(void) {	// SysTick ISR: will be executed every 10ms (configured by SysTick_Config())
  SysTickCnt++;
  cnt++;
}


int main(void) {

  RCC_ClocksTypeDef 	Clocks;
  static int32_t SysTickCnt_Hold;
  int x, y;

  SysTickCnt = 0;
  SysTickCnt_Hold = 0;

  RCC_GetClocksFreq(&Clocks);

  SysTick_Config( Clocks.HCLK_Frequency/100 - 1 );	// 100 Hz ( T=10ms)

  LCD_Init();	        // LCD initialization

  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetColors(LCD_COLOR_YELLOW, LCD_COLOR_BLUE); // TextColor, BackColor
  LCD_SetPrintPosition(0, 0);  // line 1, column 0
  printf("  HTBLA-WELS    \n");
  LCD_SetPrintPosition(1, 0);  // line 2, column 0
  printf("Fischergasse 30");
  LCD_SetPrintPosition(2, 0);  // line 3, column 0
  printf("  A-4600 Wels  ");

  LCD_SetFont(&Font8x8);		// line: 0...39	, column: 0...29
  LCD_SetPrintPosition(39, 0);		// line 39, column 0
  LCD_SetColors(LCD_COLOR_MAGENTA, LCD_COLOR_BLUE); // TextColor, BackColor
  printf("     Copyright xyz  ");


  while (1) {
    // infinite (main) loop, cycle time 100ms, Timer tick  = 10ms

    if( SysTickCnt - SysTickCnt_Hold < 10 )		  // 10 * 10ms = 100ms
    	continue;
    SysTickCnt_Hold += 10;

    // ToDo: start with your code here
    LCD_SetFont(&Font16x24);
    LCD_SetColors(LCD_COLOR_WHITE, LCD_COLOR_BLUE);
    LCD_SetPrintPosition(6, 0);	// line 6, column 0
    printf("Time:%5.1f sec", cnt/100.0);

    if (GetTouchState(&x, &y)) {
    	// ToDo Begin: check touch event

    }


  }
} // main

/**
* Check if User Button has been pressed
* @param none
* @return 1 if rising edge has been detected
*/
static int GetUserButtonPressed(void) {
  return (GPIOA->IDR & 0x0001);
}

/**
* Check if touch interface has been used
* @param xCoord x coordinate of touch event in pixels
* @param yCoord y coordinate of touch event in pixels
* @return 1 if touch event has been detected
*/
static int GetTouchState (int* xCoord, int* yCoord) {

  static TP_STATE* TP_State;
  int touchclick = 0;

  TP_State = IOE_TP_GetState();
  if (TP_State->TouchDetected) {
    *xCoord = TP_State->X;
    *yCoord = TP_State->Y;
    touchclick = 1;
  }

  return touchclick;
}

