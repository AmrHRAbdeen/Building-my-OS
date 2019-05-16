#ifndef OS_H_
#def OS_H_

/*Types Definiton*/
typedef void(*TaskCallback_t)(void);
/**********************************/

/* Define Tasks Structure ******/
typedef struct
{
		/*Task implementation*/
		TaskCallback_t Runnable;
		/*Task Predicity*/
		uint8 TaskTick;
		/*Task Offset expresses the time when the task will be executed after First Tick(i.e: Ofset from 0 start)*/
		uint8 TaskOffset;
		/*Task Priority*/
		uint8 TaskPriority;	
}task_t;

/*Init OS API*/
void OS_init(void);
/*Start OS API*/
void OS_start(void);
/*Create Task API*/
uint8 Scheduler_createTask(task_t * OS_Task);
/*Destroy Task API*/
uint8 Scheduler_destroyTask(task_t * OS_Task);

#endif