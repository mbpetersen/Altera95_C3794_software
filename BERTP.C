/************************************************************************/
/*** BERT (2172 chip) set-up and execution module					  ***/
/*** module: void write_bert()										  ***/
/**  		Set the BERT variable (unsigned char) with the desired 	  ***/
/***			Bert pattern selection 								  ***/
/************************************************************************/
/*****************************************************************/
/*** BERT SET-UP and EXECUTION ***********************************/
/*****************************************************************/
#include <DS310A.h>
#include <extdef.h>
#include <extvar.h>


/***********************************************************************/
void PRI_bert()		//** PRI or DS0 Drop&Insert BERT Processing **
/***********************************************************************/
{	
	/******************************************************/
	/***** BERT SYNC VERIFICATION ROUTINE *****************/
	/******************************************************/
	/*** First Handle going IN and Falling OUT of SYNC  ***/
	/******************************************************/
	BERT_STATUSP = MVAR(char,BERTSRADDR);		// Read BERT Status Register

	// if SYNC && in EXT clock and Clock < 10bps
	if(DatacommStat[MODE_ptr] == Mode_SYNC && DatacommStat[CLOCK_ptr] == Clock_EXT){
		if(BytesToLong(DatacommStat,DCFREQ3_ptr) == 0){
			BERT_STATUSP |= 0x10;	// force set RLOSP,  loss of SYNC
			BERT_STATUSP &= ~0x01;	// force clr SYNCP,	 no SYNC
			}
		}
	else if(DatacommStat[MODE_ptr] == Mode_ASYNC){
		if(BytesToLong(DatacommStat,DCFREQ3_ptr) == 0){
			BERT_STATUSP |= 0x10;	// force set RLOSP,  loss of SYNC
			BERT_STATUSP &= ~0x01;	// force clr SYNCP,	 no SYNC
			}
		}


	/***************************************************************************/
	if(RLOSP){ 							// If RLOS and...
		if(BERT_STATEP&0x80){   		/* if we were in SYNC (or PatLOST) */
			BERT_STATEP |= 0x40; 		/* flag loss of LOCK condition     */

		    if(BERTDC){
				DCMisc_stat |= 0x01;      // Send a Bleep in PDA
				DatacommStat[DCMISC_STAT_ptr] = DCMisc_stat;
				}
			else{
				if(MISCXIL&0x40){			// if DDS BERT is using 2172!
					if(DDSStat[DDSBERT_ptr]){	// if DDS BERT is ON
						DMisc_stat |= 0x01;     // Send a Bleep in PDA
						DDSStat[DMISC_STAT_ptr] = DMisc_stat;
						}
					}
				/*** 06/21/16
				else if(BERT_MUX_PRI){		 // If BERT is MUXd to PRI
					PMisc_stat |= 0x01;      // Send a Bleep in PDA	
					PriStat[PMISC_STAT_ptr] = PMisc_stat;
					}
				*****/
				}
			}
		}
	/***************************************************************************/        	
	if(SYNCP){      // if 2172 thinks we're in SYNC then we'd better check!           
		if(RA1A0P){ 	// If we're receiving All1's or All0's...check if we're sending ALL1's or ALL0's
			if(RALL1P && BERTP==7)  		// if we see All1 and we're sending ALL1, then...
				BERT_SYNCP = YES;  			/* VALID A1+A0 SYNC */
			else if(RALL0P && BERTP==8)  // if we see All0 and we're sending ALL0, then...
				BERT_SYNCP = YES;  			/* VALID A1+A0 SYNC */               
			else{
				BERT_SYNCP = NO;   			/* invalid A0+A1 SYNC */              
				}
			incntr = SYNCINCNT;        	/* set-up going in  cntr to 5 consecutive */
			outcntr = 1;         			/* set-up going out cntr to 1 consecutive */
			}
		else if(BERT_CONTROLP&0x20){		/* if QRW and SYNC then we're in SYNC! */
			BERT_SYNCP = YES;     			/* VALID QRW SYNC */
			incntr = SYNCINCNT;  			/* set-up going in  cntr to 5 consecutive */
			outcntr = 1;         			/* set-up going out cntr to 1 consecutive */       
			}
		else{										/* If NOT ALL1's, ALL0's or a QRW then.... */
			//for(i=0;i<5;i++)// huh??? hmmmm...let's speed up this puppy for DS3+ rates!
			check_fixed();       		/* validate fixed pattern sync */
			}
		}
	
	/***** NOT IN SYNC or we have LOS (and we're not in OOF ??? or self-lpbk ???) *****/
	else{  // if we've got an intr and it's NOT SYNC it must be RLOS
		incntr = SYNCINCNT;         /* set-up going in cntr to 5 consecutive  */
		outcntr = SYNCOUTCNT;       /* set-up going out cntr to 5 consecutive */
		BERT_SYNCP = NO;
		MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x02; /* force RESYNC */
		MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
		}
		
	/********************************************************/
	/*** Then Handle BERT SYNC state changes and flag PDA ***/
	/********************************************************************************/
	//**** ^7=IN-SYNC (1=InSYNC),  ^6=2+SYNC: (0=1stSYNC,1=subsequentSYNC)
	//*** gPatSync	(gStatus[BSR_ptr]&0xC0)	*******	if !([BSR_ptr]&0x80) is NO SYNC
	//												if [BSR_ptr]&0xC0==0x80 is SYNC
	//												if [BSR_ptr]&0xC0==0xC0 is PatLOST
	/********************************************************************************/ 
	if(BERTP && !(BERTP&0x80)){ /*** if BERT is ON (and we're not doing a lpbk), handle in/out sync ***/
		if(BERT_SYNCP){         /******** LOCKED ***********************************/
			if(!(BERT_STATEP&0x80)){ 	// If we're not already in SYNC then...
				BERT_STATEP |= 0x80;		// Set SYNC flag.
				restart_bertP();			// Reset all BERT Counters

				if(BERTDC){
					SaveBytesLong(DatacommStat,DCSECONDS3_ptr,0);	// RESET Elapsed Timer
					}
				else{
					if(MISCXIL&0x40){		// if DDS BERT is using 2172!
						if(DDSStat[DDSBERT_ptr])	// if DDS BERT is ON
							SaveBytesLong(DDSStat,DSECONDS3_ptr,0);	// RESET Elapsed Timer
						}
					/*** 06/21/16
					else if(BERT_MUX_PRI)		// If BERT MUXd to PRI
						SaveBytesLong(PriStat,PSECONDS3_ptr,0);	// RESET Elapsed Timer
					****/
					}
				}
			}
		else{ /******** NOT LOCKED *******************************/
			if(BERT_STATEP&0x80){      	/* if we were in SYNC (or PatLOST) */
				BERT_STATEP &= ~0x80;    // Clear SYNC flag.
				BERT_STATEP |= 0x40;     /* flag loss of LOCK condition     */
				if(BERTDC){
					DCMisc_stat |= 0x01;      // Send a Bleep in PDA
					DatacommStat[DCMISC_STAT_ptr] = DCMisc_stat;
					}
				else{
					if(MISCXIL&0x40){		// if DDS BERT is using 2172!
						if(DDSStat[DDSBERT_ptr]){	// if DDS BERT is ON
							DMisc_stat |= 0x01;      // Send a Bleep in PDA
							DDSStat[DMISC_STAT_ptr] = DMisc_stat;
							}
						}
					/*** 06/21/16								
					else if(BERT_MUX_PRI){		// If BERT MUXd to PRI
						PMisc_stat |= 0x01;      // Send a Bleep in PDA
						PriStat[PMISC_STAT_ptr] = PMisc_stat;	
						}
					*****/
					}
				}
			}

		if(BERTDC){
			DatacommStat[DCBSR_ptr] = BERT_STATEP;			// pass current SYNC state
			DDSStat[DDSBSR_ptr] = BERT_STATEP&(~0x80);		// force NOT IN SYNC
			//06/21/16 PriStat[PBSR_ptr] = BERT_STATEP&(~0x80);		// force NOT IN SYNC
			}
		else{	
			if(MISCXIL&0x40){		// if DDS BERT is using 2172!
				if(DDSStat[DDSBERT_ptr]){	// if DDS BERT is ON
					DDSStat[DDSBSR_ptr] = BERT_STATEP;				// pass current SYNC state
					//06/21/16 PriStat[PBSR_ptr] = BERT_STATEP&(~0x80);		// force NOT IN SYNC
					DatacommStat[DCBSR_ptr] = BERT_STATEP&(~0x80);	// force NOT IN SYNC
					}
				}
			else if(BERT_MUX_PRI){		// If BERT MUXd to PRI
				//06/21/16 PriStat[PBSR_ptr] = BERT_STATEP;				// pass current SYNC state
				DDSStat[DDSBSR_ptr] = BERT_STATEP&(~0x80);		// force NOT IN SYNC
				DatacommStat[DCBSR_ptr] = BERT_STATEP&(~0x80);	// force NOT IN SYNC
				}
			}
		}/* END if PRI BERT IS ON */          	
	/**************************************************/
	/*********** END BERT-PRI SYNC HANDLING ***********/
	/**************************************************/      		      
}



/***********************************************************************/
void write_bertP()
/***********************************************************************/
{

	BERT_STATUSP = MVAR(char,BERTSRADDR);/* Dummy READ to flush BERT Status Register */
	
	incntr  = SYNCINCNT; /* reset in/out fixed-pat sync cntr if new bert! */
	outcntr = SYNCOUTCNT;
	BERT_SYNCP = NO;
	
	/********************************************************/
	MVAR(char,PSR3_ADDR) = 0xFF; /* Pattern Set register to QRW */
	MVAR(char,PSR2_ADDR) = 0xFF; /* Pattern Set register to QRW */
	MVAR(char,PSR1_ADDR) = 0xFF; /* Pattern Set register to QRW */
	MVAR(char,PSR0_ADDR) = 0xFF; /* Pattern Set register to QRW */
	/********************************************************/
	switch(BERTP){
		case 0:     /*** BERT OFF: sending all-1's for now.... ***/
			// if not DC and in DDS_MODE (2172 routed to LXT) then any time we're not BERT we're looking for V.54!	         
			if(!BERTDC && MISCXIL&0x40){
				MISCXIL |= 0x20;			// Disable 2172 BERT Transmitter! (send All-1's)
        		MVAR(char,MISCXILADDR) = MISCXIL;
	
	         	/********************************************************/
	        	MVAR(char,PTR_ADDR)  = 0x03; /* Polynomial Tap reg for QRW */
	         	MVAR(char,PLR_ADDR)  = 0x06; /* Pattern lenth reg to 7 bits */
	         	PAT_LENGTHP = 0x06;
	         	/********************************************************/
				if((DDSStat[DRxLPBK_ptr])&0x08)	// if we're in V54, look for INV V54!
					BERT_ERRREGP = 0x30;          /* Set Tx && Rx INV */
				else
		         	BERT_ERRREGP = 0x00;      /* shut-off error insertion!   */
	         	MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
	         	/********************************************************/
	         	BERT_CONTROLP = 0x60;     /* PCR to psuedorandom, resync */
	         	MVAR(char,PCR_ADDR)  = BERT_CONTROLP; /* PCR.6 ZCS ON w/QRSS */
	         	MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
	         	MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
	         	/********************************************************/
				}
			else{
				USER_PATP = 0x00000001;
				/********************************************************/
				MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
				MVAR(char,PLR_ADDR)  = 0x00; /* Pattern lenth reg to 1 bit  */
				PAT_LENGTHP = 0x00;
				/********************************************************/
				BERT_ERRREGP = 0x00;      	/* no error insertion!         */
				MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
				/********************************************************/
				BERT_CONTROLP = 0x00;     	/* PCR to repeating pat,resync */
				MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
				MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
				MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
				/********************************************************/
				}
			break;
	  case 1:     /*** QRSS BERT ***/
	  case 41:    /**** B-TAP: QRSS ****/
         /*********************************************************/
         MVAR(char,PTR_ADDR)  = 0x10; /* Polynomial Tap reg for QRSS  */
         MVAR(char,PLR_ADDR)  = 0x13; /* Pattern lenth reg to 20 bits */
         PAT_LENGTHP = 0x13;
         /*********************************************************/
         BERT_ERRREGP = 0x00;      /* shut-off error insertion!   */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x60;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP; /* PCR.6 ZCS ON w/QRSS */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 2:     /*** 2047 BERT ***/
         /*********************************************************/
         MVAR(char,PTR_ADDR)  = 0x08; /* Polynomial Tap reg for 2047  */
         MVAR(char,PLR_ADDR)  = 0x0A; /* Pattern lenth reg to 11 bits */
         PAT_LENGTHP = 0x0A;
         /*********************************************************/
         BERT_ERRREGP = 0x00;      /* shut-off error insertion!   */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 3:     /*** 2.23 BERT ***/    
         /*********************************************************/
         MVAR(char,PTR_ADDR)  = 0x11; /* Polynomial Tap reg for 2.23  */
         MVAR(char,PLR_ADDR)  = 0x16; /* Pattern lenth reg to 23 bits */
         PAT_LENGTHP = 0x16;
         /*********************************************************/
         BERT_ERRREGP = 0x30;     /* T/R INV & no error insert!  */
         /* BERT_ERRREGP = 0x00;             * no error insertion!   */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 4:     /*** 2.20 BERT ***/     
         /*********************************************************/
         MVAR(char,PTR_ADDR)  = 0x10; /* Polynomial Tap reg for 2.20  */
         MVAR(char,PLR_ADDR)  = 0x13; /* Pattern lenth reg to 20 bits */
         PAT_LENGTHP = 0x13;
         /*********************************************************/
         BERT_ERRREGP = 0x00;             /* no error insertion!  */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 5:     /*** 2.15 BERT ***/
         /*********************************************************/
         MVAR(char,PTR_ADDR)  = 0x0D; /* Polynomial Tap reg for 2.15  */
         MVAR(char,PLR_ADDR)  = 0x0E; /* Pattern lenth reg to 15 bits */
         PAT_LENGTHP = 0x0E;
         /*********************************************************/
         BERT_ERRREGP = 0x30;     /* T/R INV & no error insert!  */
         /* BERT_ERRREGP = 0x00;             * no error insertion!   */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 6:     /*** 3:24 BERT (3-in-24) ***/
      case 40:    /**** B-TAP: 3in24 ****/
         /********************************************************/
            /* MSB */
         MVAR(char,PSR2_ADDR) = 0x44; /* Pattern Set register to FIX */
         MVAR(char,PSR1_ADDR) = 0x00; /* Pattern Set register to FIX */
         MVAR(char,PSR0_ADDR) = 0x04; /* Pattern Set register to FIX */
         USER_PATP = 0x00440004; 
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x17; /* Pattern lenth reg to 24bits */
         PAT_LENGTHP = 0x17;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 7:     /*** ALL1 BERT ***/
      case 9:     /**** OTHERS: more ****/
      case 21:    /**** B-TAP: All-1 ****/
         /********************************************************/
         USER_PATP = 0x00000001;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x00; /* Pattern lenth reg to 1 bit  */
         PAT_LENGTHP = 0x00;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 8:     /***  ALL0 BERT ***/
         /********************************************************/ 
         MVAR(char,PSR0_ADDR) = 0xFE; /* Pattern Set register to FIX */
         USER_PATP = 0x00000000;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x00; /* Pattern lenth reg to 1 bit  */
         PAT_LENGTHP = 0x00;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      /* case 9:   *** OTHERS: more   ****/
      case 10:    /*** OTHERS: V.54 BERT (was 1in16) ***/
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x03; /* Polynomial Tap reg for QRSS */
         MVAR(char,PLR_ADDR)  = 0x06; /* Pattern lenth reg to 7 bits */
         PAT_LENGTHP = 0x06;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* shut-off error insertion!   */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x60;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP; /* PCR.6 ZCS ON w/QRSS */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 11:     /**** OTHERS: 2:8 (2-in-8) ****/
         /********************************************************/
         MVAR(char,PSR3_ADDR) = 0xFF; /* MSB */ 
         MVAR(char,PSR2_ADDR) = 0xFF; 
         MVAR(char,PSR1_ADDR) = 0xFF; 
         MVAR(char,PSR0_ADDR) = 0x42;  /* 0x48 */
         USER_PATP = 0x00000042;    /* 0x48 */
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x07; /* Pattern lenth reg to 8 bits */
         PAT_LENGTHP = 0x07;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 12:     /**** OTHERS: 1:7 (1 and 7) ****/
      case 26:     /****  B-TAP: 1:7  ****/
         /********************************************************/
         MVAR(char,PSR0_ADDR) = 0x40; /* 0x40 MSB */
         USER_PATP = 0x00000040;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x07; /* Pattern lenth reg to 8 bits */
         PAT_LENGTHP = 0x07;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 13:     /**** OTHERS: more   ****/
         break;/**** pointer to users not used for bert selection */
      case 14:     /**** OTHERS: ALT ****/
      case 22:     /****  B-TAP: 1:1 ****/
         /********************************************************/
         MVAR(char,PSR0_ADDR) = 0xFD; /* Pattern Set register to FIX */
         USER_PATP = 0x00000001;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x01; /* Pattern lenth reg to 2 bits */
         PAT_LENGTHP = 0x01;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 15:     /**** OTHERS: 63 ****/
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x04; /* Polynomial Tap reg for QRW  */
         MVAR(char,PLR_ADDR)  = 0x05; /* Pattern lenth reg to 6 bits */
         PAT_LENGTHP = 0x05;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 16:    /***  OTHERS: 511 BERT ***/
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x04; /* Polynomial Tap reg for 511  */
         MVAR(char,PLR_ADDR)  = 0x08; /* Pattern lenth reg to 9 bits */
         PAT_LENGTHP = 0x08;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 17:     /**** OTHERS: more  ****/
         break;/**** pointer to users not used for bert selection */
      case 18:     /**** OTHERS: NET55 ****/
      case 19:     /**** OTHERS: DALY  ****/
         break;

	  //*** BRIDGED-TAP PATTERNS WERE HERE *****************
      case 24:     /**** B-TAP: 1:5   ****/
         /********************************************************/
         MVAR(char,PSR0_ADDR) = 0x10; /* MSB */
         USER_PATP = 0x00000010;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x05; /* Pattern lenth reg to 6 bits */
         PAT_LENGTHP = 0x05; 
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 25:     /**** B-TAP: 1:6   ****/
         /********************************************************/
         MVAR(char,PSR0_ADDR) = 0x20; /* MSB */
         USER_PATP = 0x00000020;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x06; /* Pattern lenth reg to 7 bits */
         PAT_LENGTHP = 0x06;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      /* case 26 == case 12   /**** B-TAP: 1:7   ****/

      /****************** DS3 PATTERNS *********************************/   
      case 43: /* 1100 DS3 Pattern */
         /********************************************************/
         MVAR(char,PSR0_ADDR) = 0x03; 
         USER_PATP = 0x00000003;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x03; /* Pattern lenth reg to 4 bits */
         PAT_LENGTHP = 0x03;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;
      case 44: /* 1000 DS3 Pattern */
         /********************************************************/
         MVAR(char,PSR0_ADDR) = 0x01; 
         USER_PATP = 0x00000001;
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x00; /* Polynomial Tap reg for FIX  */
         MVAR(char,PLR_ADDR)  = 0x03; /* Pattern lenth reg to 4 bits */
         PAT_LENGTHP = 0x03;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x00;     /* PCR to repeating pat,resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;         

         /********************************************************/
      case 50:  /**** SPECIAL: 511 (LPBK in PROGRESS) BERT! ****/
         /********************************************************/
         MVAR(char,PTR_ADDR)  = 0x04; /* Polynomial Tap reg for 511  */
         MVAR(char,PLR_ADDR)  = 0x08; /* Pattern lenth reg to 9 bits */
         PAT_LENGTHP = 0x08;
         /********************************************************/
         BERT_ERRREGP = 0x00;      /* no error insertion!         */
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         /********************************************************/
         BERT_CONTROLP = 0x20;     /* PCR to psuedorandom, resync */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      /* PCR.6 ZCS OFF! */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   /* load pattern */
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         /********************************************************/
         break;

      /******************
      case 0x7F:     **** OTHERS: USER 32bit ***
      ********************************************************
         MVAR(char,PSR3_ADDR) = (USER_PATP&0xFF000000)>>24; 
         MVAR(char,PSR2_ADDR) = (USER_PATP&0x00FF0000)>>16; 
         MVAR(char,PSR1_ADDR) = (USER_PATP&0x0000FF00)>>8; 
         MVAR(char,PSR0_ADDR) = (USER_PATP&0x000000FF); 
         ********************************************************
         MVAR(char,PTR_ADDR)  = 0x00; * Polynomial Tap reg for FIX  *
         MVAR(char,PLR_ADDR)  = 0x1F; * Pattern lenth reg to 32bits *
         PAT_LENGTHP = 0x1F;
         ********************************************************
         BERT_ERRREGP = 0x00;      * no error insertion!         *
         MVAR(char,EIR_ADDR)  = BERT_ERRREGP; 
         ********************************************************
         BERT_CONTROLP = 0x00;     * PCR to repeating pat,resync *
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;      * PCR.6 ZCS OFF! *
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x80;   * load pattern *
         MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
         ********************************************************
         break;
      case 0x80:     *** currently unused and undefined ***
         break;
      ***************/

		//***** LOOPBACKS USED TO BE HERE ******************
         
      default:
         break;
      }   
    	
   if(BERTP){
		if(BERTP != bert_v54)	// if sending a V54 it's a lpbk not a BERT...
	        BERT_ONP = YES;		// ...turn off this bit so we don't gate BERT processing!
		else{                   // We're sending V.54 (on purpose) so...
			MISCXIL &= ~0x20;		// ...Enable 2172 BERT Transmitter!
      		MVAR(char,MISCXILADDR) = MISCXIL;
			}

		if(BERTDC){			// was BERTDS3
		    MISCXIL2 |= DC_BERT_ON;		// BERT muxed to DC away from PRI/DS0/DDS
		    MVAR(char,MISCXIL2ADDR) = MISCXIL2;
			}
		else{
			MISCXIL2 &= ~DC_BERT_ON;	// BERT muxed away from Datacomm toward PRI/DS0/DDS
			MVAR(char,MISCXIL2ADDR) = MISCXIL2;
			}

      restart_bertP();
      BERT_STATUSP = MVAR(char,BERTSRADDR);/* Dummy READ to flush BERT Status Register */   
      }
   else{
      BERT_STATUSP = MVAR(char,BERTSRADDR);/* Dummy READ to flush BERT Status Register */
      BERT_ONP = NO;
      }
   
   //******** Clear BERT SYNC and subsequent Loss flags *********
    BERT_SYNCP = NO;
    BERT_STATEP &= ~0xC0;

	if(BERTDC){
		DatacommStat[DCBSR_ptr] = BERT_STATEP;
		}
	else{
		if(MISCXIL&0x40){		// if DDS BERT is using 2172!
			if(DDSStat[DDSBERT_ptr]){		// if DDS BERT is ON
				MISCXIL &= ~0x20;			// Enable 2172 BERT Transmitter!
      			MVAR(char,MISCXILADDR) = MISCXIL;
				DDSStat[DDSBSR_ptr] = BERT_STATEP;
				}
			}
		/*** 06/21/16
		else if(BERT_MUX_PRI)				// If BERT MUXd to PRI
	   		PriStat[PBSR_ptr] = BERT_STATEP;
		****/
		}
}
/***********************************************************************/

