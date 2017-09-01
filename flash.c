/*************************************************************************
*		Flash Functions										*
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "sys/alt_flash.h"

//===============================================================
//	#define FLASH_PARAMETERS_BLOCK	0
//	#define FLASH_PHY_BLOCK				1
//	#define FLASH_USER1_BLOCK			2
//	#define FLASH_USER2_BLOCK			3
//	#define FLASH_SOF_ELF_BLOCKS		4 	//erase from 0 through user2 block
void erase_flash_block(unsigned int erase_block)
{
	flash_region* regions;
	alt_flash_fd* fd;
	int number_of_regions;
	int ret_code=0, sector_start_address,i;

	D(1, BUG("\n______________________Erasing Flash______________________\n"));
	fd = alt_flash_open_dev(EPCS_FLASH_CONTROLLER_0_NAME);
	if (fd){
		ret_code = alt_get_flash_info(fd, &regions, &number_of_regions);
		D(1, BUG("number_of_regions: %d\n",number_of_regions));					//number of regions (1)
		D(1, BUG("regions.offset: %d\n",regions->offset));
		D(1, BUG("regions.region_size: %d\n",regions->region_size));				//#bytes per region (8388608 = 0x80:0000 or 0x0-0x7F:FFFF)
		D(1, BUG("regions.number_of_blocks: %d\n",regions->number_of_blocks));	//Blocks==Sectors,  #Num of Sectors	(128)
		D(1, BUG("regions.block_size: %d\n",regions->block_size));					//Block-size = bytes/block == bytes/Sector (65536)

		if (number_of_regions && (regions->offset == 0)){
			int num_bytes_in_sector = regions->block_size;
			int first_sector = regions->offset;									//	#0 for any number-of-blocks device
			int last_sector = regions->offset+(regions->number_of_blocks)-1;	//	#127 for 128 block device
			int phy_sector = regions->offset+(regions->number_of_blocks)-2;	//	#126 for 128 block device
			int user1_sector = regions->offset+(regions->number_of_blocks)-3;	//	#125 for 128 block device
			int user2_sector = regions->offset+(regions->number_of_blocks)-4;	//	#124 for 128 block device
																				//  #0->#123 for 128 device  possible sof+elf usage
			//int sector_start_address = last_sector * num_bytes_in_sector;

			//last_sector = parameters, phy_sector=2nd from last, spare sectors=3rd & 4th from last.
			//sof+elf_sectors=0through 4th from last.
			if(erase_block == FLASH_PARAMETERS_BLOCK){
				D(1, BUG("Erasing Flash Parameter block: %d\n",last_sector));
				sector_start_address = last_sector * num_bytes_in_sector;
				ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);
				if(ret_code != 0){	D(1, BUG("**** ERASE: Cannot Erase!  ****")); }

				}
			else if(erase_block == FLASH_PHY_BLOCK){
				D(1, BUG("Erasing Flash PHY block: %d\n",phy_sector));
				sector_start_address = phy_sector * num_bytes_in_sector;
				ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);
				if(ret_code != 0){	D(1, BUG("**** ERASE: Cannot Erase!  ****")); }
				}
			else if(erase_block == FLASH_USER1_BLOCK){
				D(1, BUG("Erasing Flash User1 block: %d\n",user1_sector));
				sector_start_address = user1_sector * num_bytes_in_sector;
				ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);
				if(ret_code != 0){	D(1, BUG("**** ERASE: Cannot Erase!  ****")); }
				}
			else if(erase_block == FLASH_USER2_BLOCK){
				D(1, BUG("Erasing Flash User2 block: %d\n",user2_sector));
				sector_start_address = user2_sector * num_bytes_in_sector;
				ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);
				if(ret_code != 0){	D(1, BUG("**** ERASE: Cannot Erase!  ****")); }
				}
			else if(erase_block == FLASH_SOF_ELF_BLOCKS){
				D(1, BUG("Erasing Flash SOF+ELF blocks: "));

				//first_sector = regions->offset;									//	#0 for any number-of-blocks device
				//user2_sector = regions->offset+(regions->number_of_blocks)-4;	//	#124 for 128 block device
				for(i=first_sector; i<user2_sector; i++){
					D(1, BUG("%d, ",i));
					sector_start_address = i * num_bytes_in_sector;
					ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);
					if(ret_code != 0){	D(1, BUG("**** ERASE: Cannot Erase Block! <exiting eraser>  ****"));  break;}
					}
				}


			/***
			 // erase from start_block_num through N num of blocks
			for(i=0; (i<num_of_blocks) && ((block_num+i) <= regions->number_of_blocks); i++){
				D(1, BUG("Erasing Flash block: %d\n",block_num+i));
				selected_sector = regions->offset+block_num+i;
				sector_start_address = selected_sector * num_bytes_in_sector;
				// Erase the last block/sector (pass offset to start address in desired block/sector and it's size
				ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);
				}
			****/
			}//end of regions!=0

		alt_flash_close_dev(fd);
		}
	else{
		D(1, BUG("**** ERASE: Cannot Open Flash device ****"));
		}
}

//===============================================================
void write_flash_parameter_block() // erases then writes Flashparameters

{
	flash_region* regions;
	alt_flash_fd* fd;
	int number_of_regions;
	int ret_code;

	D(1, BUG("\nWriting Flash: Erase & Write FlashParameters to Flash block\n"));
	fd = alt_flash_open_dev(EPCS_FLASH_CONTROLLER_0_NAME);
	if (fd) {
		ret_code = alt_get_flash_info(fd, &regions, &number_of_regions);
		D(1, BUG("number_of_regions: %d\n",number_of_regions));					//number of regions (1)
		D(1, BUG("regions.offset: %d\n",regions->offset));
		D(1, BUG("regions.region_size: %d\n",regions->region_size));				//#bytes per region (8388608 = 0x80:0000 or 0x0-0x7F:FFFF)
		D(1, BUG("regions.number_of_blocks: %d\n",regions->number_of_blocks));	//Blocks==Sectors,  #Num of Sectors	(128)
		D(1, BUG("regions.block_size: %d\n",regions->block_size));					//Block-size = bytes/block == bytes/Sector (65536)

		if (number_of_regions && (regions->offset == 0)) {
			int last_sector = regions->offset+(regions->number_of_blocks)-1;
			int num_bytes_in_sector = regions->block_size;
			int sector_start_address = last_sector * num_bytes_in_sector;

			D(1, BUG("Erasing last Flash block\n"));
			// Erase the last block/sector (pass offset to start address in desired block/sector and it's size
			ret_code = alt_erase_flash_block(fd, sector_start_address, num_bytes_in_sector);

			if(ret_code == 0) {
				// Write a particular flash block:
				// 	block_offset is the offset (from the base of flash) to start of the block
				//   data_offset is the offset (from the base of flash) where you wish to start programming
				D(1, BUG("Writing FlashParameters to last flash sector/block)\n"));
				//								block/sector offset    data_offset         *data     length
				ret_code = alt_write_flash_block (fd, last_sector, sector_start_address, FlashParameters, PARA_LENGTH);
				if(ret_code == 0) {
					D(1, BUG("Writing FlashParameters OKAY!!)\n"));
				}
				else{
					D(1, BUG("Writing FlashParameters ERR %d)\n",ret_code));
				}
			}
		}//end of regions!=0

		alt_flash_close_dev(fd);
	}
	else {
		D(1, BUG("**** ERASE-WRITE: Cannot Open Flash device ****"));
	}
}

//===============================================================
void read_flash_parameters()	// read FlashParameter block and load into array
{
	flash_region* regions;
	alt_flash_fd* fd;
	int number_of_regions;//,i;
	int ret_code;
	//unsigned char TestParameters[40];

	D(1, BUG("\nReading Flash: FlashParameter block and load into array\n"));
	fd = alt_flash_open_dev(EPCS_FLASH_CONTROLLER_0_NAME);
	if (fd) {
		ret_code = alt_get_flash_info(fd, &regions, &number_of_regions);
		D(1, BUG("number_of_regions: %d\n",number_of_regions));					//number of regions (1)
		D(1, BUG("regions.offset: %d\n",regions->offset));
		D(1, BUG("regions.region_size: %d\n",regions->region_size));			//#bytes per region (8388608 = 0x80:0000 or 0x0-0x7F:FFFF)
		D(1, BUG("regions.number_of_blocks: %d\n",regions->number_of_blocks));	//Blocks==Sectors,  #Num of Sectors	(128)
		D(1, BUG("regions.block_size: %d\n",regions->block_size));				//Block-size = bytes/block == bytes/Sector (65536)

		if (number_of_regions && (regions->offset == 0)) {
			int last_sector = regions->offset+(regions->number_of_blocks)-1;
			int num_bytes_in_sector = regions->block_size;
			int sector_start_address = last_sector * num_bytes_in_sector;

			D(1, BUG("Reading last sector/block and loading into FlashParameters array\n"));
			ret_code = alt_read_flash(fd, sector_start_address, FlashParameters, PARA_LENGTH);
			if(USE_PRINT){
				if(ret_code != 0)	D(1, BUG("\tFlash READ - failed\n"));
				//for(i=0;i<PARA_LENGTH;i++){
				//	D(1, BUG("FlashParameters array[%d]=0x%02X\n",i,FlashParameters[i]));
				//	}

				// TSE MAC A: 00-00-25-AF-xx-yy  in array like this:   MSB[3:0]=AF,25,00,00  LSB[1:0]=yy,xx
				//												stored location: 0  1  2  3            4  5
				// 10G MAC A: 00-00-25-AA-xx-yy  must convert to this: LSB[1:0]=00,00  MSB[3:0]=25,AA,xx,yy
				// stored the same screwy way - then decoded into MAC10G_0/1
				//old GUI: if(FlashParameters[0] == 0xAF && FlashParameters[1] == 0x25){
				if(FlashParameters[0] == 0x0A && FlashParameters[1] == 0x41) {  //GL: 50-64-41-0A-xx-yy
					D(1, BUG("FLASH has been initialized!\n"));
				}
				else {
					D(1, BUG("---> FLASH is NOT initialized! <---\n"));
				}
			}
		}//end of regions!=0

		alt_flash_close_dev(fd);
	}
	else {
		D(1, BUG("**** READ: Cannot Open Flash device ****"));
	}
}

//=================================================
void load_flashparameters_into_systemvars()
{
	// recover stored Parameters; Options, MAC addresses, etc. to Variables
	OPTIONS_INSTALLED = FlashParameters[PARA_OPTIONS_INSTALLED_ptr];
	OPTIONS_ENABLED = FlashParameters[PARA_OPTIONS_ENABLED_ptr];
	D(1, BUG("Flash: OPTIONS Installed:%04X Enabled:%04X (ActvL)\n",OPTIONS_INSTALLED, OPTIONS_ENABLED));

	// TSE MAC A: 00-00-25-AF-xx-yy  MSB[3:0]=AF,25,00,00  LSB[1:0]=yy,xx
	// TSE MAC B: 00-00-25-BF-xx-yy

	//Greenlee
	// TSE MAC A: 50-64-41-0A-xx-yy  MSB[3:0]=0A,41,64,50  LSB[1:0]=yy,xx
	// TSE MAC B: 50-64-41-0B-xx-yy

	MAC_0 = BytesToLong(FlashParameters,PARA_MACA_MSB3_ptr);
	MAC_1 = (unsigned int) BytesToInt(FlashParameters,PARA_MACA_LSB1_ptr);
	MAC_0B = BytesToLong(FlashParameters,PARA_MACB_MSB3_ptr);
	MAC_1B = (unsigned int) BytesToInt(FlashParameters,PARA_MACB_LSB1_ptr);
	D(1, BUG("Flash: TSE MAC-A address from flash is: %04X%04X\n",MAC_1, MAC_0));
	D(1, BUG("Flash: TSE MAC-B address from flash is: %04X%04X\n",MAC_1B, MAC_0B));

	// 10G MAC A: 00-00-25-AA-xx-yy  LSB[1:0]=00,00  MSB[3:0]=25,AA,xx,yy
	// 10G MAC B: 00-00-25-BB-xx-yy

	//Greenlee
	// 10G MAC A: 50-64-41-1A-xx-yy  LSB[1:0]=50,64  MSB[3:0]=41,1A,xx,yy
	// 10G MAC B: 50-64-41-1B-xx-yy

	//temp DEBUG don't read/replace default 10G MAC's from flash
	MAC10G_0 = BytesToLong(FlashParameters,PARA_MAC10A_MSB3_ptr); // 10G MAC addr stored into parameters in 'clean' un-scrambled
	MAC10G_1 = (unsigned int) BytesToInt(FlashParameters,PARA_MAC10A_LSB1_ptr);
	//until we have 2nd 10G port will read FF's here.....
	MAC10G_0B = BytesToLong(FlashParameters,PARA_MAC10B_MSB3_ptr);
	MAC10G_1B = (unsigned int) BytesToInt(FlashParameters,PARA_MAC10B_LSB1_ptr);
	D(1, BUG("Flash: 10G MAC-A address from flash is: %04X%04X\n",MAC10G_1, MAC10G_0));
	D(1, BUG("Flash: 10G MAC-B address from flash is: %04X%04X\n",MAC10G_1B, MAC10G_0B));

	//SERNUM3 = BytesToLong(FlashParameters,SERNUM3_ptr);
	//SERNUM4 = FlashParameters[SERNUM4]; // MSB
}

//=================================================
void save_systemvars_to_flashparameters()
{
	// Save Parameters to Flash buffer; Options, MAC addresses, etc.
	//SaveBytesInt(FlashParameters,PARA_OPTIONS1_ptr,SYS_OPTIONS);
	FlashParameters[PARA_OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	FlashParameters[PARA_OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;
	ConfigStat[OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	ConfigStat[OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;
	ConfigStatB[OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	ConfigStatB[OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;
	ConfigStat10G[OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	ConfigStat10G[OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;

	SaveBytesLong(FlashParameters,PARA_MACA_MSB3_ptr,MAC_0);
	SaveBytesInt(FlashParameters,PARA_MACA_LSB1_ptr,MAC_1);
	SaveBytesLong(FlashParameters,PARA_MACB_MSB3_ptr,MAC_0B);
	SaveBytesInt(FlashParameters,PARA_MACB_LSB1_ptr,MAC_1B);

	SaveBytesLong(FlashParameters,PARA_MAC10A_MSB3_ptr,MAC10G_0);
	SaveBytesInt(FlashParameters,PARA_MAC10A_LSB1_ptr,MAC10G_1);
//11-14	SaveBytesLong(FlashParameters,PARA_MAC10B_MSB3_ptr,MAC10G_0B);
//11-14	SaveBytesInt(FlashParameters,PARA_MAC10B_LSB1_ptr,MAC10G_1B);

	//SaveBytesLong(FlashParameters,SERNUM3_ptr,SERNUM3);
	//FlashParameters[SERNUM4] = SERNUM4;
}


//-------------------------------------------------------------------------
//===============================================================
void read_flash_qtx_block()	// read FlashParameter block and load into array
{
	flash_region* regions;
	alt_flash_fd* fd;
	int number_of_regions,i;
	int ret_code;
	//unsigned char TestParameters[40];

	D(1, BUG("\nReading Flash: QT2025 FW block (2nd to last) and display 1st 128 bytes\n"));
	fd = alt_flash_open_dev(EPCS_FLASH_CONTROLLER_0_NAME);
	if (fd){
		ret_code = alt_get_flash_info(fd, &regions, &number_of_regions);
		D(1, BUG("number_of_regions: %d\n",number_of_regions));					//number of regions (1)
		D(1, BUG("regions.offset: %d\n",regions->offset));
		D(1, BUG("regions.region_size: %d\n",regions->region_size));			//#bytes per region (8388608 = 0x80:0000 or 0x0-0x7F:FFFF)
		D(1, BUG("regions.number_of_blocks: %d\n",regions->number_of_blocks));	//Blocks==Sectors,  #Num of Sectors	(128)
		D(1, BUG("regions.block_size: %d\n",regions->block_size));				//Block-size = bytes/block == bytes/Sector (65536)

		if (number_of_regions && (regions->offset == 0)){
			int second_tolast_sector = regions->offset+(regions->number_of_blocks)-2;
			int num_bytes_in_sector = regions->block_size;
			int sector_start_address = second_tolast_sector * num_bytes_in_sector;

			D(1, BUG("Reading sector/block# %d and loading into FlashQTX array\n",second_tolast_sector));
			ret_code = alt_read_flash(fd, sector_start_address, FlashQTX, 128); // read 100 bytes
			if(USE_PRINT){
				if(ret_code != 0)	D(1, BUG("\tFlash READ - failed\n"));
				for(i=0;i<128;i++){
					D(1, BUG("FlashQTX array[%d]=0x%02X\n",i,FlashQTX[i]));
					}

				}
			}//end of regions!=0

		alt_flash_close_dev(fd);
		}
	else{
		D(1, BUG("**** READ: Cannot Open Flash device ****"));
		GIGE_STATUS = 0x01; // Cannot read FLASH device
		}
}

//===============================================================================
//===============================================================================
void read_flash_qtx_block_and_write_phy()	// read FlashParameter block and write to the phy via MDIO
{
	flash_region* regions;
	alt_flash_fd* fd;
	int number_of_regions;
	unsigned int i=0;
	int ret_code;
	unsigned char fw_byte[2];

	D(1, BUG("\nReading Flash: QT2025 FW block (2nd to last) and WRITE to QT2025 PHY!\n"));
	fd = alt_flash_open_dev(EPCS_FLASH_CONTROLLER_0_NAME);
	if (fd){
		ret_code = alt_get_flash_info(fd, &regions, &number_of_regions);
		D(1, BUG("number_of_regions: %d\n",number_of_regions));					//number of regions (1)
		D(1, BUG("regions.offset: %d\n",regions->offset));
		D(1, BUG("regions.region_size: %d\n",regions->region_size));			//#bytes per region (8388608 = 0x80:0000 or 0x0-0x7F:FFFF)
		D(1, BUG("regions.number_of_blocks: %d\n",regions->number_of_blocks));	//Blocks==Sectors,  #Num of Sectors	(128)
		D(1, BUG("regions.block_size: %d\n",regions->block_size));				//Block-size = bytes/block == bytes/Sector (65536)

		if (number_of_regions && (regions->offset == 0)){
			int second_tolast_sector = regions->offset+(regions->number_of_blocks)-2;
			int num_bytes_in_sector = regions->block_size;
			int sector_start_address = second_tolast_sector * num_bytes_in_sector;

			D(1, BUG("Reading sector/block# %d and writing to QT2025 PHY\n",second_tolast_sector));
			//The first 16kB of memory is located in the address range 3.8000h - 3.BFFFh.
			//		...This 16kB memory has an offset of 0x0 - 0x3FFFh.
			D(1, BUG("Reading Flash and Writing PHY (part 1)...\n"));
			ret_code = 0;
			for(i=0; i<0x4000 && ret_code==0; i++){				 // starting at 0x0000 through 0x3FFF
				ret_code = alt_read_flash(fd, sector_start_address+i, fw_byte, 1); // read 1 byte
				if(ret_code != 0){
					D(1, BUG("\tFlash READ - failed\n"));
					GIGE_STATUS = 0x01; // cannot Open/Read Flash device
					}
				write_ext_10G_phy(0x8000+i, 3, fw_byte[0]);						 // and write it to PHY at 3.8000-3.BFFF
				//uncomment this to read the code in the 10G
			//	D(1, BUG("Read: %02X at %04X and writing to PHY\n", fw_byte[0], i));
				}

			D(1, BUG("Read: %02X at %04X and writing to PHY\n", fw_byte[0], i-1));

			//The next 8kB of memory is located at 4.8000h-4.9FFFh.
			//		....The 8kB memory space has an offset of 0x4000h-0x5FFFh.
			D(1, BUG("\nReading Flash and Writing PHY (part 2)...\n"));
			ret_code = 0;
			for(i=0; i<0x2000 && ret_code==0; i++){			 // from 0x4000 through 0x5FFF
				ret_code = alt_read_flash(fd, sector_start_address+0x4000+i, fw_byte, 1); // read 1 byte
				if(ret_code != 0){
					D(1, BUG("\tFlash READ - failed\n"));
					GIGE_STATUS = 0x01; // cannot Open/Read Flash device
					}
				write_ext_10G_phy(0x8000+i, 4, fw_byte[0]);						 // and write it to PHY at 4.8000-4.9FFF
				//D(1, BUG("Read: %02X at %04X and writing to PHY\n", fw_byte[0], i+0x4000));
				}

			D(1, BUG("Read: %02X at %04X and writing to PHY\n", fw_byte[0], i-1));
			}//end of regions!=0

		alt_flash_close_dev(fd);
		}
	else{
		D(1, BUG("**** READ: Cannot Open Flash device ****"));
		GIGE_STATUS = 0x01; // cannot Open Flash device
		}
}










/*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, USA.    *
* All rights reserved.                                                   		*
*************************************************************************/
