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

#define TEC1_GPIO 0
#define TEC1_PIN  4

#define TEC2_GPIO 0
#define TEC2_PIN  8

#define TEC3_GPIO 0
#define TEC3_PIN  9

#define TEC4_GPIO 1
#define TEC4_PIN  9

#define LEDB_GPIO 5
#define LEDB_PIN  2

typedef struct S_GPIO_Data{
	uint32_t portNumber;
	uint32_t portPin;
	uint32_t gpioNumber;
	uint32_t gpioPin;
	uint32_t func;
}GPIOData;

const GPIOData gpioTecla1 = {1,0,0,4,FUNC0};
const GPIOData gpioTecla2 = {1,1,0,8,FUNC0};

const GPIOData gpioLed1 = {2,10,0,14,FUNC0};
const GPIOData gpioLed2 = {2,11,1,11,FUNC0};




/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

static void boardButtonsInit(void) {

   /* Config EDU-CIAA-NXP Button Pins as GPIOs */
   Chip_SCU_PinMux(1,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[4], TEC1 */
   Chip_SCU_PinMux(1,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[8], TEC2 */
   Chip_SCU_PinMux(1,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[9], TEC3 */
   Chip_SCU_PinMux(1,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO1[9], TEC4 */

   /* Config EDU-CIAA-NXP Button Pins as Inputs */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<4)|(1<<8)|(1<<9),0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<9),0);

}

static void boardLedsInit(void) {
   
   /* Config EDU-CIAA-NXP Led Pins as GPIOs */
   Chip_SCU_PinMux(2,0,MD_PUP,FUNC4);  /* GPIO5[0],  LEDR */
   Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);  /* GPIO5[1],  LEDG */
   Chip_SCU_PinMux(2,2,MD_PUP,FUNC4);  /* GPIO5[2],  LEDB */
   Chip_SCU_PinMux(2,10,MD_PUP,FUNC0); /* GPIO0[14], LED1 */
   Chip_SCU_PinMux(2,11,MD_PUP,FUNC0); /* GPIO1[11], LED2 */
   Chip_SCU_PinMux(2,12,MD_PUP,FUNC0); /* GPIO1[12], LED3 */

   /* Config EDU-CIAA-NXP Led Pins as Outputs */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<11)|(1<<12),1);

   /* Init EDU-CIAA-NXP Led Pins OFF */
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0,(1<<14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1,(1<<11)|(1<<12));

}

static void coreInit(void) {

   /* Read clock settings and update SystemCoreClock variable */
   SystemCoreClockUpdate();

}

/* Set up and initialize board hardware */
void boardInit(void) {

   /* Config Core */
   coreInit();

   /* Initializes GPIO */
   Chip_GPIO_Init(LPC_GPIO_PORT);

   /* Config EDU-CIAA-NXP Button Pins */
   boardButtonsInit();

   /* Config EDU-CIAA-NXP Led Pins */
   boardLedsInit();

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
   /* perform the needed initialization here */
   
   boardInit();

   while(1) {
      
      /* add your code here */
      
      /* Read TEC1 */
      if( !(Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, TEC1_GPIO, TEC1_PIN )) ){

         /* Write LED with 1 */
         Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed1.gpioNumber, gpioLed1.gpioPin, 1);   
         Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed2.gpioNumber, gpioLed2.gpioPin, 0);   
 

      } else{

         /* Write LED with 0 */
         Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed1.gpioNumber, gpioLed1.gpioPin, 0);   
         Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioLed2.gpioNumber, gpioLed2.gpioPin, 1);   
      }
   }
   
   return 0;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

