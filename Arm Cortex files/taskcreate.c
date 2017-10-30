//////////////////////////////////////////////////////////////////////////////
// prog: taskcreate.c
// comm: task creation and stuff: OSTaskCreateExt, OSTaskStkChk
// auth: MSC, Corrected by CB for project A 2017 - 2018
//////////////////////////////////////////////////////////////////////////////

#include "includes.h"
#include "taskcreate.h"
#include "main.h"


// Create different stacks for the tasks:
// give each thread/task/process its own stack with size
// stacks are allocated here statically, because malloc() is not supported
//OS_STK MijnTask1_Stk     [STACK_SIZE];
OS_STK LoraRead_PostTask_Stk	[STACK_SIZE];
OS_STK LoraRead_PendTask_Stk	[STACK_SIZE];
OS_STK LoraWrite_PostTask_Stk	[STACK_SIZE];
OS_STK LoraWrite_PendTask_Stk	[STACK_SIZE];
OS_STK SensorRead_Task_Stk	 	[STACK_SIZE];

// allocation of array of structures
// notes: the last structure is set to zero's, not necessary, but it simplifies
//        to disable a task: insert // in front of the line
// 1. looping thru the array (until null) and 2. copy-pasting with the last comma
TASKDATA tasks[] =
{
//  name           stack              priority            stacksize   option
// -------------------------------------------------------------------------------------
	// in studentstuff.c
//{   MijnTask1,     MijnTask1_Stk,     MIJNTASK1_PRTY,     STACK_SIZE, OS_TASK_OPT_STK_CHK },

{	LoraRead_PostTask,	LoraRead_PostTask_Stk,	MBOXREADPOST_PRTY,	STACK_SIZE, OS_TASK_OPT_STK_CHK },
{	LoraRead_PendTask,	LoraRead_PendTask_Stk,	MBOXREADPEND_PRTY,	STACK_SIZE, OS_TASK_OPT_STK_CHK },
{	LoraWrite_PostTask, LoraWrite_PostTask_Stk, MBOXWRITEPOST_PRTY, STACK_SIZE, OS_TASK_OPT_STK_CHK },
{	LoraWrite_PendTask, LoraWrite_PendTask_Stk, MBOXWRITEPEND_PRTY, STACK_SIZE, OS_TASK_OPT_STK_CHK },
{	SensorRead_Task,	SensorRead_Task_Stk,	SENSORREAD_PRTY,	STACK_SIZE,	OS_TASK_OPT_STK_CHK },

    // laatste, niet wissen: nodig om for-loop af te sluiten
{   NULL,          NULL,              0,                  0,          0                    }
};


//////////////////////////////////////////////////////////////////////////////
// func: createTasks
// args: none
// comm: all tasks are created in a loop, using the TASKDATA array
// note: the stacks are allocated statically; dynamic allocation would be an
//       improvement, but malloc() is not supported.
//////////////////////////////////////////////////////////////////////////////
void CreateTasks()
{
	// using a pointer to a structure: pointer is incremented
	PTASKDATA ptd = tasks; // tasks == &tasks[0]: both are addresses of first struct
	for (; ptd->taskname != NULL; ptd++)
	{
		OSTaskCreateExt(
			ptd->taskname,	 					// taskname
			NULL, 								// not used
			ptd->stack + ptd->stacksize-1, 		// top of stack
			ptd->priority,						// priority
            ptd->priority, 						// not used
			ptd->stack, 						// bottom of stack
			ptd->stacksize, 					// size of stack
			NULL, 								// not used
			ptd->option);						// enable stackchecking
	}
}


//////////////////////////////////////////////////////////////////////////////
// func: displayStackData
// args: priority (== process-id)
// comm: functions retrieves and displays StackData-information
//       the os-call is optional and set in the STACKDATA array
// note: function is important, to check if task has enough memory, or
//       if task has too much memory
//////////////////////////////////////////////////////////////////////////////
void displayStackData(int priority)
{
    OS_STK_DATA stackdata; // allocate structure
	INT8U       error;

	if ((error = OSTaskStkChk(priority, &stackdata))) // get data onto this struct
	    return;

    UART_puts("\n\rStackData for process "); UART_putint(priority);
    UART_puts(", Usage=");     UART_putint((int)stackdata.OSUsed/4);
    UART_puts(" - Free=");	 UART_putint((int)stackdata.OSFree/4);
}


void displayAllStackData()
{
	PTASKDATA ptd = tasks; // tasks == &tasks[0]: both are addresses of first struct
	for (; ptd->taskname != NULL; ptd++)
		displayStackData(ptd->priority);
}
