#include "main.h"
#include "chip.h"

extern const GPIOData gpioLed1;
extern const GPIOData gpioLed2;
extern const GPIOData gpioLed3;

uint32_t state;
uint32_t sleepCounter;
uint32_t nextState;

uint32_t (*pReadSw)(void);

void (*pCallbackBtnPressed)(void);
void (*pCallbackBtnReleased)(void);

void pulsador_init(uint32_t (*functionPtrRead)(void))
{
	sleepCounter=0;
	pReadSw = functionPtrRead;

	if(pReadSw())
		state = STATE_BUTTON_DOWN;
	else
		state = STATE_BUTTON_UP;

	pCallbackBtnPressed = 0;
	pCallbackBtnReleased = 0;
}

void pulsador_setPressedCallback(void (*functionPtr)(void))
{
	pCallbackBtnPressed = functionPtr;
}
void pulsador_setReleasedCallback(void (*functionPtr)(void))
{
	pCallbackBtnReleased = functionPtr;
}


void pulsador_tick(void)
{
	if(sleepCounter>0)
		sleepCounter--;
}

void pulsador_stateMachine(void)
{
	switch(state)
	{
		case STATE_BUTTON_UP:
			if(pReadSw()) 
			{
				sleepCounter = TIMEOUT_FALLING;
				state = STATE_BUTTON_FALLING;
			}
		break;

		case STATE_BUTTON_FALLING:
		case STATE_BUTTON_RAISING:
			if(sleepCounter>0)
				break;
			if(pReadSw())
			{ 
				state = STATE_BUTTON_DOWN;
				pCallbackBtnPressed();
			}
			else
			{
				state = STATE_BUTTON_UP;			
				pCallbackBtnReleased();
			}
		break;

		case STATE_BUTTON_DOWN:
			if(!pReadSw()) 
			{
				sleepCounter = TIMEOUT_RAISING;
				state = STATE_BUTTON_RAISING;
			}
		break;

	}
}


