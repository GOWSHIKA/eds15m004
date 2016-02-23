// 0.Documentation Section 
// 
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts .
// Jonathan Valvano
// November 3, 2013


#include "PLL.h"
#include "tm4c123gh6pm.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
void Port_Init(void);        // start sound output
void SysInit(void); 				//initialize SysTick timer
void SysLoad(unsigned long period); //Load reload value

int main(void){
  PLL_Init();    
	// initialize output and interrupts
  EnableInterrupts();
  /*Initialize necessary ports and timers here*/
	Port_Init();
	SysInit();
	SysLoad(40000);
	while(1)
	{
	}
  
}
// **************Sound_Init*********************
// Initialize SysTick periodic interrupts
// Input: none
// Output: none
void Port_Init(void){ unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x01;     // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
   
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog on PA
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTA_DIR_R = 0x40;          // 5) PA6 as output
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) disable alt funct on PA7-0
  GPIO_PORTA_PUR_R = 0x00;          // disable pull-up 
  GPIO_PORTA_DEN_R = 0x40;          // 7) enable digital I/O on PF4-0

  /*PORT Initialization*/
}

void SysLoad(unsigned long period){
	NVIC_ST_RELOAD_R = period -1;
	
}

void SysInit(void){
           
	NVIC_ST_CTRL_R = 0;   
   	
  NVIC_ST_CURRENT_R = 0;        // any write to current clears it
  NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0                
  NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
	
} 
// Interrupt service routine
// 
void SysTick_Handler(void){
	
	
/*Your code goes here*/
		GPIO_PORTA_DATA_R ^=0x40;
	  //GPIO_PORTA_DATA_R |=~(0x40);
	   
	
	}



