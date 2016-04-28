// Sound.c, derived from SysTickInts.c

// Runs on LM4F120 or TM4C123, 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// September 15, 2013
// Port B bits 2-0 have the 3-bit DAC
// Port F is onboard LaunchPad switches and LED
// Port F bit 4 is negative logic switch to play sound, SW1
// SysTick ISR: PF3 ISR heartbeat
#include "tm4c123gh6pm.h"

unsigned char Index;  

// 4-bit 32-element sine wave
const unsigned char SineWave[32] = {	
  8,9,11,12,13,14,14,15,15,15,14,	
  14,13,12,11,9,8,7,5,4,3,2,	
  2,1,1,1,2,2,3,4,5,7};

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x0F;      // no analog 
  GPIO_PORTB_PCTL_R &= ~0x00000FFF; // regular function
  GPIO_PORTB_DIR_R |= 0x0F;      // make PB3-0 out
  GPIO_PORTB_AFSEL_R &= ~0x0F;   // disable alt funct on PB3-0
  GPIO_PORTB_DEN_R |= 0x0F;      // enable digital I/O on PB3-0
}

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none
void Sound_Init(unsigned long period){
  DAC_Init();          // Port B is DAC
  Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 715
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
  GPIO_PORTF_DATA_R ^= 0x0F;     // 
  Index = (Index+1)&0x1F;  
  DAC_Out(SineWave[Index]); 
}


