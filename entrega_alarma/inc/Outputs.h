
#define OUTPUT_OFF	0
#define OUTPUT_ON	1

#define OUTPUT_SIREN			0
#define OUTPUT_LED_ARMED		1
#define OUTPUT_LED_DISARMED		2



void Board_Outputs_Init(void);
void Board_Outputs_set(uint32_t lightNum, uint32_t value);


