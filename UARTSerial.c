#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include "inc/tm4c123gh6pm.h"
#include "UARTSerial.h"

// Initialize the UART0 for 115,200 baud rate (assuming 16 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
void UART0_Init(void)
{
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0; 	//Enable UART0
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; 	//Enable clock to GPIO Port A for UART0
	UART0_CTL_R &= ~UART_CTL_UARTEN; 					//Disable UART0 before configuring the baudrate
	UART0_IBRD_R = 8;                     		// IBRD = int(16,000,000 / (16 * 115,200)) = int(8.680)
  UART0_FBRD_R = 44;                    		// FBRD = round(0.680 * 64 + 0.5) = 44
	UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);// Enable UART0 line control for 8 bit word, no parity, one stop bit, FIFOs
  UART0_CTL_R |= UART_CTL_UARTEN;       					// Enable UART0
	GPIO_PORTA_AFSEL_R |= 0x03;           					// Enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             					// Enable digital I/O on PA1-0
																									// Configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          					// Disable analog functionality on PA
}


// Initialize the UART1 for 115,200 baud rate (assuming 16 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
void UART1_Init(void)
{
	SYSCTL_RCGC1_R |= 0x00000002;  // Enable UART1
  SYSCTL_RCGC2_R |= 0x00000004;  // Enable clock to GPIO port C for UART1
  UART1_CTL_R &= ~0x00000001;    // Disable UART1 before configuring teh baudrate 
  UART1_IBRD_R = 8;     				 // IBRD = int(16,000,000/(16*115,200)) = int(8.680)
  UART1_FBRD_R = 44;     				 // FBRD = round(0.680 * 64 + 0.5) = 44
  UART1_LCRH_R = 0x00000070;  	 // Enable UART1 line control for 8 bit word , no parity bits, one stop, FIFOs
  UART1_CTL_R |= 0x00000001;     // Enable UART1
  GPIO_PORTC_AFSEL_R |= 0x30;    // Enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // Configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_AMSEL_R &= ~0x30;   // Disable analog on PC5-4
}

void UART0_OutChar(char data)
{
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}


char UART0_InChar(void)
{
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((char)(UART0_DR_R&0xFF));
}

void UART0_PutString(const char input[])
{
	uint8_t bufIndex;
	bufIndex = 0u;
	/* Blocks the control flow until all data has been sent */
   while(input[bufIndex] != 0)
   {
       UART0_OutChar(input[bufIndex]);
       bufIndex++;
   }
}

void UART1_OutChar(char data)
{
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}


char UART1_InChar(void)
{
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((char)(UART1_DR_R&0xFF));
}


// at least one of three things has happened:
// hardware TX FIFO goes from 3 to 2 or less items
// hardware RX FIFO goes from 1 to 2 or more items
// UART receiver has timed out
void UART1_Handler(void)
{
  if(UART0_RIS_R&UART_RIS_TXRIS){       // hardware TX FIFO <= 2 items
    UART0_ICR_R = UART_ICR_TXIC;        // acknowledge TX FIFO
    // copy from software TX FIFO to hardware TX FIFO
    copySoftwareToHardware();
    if(TxFifo_Size() == 0){             // software TX FIFO is empty
      UART0_IM_R &= ~UART_IM_TXIM;      // disable TX FIFO interrupt
    }
  }
  if(UART0_RIS_R&UART_RIS_RXRIS){       // hardware RX FIFO >= 2 items
    UART0_ICR_R = UART_ICR_RXIC;        // acknowledge RX FIFO
    // copy from hardware RX FIFO to software RX FIFO
    copyHardwareToSoftware();
  }
  if(UART0_RIS_R&UART_RIS_RTRIS){       // receiver timed out
    UART0_ICR_R = UART_ICR_RTIC;        // acknowledge receiver time out
    // copy from hardware RX FIFO to software RX FIFO
    copyHardwareToSoftware();
  }
}

void UART1_PutString(const char input[])
{
	uint8_t bufIndex;
	bufIndex = 0u;
	/* Blocks the control flow until all data has been sent */
   while(input[bufIndex] != 0)
   {
       UART1_OutChar(input[bufIndex]);
       bufIndex++;
   }
}

