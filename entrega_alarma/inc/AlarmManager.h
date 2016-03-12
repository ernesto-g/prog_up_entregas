
#define AM_STATE_DISARM				0
#define AM_STATE_DISARM2			1
#define AM_STATE_WAIT_FOR_ARM		2
#define AM_STATE_ARMING				3
#define AM_STATE_WAITING_ARMING		4

#define AM_STATE_ARMED				5

#define AM_STATE_WAIT_FOR_DISARM	6
#define AM_STATE_DISARMING			7
#define AM_STATE_WAITING_DISARMING	8

#define AM_STATE_INTRUDER				9
#define AM_STATE_INTRUDER2				10
#define AM_STATE_INTRUDER_WAIT_PASS		11



void am_init(void);
void am_stateMachine(void);

