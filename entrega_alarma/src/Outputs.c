#include "chip.h"
#include "Outputs.h"
#include "gpio.h"


const GPIOData gpioLed1 = {2,10,0,14,FUNC0};
const GPIOData gpioLed2 = {2,11,1,11,FUNC0};
const GPIOData gpioLed3 = {2,12,1,12,FUNC0};

void Board_Outputs_Init(void)
{

   /* Config EDU-CIAA-NXP Led Pins as GPIOs */
   Chip_SCU_PinMux(gpioLed1.portNumber,gpioLed1.portPin,MD_PUP,gpioLed1.func);
   Chip_SCU_PinMux(gpioLed2.portNumber,gpioLed2.portPin,MD_PUP,gpioLed2.func);
   Chip_SCU_PinMux(gpioLed3.portNumber,gpioLed3.portPin,MD_PUP,gpioLed3.func);

   Chip_SCU_PinMux(2,0,MD_PUP,FUNC4);  /* GPIO5[0],  LEDR */
   Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);  /* GPIO5[1],  LEDG */
   Chip_SCU_PinMux(2,2,MD_PUP,FUNC4);  /* GPIO5[2],  LEDB */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2),1);
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2));


   /* Config EDU-CIAA-NXP Led Pins as Outputs */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioLed1.gpioNumber,(1<<gpioLed1.gpioPin),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioLed2.gpioNumber,(1<<gpioLed2.gpioPin),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioLed3.gpioNumber,(1<<gpioLed3.gpioPin),1);

   /* Init EDU-CIAA-NXP Led Pins OFF */
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, gpioLed1.gpioNumber,(1<<gpioLed1.gpioPin));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, gpioLed2.gpioNumber,(1<<gpioLed2.gpioPin));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, gpioLed3.gpioNumber,(1<<gpioLed3.gpioPin));

}

void Board_Outputs_set(uint32_t lightNum, uint32_t value)
{
	uint32_t v=1;
	if(value==OUTPUT_OFF)
		v=0;

	switch(lightNum)
	{
		case OUTPUT_SIREN:
			Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed2.gpioNumber, gpioLed2.gpioPin, v);
			break;
		case OUTPUT_LED_ARMED:
			Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed1.gpioNumber, gpioLed1.gpioPin, v);
			break;
		case OUTPUT_LED_DISARMED:
			Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed3.gpioNumber, gpioLed3.gpioPin, v);
			break;
	}
}




