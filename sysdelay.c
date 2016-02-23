// 0.Documentation Section 
// SoftSound.c
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at 500Hz.
// Jonathan Valvano
// November 3, 2013


#include "PLL.h"
#include <stdio.h>
#include "tm4c123gh6pm.h"
// Constant declarations to access port registers using 
// symbolic names instead of addresses

void SysTick_Wait10ms(unsigned long delay);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
void Port_Init(void);        // start sound output
void SysInit(void);
void SysLoad(unsigned long period);
unsigned long int j;
unsigned long int k;
unsigned int i=0;
// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  PLL_Init();   
  	// 80 MHz
   
	// initialize output and interrupts
	SysInit();
	Port_Init();
 // EnableInterrupts();
	/*Initialize ports and timers*/
	while(1)
	{
		for(j=80000;j<=160000;j=j+8000)
	{
		for(k=0;k<=3;k++)
		{
			
		  GPIO_PORTA_DATA_R|=0x20;
			SysLoad(j);
			GPIO_PORTA_DATA_R&=~(0x20);
			SysLoad(1600000-j);
		}
	}
	
  
}
	}
// **************Port_Init*********************
// Initialize SysTick periodic interrupts
// Input: none
// Output: none
void Port_Init(void){ unsigned long volatile delay;
  /*PORT A or PORTF Inititalisation*/
 printf("in loop");
	SYSCTL_RCGC2_R |= 0x01;     // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
   
  GPIO_PORTA_AMSEL_R &=~ 0x20;        // 3) disable analog on PA
  GPIO_PORTA_PCTL_R &=~ 0x00F00000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTA_DIR_R |= 0x20;          // 5) PA6 as output
  GPIO_PORTA_AFSEL_R &= ~0x20;        // 6) disable alt funct on PA7-0
  //GPIO_PORTA_PUR_R |= 0x00;          // disable pull-up 
  GPIO_PORTA_DEN_R |= 0x20;          // 7) enable digital I/O on PF4-0

}

void SysLoad(unsigned long period){

	
	NVIC_ST_RELOAD_R = period-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
	
}

void SysInit(void){
	
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;
          
} 

void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<=delay; i++){
    SysLoad(800000);  // wait 10ms
  }
}




