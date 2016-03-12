#include "chip.h"
#include "serialPort.h"
#include "UserInterface.h"

static uint32_t uiStatus;

const static int8_t* MSG_ENTER_PASS = "Enter password:";

static uint32_t passIndex;
static uint8_t passArray[4];


static uint32_t flagPassEntered;
static int8_t* customMsg;

void ui_init(void)
{
	uiStatus = UI_STATE_IDLE;
}

void ui_stateMachine(void)
{

	switch(uiStatus)
	{
		case UI_STATE_IDLE:
		{
			break;
		}
		case UI_STATE_ENTER_PASS :
		{
			Board_UART_writeTxBuffer(MSG_ENTER_PASS,strlen(MSG_ENTER_PASS));
			Board_UART_sendTxBuffer();
			uiStatus = UI_STATE_WAIT_FOR_PASS;
			passIndex=0;
			flagPassEntered=0;
			break;
		}
		case UI_STATE_WAIT_FOR_PASS:
		{
			int32_t c = Board_UART_Read();
			if(c!=-1)
			{
				Board_UART_writeByte((uint8_t)c); // echo
				passArray[passIndex] = (uint8_t)c;
				passIndex++;
				if(passIndex>=4)
				{
					uiStatus = UI_STATE_PASS_ENTERED;
				}
			}
			break;
		}
		case UI_STATE_PASS_ENTERED:
		{
			flagPassEntered=1;
			uiStatus = UI_STATE_IDLE;
			break;
		}
		case UI_STATE_SHOW_MESSAGE:
		{
			if(customMsg!=0)
			{
				Board_UART_writeTxBuffer(customMsg,strlen(customMsg));
				Board_UART_sendTxBuffer();
				customMsg = 0;
			}
			uiStatus = UI_STATE_IDLE;
		}
	}
}

uint32_t ui_isPassEntered(void)
{
	return flagPassEntered;
}

void ui_showEnterPass(void)
{
	passIndex=0;
	flagPassEntered=0;	
	uiStatus = UI_STATE_ENTER_PASS;
}

void ui_showMessage(int8_t* msg)
{
	customMsg = msg;
	uiStatus = UI_STATE_SHOW_MESSAGE;
}

int8_t* ui_getPassword(void)
{
	return passArray;
}

