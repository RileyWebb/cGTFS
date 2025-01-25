#include <stdbool.h>

extern bool ui_running;
void UI_Init();
void UI_Update();

//ui_log.c
extern bool ui_logEnabled;
void UI_DrawLog();