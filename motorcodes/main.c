#include "PLL.h"
#include "tm4c123gh6pm.h"

void DisableInterrupts(void);// Disable interrupts
void EnableInterrupts(void);// Enable interrupts
void WaitForInterrupt(void);// low power mode
void PortA_Init(void); // initialize port A
void SysHandler(void);
void SysInit(void);//initialize SysTick timer
void SysLoad(unsigned long period);//Load reload value

int main(void)
{
	unsigned long T=1600000 ;
	int i,j;
	PLL_Init();
	SysHandler();
	PortA_Init();
	SysInit();
	SysLoad(T);

	while(1)
	{
for(j=80000; j <160000 ; j=j+8000){
	for(i=0;i<4;i++){

	 
GPIO_PORTA_DATA_R|=0x20;
SysLoad(j);	
GPIO_PORTA_DATA_R&=~0x20 ;
SysLoad(T-j);	
	}
}
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
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and no interrupts
}
void SysHandler(void)
{
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup  
	NVIC_ST_CTRL_R = 0x00000005; //// enable with core clock and no interrupts

} 

