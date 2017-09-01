/*************************************************************************
*		SFP Functions										*
*************************************************************************/
#include "gigle.h"
#include "extvars.h"

#include "alt_2_wire.h"


// STR Notes - Pos clk data into SFP, neg clk data out  max clk 100KHz follows AT24C01
// A2 is the DEVICE address (always A) Read then word address (104 or 105)
// #define SFP_Power A2h;
// var =sfp_reg_read;
// RX power at A2 byte 104MSB & 105LSB

// note; you can't write anything at A0 so no driver for that.....
void sfp_reg_write_diag_byte (_Bool sfpa, int address, unsigned char data)
{
    int ret;

    if(sfpa){
		alt_2_wireStart_A();
		ret = alt_2_wireSendByte_A(SFP_ADDR_DIAG_A2);
		ret = alt_2_wireSendByte_A(address);
		ret = alt_2_wireSendByte_A(data);
		alt_2_wireStop_A();
    }
    else{
		alt_2_wireStart_B();
		ret = alt_2_wireSendByte_B(SFP_ADDR_DIAG_A2);
		ret = alt_2_wireSendByte_B(address);
		ret = alt_2_wireSendByte_B(data);
		alt_2_wireStop_B();
    }
}
void sfp_reg_write_diag_word (_Bool sfpa, int address, int data)
{
    int ret;
    if(sfpa){
		alt_2_wireStart_A();
		ret = alt_2_wireSendByte_A(SFP_ADDR_DIAG_A2);
		ret = alt_2_wireSendByte_A(address);
		ret = alt_2_wireSendByte_A(data >> 8);
		ret = alt_2_wireSendByte_A(data & 0xFF);
		alt_2_wireStop_A();
    }
    else{
		alt_2_wireStart_B();
		ret = alt_2_wireSendByte_B(SFP_ADDR_DIAG_A2);
		ret = alt_2_wireSendByte_B(address);
		ret = alt_2_wireSendByte_B(data >> 8);
		ret = alt_2_wireSendByte_B(data & 0xFF);
		alt_2_wireStop_B();
    }
}

//==== mbp Read any one byte from SFP ==========================
// sfp_reg_read_one_byte (SFP_ADDR_CORE_A0, Vendor_name1);
int sfp_reg_read_one_byte (_Bool sfpa, int base_address, int reg_address)
{
    int data;
    if(sfpa){
		alt_2_wireStart_A();
		alt_2_wireTransmitByte_A(base_address);
		alt_2_wireTransmitByte_A(reg_address);
		alt_2_wireStart_A();
		alt_2_wireTransmitByte_A(base_address|SFP_READ);
		data = alt_2_wireReadByte_A(SEND_NACK);
		alt_2_wireStop_A();
    }
    else{
		alt_2_wireStart_B();
		alt_2_wireTransmitByte_B(base_address);
		alt_2_wireTransmitByte_B(reg_address);
		alt_2_wireStart_B();
		alt_2_wireTransmitByte_B(base_address|SFP_READ);
		data = alt_2_wireReadByte_B(SEND_NACK);
		alt_2_wireStop_B();
    }

    return data;
}

//== mbp's read an array field from the sfp ============================
// sfp_read_field(VendorName, SFP_ADDR_CORE_A0, Vendor_name1, 16);
void sfp_read_field_A(unsigned char* array, int base_address, int start_address, unsigned int num_to_read)
{
    int i=0;

    alt_2_wireStart_A();

    alt_2_wireTransmitByte_A(base_address);
    alt_2_wireTransmitByte_A(start_address);
    alt_2_wireStart_A();
    alt_2_wireTransmitByte_A(base_address|SFP_READ);

    for(i=0;i<num_to_read;i++){			// read num# consecutive bytes
    	//usleep(100);
    	if(i == num_to_read-1)
    		array[i] = alt_2_wireReadByte_A(SEND_NACK);	// On last read send NACK
    	else
    		array[i] = alt_2_wireReadByte_A(SEND_ACK);
    	}

    alt_2_wireStop_A();
}
void sfp_read_field_B(unsigned char* array, int base_address, int start_address, unsigned int num_to_read)
{
    int i=0;

    alt_2_wireStart_B();

    alt_2_wireTransmitByte_B(base_address);
    alt_2_wireTransmitByte_B(start_address);
    alt_2_wireStart_B();
    alt_2_wireTransmitByte_B(base_address|SFP_READ);

    for(i=0;i<num_to_read;i++){			// read num# consecutive bytes
    	//usleep(100);
    	if(i == num_to_read-1)
    		array[i] = alt_2_wireReadByte_B(SEND_NACK);	// On last read send NACK
    	else
    		array[i] = alt_2_wireReadByte_B(SEND_ACK);
    	}

    alt_2_wireStop_B();
}

//== mbp's read an array field from the sfp ============================
// sfp_read_field(VendorName, SFP_ADDR_CORE_A0, Vendor_name1, 16);
void sfp_A_read_block_into_Array(unsigned char* array, int base_address, int start_address, unsigned int num_to_read, unsigned int offset)
{
	int i=0;//,j=0;
	alt_2_wireStart_A();

	alt_2_wireTransmitByte_A(base_address);
	alt_2_wireTransmitByte_A(start_address);
	alt_2_wireStart_A();
	alt_2_wireTransmitByte_A(base_address|SFP_READ);

	for(i=0;i<num_to_read;i++){			// read num# consecutive bytes
		if(i == num_to_read-1)
			array[i+offset] = alt_2_wireReadByte_A(SEND_NACK);	// On last read send NACK
		else
			array[i+offset] = alt_2_wireReadByte_A(SEND_ACK);
		}

	alt_2_wireStop_A();
}
void sfp_B_read_block_into_Array(unsigned char* array, int base_address, int start_address, unsigned int num_to_read, unsigned int offset)
{
    int i=0;
    alt_2_wireStart_B();

    alt_2_wireTransmitByte_B(base_address);
    alt_2_wireTransmitByte_B(start_address);
    alt_2_wireStart_B();
    alt_2_wireTransmitByte_B(base_address|SFP_READ);

    for(i=0;i<num_to_read;i++){			// read num# consecutive bytes
    	if(i == num_to_read-1)
    		array[i+offset] = alt_2_wireReadByte_B(SEND_NACK);	// On last read send NACK
    	else
    		array[i+offset] = alt_2_wireReadByte_B(SEND_ACK);
    	}

    alt_2_wireStop_B();
}

void read_sfp_name()
{
	unsigned char i=0;
	unsigned char vendor_namea[16],vendor_nameb[16];
//    alt_two_wire * sfp_serial;
//    alt_two_wire serial_wire;

    for(i=0;i<16;i++){
    	vendor_namea[i] = 0;
    	vendor_nameb[i] = 0;
    }

//    sfp_serial = &serial_wire;

//	sfp_serial->scl_pio = SFP_A_SCL;	// setup for Port A - SFP
//    sfp_serial->sda_pio = SFP_A_SDA;
//    alt_2_wireInit(sfp_serial);
    alt_2_wireInit_A();

    //i = sfp_reg_read_one_byte(SFP_ADDR_DIAG_A2, 128);
    //sfp_reg_write_diag_byte(128, 0x65);
    //i = sfp_reg_read_one_byte(SFP_ADDR_DIAG_A2, 128);

    //sfp_reg_write_diag_byte(Status_Control, 0x40);	// ^6=Tx DISABLE LED
//    sfp_read_field(vendor_namea, SFP_ADDR_CORE_A0, Vendor_name1, 16);
    //vendor_namea[0] = sfp_reg_read_one_byte(SFP_ADDR_CORE_A0, Vendor_name1);
    //vendor_namea[1] = sfp_reg_read_one_byte(SFP_ADDR_CORE_A0, Vendor_name2);
    //vendor_namea[2] = sfp_reg_read_one_byte(SFP_ADDR_CORE_A0, Vendor_name3);
    sfp_A_read_block_into_Array(SpecialStat, SFP_ADDR_CORE_A0, 0, 128, 2);	// start offset into array at 2 (Link+ID bytes)
    sfp_A_read_block_into_Array(SpecialStat, SFP_ADDR_DIAG_A2, 0, 128, 130);	// start offset into array at next byte after 129 (2+127)

   // i = sfp_reg_read_one_byte(SFP_ADDR_DIAG_A2, 96);
   // i = sfp_reg_read_one_byte(SFP_ADDR_DIAG_A2, 97);
///***
//	sfp_serial->scl_pio = SFP_B_SCL;	// setup for Port B - SFP
//	sfp_serial->sda_pio = SFP_B_SDA;
//	alt_2_wireInit(sfp_serial);
    alt_2_wireInit_B();

    //sfp_read_field(vendor_nameb, SFP_ADDR_CORE_A0, Vendor_name1, 16);
	//i = sfp_reg_read_one_byte(SFP_ADDR_CORE_A0, Vendor_name1);
    sfp_B_read_block_into_Array(SpecialStatB, SFP_ADDR_CORE_A0, 0, 128, 2);	// start offset into array at 2 (Link+ID bytes)
    sfp_B_read_block_into_Array(SpecialStatB, SFP_ADDR_DIAG_A2, 0, 128, 130);	// start offset into array at 2+127

//    ***/

}

//==================================================================================
void read_sfp_info(_Bool sfp_a)
{
//    alt_two_wire * sfp_serial;
//    alt_two_wire serial_wire;

//    sfp_serial = &serial_wire;

	if(sfp_a){
		//sfp_serial->scl_pio = SFP_A_SCL;	// setup for Port A - SFP
		//sfp_serial->sda_pio = SFP_A_SDA;
		//alt_2_wireInit(sfp_serial);
		alt_2_wireInit_A();

		sfp_A_read_block_into_Array(SpecialStat, SFP_ADDR_CORE_A0, 0, 128, 2);	// start offset into array at 2 (Link+ID bytes)
		sfp_A_read_block_into_Array(SpecialStat, SFP_ADDR_DIAG_A2, 0, 128, 130);	// start offset into array at 2+127
		}
	else{
		//sfp_serial->scl_pio = SFP_B_SCL;	// setup for Port B - SFP
		//sfp_serial->sda_pio = SFP_B_SDA;
		//alt_2_wireInit(sfp_serial);
		alt_2_wireInit_B();

		sfp_B_read_block_into_Array(SpecialStatB, SFP_ADDR_CORE_A0, 0, 128, 2);	// start offset into array at 2 (Link+ID bytes)
		sfp_B_read_block_into_Array(SpecialStatB, SFP_ADDR_DIAG_A2, 0, 128, 130);	// start offset into array at 2+127
		}

}

//==================================================================================
void read_sfp_power(_Bool sfp_a)
{
    //alt_two_wire * sfp_serial;
    //alt_two_wire serial_wire;

    //sfp_serial = &serial_wire;

    if(sfp_a){
		//sfp_serial->scl_pio = SFP_A_SCL;	// setup for Port A - SFP
		//sfp_serial->sda_pio = SFP_A_SDA;
		//alt_2_wireInit(sfp_serial);
    	alt_2_wireInit_A();

//*** DEBUG
		unsigned char dmi_support = sfp_reg_read_one_byte(PortA, SFP_ADDR_CORE_A0, Diagnostic_Monitoring_Type);
		//if(byte92&0x40) != 0){		// if bit.6==1 DMI is implemented

		if(dmi_support == 0xFF || dmi_support == 0x7F || (dmi_support&0x40) != 0x40){	// if reading crap or bit.6!=1 (no DMI)
			ConfigStat[PHY_OPT_POWER1_ptr] = 0xFF; ConfigStat[PHY_OPT_POWER0_ptr] = 0xFF;	// flag GUI to "---" SFP-A RxPower
			ConfigStatB[PHY_OPT_POWER1_ptr] = 0xFF; ConfigStatB[PHY_OPT_POWER0_ptr] = 0xFF;
			ConfigStat10G[PHY_OPT_POWER1_ptr] = 0xFF; ConfigStat10G[PHY_OPT_POWER0_ptr] = 0xFF;
			return;	// ....then skip trying to read/load power
			}
//****/
		sfp_A_read_block_into_Array(ConfigStat, SFP_ADDR_DIAG_A2, 104, 2, PHY_OPT_POWER1_ptr);	// load power at PWR1 (PWR1,PWR0)
		ConfigStatB[PHY_OPT_POWER1_ptr] = ConfigStat[PHY_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStatB[PHY_OPT_POWER0_ptr] = ConfigStat[PHY_OPT_POWER0_ptr];
		ConfigStat10G[PHY_OPT_POWER1_ptr] = ConfigStat[PHY_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStat10G[PHY_OPT_POWER0_ptr] = ConfigStat[PHY_OPT_POWER0_ptr];
    	}
    else{
		//sfp_serial->scl_pio = SFP_B_SCL;	// setup for Port B - SFP
		//sfp_serial->sda_pio = SFP_B_SDA;
		//alt_2_wireInit(sfp_serial);
    	alt_2_wireInit_B();

    	//sfp_reg_write_diag_byte(PortB, Status_Control, 0x40);	// ^6=Tx DISABLE Tx Laser

		unsigned char dmi_support = sfp_reg_read_one_byte(PortB, SFP_ADDR_CORE_A0, Diagnostic_Monitoring_Type);
		if(dmi_support == 0xFF || dmi_support == 0x7F || (dmi_support&0x40) != 0x40){	// if reading crap or bit.6!=1 (no DMI)
			ConfigStat[PHYB_OPT_POWER1_ptr] = 0xFF; ConfigStat[PHYB_OPT_POWER0_ptr] = 0xFF;	// flag GUI to "---" SFP-B RxPower
			ConfigStatB[PHYB_OPT_POWER1_ptr] = 0xFF; ConfigStatB[PHYB_OPT_POWER0_ptr] = 0xFF;
			ConfigStat10G[PHYB_OPT_POWER1_ptr] = 0xFF; ConfigStat10G[PHYB_OPT_POWER0_ptr] = 0xFF;
			return;	// ....then skip trying to read/load power
			}

		sfp_B_read_block_into_Array(ConfigStatB, SFP_ADDR_DIAG_A2, 104, 2, PHYB_OPT_POWER1_ptr);	// load power at PWR1 (PWR1,PWR0)
		ConfigStat[PHYB_OPT_POWER1_ptr] = ConfigStatB[PHYB_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStat[PHYB_OPT_POWER0_ptr] = ConfigStatB[PHYB_OPT_POWER0_ptr];
		ConfigStat10G[PHYB_OPT_POWER1_ptr] = ConfigStatB[PHYB_OPT_POWER1_ptr];
		ConfigStat10G[PHYB_OPT_POWER0_ptr] = ConfigStatB[PHYB_OPT_POWER0_ptr];
    	}

}



void sfp_transmit_laser(_Bool sfpa, _Bool enable)
{
	if(sfpa)
		alt_2_wireInit_A();
	else
		alt_2_wireInit_B();

	if(enable){
		sfp_reg_write_diag_byte(sfpa, Status_Control, 0);		// ^6= ENABLE Tx LED
		D(1, BUG( "\n<> SFP TRANSMIT LASER: ON!)\n"));
		}
	else{
		sfp_reg_write_diag_byte(sfpa, Status_Control, 0x40);	// ^6= DISABLE Tx LED (Laser)
		D(1, BUG( "\n<> SFP TRANSMIT LASER: OFF!)\n"));
		}

}






/*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, USA.    *
* All rights reserved.                                                   		*
*************************************************************************/
