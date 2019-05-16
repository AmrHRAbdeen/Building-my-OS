#define NUM_OF_TASKS 4


/*******Tasks Section **********/
static void LedOn(){}
static void LedOff() {} 
static void SEVEN_SEG_ON(){} 
static void SEVEN_SEG_OFF() {}
/********************************/

/****** Define Array of Tasks*****/
const callback_t TasksCallBacks[NUM_OF_TASKS]={LedOn , LedOff , SEVEN_SEG_ON , SEVEN_SEG_OFF};

/*Define Number of Ticks per each Task*/
const uint8 taskTick[NUM_OF_TASKS]={1,2,3,1};

uint8 App_Handler_V1_index;

uint8 App_Handler_V3_Counter;

uint32 OS_Scheduler_Offset_Counter;

void App_init(void)
{
	sysTick_setCallback(App_Handler);
}

/*Executes One function per sysTick Exception*/
static void App_Handler_V1(void)
{
	TasksCallBacks[App_Handler_V1_index]();
	App_Handler_V1_index++;
	if(App_Handler_V1_index ==2)
	{
		index=0;
	}
}

/*Executes NUM_OF_TASKS/Functions in order at the same sysTick Exception*/
static void App_Handler_V2(void)
{
	uint8 i;
	for(i=0;i<NUM_OF_TASKS;i++)
	{
		TasksCallBacks[i]();
	}
}

/*Executes Several Functions in differeent sysTick Exceptions*/

static void App_Handler_V3(void)
{
	App_Handler_V3_Counter++;
	
	for(i=0;i<NUM_OF_TASKS;i++)
	{
		if(App_Handler_V3_Counter % taskTick[i] == 0)
		{
			TasksCallBacks[i]();
			
			/*Analyze Behaviour:
			
				in First sysTick Exception:
					iteration 0 : 1 % 1 =0 -> True -> TasksCallBacks[0]()
					iteration 1 : 1 % 2 =1 -> False
					iteration 2 : 1 % 3 =3 -> False
					iteration 3 : 1 % 1 =0 -> True -> TasksCallBacks[3]() 	
				
				in Second SysTick Exception:
					iteration 0 : 2 % 1 =0 -> True -> TasksCallBacks[0]()
					iteration 1 : 2 % 2 =0 -> True -> TasksCallBacks[1]()
					iteration 2 : 2 % 3 =2 -> False
					iteration 3 : 2 % 1 =0 -> True -> TasksCallBacks[3]()
				
				And So on ... ,,,				
			
			*/	
		}
	}
}
/******************* Redesign OS *****************************************/
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
uint8 OS_Scheduler_Counter;

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
		/*OS_Scheduler_Counter is at bottom of the for loop , to enable the execuution of all tasks at the
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
	sysTick_setCallback(OS_Scheduler);
}

/*OS_start*/
void OS_start(void)
{
	sysTick_Enable();
}

/***********************************/
/*Application One*/

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
/*****************************************************/
/* Main*/
void main(void)
{
	OS_init();
	App1_init();
	OS_start();
	while(1);
}
/*******************************************************/