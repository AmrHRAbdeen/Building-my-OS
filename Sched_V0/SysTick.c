/**********************************************************
SysTick Prog.c
***********************************************************/
/*Systick Registers 
SysTick Status and Control register (STK_CRTL)
SysTick reload value register (STK_LOAD)
SysTick current value register (STK_VAL)
*/

/*Define Number of Tasks*/
#define NUM_OF_TASKS 2

/*Types Definiton*/
typedef void(*TaskCallback_t)(void);
/**********************************/

/*Define Array of pointers to tasks*/
const TaskCallback_t sysTickCallbackPtr[NUM_OF_TASKS]={NULL,NULL};

extern void sysTick_init(void)
{
	/*Configure Timer Clock (Prescalar) (HCLK/8)*/
	STK->CTRL &=~(1<<2); 
	/*Enable Timer Interrupt*/
	STK->CTRL |=(1<<1);
}

extern void sysTick_Enable(void)
{
	STK->CTRL |=(1<<0);	
}

extern void sysTick_Disable(void)
{
	STK->CTRL &=~(1<<0);
}

extern void sysTick_setTimeIntevalMs(uint32 time_ms)
{
	/*Calculations
	-----------------
	SysTick Clock = HCLK max / 8 = 72/8 = 9 MHz 
	T= 1/9000000 = 1.1*(10^-7) * 9000 = 1 mSec.
	reload value = 9000 * time_ms
	*/
	STK->LOAD=9000*time_ms;	
}

void sysTick_Handler(void)
{
	if(funCallbackPtr != NULL)
	{
		sysTickCallbackPtr();
	}
}

void sysTick_setCallback(TaskCallback_t funCallbackPtr)
{
		if(funCallbackPtr != NULL)
		{
			sysTickCallbackPtr=funCallbackPtr;
		}
}