/*************************************************************************
*		Misc. Functions													 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"



void write_traffic_reg(unsigned short device, unsigned int reg_addr, unsigned int reg_data)
{
     if(device ==0)
	   IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, reg_addr, reg_data);
	 else
	   IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, reg_addr, reg_data);
}

unsigned int read_traffic_reg(unsigned short device, unsigned int reg_addr)
{
    unsigned long read_data;
	if(device ==0)
		read_data = IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, reg_addr);
	 else
		read_data = IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, reg_addr);
    return(read_data);
 }



//***********************************************************************
// mbp's Simple rounding function.... supports pos and neg
//***********************************************************************
//long Round(double number)
//{
//    return (number >= 0) ? (long)(number + 0.5) : (long)(number - 0.5);
//}

//***********************************************************************
// Convert 4 Bytes to a Long Integer
// 		Pass:	 Pointer to Array and the index to the MSB-Byte:
//				 Example: tmpLongInt = BytesToLong(Status,BPV_COUNT3_ptr);
//
//		Return:	 Long Integer
//
//		I.e.:	4 bytes in array 0x12,0x34,0x56,0x78 is converted to 0x12345678
//***********************************************************************
unsigned long BytesToLong(unsigned char* array,unsigned short msb)
{
unsigned long tmpLong=0;

	// Long Integer is: "array[3],array[2],array[1],arrray[0]"  (ie 0x12,34,56,78)
	tmpLong =  (unsigned long) array[msb]<<24;
	tmpLong += (unsigned long) array[msb+1]<<16;
	tmpLong += (unsigned long) array[msb+2]<<8;
	tmpLong += (unsigned long) array[msb+3];

//	if(tmpLong > 0xFFFF0000)	/* clamp max value */
//		return(0xFFFF0000);
//	else
		return(tmpLong);
}






unsigned short BytesToInt(unsigned char* array,unsigned short msb)
{
unsigned short	tmpint=0;

	// Integer is: "array[1],arrray[0]"  (ie 0x12,34)
	tmpint =  (unsigned short) array[msb]<<8;
	tmpint += (unsigned short) array[msb+1];

//	if(tmpint > 0xFFF0)	  	// > 65520
//		return(0xFFF0);		// Clamp at 65520
//	else
		return(tmpint);
}

//***********************************************************************
// Save a LongInt into 4 separate bytes starting at array[msb]
// 		Pass:	 Pointer to Array and the index to the MSB-Byte
//				 Example: SaveBytesLong(Status,BPV_COUNT3_ptr,longint);
//
//		Return:	 nothing
//
//		I.e.:	0x12345678 is converted to 0x12,0x34,0x56,0x78 and loaded into array
//***********************************************************************
void SaveBytesLong(unsigned char* array,unsigned short msb,unsigned long longint)
{
unsigned long tmpl=0;
	tmpl = longint;
	array[msb]   = longint>>24;	// Set array[3] to MSB
	array[msb+1] = longint>>16;	// Set array[2]
	array[msb+2] = longint>>8;	// Set array[1]
	array[msb+3] = longint;		// Set array[0] to LSB

	//if(msb == EMON_RX_CYCLE_END_CNT13_ptr){
	//	D(1, BUG("\n***** SaveBytesLong: (13,12,11,10) array[msb],+1,+2,+3 = %u, %u, %u, %u\n",array[msb],array[msb+1],array[msb+2],array[msb+3]));
	//	}
}


//***********************************************************************
// Save a Double into 4 separate bytes starting at array[msb]
// 		Pass:	 Pointer to Array and the index to the MSB-Byte
//				 Example: SaveBytesLong(Status,BPV_COUNT3_ptr,longint);
//
//		Return:	 nothing
//
//		I.e.:	0x12345678 is converted to 0x12,0x34,0x56,0x78 and loaded into array
//***********************************************************************
/*void SaveBytesDouble(unsigned char* array,unsigned short msb,double doubleint)
{
double tmpl=0;
	tmpl = doubleint;
	array[msb]   = doubleint>>56;	// Set array[7] to MSB
	array[msb+1] = doubleint>>48;	// Set array[6]
	array[msb+2] = doubleint>>40;	// Set array[5]
	array[msb+3] = doubleint>>32;		// Set array[4]
	array[msb+4] = doubleint>>24;		// Set array[3]
	array[msb+5] = doubleint>>16;		// Set array[2]
	array[msb+6] = doubleint>>8;		// Set array[1]
	array[msb+7] = doubleint;		// Set array[0] to LSB
	//if(msb == EMON_RX_CYCLE_END_CNT13_ptr){
	//	D(1, BUG("\n***** SaveBytesLong: (13,12,11,10) array[msb],+1,+2,+3 = %u, %u, %u, %u\n",array[msb],array[msb+1],array[msb+2],array[msb+3]));
	//	}
}*/

void SaveBytesInt(unsigned char* array,unsigned short msb,unsigned short tmpint)
{
	array[msb]   = tmpint>>8;	// Set array[1] to MSB
	array[msb+1] = tmpint;		// Set array[0]
}

//***********************************************************************
// COMPUTE CHECKSUM
// 		Pass Counter value:
//			if > 9999 pass pointer to string ">999"
//			else	  pass pointer of the counter converted to a string
//***********************************************************************
unsigned char CalcChecksum(unsigned char* array,unsigned short size)
{
unsigned short	i=0;
unsigned char	checksum=0;
   checksum=0;
	for(i=0; i < size; i++)
		checksum ^= array[i];
	return(checksum);
}
/***********************************************************************/

//********************************************************************************
//Serial Port: Subroutine to Tx a character (tx_char)
//    return value 1 means sent successfully, -1 Tx failed (buffer full)
//********************************************************************************
int _putchar(int tx_char)
{

  int sr = IORD_ALTERA_AVALON_UART_STATUS(UART_0_BASE);	//Get the value of the status register
  if ((tbuff_ptr == MaxStatus || tbuff_ptr==0) && sr&0x40)	//Check if Tx buffer=empty and the serial port state is ready to send
  {
	  IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE, tx_char);	//transmit buffer is empty, Tx data tx_char
  }
  else
  {
	  TxBuffer[tbuff_ptr++] = tx_char;

	  if(tbuff_ptr == MaxStatus)		//If the Tx buffer is now FULL!
		  return (-1);

	  UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_TRDY_MSK;		    // Enable the serial port Transmit interrupt
	  IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER

  }
  return 1;
}

/*****
 * int _putchar(int tx_char)
{
  int size=0; 	// Tx buffer size

  int sr = IORD_ALTERA_AVALON_UART_STATUS(SERIAL_IO_BASE);	//Get the value of the status register
  if ((TxStart == TxEnd) && sr&0x40)	//Check if Tx buffer=empty and the serial port state is ready to send
  {
	  IOWR_ALTERA_AVALON_UART_TXDATA(SERIAL_IO_BASE,tx_char);	//transmit buffer is empty, Tx data tx_char
  }
  else
  {
	  if (TxStart >= TxEnd)									//calculate Tx buffer size
		  size = - TxEnd;
	  else
		  size = ((MaxStatus - 1) - TxEnd) + TxStart;		//circular buffer

	  if(size > (TXBUFSIZE - 3))	//If the send buffer is full, return w/Tx FAILED!
		  return (-1);

	  TxBuf[TxStart] = tx_char;		//Append new Tx data into the Tx buffer (at the end)
	  if (++TxStart >(TXBUFSIZE - 1))
		  TxStart = 0;

	  IOWR_ALTERA_AVALON_UART_CONTROL(SERIAL_IO_BASE, 0x40|0x10);	// Enable interrupt
  }
  return 1;
}
*********/



/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
