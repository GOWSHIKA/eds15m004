
#include <stdint.h>
#include "tm4c123gh6pm.h"
//#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
//#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
//#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
//#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
//#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
//#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
//#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
	
void PortD_Init(void);
void Delay(unsigned long time);
unsigned long SW1,SW2;  // input from PF4,PF0
int main(void){
	long led;
	PortD_Init();
	
while(1){
    SW1 = GPIO_PORTD_DATA_R&0x01;     // read PD0 into SW1
    SW2 = GPIO_PORTD_DATA_R&0x02;     // read PD1 into SW2
    if(SW1&&SW2){                     // both pressed
      GPIO_PORTD_DATA_R = 0x0C;       // BOTH LED are ON
    } else                           
      if(SW1&&(!SW2)){                // just SW1 pressed
        GPIO_PORTD_DATA_R = 0x04;     // LED1 is ON
      } else                        
        if((!SW1)&&SW2){              // just SW2 pressed
          GPIO_PORTD_DATA_R = 0x08;   // LED2 is ON
        }else if((!SW1) && (!SW2))
					{                        // neither switch
          GPIO_PORTD_DATA_R = 0x00;//BOTH OFF
					}
          else{
              GPIO_PORTD_DATA_R=0x00;
					}						// LED is off
        }
      }
    
  
void PortD_Init(void)
	{volatile unsigned long delay;
		SYSCTL_RCGC2_R |= 0x08;           // Port D clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTD_DIR_R |= 0x0C;         // PD3 PD2 output
  GPIO_PORTD_AFSEL_R &= ~0x09;      // not alternative
  GPIO_PORTD_AMSEL_R &= ~0x09;      // no analog
  GPIO_PORTD_PCTL_R &= ~0x0000F00F; // bits for PD3, PD0
  GPIO_PORTD_DEN_R |= 0x0F;         // enable PD3, PD2 ,PD2 , PD0
		 
		

	}
	
	
	void Delay(unsigned long time)
	{
		unsigned long i;
		while(time>0)
		{i=1333333;
			while(i>0)
			{i=i-1;
			}
			time=time-1;
		}
	}
