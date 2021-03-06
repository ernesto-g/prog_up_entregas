
#define UI_STATE_IDLE			0
#define UI_STATE_ENTER_PASS		1
#define UI_STATE_WAIT_FOR_PASS	2
#define UI_STATE_PASS_ENTERED	3
#define UI_STATE_SHOW_MESSAGE	4

void ui_init(void);
void ui_stateMachine(void);
uint32_t ui_isPassEntered(void);
void ui_showEnterPass(void);
void ui_showMessage(int8_t* msg);
int8_t* ui_getPassword(void);

