#ifndef SEMAFORO_H
#define SEMAFORO_H


#define LIGHT_OFF	0
#define LIGHT_ON	1

#define LIGHT_RED		0
#define LIGHT_YELLOW	1
#define LIGHT_GREEN		2

#define STATE_RED			0
#define STATE_RED_YELLOW	1
#define STATE_GREEN			2
#define STATE_YELLOW		3
#define STATE_WAIT			4


#define DELAY_RED			5000
#define DELAY_RED_YELLOW	2000
#define DELAY_GREEN			3000
#define DELAY_YELLOW		2000



void semaforo_setLight(uint32_t lightNum, uint32_t value);
void semaforo_tick(void);
void semaforo_init(void);
void semaforo_stateMachine(void);



#endif
