#include "OS.h"
#include "App.h"
/* Main*/
void main(void)
{
	OS_init();
	App1_init();
	OS_start();
	while(1);
}
/*******************************************************/