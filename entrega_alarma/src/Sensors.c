#include "chip.h"
#include "Sensors.h"
#include "gpio.h"

const GPIOData gpioSw1 = {1,0,0,4,FUNC0};

const GPIOData gpioSw2 = {1,1,0,8,FUNC0};
const GPIOData gpioSw3 = {1,2,0,9,FUNC0};
const GPIOData gpioSw4 = {1,6,1,9,FUNC0};

void Board_Sensors_Init(void)
{
   Chip_SCU_PinMux(gpioSw1.portNumber,gpioSw1.portPin,MD_PUP|MD_EZI|MD_ZI,gpioSw1.func); /* GPIO0[4], TEC1 */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioSw1.gpioNumber,(1<<gpioSw1.gpioPin),0);

   Chip_SCU_PinMux(gpioSw2.portNumber,gpioSw2.portPin,MD_PUP|MD_EZI|MD_ZI,gpioSw2.func); /* GPIO0[4], TEC2 */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioSw2.gpioNumber,(1<<gpioSw2.gpioPin),0);

   Chip_SCU_PinMux(gpioSw3.portNumber,gpioSw3.portPin,MD_PUP|MD_EZI|MD_ZI,gpioSw3.func); /* GPIO0[4], TEC3 */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioSw3.gpioNumber,(1<<gpioSw3.gpioPin),0);

   Chip_SCU_PinMux(gpioSw4.portNumber,gpioSw4.portPin,MD_PUP|MD_EZI|MD_ZI,gpioSw4.func); /* GPIO0[4], TEC4 */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioSw4.gpioNumber,(1<<gpioSw4.gpioPin),0);

}

uint32_t Board_Sensors_getValue(uint32_t sensorNumber)
{
	switch(sensorNumber)
	{
		case SENSOR_DOOR:return !Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioSw1.gpioNumber, gpioSw1.gpioPin);
		case SENSOR_WINDOW_0:return !Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioSw2.gpioNumber, gpioSw2.gpioPin);;
		case SENSOR_WINDOW_1:return !Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioSw3.gpioNumber, gpioSw3.gpioPin);;
		case SENSOR_WINDOW_2:return !Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioSw4.gpioNumber, gpioSw4.gpioPin);;
	}
}


