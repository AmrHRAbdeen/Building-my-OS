/*
	@OS.c
	@Description: Basic OS Scheduler
	@Author: Amr Abdeen
	@Date: 22 April , 2019
*/

/******************* OS *****************************************/
const task_t Tasks[NUM_OF_TASKS]=
{
	{NULL ,0,0,0},
	{NULL,0,0,0},
	{NULL,0,0,0},
	{NULL,0,0,0}
};

/*Creating Array of pointers to tasks*/
const task_t * pTasks[NUM_OF_TASKS]={NULL,NULL,NULL,NULL};

/*Global Variables needed to be used*/
uint32 OS_Scheduler_Counter;
uint32 OS_Scheduler_Offset_Counter;

/*Creating OS Scheduler API using Array of Tasks*/
static void OS_Scheduler(void)
{
		for(i=0;i<NUM_OF_TASKS;i++)
		{
			if(OS_Scheduler_Offset_Counter >= Tasks[i].Offset)
			{ 
				if( (OS_Scheduler_Counter % Tasks[i].TaskTick == 0))
				{
					Tasks[i].Runnable();
				}
			}	
		}
		/*
		  OS_Scheduler_Counter is at bottom of the for loop , to enable the execuution of all tasks at the
		  begining of the Scheuler then the repetition of each task will be according its TaskTick attribute.
		*/
		OS_Scheduler_Counter++;
		OS_Scheduler_Offset_Counter++;
}

/*Creating OS Scheduler API using Array of pointers to Tasks*/
static void OS_Scheduler_V2(void)
{
		for(i=0;i<NUM_OF_TASKS;i++)
		{
			if(OS_Scheduler_Offset_Counter >= pTasks[i]->TaskOffset)
			{
				if( (OS_Scheduler_Counter % pTasks[i]->TaskTick == 0))
				{
					pTasks[i]->Runnable();
				}
			}	
		}
		/*OS_Scheduler_Counter is at bottom of the for loop , to enable the execuution of all tasks at the
		  begining of the Scheuler then the repetition of each task will be according its TaskTick attribute.
		*/
		OS_Scheduler_Counter++;
		OS_Scheduler_Offset_Counter++;
}

/*Create Task API*/
uint8 Scheduler_createTask(task_t * OS_Task)
{
	uint8 Local_ErrorState=ERROR;
	
	if( Tasks[OS_Task->TaskPriority]== NULL)
	{
		Tasks[OS_Task->TaskPriority]=OS_Task;
		Local_ErrorState=NO_ERROR;
	}
	
	return Local_ErrorState;
}

/*Destroy Task API*/
uint8 Scheduler_destroyTask(task_t * OS_Task)
{
	uint8 Local_ErrorState=ERROR;
	if( Tasks[OS_Task->TaskPriority]!= NULL)
	{
		Tasks[OS_Task->TaskPriority]=NULL;
		Local_ErrorState=NO_ERROR;
	}	
	return Local_ErrorState;
}
/*OS_init API*/
void OS_init(void)
{
	sysTick_init();
	sysTick_setTimeIntevalMs(5);
	sysTick_setCallback(OS_Scheduler);/*we will change this later !!*/
}

/*OS_start*/
void OS_start(void)
{
	sysTick_Enable();
}
