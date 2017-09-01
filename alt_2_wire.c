/*
 * alt_2_wire.h - code to support 2 wire protocol to support 
 * reading and writing to 2 wire device, eg. eeproms memory
 *
 * Author: Percy Chen
 * Date  : August 17/2007
 */

#include "gigle.h"
#include "alt_2_wire.h"
#include "extvars.h"

int alt_2_wireDelay = 20;	// set delay to 10uS = 100kHz
//alt_two_wire* bus;

//int SDA = 1;
//int SCL = 1;
//int debug = 0;

/***
static inline void setupDelay() {
	int i;
	for(i = 0; i < alt_2_wireDelay; i++)
		;
	   //usleep(1);							// sleep 1uS
}
***/
static inline void setupDelay() {
	usleep(10);  //udelay()
}

static void pulldownSDA_A()
{

  IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DIR_REG, SDA_A_ENABLE);// direction = out
  IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DATA_REG, 0x0);         // data = 0

}
static void pullupSDA_A()
 {
	IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DIR_REG, SDA_A_ENABLE);// direction = out
    IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DATA_REG, 0x1);           // data = 1

 }

static void pulldownSDA_B()
{
	  IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DIR_REG, SDA_B_ENABLE);// direction = out
	  IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DATA_REG, 0x0);         // data = 0

}
static void pullupSDA_B()
 {
	IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DIR_REG, SDA_B_ENABLE);// direction = out
    IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DATA_REG, 0x2);         // data = 1
 }

static void tri_state_sda()
{
  IOWR_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DIR_REG, SDA_TRI_STATE);// output
}

static void pulldownSCL_A()	{PIO2_Content &= PORTA_SFP_A_SCL_MASKLOW; IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content); setupDelay(); }
static void pullupSCL_A()	{PIO2_Content |= PORTA_SFP_A_SCL_HIGH; IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content); setupDelay(); }
static void pulldownSCL_B()	{PIO2_Content &= PORTB_SFP_B_SCL_MASKLOW; IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content); setupDelay(); }
static void pullupSCL_B()	{PIO2_Content |= PORTB_SFP_B_SCL_HIGH; IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content); setupDelay(); }


static int readSDA_A()
{
    int read_data;
    read_data = IORD_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DATA_REG);
	return(read_data &= 0x1);
}
static int readSDA_B()
{
    int read_data;
    read_data = IORD_8DIRECT(I2C_CONTROLLER_0_BASE, SDA_DATA_REG);
    read_data >>= 1;
	return(read_data &= 0x1);
}

void alt_2_wireStart_A()
{
	pullupSCL_A();
	pullupSDA_A();
	pulldownSDA_A();
	pulldownSCL_A();
	tri_state_sda();
}
void alt_2_wireStart_B()
{
	pullupSCL_B();
	pullupSDA_B();
	pulldownSDA_B();
	pulldownSCL_B();
	tri_state_sda();
}

void alt_2_wireStop_A()
{

	pulldownSDA_A();
 	pullupSCL_A();
	pullupSDA_A();
	tri_state_sda();
}
void alt_2_wireStop_B()
{
	pulldownSDA_B();
 	pullupSCL_B();
	pullupSDA_B();
	tri_state_sda();
}

int alt_2_wireSendByte_A(int byte)
{
    int i, ret;

    for(i = 7; i > -1; i--) {
        (byte & (1 << i))?(pullupSDA_A()):(pulldownSDA_A());
        pullupSCL_A();
        pulldownSCL_A();
    }

    tri_state_sda();
    pullupSCL_A();
    ret = readSDA_A();	// capture ack and return ret
    pulldownSCL_A();
   return ret;
}
int alt_2_wireSendByte_B(int byte)
{
    int i, ret;
    for(i = 7; i > -1; i--) {
        (byte & (1 << i))?(pullupSDA_B()):(pulldownSDA_B());

        pullupSCL_B();
        pulldownSCL_B();
    }
    tri_state_sda();
    pullupSCL_B();
    ret = readSDA_B();	// capture ack and return ret
    pulldownSCL_B();
   return ret;
}

void alt_2_wireTransmitByte_A(int byte)
{
    int i, ret;

    for(i = 7; i > -1; i--) {
        (byte & (1 << i))?(pullupSDA_A()):(pulldownSDA_A());
        pullupSCL_A();
        pulldownSCL_A();
    }

    tri_state_sda();
    pullupSCL_A();
    ret = readSDA_A();
    pulldownSCL_A();
}
void alt_2_wireTransmitByte_B(int byte)
{
    int i, ret;

    for(i = 7; i > -1; i--) {
        (byte & (1 << i))?(pullupSDA_B()):(pulldownSDA_B());

        pullupSCL_B();
        pulldownSCL_B();
    }
    tri_state_sda();
    pullupSCL_B();
    ret = readSDA_B();
    pulldownSCL_B();
}


int alt_2_wireReadByte_A(int ackControl) {
	int i, val = 0, ack;
    tri_state_sda();
	for(i = 7; i > -1; i--) {
		pullupSCL_A();
		val |= (readSDA_A() << i);
		pulldownSCL_A();
	}

	switch(ackControl) {
		case EXPECT_ACK:
			pullupSCL_A();
			ack = readSDA_A();
			pulldownSCL_A();
			if(ack) return -1;
			break;
		case SEND_ACK:
			pulldownSDA_A();
			pullupSCL_A();
			pulldownSCL_A();
			pullupSDA_A();
			break;
		case SEND_NACK:
			pullupSDA_A();
			pullupSCL_A();
			pulldownSCL_A();
			pulldownSDA_A();
			break;
		default:
			alt_2_wireStop_A();
            return -1;
	}
    tri_state_sda();
	return val;
}
int alt_2_wireReadByte_B(int ackControl) {
	int i, val = 0, ack;
    tri_state_sda();
	for(i = 7; i > -1; i--) {
		pullupSCL_B();
		val |= (readSDA_B() << i);
		pulldownSCL_B();
	}

	switch(ackControl) {
		case EXPECT_ACK:
			pullupSCL_B();
			ack = readSDA_B();
			pulldownSCL_B();
			if(ack) return -1;
			break;
		case SEND_ACK:
			pulldownSDA_B();
			pullupSCL_B();
			pulldownSCL_B();
			pullupSDA_B();
			break;
		case SEND_NACK:
			pullupSDA_B();
			pullupSCL_B();
			pulldownSCL_B();
			pulldownSDA_B();
			break;
		default:
			alt_2_wireStop_B();
            return -1;
	}
    tri_state_sda();
	return val;
}

//void alt_2_wireSetDelay(int delay) {
//	alt_2_wireDelay = delay;
//}
/****
void alt_2_wireInit(alt_two_wire* wires) {
    bus = wires;
    IOWR_ALTERA_AVALON_PIO_DIRECTION(bus->sda_pio, ALTERA_AVALON_PIO_DIRECTION_OUTPUT);
}
***/
void alt_2_wireInit_A()
{
  tri_state_sda();

}
void alt_2_wireInit_B()
{
	tri_state_sda();
}
