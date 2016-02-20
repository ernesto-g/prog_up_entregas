#include "main.h"
#include "chip.h"

extern const GPIOData gpioLed1;
extern const GPIOData gpioLed2;
extern const GPIOData gpioLed3;

uint32_t state;
uint32_t sleepCounter;
uint32_t nextState;

void semaforo_init(void)
{
	state=STATE_RED;
	sleepCounter=0;

}

void semaforo_setLight(uint32_t lightNum, uint32_t value)
{
	uint32_t v=1;
	if(value==LIGHT_OFF)
		v=0;

	switch(lightNum)
	{
		case LIGHT_RED:
			Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed2.gpioNumber, gpioLed2.gpioPin, v);
			break;
		case LIGHT_YELLOW:
			Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed1.gpioNumber, gpioLed1.gpioPin, v);
			break;
		case LIGHT_GREEN:
			Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed3.gpioNumber, gpioLed3.gpioPin, v);
			break;
	}
}


void semaforo_tick(void)
{
	if(sleepCounter>0)
		sleepCounter--;
}

void semaforo_stateMachine(void)
{

	switch(state)
	{
		case STATE_RED:
			semaforo_setLight(LIGHT_RED,LIGHT_ON);
			semaforo_setLight(LIGHT_GREEN,LIGHT_OFF);
			semaforo_setLight(LIGHT_YELLOW,LIGHT_OFF);
			sleepCounter = DELAY_RED;
			nextState = STATE_RED_YELLOW;
			state = STATE_WAIT;
		break;
		case STATE_RED_YELLOW:
			semaforo_setLight(LIGHT_RED,LIGHT_ON);
			semaforo_setLight(LIGHT_GREEN,LIGHT_OFF);
			semaforo_setLight(LIGHT_YELLOW,LIGHT_ON);
			sleepCounter = DELAY_RED_YELLOW;
			nextState = STATE_GREEN;
			state = STATE_WAIT;
		break;
		case STATE_GREEN:
			semaforo_setLight(LIGHT_RED,LIGHT_OFF);
			semaforo_setLight(LIGHT_GREEN,LIGHT_ON);
			semaforo_setLight(LIGHT_YELLOW,LIGHT_OFF);
			sleepCounter = DELAY_GREEN;
			nextState = STATE_YELLOW;
			state = STATE_WAIT;
		break;
		case STATE_YELLOW:
			semaforo_setLight(LIGHT_RED,LIGHT_OFF);
			semaforo_setLight(LIGHT_GREEN,LIGHT_OFF);
			semaforo_setLight(LIGHT_YELLOW,LIGHT_ON);
			sleepCounter = DELAY_YELLOW;
			nextState = STATE_RED;
			state = STATE_WAIT;
		break;


		case STATE_WAIT:
			if(sleepCounter==0)
				state = nextState;
		break;

	}
}


