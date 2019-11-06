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

// function declarations
// ...
static int GetTouchState (int* xCoord, int* yCoord);
static int GetUserButtonPressed ();

void SysTick_Handler(void) {	// SysTick ISR: will be executed every 10ms (configured by SysTick_Config())
  SysTickCnt++;
}

/**
* Check if User Button has been pressed
* @param none
* @return 1 if rising edge has been detected
*/
static int GetUserButtonPressed(void) {
  return (GPIOA->IDR & 0x0001);
}

int main(void) {

  RCC_ClocksTypeDef 	Clocks;
  static int32_t SysTickCnt_Hold;
  int x, y;

  SysTickCnt = 0;
  SysTickCnt_Hold = 0;
  x = 0;
  y = 0;

  RCC_GetClocksFreq(&Clocks);

  SysTick_Config( Clocks.HCLK_Frequency/100 - 1 );	// 100 Hz ( T=10ms)
															// Inline-Funktion siehe core_cm4.h (ARM-Grundmodul)
  LCD_Init();	        // LCD initiatization
  LCD_LayerInit(); 	// LCD Layer initiatization
  LTDC_Cmd(ENABLE);  	// Enable the LTDC 
  LCD_SetLayer(LCD_FOREGROUND_LAYER);	// Set LCD foreground layer

  if (IOE_Config() != IOE_OK) {   
    LCD_Clear(LCD_COLOR_RED);
    LCD_SetTextColor(LCD_COLOR_BLACK); 
    LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"   IOE NOT OK      ");
    LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"Reset the board   ");
    LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"and try again     ");
    return 0;
  }		

  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetColors(LCD_COLOR_YELLOW, LCD_COLOR_BLUE); // TextColor,BackColor
  LCD_SetPrintPosition(0,0);  // line 1, column 0
  printf("  HTBLA-WELS    \n");
  LCD_SetPrintPosition(1,0);  // line 2, column 0	
  printf("Fischergasse 30");
  LCD_SetPrintPosition(2,0);  // line 3, column 0	
  printf("  A-4600 Wels  ");

  LCD_SetFont(&Font8x8);		// line: 0...39	, column: 0...29
  LCD_SetPrintPosition(39,0);		// line 39, column 0
  LCD_SetColors(LCD_COLOR_MAGENTA, LCD_COLOR_BLUE); // TextColor,BackColor
  printf("  Copyright Michael Schmid  ");

  // ToDo Begin: Draw Buttons


  // ToDo End: Draw Buttons

  while (1) {
    // Bearbeitungsschleife mit Zykluszeit 100ms, Timer Count = 10ms 

    if( SysTickCnt - SysTickCnt_Hold < 10 ) continue;  // 10 * 10ms = 100ms
    SysTickCnt_Hold += 10;

    if (GetTouchState(&x, &y)) {
      // ToDo Begin: Auswertung Touchclick


      // ToDo End: Auswertung Touchclick
    }	

    // ToDo Begin: Stoppuhr


    // ToDo End: Stoppuhr

  }
} // main

/**
* Check if touch interface has been used
* @param xCoord x coordinate of touch event in pixels
* @param yCoord y coordinate of touch event in pixels
* @return 1 if touch event has been detected
*/
static int GetTouchState (int* xCoord, int* yCoord) {

  static TP_STATE* TP_State;
  int touchclick;

  touchclick = 0;

  TP_State = IOE_TP_GetState();
  if (TP_State->TouchDetected) {
    *xCoord = TP_State->X;
    *yCoord = TP_State->Y;
    touchclick = 1;
  }	

  return touchclick;
}
