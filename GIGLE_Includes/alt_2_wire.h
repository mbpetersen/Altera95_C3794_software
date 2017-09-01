/*
 * alt_2_wire.h - code to support 2 wire protocol to support 
 * reading and writing to 2 wire device, eg. eeproms memory
 *
 * Author: Percy Chen
 * Date  : August 17/2007
 */
#ifndef _ALT_2_WIRE_H
#define _ALT_2_WIRE_H

#define SDA_DATA_REG      0X0
#define SDA_DIR_REG       0X1
#define SDA_A_ENABLE      0X1
#define SDA_B_ENABLE      0X2
#define SDA_TRI_STATE     0X0


//#define ALTERA_AVALON_PIO_DIRECTION_A_INPUT  0	// old
//#define ALTERA_AVALON_PIO_DIRECTION_A_OUTPUT 1	// old

//moved to hardware_def.h
//#define ALTERA_AVALON_PIO_DIRECTION_A_INPUT_MASK  0xFE
//#define ALTERA_AVALON_PIO_DIRECTION_A_OUTPUT 	  0x01
//
//#define ALTERA_AVALON_PIO_DIRECTION_B_INPUT_MASK  0xFD
//#define ALTERA_AVALON_PIO_DIRECTION_B_OUTPUT 	  0x02

/***
typedef struct alt_two_wire
{
  alt_u32 scl_pio;
  alt_u32 sda_pio;
} alt_two_wire;
***/
//void alt_2_wireInit(alt_two_wire* wires);
void alt_2_wireInit_A();
void alt_2_wireInit_B();
//void alt_2_wireSetDelay(int delay);	// hardcoded to 100kHz
void alt_2_wireReset_A();
void alt_2_wireInputSDA_A();
//----------------------------
void alt_2_wireStart_A();
void alt_2_wireStart_B();
void alt_2_wireStop_A();
void alt_2_wireStop_B();

int alt_2_wireSendByte_A(int byte);
int alt_2_wireSendByte_B(int byte);
int alt_2_wireReadByte_A(int ackControl);
int alt_2_wireReadByte_B(int ackControl);

void alt_2_wireTransmitByte_A(int byte);
void alt_2_wireTransmitByte_B(int byte);

enum { EXPECT_ACK, SEND_ACK, SEND_NACK };

#endif
