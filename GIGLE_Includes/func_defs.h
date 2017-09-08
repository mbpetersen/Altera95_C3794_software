#ifndef FUNC_DEF_H_
#define FUNC_DEF_H_

#define VAL_TAG "Value: "

void snap_tse_a_frame_counters();

void sfp_transmit_laser(_Bool port, _Bool enable);
void sfp10G_transmit_laser(_Bool sfpa, _Bool enable);

//=== Flash routines ==============
void erase_flash_block(unsigned int erase_block);
void write_flash_parameter_block(); // erases then writes Flashparamters
void read_flash_parameters();		// read FlashParameter block and load into array
void load_flashparameters_into_systemvars();
void save_systemvars_to_flashparameters();
void read_flash_qtx_block();
void read_flash_qtx_block_and_write_phy(); // for 10G PHY

//=== C37.94 routines ======================================================
void init_debug_test();
void init_c37dot94();
void verify_register(unsigned int addr, unsigned int expected, unsigned int mask);
void transition_register_bit(unsigned int addr, unsigned int mask);
void dump_C3794_status();
void set_internal_loopback();
void clear_internal_loopback();
void set_far_end_loopback();
void clear_far_end_loopback();
void set_IDLE_code(unsigned char value);
void set_XMT_N_CHANNELS(unsigned char value);
void set_RCV_N_CHANNELS(unsigned char value);
void wait_for_rcv_clock_locked();
void wait_for_LOS_GOOD();
void set_test_pattern();
void load_test_pattern();
void start_BERT_test();
void stop_BERT_test();
void restart_bert();
void Invert_BERT_pattern();
void Insert_BERT_biterror();
void set_C3794_BER(unsigned char BER_value);
void config_c37_hw();
void issue_UI_command(unsigned short msg_element, unsigned char value);
void update_alarms_event_status();
void process_bert_sync();
void process_bert_errors();
void process_alarms_events();
void update_history();

void check_fixed(void);
void reset_C3794_blocks();
void gate_C3794_blocks();
unsigned char wait_for_C3794_clk();
void write_ppm_offset(signed short ppm_value);
//void run_C3794_BERT_test1();
//void c3794_processing();
void c3794_task(void* pdata);

//=== 10G routines =========================================================
void enable_PHY_clock(_Bool state);
void init_10G_external_phy(_Bool load_firmware);
void config_10G_external_phy();
void loopback_xgxs_sys_10G_external_phy(_Bool enable);
void init_10G_xaui();
void XAUI_SERIAL_SELFLPBK(_Bool enable);
void init_10G_SC_FIFO(unsigned char fifo_mode);
void init_10G_MAC();
void reset_ERR4_DC_FIFOS();
void reset_DC1_SC0_FIFOS();
void reset_DCFIFO5_FIFOS();
void CLR_10G_MAC_STATS();
void SET_10G_MAC_RX_ADDR(unsigned int mac_address1, unsigned long mac_address0);
void SET_10G_MAC_TX_ADDR(unsigned int mac_address1, unsigned long mac_address0);

void set_10G_PHY_reset(_Bool state);
void reset_10G_MAC_FIFOS();
void set_reset_10GMAC_FIFOS();
void clear_reset_10GMAC_FIFOS();

void write_ext_10G_phy(unsigned int reg, unsigned int devad, unsigned long value);
unsigned long read_ext_10G_phy(unsigned int reg, unsigned int devad);
void read_10G_sfp_info(_Bool selected_port);
void sfp_A_read_10G_block_into_Array(unsigned char* array, int base_address, int start_address, unsigned int num_to_read, unsigned int offset);
void sfp_B_read_10G_block_into_Array(unsigned char* array, int base_address, int start_address, unsigned int num_to_read, unsigned int offset);
void read_10G_sfp_power(_Bool selected_port);
void set_10G_MAC_line_loopback(_Bool state);
void set_10G_MAC_self_loopback(_Bool state);

void config_hardware10G();

//long Round(double number); //mbp's rounding because I couldn't find hte system round in math.h <idiot>

//=== 10G Traffic Generation routines =====================================
//old void set10G_FPSPauseCount(unsigned int value);
void dump_gen_settings(unsigned int stream);
void dump_mon_status(unsigned int stream);
void dump_MAC_settings();
void read_10G_MAC_Stats();
unsigned long read_10GMAC_64bit_stats(unsigned long base, unsigned int offset);
void SETMON_CLEAR_CNTS_AND_START();
void SETMON_STOP();
void SET_GEN_MON_TO_STREAM(unsigned int stream);
void SETMON_TOTALNUMPKTS(unsigned long long burst_size, unsigned short number_packet_msb);
void SETGENMONHDR_10GMAC_SOURCE_ADDR(unsigned int mac_address1, unsigned long mac_address0);
void SETGENMONHDR_10GMAC_DEST_ADDR(unsigned int mac_address1, unsigned long mac_address0);
void SETGEN_STOP();
void CLRGEN_STOP();
void SETGEN_START();
void SETGEN_STREAM_NUMPKTS(unsigned long value, unsigned short number_packet_msb);
//old void SETGEN_LENGTHFIXED();
//old void SETGEN_LENGTHRANDOM();
//old void SETGEN_PAYLOADFIXED();
//old void SETGEN_PAYLOADRANDOM();
void SET_TRAFFIC_GEN_RNDLEN_BY_STREAM(_Bool value);	// pass ON or OFF;
void SET_TRAFFIC_GEN_RNDPAY_BY_STREAM(_Bool value);
void set_generator_10G_load(unsigned short packet_length, float load_rate);	// pass percentage desire 90=90%, 1 to 100
void set_generator_tse_load(unsigned short packet_length, float load_rate);	// pass TSE percentage desire 90=90%, 1 to 100
//void SETGEN_TRAFFIC_GEN_PKTPERCENT(unsigned int value);
void SETGEN_TRAFFIC_GEN_PKTPERCENT(float value);
void SETGEN_PKTLENGTH_BY_STREAM(unsigned int value);
void SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(unsigned int value);
void SET_TRAFFIC_GEN_GBLCONFIG(unsigned short line_rate);
void SETGEN_TSEMAC_DEST_ADDR(unsigned int mac_address1, unsigned long mac_address0);
void SETGEN_TSEMAC_SOURCE_ADDR(unsigned int mac_address1, unsigned long mac_address0);
void SETMON_MAC_SOURCE_DEST_ADDR();

void collect_10G_MAC_test_stats();
void collect_TSE_MAC_test_stats();

void run_packet_test();
unsigned int MON_ERR_PKTS();
void wait_for_packets();
void test_complete();
void read_10G_phy_operating_mode();
unsigned long  long GET_GEN_PKT_TX_CNT();
unsigned long  long set_num_test_packets_by_secs(unsigned int packet_length, unsigned long seconds, float stream_percent);

//=== Multi-tasking def's =========
void link_task(void* pdata);
void c37dot94_task(void* pdata);
void config_gui_link();

//=== ISR's ====================
void timer1_ISR(void *context);
void serial_ISR(void *context);
void handle_sfp_ISR(void* context);
void handle_aggr_ISR(void* context);

void config_gui_link();
void config_hardware();
void config_multistream();

void reset_jitter_detector();
unsigned long BytesToLong(unsigned char* array,unsigned short msb);
unsigned short BytesToInt(unsigned char* array,unsigned short msb);
void SaveBytesLong(unsigned char* array,unsigned short msb,unsigned long longint);
void SaveBytesInt(unsigned char* array,unsigned short msb,unsigned short tmpint);
unsigned char CalcChecksum(unsigned char* array,unsigned short size);
int _putchar(int tx_char);

// ETHERNET functions
//void configure_tse0_mac();
//void configure_tse1_mac();
void configure_tse_mac(unsigned int base);
void wait_for_tseMacReset(unsigned int base);
void tseMacWriteCommandConfigAndReset(unsigned int base);
void start_ethernet_generator();
void read_tse_stat_counters();
void configure_tse0_to_phya();
void configure_tse1_to_phyb();
void configure_phya_copper();		// TSE0
void configure_phyb_copper();		// TSE1
//void set_phya_clockrate(unsigned char newvalue);
//void set_phyb_clockrate(unsigned char newvalue);
void configure_TxClkA_speed();
void configure_TxClkB_speed();
void set_10G_clock_source(unsigned int newvalue);	// INT156_CLOCK or FPLLOUT_CLOCK
void configure_phy_rx_delay_timing(unsigned int base);
void configure_phy_rx_aligned_timing(unsigned int base);
void read_phy_operating_mode(unsigned int base);
void select_copper_operation(unsigned int base);
void select_fiber_operation(unsigned int base);
void fiber_power_down(unsigned int base);			// ...and sets copper operation
void copper_power_down(unsigned int base);			// ...and sets fiber operation
void restart_copper_auto_negotiation(unsigned int base);
void restart_fiber_auto_negotiation(unsigned int base);
void configure_phya_fiber();
void configure_phyb_fiber();
void configure_phy_fiber(unsigned int base);
void configure_100FX_operation(unsigned int base);
void exit_100FX_operation(unsigned int base);

void set_PortA_LED(_Bool state);
void set_PortB_LED(_Bool state);
double getOctetsTransmittedOK_64(unsigned int base);
double getOctetsReceivedOK_64(unsigned int base);
double getEtherStatsOctets_64(unsigned int base);

//old TSE Traffic Gen & Mon*******************************
//double getBytes(unsigned int base);
//double getSeconds(unsigned int base);
//double getLatency(unsigned int base);
//double getLatencyPlusBuffer(unsigned int base);
//float getPacketsExpected(unsigned int mon_base);
//unsigned int getPacketsSent(unsigned int gen_base);
//unsigned int getErrorPacketsReceived(unsigned int mon_base);
//unsigned int getValidPacketsReceived(unsigned int mon_base);
//unsigned int getPacketBuildCntReg(unsigned int gen_base);
//unsigned int getClockCount(unsigned int gen_base);
//unsigned int getFPSPauseCount(unsigned int gen_base);
//void setFPSPauseCount(unsigned int gen_base, unsigned int value);
//void TestDone_GetResults_PortA();
//void DisplayTestResults_PortA();

// sfp crap.........................
void read_sfp_name();	// reads both A & B
void read_sfp_info(_Bool sfp_a);
void read_sfp_power(_Bool sfp_a);
void read_sfp_power(_Bool sfp_a);

//mbp from helpers.h
float calculateBytesPerSecond(void);
float calculatePacketsPerSecond(void);
//void formatUnits(char * string, float number, int decimals);
unsigned convertStringToHex(char * output, char *input, int numberBits);
int convertHexToString(char * output, char *input, int numberBits);
int validateHexString(char * input);
void stripLeadingHex(char ** input);
//=========================================================================================
void SETMONHDR_10GMAC_DEST_ADDR(unsigned int mac_address1,   unsigned long mac_address0);//ck
void SETMON_MAC_SOURCE_ADDR(unsigned int mac_address1, unsigned long mac_address0);//ck
void SET_GEN_MON_MAC_DEST_ADDR(unsigned int mac_address1, unsigned long mac_address0);//ck
void SET_TRAFFIC_GEN_JUMBO_BY_STREAM(_Bool value);
void SET_TRAFFIC_GEN_LENOFS_BY_STREAM(unsigned int value);
void SET_SECOND_GEN_LENOFS_BY_STREAM(unsigned int value);
void SET_GEN_IPV4_HCSOFS_BY_STREAM(_Bool state, unsigned int value);
void SET_TRAFFIC_GEN_ETH2(_Bool state);
void SETMON_L2_HEADER_MASK_MACFRAME();
void SET_TRAFFIC_GEN_ILEN_BY_STREAM(unsigned short length_offset,unsigned int packet_length);
void SET_SECOND_GEN_ILEN_BY_STREAM(unsigned short length_offset);
void SET_GEN_IPV4_IHCS_BY_STREAM(unsigned int value);
void SET_GEN_TYPE(unsigned long value);
void SET_SECOND_LENGTH_ENGINE_BY_STREAM(_Bool value);
void SETMON_START();
void CLR_GEN_HEADER_BY_STREAM(unsigned int start_hdr_addr, unsigned int end_hdr_addr);
void CLR_MON_HEADER_BY_STREAM(unsigned int start_hdr_addr, unsigned int end_hdr_addr);
void CLR_MON_MASK_BY_STREAM(unsigned int start_msk_addr, unsigned int end_msk_addr);
void DISABLE_MON_MASK();
void DEBUG_MON_STAT_BY_STREAM(unsigned int stream);
void DEBUG_MON_ERRMSG();
void DEBUG_GEN_MSG(unsigned int stream);
void read_mon_status(unsigned int stream);
void write_traffic_reg(unsigned short device, unsigned int reg_addr, unsigned int reg_data);
unsigned int read_traffic_reg(unsigned short device, unsigned int reg_addr);
void clearTotalResults();

void CONFIGURE_TEST_STREAMS(unsigned int stream);
void StartTest();
void CONFIGURE_TRAFFIC_STREAMS();
void SET_GEN_MON_TEST_BY_STREAM(unsigned int stream, unsigned int packet_length, unsigned long seconds, float stream_percent, unsigned long header_length);
void CLR_GEN_MON_HDRS_BY_STREAM();

void SET_GEN_BY_STREAM(unsigned int packet_length, unsigned long header_length);
void SET_MON_BY_STREAM(unsigned long header_length);
void SET_GENMON_MAC_ADDR_BY_STREAM();	// set HDR0, HDR1 & HDR2;
void SET_MON_IP_ADDR_BY_STREAM();		// Write source IP from GEN HDR into dest IP of MON HDR
void SET_NUMPKTS_BY_STREAM(unsigned int packet_length, unsigned long seconds, float stream_percent);
void SET_FPSLOAD_BY_STREAM(unsigned int packet_length, float stream_percent);



void config_frame_structure();
void config_frame_raw802_3();		// Our Std Layer-2 (802.3)
void config_frame_ethII();				// EthernetII - same as raw 802.3 replace length with type=0x0800!
void config_frame_vlan();				// VLAN
void config_frame_stacked_vlan();	// STACKED VLAN
void config_frame_ipv4();		 		// IPv4
void config_frame_ipv4_vlan()	; 		// VLAN + IPv4
void config_frame_ipv4_stacked_vlan();	 // has_stacked_vlan_mac_ipv4
void config_frame_ipv6();		 		// IPv6
void config_frame_ipv6_vlan()	; 		// VLAN + IPv6
void config_frame_ipv6_stacked_vlan();	 // has_stacked_vlan_mac_ipv6
void Write_Gen_Traffic(unsigned int header_length);
void Write_Mon_Mask(unsigned int header_length);
//to get consolidated results for all streams
void GetTotalResults();


void INIT_TRAFFIC_STREAM_RAW802_3();
void DUMP_GEN_CONFIGS();
void DUMP_GEN_HDRS_BY_STREAM();
void DUMP_MON_HDRS_BY_STREAM();
void DUMP_MON_MASK_BY_STREAM();
void DBG_MON_STATS();
void SET_GEN_CONFIG_1(unsigned int packet_length,unsigned int header_length,unsigned int gen_number_seconds,float egen_line_load_rate);
void SET_GEN_CONFIG_2(unsigned int packet_length);
void SET_GEN_CONFIG_3(unsigned int packet_length, float stream_percent,unsigned int header_length);
void SET_RND_GENCONFIG1_PARAMS(unsigned int traffic_gen_pkt_length,unsigned int gen_number_seconds,float egen_line_load_rate);
void update_headers(short stream_no);
void SET_GEN_PACKET_LENGTH();
void SET_SECONDS();
void SET_LINE_LOAD_RATE();
void SET_TEST_PARAMS();
//void SET_TRAFFIC_PKTLENGTH(unsigned int packet_length,float egen_line_load_rate);					// CONFIG_PKLEN
void SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(unsigned int header_length);			// CONFIG_HDRLEN
void GET_EMIX_AVG_PKTLEN();
void SET_GEN_EMIX_BY_STREAM();
void SEND_LOOPBACK_PACKET();
void SEND_HELLO_PACKET();
void ENTER_LOOPBACK(_Bool swapIP);
void EXIT_LOOPBACK();
void loopback_event_processing();

void SET_LOOPBACK_CONFIGURATION(int packet_type);
void SET_LOOPBACK_CONFIGURATION_L3(int packet_type);
void WAIT_FOR_LOOPBACK_PACKETS();
void WRITE_GEN_MON_HDR_MASK(_Bool WRITE_GEN);
int isTSELinkActive(void);
//float buildUp(unsigned char* array,int *index);
void SET_EMIX_OFFSET(unsigned char offset);
//========================================================
#endif /*FUNC_DEF_H_*/
