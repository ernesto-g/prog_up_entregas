#ifndef PULSADOR_H
#define PULSADOR_H


#define LIGHT_OFF	0
#define LIGHT_ON	1

#define LIGHT_RED		0
#define LIGHT_YELLOW	1
#define LIGHT_GREEN		2

#define STATE_BUTTON_UP			0
#define STATE_BUTTON_FALLING	1
#define STATE_BUTTON_RAISING	2
#define STATE_BUTTON_DOWN		3

#define TIMEOUT_RAISING		40 // esta exagerado para que se vea
#define TIMEOUT_FALLING		40 // esta exagerado para que se vea


void pulsador_tick(void);
void pulsador_init(uint32_t (*functionPtrRead)(void));
void pulsador_stateMachine(void);

void pulsador_setPressedCallback(void (*functionPtr)(void));
void pulsador_setReleasedCallback(void (*functionPtr)(void));

#endif
