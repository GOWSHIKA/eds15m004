#include "PLL.h"
#include "tm4c123gh6pm.h"

void DisableInterrupts(void);// Disable interrupts
void EnableInterrupts(void);// Enable interrupts
void WaitForInterrupt(void);// low power mode
void PortA_Init(void);// start sound output
void PortF_Init(void);
void SysFun(void);
void SysInit(void);//initialize SysTick timer
void SysLoad(unsigned long period);//Load reload value

int main(void)
{
	PLL_Init();
	SysFun();
	PortA_Init();
	PortF_Init();
	SysInit();
	SysLoad(800000);

	while(1)
	{
		GPIO_PORTA_DATA_R |= (0x20);
		GPIO_PORTF_DATA_R |= (0x02);
		SysLoad(800000); 
		GPIO_PORTA_DATA_R &= ~(0x20);
		GPIO_PORTF_DATA_R &= ~(0x02);
		SysLoad(800000);
	}
}

void PortA_Init(void)
{
	unsigned long volatile delay;
	/*PORT Initialization*/
	SYSCTL_RCGC2_R |= 0x01; // Port a clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTA_DIR_R |= 0x20; // PA5 output
	GPIO_PORTA_AFSEL_R &= ~0x20; // not alternative
	GPIO_PORTA_AMSEL_R &= ~0x20; // no analog
	GPIO_PORTA_PCTL_R &= ~0x00F00000; // bits for PA5
	GPIO_PORTA_DEN_R |= 0x20; // enable PA5
}

void PortF_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x20;// clock for Port F
	delay = SYSCTL_RCGC2_R;// wait 3-5 bus cycles 	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock GPIOPortF
	GPIO_PORTF_CR_R = 0x02; // allow changes to PF1
	// only PF0 to be unlocked, other bits can't be
	GPIO_PORTF_AMSEL_R &=~ 0x02;// disable analog
	GPIO_PORTF_PCTL_R &= ~0x0F0;// bits for PF1
	GPIO_PORTF_DIR_R |= 0x02;// PF1 input
	GPIO_PORTF_AFSEL_R &=~ 0x02;//disable alt func
	GPIO_PORTF_DEN_R = 0x1F;// enable digital I/O
}

void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears  
	while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag 
		}
}
void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;// priority 0
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}
void SysFun(void)
{
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup  
	NVIC_ST_CTRL_R = 0x00000005; 
} 

