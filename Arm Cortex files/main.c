//////////////////////////////////////////////////////////////////////////////
// prog: main.c:: output: US_METHODS
// comm: demonstrates the basic usage of uc/OSII-methods, such as
//       taskcreation, mailboxing, flagging, queueing, semaphores (mutexes)
// auth: MSC, Corrected by CB for project A 2017 - 2018
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "taskcreate.h" // for stacksize
#include "main.h"       // defines, external declarations, enums, prototypes

//sensor data
long DATA;

int Uart_debug_out = 0xffff; // debug output to uart for all tasks
//int Uart_debug_out = INTERRUPT_DEBUG_OUT; // start alleen moet interrupt output


// GLOBAL DATA, in main.h defined 'extern'
// handles, used for q, mailboxes and flags
// defined as a pointer; the os will allocate a specific OS_EVENT on it
OS_STK              InitTaskStk[STACK_SIZE]; // stack of inittask
OS_EVENT           *SensorHandle;   // name of handle on 1 instance of mutex-mechanism
OS_EVENT           *LORA_ReadHandle;    // name of handle on 1 instance of mailbox-mechanism
OS_EVENT           *LORA_WriteHandle;
OS_FLAG_GRP		   *FlagReadHandle;
OS_FLAG_GRP		   *FlagWriteHandle;

// stuff for the queue: 1. handle, 2. my data area, 3. q-array for OS
OS_EVENT           *QueueHandle;        // name of handel on 1 instance of queue-mechanism
Q                   data_queue[QSIZE];  // data queue, in this case array of Q-structs
void*               os_queue[QSIZE];    // queue with void-pointers for OS, holds addresses of data-q-members

// application name
char *version = "uc_project"; // name of this application, will be displayed on uart and lcd
char *LORA_version;

//message timer to lcd
void DisplayMsgTimer(void)
{

}

// put some output to uart
void DisplayOSData(void)
{
	/*char *functionality =
"UC_METHODS laat de basale werking van verschillende uC-mechanismen zien. \
zoals queueing, mailboxing, flagging, semaphores en hoe je makkelijk tasks \
creeert (taskcreate.c) en de bidirectionele omgang met de UART-comport. \n\r\
main.c:       startup code, hardware drivers initialization creatie van handles op gebruikte mechanismen \n\r\
taskcreate.c: automatische creatie van alle user-tasks \n\r\
mutex.c:      basics van mutex semaphores \n\r\
mbox.c:       basics van mailboxes \n\r\
flag.c:       basics van flagging \n\r\
queue.c:      basics van queues \n\r\
interrupt.c:  basics van interrupt-handling \n\r\
terminal-keys 1-5: toggle debug output per task \n\r\
Michiel Scager (june 2015)\n\r";

    UART_puts(functionality);
	UART_puts("\t[uC/OS-II versie ");    UART_putint(OSVersion());
    UART_puts(" voor de CORTEX M4 op "); UART_putint(SystemCoreClock/1000000);
    UART_puts(" MHz]\n\r");
    */
}

// create all handles
// if a new handle is needed, create it here
// note: first, define the handle globally above, then also in main.h as external
// author MScager
void CreateHandles()
{
	INT8U error;

	SensorHandle = OSMutexCreate(MUTEX_PRTY, &error); // for sensor.c
	if (error)
    {
   		UART_puts("\n\rmutexcreate error=");
   		UART_putint(error);
   		UART_puts("\n\r");
    }
	LORA_ReadHandle    = OSMboxCreate(NULL); // for loraread_mailbox.c
	LORA_WriteHandle   = OSMboxCreate(NULL); // for lorawrite_mailbox.c
	FlagReadHandle	   = OSFlagCreate(0x00, &error);
	if (error)
    {
   		UART_puts("\n\rflagcreate error=");
   		UART_putint(error);
   		UART_puts("\n\r");
    }
	FlagWriteHandle	   = OSFlagCreate(0x00, &error);
	if (error)
    {
   		UART_puts("\n\rflagcreate error=");
   		UART_putint(error);
   		UART_puts("\n\r");
    }

	QueueHandle = OSQCreate(&os_queue[0], QSIZE);
}	
   

//////////////////////////////////////////////////////////////////////////////
// func: InitTask
// args: void *pdata, needed by os
// comm: This is the initialisation-task. It's the first task to run, and 
//       it will peform some tasks that need to be done after uC/OS-II 
//       has been started. Most important task here is starting the timer.
// note: this task deletes itself when finished to free memory
//////////////////////////////////////////////////////////////////////////////
void InitTask(void *pdata)
{
	__asm("CPSID   I"); // disable interrupts

   	LCD_puts(version);
	UART_puts(version); UART_puts("\n\r");

	DisplayOSData();  // output to uart of some data

	CreateHandles();  // create all handles
	CreateTasks();    // start all threads/tasks

	OS_CPU_SysTickInit(840000);	// 84000 = 1kHz AHB divider staat op 2!
	__asm( "CPSIE   I"); // enable interrupts
   	OSTaskDel(OS_PRIO_SELF); // This task is no longer needed; delete it
}


int main (void)
{
	SystemInit();	// Set SystemCLK

	// initialize all board-outputs
	DELAY_init();
	UART_init();
	LCD_init();
	LED_init();
	BUZZER_init();
    KEYS_init();

    //LORA
    UART_puts("\r\nStart LORA_init...\r\n");
    LORA_init();
    LORA_gets(LORA_version);
    //LCD_puts(LORA_version);
    UART_puts("\r\ninit done...\r\n");
    while(LORA_connect() == FALSE)
    {
    	DELAY_ms(50);
    }
    UART_puts("\r\nLORA done...\r\n");

    OSInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 24 );
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 28 );
	GPIOD -> MODER |= ( 1 << 30 );

    OSTaskCreate(InitTask, NULL, &InitTaskStk[STACK_SIZE-1], INITTASK_PRTY);
	OSStart();

	// --TODO--
	//Niet zo gebruiken
	//PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

	// Nothing comes beyond this point
    while(TRUE);
}

