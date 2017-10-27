#define MSGEND "\r\n" //command terminater
#define SPACE " " // space char

//LoRa command defines
#define LSYS    "sys"
#define LMAC    "mac"
#define LRADIO	"radio"
#define LUNCNF	"uncnf"

#define LGET    "get"
#define LSET    "set"
#define LJOIN   "join"
#define LSAVE   "save"
#define LTX     "tx"
#define LRX		"rx"
#define LPAUSE	"pause"
#define LRESUME	"resume"

#define LHWEUI  "hweui"
#define LDEVEUI "deveui"
#define LDEVADDR "devaddr"
#define LAPPKEY "appkey"
#define LAPPEUI "appeui"
#define LSTATUS "status"
#define LCH     "ch"
#define LOTAA	"otaa"
#define LABP	"abp"
#define LAPPSKEY "appskey"
#define LNWKSKEY "nwkskey"

//LORA responce define
#define LOK				"ok"
#define LACCEPTED		"accepted"
#define LBUSY			"busy"
#define LKEYSNOTINIT	"keys_not_init"

#define LORA_NETWORK	"260113E9"
#define LORA_APPKEY		"64B2100F98C1A4EA727AFA507DD86BBF"
#define LORA_APPEUI		"70B3D57ED000795C"

#define LORA_APPSKEY	"67B7E849DB17EDAAE4C61C09687DFD58"
#define LORA_NWKSKEY	"BD7718DB0CE5F8FD56BEF809DC38A35E"

#define LORA_SIZE 		50

#define LORA_PORT		"1"

//LORA_combine commands
//SYS
#define LORA_GETHWEUI	0xF0

//radio
#define LORA_RX			0xE0

//MAC
#define LORA_JOIN		0x01
#define	LORA_SAVE		0x02
#define LORA_TX			0x03

#define LORA_PAUSE		0x04
#define LORA_RESUME		0x05

#define LORA_GETDEVEUI	0x10
#define LORA_GETAPPEUI	0x11
#define LORA_GETSTATUS	0x12
#define LORA_GETCH		0x13
#define LORA_GETDEVADDR 0x14

#define LORA_SETDEVEUI	0x20
#define LORA_SETAPPEUI	0x21
#define LORA_SETAPPKEY	0x22
#define LORA_SETCH		0x23
#define LORA_SETDEVADDR 0x24
#define LORA_SETAPPSKEY 0x25
#define LORA_SETNWKSKEY	0x26

void LORA_init(void);
void LORA_puts(char *s);
char LORA_get(void);
void LORA_gets(char *s);
void LORA_combine(INT8U command, char *parameter, char *target);
void LORA_register(char *mode);
INT8U LORA_connect(void);
