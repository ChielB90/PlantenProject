//////////////////////////////////////////////////////////////////////////////
// prog: main.h
// comm: external definitions and prototypes for main
// auth: MSC, Corrected by CB for project A 2017 - 2018
//////////////////////////////////////////////////////////////////////////////

// debug output stuff
#define OUTPUT_DEBUG

extern int Uart_debug_out; // bitmask-toggle key for task-debug-output
#define MUTEX_DEBUG_OUT   	0x01 // bit 1 used to toggle mutex-task output
#define MAILBOX_DEBUG_OUT 	0x02 // bit 2 used to toggle mbox-task output
#define FLAG_DEBUG_OUT    	0x04 // bit 3 used to toggle flag-task output
#define QUEUE_DEBUG_OUT   	0x08 // bit 4 used to toggle queue-task output
#define INTERRUPT_DEBUG_OUT 0x10 // bit 5 used to toggle interrupt-task output


#define WAIT_FOREVER        0   // to force all OSPends (flag, mutex, mbox etc.) to wait for incoming signal
#define LOOP_DELAY          30  // standard delay time for tasks
#define LED_DELAY           50  // to slow leds down
#define QSIZE               10  // 10 q-members
#define LQSIZE 				32

//flag defines
#define READY 0x01

//Struct DATA define
#define CAYENNE

#ifdef CAYENNE
#define DATASIZE 6
#endif
#ifndef CAYENNE
#define DATASIZE 10
#endif

// priorities for all tasks and prty of mutex-semaphore itself
enum {
    INITTASK_PRTY = 5,

    // careful, this one should be highest of all using tasks
    MUTEX_PRTY = 8,

    SENSORREAD_PRTY = 10,

    MBOXWRITEPOST_PRTY,
    MBOXWRITEPEND_PRTY,

    MBOXREADPOST_PRTY,
    MBOXREADPEND_PRTY,

    DUMMY
};


// handles, used for semaphore, mailbox, flag, queue
// defined as a pointer; the os will allocate an OS_EVENT on it
//extern OS_EVENT    *MutexHandle;
//extern OS_FLAG_GRP *FlagHandle;
extern OS_EVENT		 *SensorHandle;
extern OS_EVENT		 *LORA_ReadHandle;
extern OS_EVENT		 *LORA_WriteHandle;
extern OS_FLAG_GRP	 *FlagReadHandle;
extern OS_FLAG_GRP	 *FlagWriteHandle;
extern OS_EVENT      *LoraQHandle;

// q structure
typedef struct queue
{
 	int   nr;        // counter
	char  text[17];  // text to display
} Q, *PQ;

extern Q     data_queue[]; // data queue, in this case array of Q-structs
extern void* os_queue[];   // pointer queue for OS


extern void*               lora_queue[LQSIZE];    // queue with void-pointers for OS, holds addresses of data-q-members

// function prototypes for various tasks to prevent compiler warnings
extern void InitTask      (void *pdata);
extern void InitMutex     (void);
extern void DisplayOSData (void);
extern void CreateHandles (void);
extern void displayAllStackData(void);

// function prototypes of threads/tasks/processes to prevent compiler warnings
//extern void MijnTask1     (void *pdata);
extern void LoraRead_PostTask(void *pdata);
extern void LoraRead_PendTask(void *pdata);
extern void LoraWrite_PostTask(void *pdata);
extern void LoraWrite_PendTask(void *pdata);
extern void SensorRead_Task(void *pdata);

extern float getData(char address);
extern void I2Cinit();
