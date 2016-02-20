/* Copyright 2014, 2015 Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 * Copyright 2015, Eric Pernia
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Bare Metal example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal example source file
 ** @{ */

/*
 * Initials    Name
 * ---------------------------
 * ENP          Eric Pernia
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151104   v0.0.1   ENP   First version
 */

/*==================[inclusions]=============================================*/
#include "main.h"       /* <= own header */


#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif



/*==================[macros and definitions]=================================*/
const GPIOData gpioLed1 = {2,10,0,14,FUNC0};
const GPIOData gpioLed2 = {2,11,1,11,FUNC0};
const GPIOData gpioLed3 = {2,12,1,12,FUNC0};

const GPIOData gpioSw1 = {1,0,0,4,FUNC0};

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void SysTick_Handler(void) {
	pulsador_tick();
}
/*==================[external functions definition]==========================*/

static void boardButtonsInit(void) {

   /* Config EDU-CIAA-NXP Button Pins as GPIOs */
   Chip_SCU_PinMux(gpioSw1.portNumber,gpioSw1.portPin,MD_PUP|MD_EZI|MD_ZI,gpioSw1.func); /* GPIO0[4], TEC1 */

   /* Config EDU-CIAA-NXP Button Pins as Inputs */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, gpioSw1.gpioNumber,(1<<gpioSw1.gpioPin),0);
}

static void boardLedsInit(void) {
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

static void coreInit(void) {

   /* Read clock settings and update SystemCoreClock variable */
   SystemCoreClockUpdate();
   SysTick_Config(SystemCoreClock / 1000);	

}

/* Set up and initialize board hardware */
void boardInit(void) {

   /* Config Core */
   coreInit();

   /* Initializes GPIO */
   Chip_GPIO_Init(LPC_GPIO_PORT);

   boardButtonsInit();

   /* Config EDU-CIAA-NXP Led Pins */
   boardLedsInit();

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

uint32_t readSw1(void)
{
	return !Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioSw1.gpioNumber, gpioSw1.gpioPin);
}


void eventPressed(void)
{
	semaforo_setLight(LIGHT_GREEN,LIGHT_ON);
}
void eventReleased(void)
{
	semaforo_setLight(LIGHT_GREEN,LIGHT_OFF);
}

/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   	boardInit();

	pulsador_init(readSw1); // configuro funcion de lectura de pulsador
	pulsador_setPressedCallback(eventPressed); // configuro callback de presionado
	pulsador_setReleasedCallback(eventReleased); // configuro callback de release


   	while(1) {
      
		// para debug estado actual del pulsador
		if(readSw1())
			semaforo_setLight(LIGHT_RED,LIGHT_ON);
		else
			semaforo_setLight(LIGHT_RED,LIGHT_OFF);
		//______________________________________

		pulsador_stateMachine();
  

	}   
   return 0;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

