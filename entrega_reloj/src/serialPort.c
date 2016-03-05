#include "chip.h"
#include "serialPort.h"

static uint8_t txBuffer[TX_BUFFER_SIZE];
static uint32_t txBufferIndex;

static uint32_t Board_UART_Write(uint8_t* buffer, uint32_t size);

void Board_UART_Init(void)
{
	Chip_UART_Init(LPC_USART2);
	Chip_UART_SetBaud(LPC_USART2, 115200);
	Chip_UART_SetupFIFOS(LPC_USART2, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0);
	Chip_UART_TXEnable(LPC_USART2);

	Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1: UART2_TXD */
	Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2: UART2_RXD */

	/* Restart FIFOS: set Enable, Reset content, set trigger level */
   	Chip_UART_SetupFIFOS(LPC_USART2, UART_FCR_FIFO_EN | UART_FCR_TX_RS | UART_FCR_RX_RS | UART_FCR_TRG_LEV0);
   	/* dummy read */
   	Chip_UART_ReadByte(LPC_USART2);
   	/* enable rx interrupt */
   	Chip_UART_IntEnable(LPC_USART2, UART_IER_RBRINT);

	txBufferIndex = 0;
}

uint32_t Board_UART_writeTxBuffer(uint8_t* buffer,uint32_t len)
{
	uint32_t i;
	for(i=0; i<len; i++)
	{
		if(txBufferIndex<TX_BUFFER_SIZE)
		{
			txBuffer[txBufferIndex] = buffer[i];
			txBufferIndex++;
		}	
		else
		{
			return 1;
		}
	}
	return 0;
}


void Board_UART_sendTxBuffer(void)
{
	Board_UART_Write(txBuffer,txBufferIndex);
	txBufferIndex=0;
}


static uint32_t Board_UART_Write(uint8_t* buffer, uint32_t size)
{
   uint32_t ret = 0;

   while(ret<size)
   {
       while((Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE) == 0){}

       Chip_UART_SendByte(LPC_USART2, buffer[ret]);
       /* bytes written */
       ret++;
   }
   return ret;
}

