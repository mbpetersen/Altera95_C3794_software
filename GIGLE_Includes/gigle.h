/*************************************************************************
*		gigle.h															 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#ifndef GIGLE_H_
#define GIGLE_H_


#ifndef USE_PRINT
#define USE_PRINT 0
#endif

#define D(v, x) if ((v) <= USE_PRINT) x
#define BUG(x, ...) printf(x, ## __VA_ARGS__)
//D(2, BUG( "Some register: %08lx=%08x\n", reg, data ) );
// the debug level is changed by setting a #define DEBUG greater than 0.
// need to use 1 since -D sets it to 1. (more thinking if want levels)!

#include "includes.h"
#include "math.h"
#include "system.h"
#include <alt_types.h>
#include "sys/alt_irq.h"
#include "unistd.h"		// for usleep()  ...only use in some instances since we have uC/OS
#include "altera_avalon_uart_regs.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include "triple_speed_ethernet_regs.h"
#include "confstat.h"
#include "confstat_ms.h"
#include "func_defs.h"
#include "hardware_def.h"
#include "tcl_defs.h"
#include "ConfigData.h"
#include "ConfigMsData.h"
#include "Header.h"
#include "Messages.h"
#include "SfpModuleInfoData.h"
#include "StatusData.h"
#include "SystemInfoData.h"

#ifdef USE_PRINT
#include <stdio.h>
#endif

//#include <stdio.h> // DEBUG

// Definition of Task Priorities
#define MAIN_TASK_PRIORITY      1
#define LINK_TASK_PRIORITY      2
#define C3794_TASK_PRIORITY     3	//temp for testing....


#define NO	0
#define YES	1
#define OFF	0
#define ON	1
#define FALSE	0
#define TRUE	1
#define INACTV	0
#define ACTV	1
#define LOW		0
#define HIGH	1

#define PortA	1
#define PortB	0
#define GEN     0
#define MON     1
/*********************************/
/* VERSION CONTROL definitions   */
/*********************************/
#define GIGE_HW_VER		1		// rev1 to rev255
#define GIGE_FW_VER1	0x0C	// ver1ver0: 0 -> 65535 (FFFF),  65535/1000 = v0.0 to v65.535
#define GIGE_FW_VER0	0x08	// FW Supports Single Port 10G!!
// 0x03FC = 1020 -> 1020/1000 = v1.020 !
// 0x0406 = 1030 -> 1030/1000 = v1.030 !
// 0x0410 = 1040 -> 1040/1000 = v1.040 !
// 0x0411 = 1041 -> 1041/1000 = v1.041 ! 10G boot/init fault reporting
// 0x041A = 1050 -> 1.050   VLAN+ Updates
// 0x041B = 1051 -> 1.051	Add 100FX support for A & B.
// 0x041C = 1052 -> 1.052	Add IPv6
// 0x041D = 1053 -> 1.053	End-to-End test modification + 802.3/Eth || pkt_ptr value changed
// 0x07D0 = 2000 -> 2.000	MS- to create flash for Ken.
// 0x0834 = 2100 -> 2.100   MS with Lpbk.
// 0x0898 = 2200 -> 2.200   MS with lpbk.
// 0x08FC = 2300 -> 2.300   MS with lpbk.
// 0x960 =  2400 -> 2.400
// 0x9C4 = 2500 ->  2.500
// 0xA28 = 2600 ->  2.600
// 0xA8C = 2700 ->  2.700
// 0xAF0 = 2800 ->  2.800
// 0xB54 = 2900 ->	2.900
// Receive & Transmit buffer size
//#define TXBUFSIZE 256
//#define RXBUFSIZE 256




void init_gige();
void config_link();
void serial_ISR(void* context);   	// Serial Port Interrupt Service Routine
unsigned char CalcChecksum(unsigned char* array,unsigned short size);
int _putchar(int in_char);     		// Transmit a char
//int _puts(unsigned char* str);  	// Transmit  a string
//int processRxBuffer();              // handle the receiving of buffer the data stream

#endif /* GIGLE_H_ */
/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
