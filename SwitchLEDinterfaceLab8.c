// ***** 1. Pre-processor Directives Section *****

#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

unsigned long In;  // input from PE0

// FUNCTION PROTOTYPES: Each subroutine defined

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay(unsigned long time);  // Delay100ms
void PortE_Init(void);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10 k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).

int main(void){ 
	
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
	
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz  // activate grader and set system clock to 80 MHz
  PortE_Init();
	
  EnableInterrupts();           						// enable interrupts for the grader
  while(1) {
     Delay(1);
     In = GPIO_PORTE_DATA_R&0x01;   					// read PE0 into In
     if (In == 0x01) {
         GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R ^ 0x02;  // toggle the LED
     } 
	 else {
         GPIO_PORTE_DATA_R |= 0x02;         			// the LED ON (make PE1 =1). 
	 }
  }
}

void PortE_Init(void){ volatile unsigned long delay;
	
  SYSCTL_RCGC2_R |= 0x00000010;       // Enable clock
  delay = SYSCTL_RCGC2_R;          	  // Delay for clock to start
  GPIO_PORTE_AMSEL_R &= ~0x03;        // Disable analog on PE0, PE1
  GPIO_PORTE_PCTL_R &= ~0x000000FF;   // GPIO clear bit PCTL on PE0, PE1
  GPIO_PORTE_DIR_R &= ~0x01;          // PE0 input, 
  GPIO_PORTE_DIR_R |= 0x02;           // PE1 output  
  GPIO_PORTE_AFSEL_R &= ~0x03;        // No alternate function on PE0, PE1   
  GPIO_PORTE_DEN_R |= 0x03;           // Enable digital pins PE0, PE1
  GPIO_PORTE_DATA_R |= 0x02;          // Start with the LED ON (PE1 = 1) 
}

// Delay to avoid switch bouncing 
void Delay(unsigned long time) {
  unsigned long counter;
  while(time > 0) {
    counter = 1333333;  							// 100ms (from other lab)
    while(counter > 0) {
      counter--;
    }
    time--;
  }
}
