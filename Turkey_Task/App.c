/*
	@Application One
	@Description: Toggle LED
	@Author: Amr Abdeen
	@Date: 22 April , 2019
*/

const task_t LEDON_Task;

LEDON_Task.Runnable=LED_ON();
LEDON_Task.Priority=0;
LEDON_Task.TaskTick=1;
LEDON_Task.TaskOffset=0;

const task_t LEDOFF_Task;
LEDOFF_Task.Runnable=LED_OFF();
LEDOFF_Task.Priority=1;
LEDOFF_Task.TaskTick=200;
LEDOFF_Task.TaskOffset=0;

void App1_init(void)
{
		Scheduler_createTask(&LEDON_Task);
		Scheduler_createTask(&LEDOFF_Task);
}