Readme - MicroC/OS-II Version

>>> When changing between project Configuration (Release, Debug)
>>> ...ReFresh the Connection before dumping to the HW (disconnecting (red sq) doesn't do it!)
>>> ...or it dumps the last Configuration Elf instead! - STOOOPID MACHINE

___NIOS II Application Properties_____
NOTE: Debug Level: Off (don't build Symbol table into Elf) On (do build) 1,2 or 3 logging level
NOTE: Optimization level <--- Altera recommends Level-2 for good NIOS II execution speed BUT;
	1) sfp reads (power) are intermittent, but our for-loop based delay could be cause
	2) GUI cfg_hw cmd to refresh sfp - never elicits "Sending Port-X SpecialStat[] packet" msg!  why?? 
	

_____2013_________________________________________________
		NOTE THIS VERSION WILL HAVE empty (FF/00) MAC's in FLASH.....		
03/26	tse+10G Project+bsp built and TSE tested (quick) OK.  2048-FIFO's
04/02 ...updated to match GIGLE_OS
	flash.c: Add QTX flash block read - to confirm mdio.binary conversion and flash-load!

06/14			Add single Port-A 10G Support NEARLY complete!
				GUI v1.07 (uses FW flag to enable 10G access)
				Packet tests Quick & RFC
				Link status & power measurement
				SFP+ info gathering and loading
				1G/10G mode swapping checked
				
				only remaining items are;
					*) there's some MAC stat's not in 10G MAC and vice versa - think about it! 
					0) mixed TSE and 10G results viewing? - don't use flag use status link ID type ??					
					2) MAC -> Flow control (defined - just not implemented)
					3) PHY -> are we in Auto-Neg mode?
					4) SFP+ Transmit OFF control
					5) Calibration of Latency - when we have at least 2 units for sampling!
					
06/25	sof & sopcinfo GOOD (6/24/13-3:02pm,3:56pm).  Updated 10G Broadcast frame support in traffic_controller:mon					
FW: v1.030

_____2014_________________________________________________
02/12	Rebuild DSP and PROJ with 10G license

02/13	10G Test durations funky????
	//********** WHAT's GOING ON HERE??????? mbp 2/13/2014
	//SUM_DURATION_CYCLES = SUM_DURATION_CYCLES/8.0;	// descale from our verilog *8!!
	//**********
	...Why is verilog/8 no longer correct?
	
	--- NOTE: 1G latency is mostly -51nS ---  need to recalibrate!
	
03/17	MAC address writing to Flash spanked - was masked to 0000 (due to 10G MAC descrambler)
			ReWrite using int rather than RxBuffer[] to preserve settings.


04/08	New 10G Qsys to test Port passage: A to GUI via B
	cfg_hw.c:415: error: 'AVALON_ST_LOOPBACK_SOPC_WRAPPER_0_BASE' undeclared (first use in this function)
		comment out starting at line 411....
	main.c:91: error: 'AVALON_ST_LOOPBACK_SOPC_WRAPPER_0_BASE' undeclared (first use in this function)
		comment out starting at line 89....

	main.c:53  FORCE PROM mode...
		//mbp tmp: TEMP DEBUG
    	MACA_COMMAND_CONFIG |= 0x10; // force promiscuous mode!
    	MACB_COMMAND_CONFIG |= 0x10; // force promiscuous mode!   	
    	
    NOTE: bumped B-FIFO size to 512 support A-to-B routing (to support big frames)
    WILL need to bump to 4096 to support 9600 frame size (2420).
 
 	LAN-to-WAN dev mode:
 	...initialize TSE FIFO's to support new operational modes! 
 		A: RX_Section_Full = RX_SECTION_FULL_CUT; // cut-through to maintain generator/monitor functionality
 		B: RX_Section_Full = RX_SECTION_FULLB_SAF;// store & forward to support LAN-WAN mode as well as loopback function!   
    	NOTE: LAN-to-WAN mode: B-speed <= A-speed  (B connected to LAN or GUI.)
		
	TEMP DEBUG: 10G in Promiscuous mode	
		conf10g.c: line 330 changed as follows;
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_CONTROL_ADDR, EN_ALLMCAST_BIT|EN_ALLUCAST_BIT);

5/13-17________ TSE & 10G: clean-up for LAN-WAN (Ping/Browse mode for ) ________
FW:v1.040	sof & spocinfo: 5/12/14 7:36p/6:53p
GUI:v1.09	FPGA-Qsys: Verilog tweaks - LAN-WAN modes and reduced resource utilization
flash date(s) of sof+elf ***
5/29/14		 	R&R avalon_st_loopback with New homegrown Muxes 2:1 and 3:1
tested				st_mux_2_to_1, st_mux_3_to_1, st_mux10G_3_to_1
(FWv4.102)		R&R splitters with New homegrown Splitter with HS-line mux
see btm			guisys_avalon_st_splitter .sv - configurable splitter: support various ports and bus sizes
					>> Still need to add flexible config flags that update User on Mode interaction. 
				New homegrown TSE: MAC_ADDR_SWAPPER
					tse_addr_swapper
				R&R TSE Traffic controller & EGEN
				R&R 10G Traffic controller, GEN & MON
					MON: init,clr & start MON (10G end-to-end), remove rx_mac_status "sink1" (not used)				
					GEN: add GEN running flag to support end-to-end test

		Remove DEBUG promiscuous mode(s): TSE & 10G	
		
		TSE: DC-FIFO's A:Cut-through (as previous)  B: Store & Forward
			new #define's A&B:  RX_SECTION_EMPTY 	TRIPLE_SPEED_ETHERNET_0_RECEIVE_FIFO_DEPTH-16
								TX_SECTION_EMPTY 	TRIPLE_SPEED_ETHERNET_0_TRANSMIT_FIFO_DEPTH-16
		10G: SC-FIFO Store & Forward (as previous) 			
		
		Write drivers for LAN-WAN/NORMAL/TEST & LPBK modes for TSE A, B and 10G.
		NOTE LANWAN mode requires Promiscuous modes for both A & B ports (TSE:A & 10G:A as selected)
			Since Prom mode is in MAC config - GUI will force this mode upon LANWAN selection.
		
		Update cfg_hw.c/cfg10_hw.c for CORE_SIGROUTE_MODES_ptr:	//69 ^0:0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK), 1=TSE_LANWAN_MODE, ^1:0=10G_NORMAL_MODE,1=10G_LANWAN_MODE (10G+B=LANWAN,A=GEN/MON)
			TSE - GUI allows change only in PORT-A.
		
		New B-LPBK enable/disable operation.
		Tweak results.c for only DBG dump A-stat's 
		Update config.c: display Promiscuous Mode State upon change! 
		Update gui_link.c: display link-up on A, B and 10G-A for state analysis

		gui_link.c: Add LAN-WAN mode checker - ensure FPGA routing is correct based on mode (TSE/10G) we're in!
			// only if we're in 'A' - don't use 'B' to change this state, in case we're just going to peek at B stats!
		
		Fix B-port LPBK & SWAP (GUI<->FW out-of-sync) cfg_hw.c,cfg10_hw.c 
		
		FIX packet10g.c: start/stop issues with cycling 10G...TSE...A...B
			should perhaps rewrite for cleaner - TSE-style operation.
		NOTE: some shared var's Start/stop flags, EMON_TIMEOUT, etc. shared between TSE & 10G Generators
			IF we want to support dual GEN/MON in future - will need to create independent flags.

		config10g.c: send 0 for non-valid AlignmentErr in 10G. GUI displays "n/a"
		
		config10g.c, config_10G_external_phy(): NOTE since our 10G PHY has TXON n/c - it reads LOW and disables ability to Turn OFF Tx Laser
			so set INVPAD bit on that input so it reads HIGH, now we can control Tx Laser.
		write_ext_10G_phy(0xD003, 1, (read_ext_10G_phy(0xD003, 1))|0x0001); // set TXONPAD to INV input!
		
		confstat.h: PHY_SFP_MISC_ptr  ^2: SFP+ 10G Port-A Laser ON/OFF control
		Driver for 10G Tx enable/disable
			config10g.c: sfp10G_transmit_laser(_Bool sfpa, _Bool enable)
		Update cfg_hw.c & cfg10_hw.c for 10G SFP+ Tx enable/disable
		
		Add MAC10A_COMMAND_CONFIG for 10G MAC config changes.
		config10g.c: init_10G_MAC() update for MAC10 config changes
			cfg10_hw.c: MAC10 update to execute config changes (currently PROM mode)
			
		status.read_phy_operating_mode():  A & B PHY state's - also cc to ConfigStat10G 
		Add new 10G PHY status 1 & 0: #define PHY10GA_OPERATING_MODE1_ptr		MaxConfig2+37
		status.read_phy_operating_mode(): save to new 10G PHY state ptr, cc to A & B stat's
		Add new 1 & 0: PHY10G_OPT_POWER1_ptr	MaxConfig2+23	// 10G-A Power
		Update read_10G_sfp_power(_Bool selected_port) for 10G power port.


CURRENT SOF/SOPCINFO STATE: 5/12/14-7:36p/6:53p
	NOTE: 10G MUST boot with SFP+ Module INSTALLED.  Will however Link-up upon subsequent removal and re-installation.
		NEED TO LOOK FURTHER <<<<<<<<<<<<<<<<<
		....seems to be sof related - but not fully isolated: 
			one PCB works with v1.030 6/24/13 sof but NOT with current/latest sof
			XAUI Rx/Tx Lock/Link-up Issue!  (ground-hog day) 
	TSE A-B LAN/WAN mode fully functional NOTE: B (LAN port) speed must be <= A (WAN port)
	TSE-B Lpbk fully functional
	TSE-B MAC SWAP fully functional
	10G-B LAN/WAN mode functional streaming FIXED LEN ONLY at 100M (random has issues)
		10M fully functional (random as well)
	LAN/WAN mode provides support for PING, traceroute and Browse over WAN port
	10G Rx to Tx loopback in Qsys - non-functional - so no FW implemented
	10G End-to-END test mode added/fixed/supported 

5/28	Update cfg_hw & cfg10_hw for SFP A, B & 10G-A Tx ON/OFF consistency from any Port settings on GUI!	
5/29	RELEASE build & Flash Files created for AT&T.  Much analysis on 10G Latency Cal...
			verilog->NIOS->GUI; GUI .clone thread spanking statusResultsArray[0] 			
			
6/18	PHY10G_OPT_POWER1/0_ptr in duplicate location - relocate to new spot!  FW-GUI must be paired or no 10G Optical PWR!
v1.040		GUI v1.10 and FW v1.040
		OOP's: previous FW ver will display as 4.102!  fixed typo in ver coding in gigle.h	
		
7/07	Implement Jumbo Mode in MAC
			works 10M and 1G to 9600.  100M fails (worked to 9000)
			PHY must be configured to support Jumbo in 100M modes!!!
				PHY_ENABLE_COPPER_JUMBO  Implemented for both A & B.	
			FYI: PHY jumbo is 10k max	
			
		Implement Jumbo mode in tse_traffic controller - GEN/MON
			Fails 100M, 10M Copper & 1G Fiber worked...1G Copper??
			Determine that GEN - pause is too long due to !Tx_RDY
			Created and add dynamic Compensator to count and adjust Pause wait due to !Tx_RDY
			...Pause wait is reduced by time waiting on Tx_RDY during packet creation "on-the-fly" 
				Passes 100M tse GEN - through MON.  10M failed and 1G copper fails, 1G Fiber passes
				Bump GEN wait_cntr for !Tx_RDY to 32-bits --> 10M copper works
			Determination: the PHY 100M tweak for jumbo - spanks 1G Copper. 
			...must adjust PHY config for Jumbo based on SPEED
			
		10G Jumbo...
			Good to 4000.  Craps at 4100, Pausing... FIFO backing up
				Like TSE: FCS error spewing above 4100....
			What is FIFO depth 512 x 64
			init_10G_SC_FIFO() ...forcing MAC into cut-through mode....
				Works all the way to 9600.
			WHY are we in STORE & FORWARD mode??
			are there Tx_RDY issues as TSE GEN??	
7/29	TEMP >>> config10g.c:init_10G_SC_FIFO() ...force MAC into cut-through mode....
			potential issue: MAC drop corrupt Frames not activated in cut-through

	>>>TRY: REDUCE 10G SC_FIFO depth - should not need to be 512 deep in cut-through mode.
	
	NEED TO DO.  TSE B-port.  Dynamic toggle between Store & forward and Cut-through.
		Store & forward REQUIRED for LAN-WAN mode.
		cut-through REQUIRED for Jumbo support...(or deeper FIFO depth to support full 9600-octet packet)
		
10/22	hw_def.h: def cut_thru and S&F modes and config10gupdate init_10G_SC_FIFO(unsigned char fifo_mode)		

11/17	100M copper intermittent Jumbo.  independent analysis of looped-cable vs via B-port: It's B!
		Update: config.config_phyb_copper(): PHYB_ETH_SPEED == speed_100M was using PHY_ETH_SPEED
			so in/out of Speeds in A intermittently incorrectly configured B.
			
			
11/25	...start of port to Greenlee MAC addresses - requires both GUI and ELF updates...
			cfg_hw.c: look for greenlee commented out
		Comment out 10G-B port MAC usage for now - until we add 2nd 10G port!
			flash.c:save_systemvars_to_flashparameters  (recover will show FF's)
			review flash.c
		flash.c:read_flash_parameters() must check for Greenlee MAC's - when cut-over.
			
12/10 Manufacturing debug/analysis tool.....
v1.041	GigE INIT/DEBUG/FAULT STATUS: currently 10G boot/init faults
		//#define GIGERESULT_ptr	MaxConfig2+4	// GIGE HW-FW status/debug info
			GIGE_STATUS: FAILURE CODES
			0x00:	// pre-init	
			0x01;	// cannot Open/Read Flash device
			0x02; 	// No 10G PHY Heartbeat.
			0x03; 	// 10G PHY init incomplete - Timeout
			0x04; 	// 10G XAUI Tx not ready
			0x05; 	// 10G XAUI Rx not ready
			0x06; 	// 10G XAUI Both Tx & Rx not ready
12/11	Update to fix AMCC PHY I2C bug  -> config10.g:
			create: issue_10G_phy_I2C_stop_seq()
			call: in init 10G PHY - prior to loading FW	

12/15	Set B-Rx-FIFO back to SAF as needed for LAN-WAN mode (bumping FIFO to support Jumbo)
			Both 10G FIFO's (from 512 to 2048) and B-Rx-FIFO (from 512 to 4096) Need to be bumped to Support Jumbo.
			(in current architecture)
			NOTE Cut-through FIFO's can probably be changed down to 16
			NOTE 10G may not require large SAF FIFO's - but would have to tweak gEN/MON to supprt cut-through mode.
			 			
			>>>>>>>>>			
			NOTE NOTE NOTE NOTE - MAC-B FIFO in cut through mode - NO GO for LAN-WAN mode
			<<<<<<<<<
			FIFO SETTINGS and Why:
				TSE A TxFIFO  cut-through  (for GEN)
		  				  RxFIFO  cut-through  (for MON architecture)
				TSE B TxFIFO  cut-through  (default)
		  				  RxFIFO  store&forward req'd for LAN-WAN and Loopback
		  		  					   due to depth - won't work for Jumbo, cut-through works but won't support Jumbo lan-wan (do you need it?)
				10G   TxFIFO  store&forward (default for 10G GEN)
		  				 RxFIFO  store&forward (default for 10G MON)
		  		  					  due to depth - won't work for Jumbo, cut-through proves it but spanks MON.
		  		  					  and in cut-through MAC stops dropping corrupt Frames
			>>>>>>>>>


02/18-02/xx	Major FW update & overhaul to support updated GEN & MON.
v1.05---REQUIRES---GUI v1.12---------
		Update to Greenlee MAC addresses - (mated to updated GUI)
		many many updates/changes to support new GEN & MON.  Multiple GEN/MON iterations to get functioning variant
		Updated 10G GEN+MON functioning.  RND spanked - EMIX added - EMIX spanked, rx_byte_counter poo'd - new MON fixed
		Install VLAN, Stack-VLAN header generation and verify execution into Packets...
		Integrate & confirm FIXED & RND (EMIX) at various FrameSizes.
		Integrate and confirm SEQ and PRBS payload for all modes. 
		Integrate GUI control and settings for VLAN modes - regression test
03/16	JUMBO & VLAN @ 10G complete - backed up

03/18	Complete partial port for new Qsys Project with no TSE GEN/MON
				confirm 10G traffic - FAIL.  no MON data.  :(
				Bad location/missing constraints file.
03/19	New Qsys architecture fo single Traffic Engine:  10G fully functional.

03/23	ReWrite system around Single Traffic Engine and new Splitter/Mux
				Significant cfg_hw.c updates (<--cfg10_hw.c)
				cfg_10_hw.c updates (consolidation of dual traffic engines to single traffic engine) 
				create new set_generator_tse_load() to support TSE speeds
				packet10g.c: set_generator_load() 		
				main.c 
				packet10g.c: rewrite to support 10G & TSE precipitates to main.c, cfg_hw & cfg10_hw.c 
				precipitations into results.c as well...
				rewrite GEN/MON stat collector to pass to TSE ConfStat array or 10G array as appropriate.
					>>> will need to setup GUI to prevent switching while test is in progress (if I don't already)

		>> CLEAN-OUT all old TSE GEN/MON crap
				Rip out all the old TSE Traffic Engine functions & modules
				
			GEN/MON - Rx delay through TSE S&F FIFO impacting short term test Rx cycle_counter.
				The S&F delay coupled with MON 6.4nS 64-bit read results in shorter cycle interval count for packet stream - by almost the full first packet size. 
				Add Cal/adjuster to compensate for this delay - So add 1st Frame Size time in 6.4nS clks (10G MON rate) to cycle counter.	
			TSE-0 (A-port) FIFO settings: Rx has been bumped to 9600 - but Tx impacting 9600 - set to same as Rx - even thought it doesn't need to be this deep.
				Works up to 8744 - so we're running into a flow issue:
				Since we're at 64% just bump to same depth as Rx (from 512 to 4096) and review later for optimum setting				
		>> REBUILD MON to utilize Rx_Rdy signal from MAC's so we don't need deep FIFO's to support 9600 byte Frames.
				We're wasting a LOT of FPGA resources on FIFO's that with proper design can be significantly reduced. - Shifting to cut-through in most cases.
				
		>> Build in support for IPv4 and Layer-2: 802.3 & Eth.II packet types.
				
====== VLAN + JUMBO == COMPLETE == WAITING FOR AT&T Mike Spear MEETING =======================	

					
	Awaiting full & final regression ---> need 4-5 GIGE PCB's with '65' parts.
	
05/05	MFG cannot build PCB's with Altera '65' parts _AND_ no AT&T meeting on the horizon
			Since full & final regression Not completed - Engr.Dept. decision to move to Pendleton Traffic Engine
			 
	---- all kinds of new Traffic Engine tweaks and debug code-----
	Confirmed: Full Single stream TSE and 10G - across all speeds & frame sizes - including VLAN and Jumbo
	Confirmed: Full Multi-Stream TSE & 10G - across all speeds & Frame Sizes - using VLAN ID's as differentiator for MON
	Confirmed: Multi-Stream FPS rate precision & accuracy at 10G >0.001% !!  :)
	Confirmed: Delay/Latency measurements: Single Stream TSE & 10G with various frame sizes and speeds
	_________: Delay/Latency measurements: Multi-Stream	
						BUG: GEN packet spacing in multi-stream incorrect TimeStamp insertion and subsequent incorrect multi-stream delay measurements
								(any stream other than stream 0 - the 1st stream)  								
07/30	Mutli-Stream TS bug found and Partially fixed - but symptoms persist.
			...purely in FPGA. GEN & MON

08/05	Enhanced Flash Erase functions added:  erase_flash_block(unsigned int erase_block)			
				Erase Block by passed parameter: 
					//	#define FLASH_PARAMETERS_BLOCK	0	// erase parameters (last block)
					//	#define FLASH_PHY_BLOCK				1	// 2nd to last block
					//	#define FLASH_USER1_BLOCK				2	// 3rd from top
					//	#define FLASH_USER2_BLOCK				3	// 4th from top
					//	#define FLASH_SOF_ELF_BLOCKS		4 	//erase sof+elf,  from 0 through 1 before user2 block
				Updated Eth-GUI with new SysConfig Dialog access;
					TSE MAC-A: FFFFFFFF or 00000000  (0's added to support erased region)
					long-press port 'B' button (within 4 seconds of App start)
				Oop's!  had 10G PHY 2.5s wait for init completion set to 250s!  Yikes!!

				(Stanley finds)
				BUG: sfp.c:sfp_B_read_block_into_Array() Stoping A instead of B - Oop's!
				BUG: cfg_hw.c:READ_SFPB_DATA = YES; missing in MISC_ptr case! (only done in 10G)
		
				Update Engine for UI control:
					SET_GEN_MON(stream, traffic_gen_pkt_length, 1, egen_line_load_rate, HDRLEN_VLAN);
					Future UI settable var for num_of_streams configured.
					Define stream_config[i] array that consists of: Config[73-145]  (traffic_gen_pkt_length, egen_line_load_rate, and header info)
				
08/13	DIFF all delta's (mine, subversion & Chong's cut) and rebuild all with 08/12/2015 FPGA load. (GEN patch to resolve multi-stream delay)			

08/14	Modify: SET_GEN_MON(), cfg_hw, cfg10_hw, packet10g for UI control of frame size, fps load% and test duration.
					   Recover and cleanup where necessary GEN/MON stats & results for correct UI presentation.
08/18					   
			main.c:  When module removed execcute these to keep FPLLOUT clock active and output on the pin to keep the GEN&MON running!! (stanley find)
				(prior to while() and upon out - both instances of: EXTERNAL_PHY_BOOT_COMPLETE
							write_ext_10G_phy(0xF052, 1, 0x0022);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX PWDN!
							write_ext_10G_phy(0xC30A, 1, 0x007B);   // RE-Enable FPLLOUT^1
				and upon in:
							write_ext_10G_phy(0xF052, 1, 0x0020);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX & RFX PWDN!
									   
			init_10G_external_PHY():		   
					   write_ext_10G_phy(0xF052, 1, 0x0022);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX PWDN!
					   
					   
		Major MASSIVE Re-Write of Traffic setup - from debug test_function to UI Interaction.
		With support toward up & coming Multi-stream support.
		A large set ported to UI:  Remaining aspects is UI control/passage of the header settings
		
		Quick 1-day 100FX check:
			configure_100FX_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
			Temp called in cfg_hw.c: case PHY_CONFIG1_ptr:
			in link_task.c: DEBUG 100FX: read_phy_operating_mode_printf(TRIPLE_SPEED_ETHERNET_0_BASE);
			100FX Signal and Link recognized in FW - UI is befuddled.  No data Xfer. Must reboot to get out of 100FX.
				need 100FX exit  function(). 
		
>>>>> decouple UI,  cfgX_hw.c: EGEN_NUM_PACKETS3_ptr for NumPkts since we're using EGEN_NUM_SECS1_ptr now!!!	

08/22-24	Remove HDR settings from SET_GEN_BY_STREAM() and re-build into cfg_hw()
		HDRLEN_ETH:	 	raw-802.3		Length field or 0x8870 if Jumbo
		HDRLEN_ETH:	 	Eth-II  		Type field (0x0800)	
		HDRLEN_VLAN:	VLAN frames		802.3 w/length
		HDRLEN_SVLAN:	Stacked-VLAN	802.3 w/length
		 <---> nominal support for independent layer-2 w/VLAN & IP settings.  Would require cfg_hw() restructure.  Hold for PhaseII
		HDRLEN_IP4:	//IPv4 UI --> FW expectations.....
			//HDR3=0x0800+IP1.32				EGEN_IP1_HEADER3_ptr
			//HDR4=IP1.10+IP2.32				EGEN_IP2_HEADER3_ptr
			//HDR5=IP2.10+IPV4_HDR.32			EGEN_IPV4_HDR3_ptr
			//HDR6=IPV4_HDR.10+IPV4_SRC.32		EGEN_SRC1_IP3_ptr
			//HDR7=IPV4_SRC.10+DEST1_IP.32		EGEN_DEST1_IP3_ptr
			//HDR8= DEST1_IP.10 + 0x00		 
		HDRLEN_VLANIP4:
		//HDRLEN_SVLANIP4: ---not done---
>>>>> NOTHING PORTED TO cfg10_hw() until confirmed with Wireshark at 10 or 100M!
	Confirm various Packet types 1st then port to 10G!  >>> COMPLETED & TESTED
	----> Wireshark confirmation remains.		
<>	CRITICAL to consider toggling between TSE & 10G by user speed select w/o touching any FrameConfig settings!
08/27/2015: OSP Demo Cut...  Release ELF created and flashed.
	BUG:	EMIX modes - especially when toggling betweek TSE and 10G.  <--fixed.
	BUG: 10G SRC MAC not correct in HDRS!	<-- fixed.	  
	
	cf_hw.c: COMMENT OUT:
		//erase_flash_block(FLASH_SOF_ELF_BLOCKS);
		//erase_flash_block(FLASH_PHY_BLOCK); 
<>	Only allow: erase_flash_block(FLASH_PARAMETERS_BLOCK); for now! <--- until UI cleaned-up! 				

09/15/2015	Install latest sof/sopcinfo for extra 8k mem_space as well as TSE I2C external Tri-state fix (Chong)
		alt_2_wire.c/.h update with I2C fix.  For Finisar.

=== Project OPTIMIZATION LEVEL 2 ===		

09/16	Add new function: set_10G_clock_source() to mux 156.25M clock from FPLLOUT or IntlOSC.
		Init to internal in case no 10G SFP+ module installed.
		Toggled in gui_link.c when we switch between 10G and TSE port A (test) 
	
09/22 complete	100FX on Port A only ->> port to old Released 45-variant FW and GUI.  :(

=== _BSP: OPTIMIZATION LEVEL 2 and No C++ ===
10/02  V1.050 Pilto RELEASE.  Note: built upon small 10G FIFO's  (built on Chong's PC for flash files - I have identical version)
SVN clean: rev 25716  ....includes mfg Flash files (built on Chong's PC, rebuild & confirmed on Mike's PC).

10/09	Complete integration of 100FX into current '65' variant.
		BUG: 100copper --> 100Fiber phy_config not called.
			Add uages of PHY_CONFIG_RESTART_ANEG_MASK as a PHY_CONFIG0 kicker bit - called on Mode changes.
10/13	To finalize 100FX Regression testing make and flash 100FX version
1.051	gigle.h: 0x041B = 1051 -> 1.051	Add 100FX support for A & B..
<<<<<<< .mine

December	IPv6 frame_config() functions.
1.052

1/6		End To End Test changes
1.053 
||||||| .r25803
=======
			Some minor Flash erase (SYSCONFIG) cleanup - to allow cyclical erase commands.

>>>>>>> .r25975


Ver 2.1:
MS and Lpbk merge

ver 2.2:
main.c: The single stream test wwould run only on stream 0. Now allowed to run on any stream.
		This is necessary for the Service Configuration test in Y.1564.

ver 2.3
Packet10g.c: wait_for_packets(); - replaced the previous current_pkt_count with the global MON rcv pointer
								 - The total number of packets Tx by the GEN are calculated by polling each stream. (GET_GEN_PKT_TX_CNT();)
** yet to implement - product key information.
Included Write_Mon_Mask(header_length);			

ver 2.4
 Packet10g.c: wait_for_packets() - removed the summation of the Tx'd Gen packets and replaced it with the global counter. This uses sof b8 
	
ver 2.5
1. read_mon_status clean debug
2. No change in the internal MTU size.
3. Setting MAC_ADDR_MSB03 in the init to read MAC address for A,B and 10G. Earlier the MAC addresses were sent in MACA,MACB and MAC10G
4. Including the Swap IP function - swaps the IP address (Src and Dest) to align with appropriate masking.
5. The MAC_Frame_Length is not changed with the change in the value when accessing the 'H' button in EMIX.The value should not affect the internal MTU size.
6. Bug in setting MON addresses - fixed in packet10g.c new method SETMON_DEST_SRC_MAC_ADDR();
7. flr value overflowing now handled by typecasting.

ver 2.6
cfg_hw.c:
1.Fixed a bug where after a MS test, consolidated results would be requested in MISC_ptr while TRAFFIC_NUM_OF_STREAMS ptr would be called after requested stream 0 which would cause stream 0 results to be printed to the UI for the consolidated page. MISC_ptr bit 7 is no longer cleared after being read and a 1 simply indicates consolidated results while a 0 + the stream # from TRAFFIC_NUM_OF_STREAMS indicates a request for a specific stream
2.Decoupled LOOPUPTSE and LOOPDOWNTSE into separate variables for A and B
3.Modified the variable loopbackStatPtr from a char to unsigned char in order to go to positive values up to 255. Was changed post-merge to a char preventing us from saving the information for >3 responding units
4.Added check in interrupt in the event that the sniffer bucket entries have filled up and bit 7 was high; we now clear in this case to prevent us from being stuck in the interrupt
5.Additional debug statements to help with debugging the loopback sequencing 
6.EMIX change - now includes mplsTags along with vlanTags in calculation of avg.


ver 2.7

1.set_num_test_packets_by_secs():
The duration is now 4 bytes.
	if (seconds > 0xFFFFFFFF) { 
		seconds = 0xFFFFFFFF;
2. StartTest() for MS updated for 10G
3. gui_link: 
	> boolean values GIGE10G and GIGEA are set and reset according to the packet ID rcvd. Earlier checks were always done for 10G first, so this case was not handled.
	> The clearing of StartStop_ptr soon after a test stop is sent.
	This is not required as the FW can keep sending 0x40 and its the UI's job to recognize the 0x40 and then send a 0x00 to indicate the job. Clearing this value makes the FW Tx 0x10 which might be detected by the UI as start of a test.
4. removed one_sec_task.c file
NOTE: To determine code size/utilization:
	Use 'nios2-elf-size -A' or simply 'size -A on the .elf file.
	 			
v2.8
1. Update_headers separately for MS and SS in main.c instead of in Configure_streams because y.1564 service config is a single stream test with the same offset as that of stream 0.
2. packet10g.c The result values for MS and SS are separated out as the number of streams remains constant but not the stream_no for a y.1564 service config test.
	> in set_num_packets_by_stream the duration is clipped to 0xFFFF F
	 if (seconds > 0xFFFFF)	seconds = 0xFFFFF;
	> test_complete.c debug message updated to both 10g and TSE regarding test stop 
3. Removed dbg message from setting mac address	  

v2.9
1. Updated one second alarm in main.c to update ET for TSE-A and 10G-A when they are in loopback as well (get reset when first entering loopback)
2. Clearing Test DONE flag in STARTSTOP_ptr in gui_link in order to prevent DONE flag from being set to the UI twice

v2.91
1. In intr, changed the mask to 0x03 from 0x07; was changed to 0x07 when messing with 10G SFP+ interrupt and wasn't changed back
ConfigStat[SFP_STAT_ptr] |= (pio_sfp_state&0x03)<<1;
2. In gui_link, added an extra condition checking if TxBuffer[STARTSTOP_ptr] = 0x50 as well; the first condition itself was not enough in the case where 
the user would exit the app and reopen it after a test had finished running.
if(((RxBuffer[STARTSTOP_ptr] & 0x30) == 0) && (TxBuffer[STARTSTOP_ptr] == 0x50)) {

v3.0
1. In gui_link, changed the if condition for clearing the done flag to RxDONEdetected and will be changing the UI to send a 0x40 back whenever the FW sends it as 
an acknowledgement that it has seen it.

v3.01
1. Changed condition in cfg_hw for STARTSTOP_ptr back to "else if(RxTestStartGuiBit == 0x00){" rather than checking if STARTSTOP_ptr was exactly 0x00. This coincides
with us modifying the messaging structure for just the test started flag (0x10) to be cleared when a test is stopped by the UI.

v3.02
1. Fixed bug in cfg_hw for CLEAR_ptr where if condition in if-else block only checked if port B's bit had changed and everything else fell through to the else. Changed 
this so that the else is now an else if to check if port A's bit has changed as well. Code for CLEAR_ptr also restructured to group all port A and port B code together.
			
v3.03
1. Setting MISC_ptr flag (0x80) for consolidated results after a MS test in order to match the status of the UI. UI is also setting 0x80 after a MS test as well.

v3.04
1.packet10g.c: Clearing the elapsed time pointer SECONDS3_ptr before starting any test. 		

v3.05
1. Moved the MAC stats reads from link_task.c to main.c inside one_sec_alarm --> MAC stats now updated every second rather than every 600ms

v3.06
1. Fixed bug in port B loopback response where it read the wrong byte to determine if IPv4 was enabled

v3.07
1. Checking if we're running 10G for a MS test in order to read the correct # of streams from ConfigStat10G instead of ConfigStat
2. Checking ConfigMS if running a MS test in order to write source IP into destination IP field in MON

v3.08
1. Loopback status being updated in ConfigStatB when user goes into A-to-B mode.
2. Emix average now no longer returns a 0 when there are no VLAN or MPLS tags 
			
NOTE: Must have system plugged in and powered up prior to opening NIOS Eclipse for debugger to install!
	  Breakpoints: Cannot find source-code lines with two obj folders - so must clean & build only ONE elf 
======== Building RELEASE and/or DEBUG Elf files ========
	1)	* Project:    Rht^Clk    ...Properties
	2)				  select	Nios II Application Properties
	3)				  select    Manage Configurations...
	4)				  select desired: "Release or Debug"
	5)				  select    "Set Active"
	ReBUILD.


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
STEPS to BUILD with new bsp & sopcinfo

	1)	*_bsp Project:    Rht^Clk ...NIOSII...generate bsp
	2)					  Rht^Clk ...BUILD Project (build bsp)
	3)  XX Project:		  Rht^Clk ...BUILD Project (buildSW project)
	
__________________________________________________________________________________________
NOTES:
MDIX Status change: can be an INTR and monitored from reg 1A bit 13.
We're currently configured for Auto-MDIX - eliinating need for cross-over cables or ports!
100/100/10 PHY Extended Status Reg (Addr 11) 
	bit 13: MDI crossover state 1=crossover, 0=normal
