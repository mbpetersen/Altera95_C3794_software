#ifndef C3794_DEF_H
#define C3794_DEF_H

// ___________________________________________
//     IDT Clock Control  (R/W)
// ___________________________________________
#define CLKCTRL_BASE 	0x00184000
#define CLKCTRL_REG		0x00
// 		Structure:     		bits
// CLKCTRL_PPM_VALUE		0:8		9-bit 2's complement: Write PPM offset  (+/- 150)
// CLKCTRL_STROBE		  	15		1-bit  Write Actv-H to gate new PPM value (self-clearing)
// CLKCTRL_ALIVE		  	31		1-bit  Read Actv-H when Clock is Alive (Active & running)
//
#define	CLKCTRL_REG_PPM_MASK	0x01FF
#define	CLKCTRL_REG_STROBE_MASK	0x8000
#define	CLKCTRL_REG_ALIVE_MASK	0x80000000
//CLKCTRL_register = 0;
//
// C37.94 block must be put into reset when updating IDT CLK (allow 500mS for clock stabilization)
// Put into RESET,  Set and Strobe new Value,  poll Alive to confirm stable clock, exit RESET state
// Re-write all C37 block registers

// init C3794 to RESET at boot.
//#define	RESET_C3794_mask		0x0002	// PIO3 bit.1  Actv H (use PIO3_Content)
//PIO3_Content = IORD_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE);
//IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content | RESET_C3794_mask);
//
//#define	C3794_READY	(C3794_Status&0x80)



// ___________________________________________
//     Control address parameters (R/W)
//     Address range 0 to 15 
// ___________________________________________
#define ADDR_RUNFRAMES         0x000 // (wd add 0x0)    // Run test for this many frames
#define ADDR_XMT_N_CHANNELS    0x004 // (wd add 0x1)    // Configure N from 1 to 12
#define ADDR_RCV_N_CHANNELS    0x008 // (wd add 0x2)    // Configure N from 1 to 12

#define ADDR_MATCH 0x00c // (wd add 0x03  def=0x24
//    Bit.7:SyncIntr, Bit.6:DeSyncIntr, Bit.5:AutoResync, Bit.43210:desync level
//    def:      0            0                 1              0x04
// Match settings register
#define MATCH_DESYNC_LEVEL_OFS               0
#define MATCH_DESYNC_LEVEL_MASK              0x1f
#define MATCH_DESYNC_LEVEL_SIZE              5
#define MATCH_AUTO_RESYNC_OFS                5
#define MATCH_AUTO_RESYNC_MASK               0x20
#define MATCH_DESYNC_INT_EN_OFS              6
#define MATCH_DESYNC_INT_EN_MASK             0x40
#define MATCH_SYNC_INT_EN_OFS                7
#define MATCH_SYNC_INT_EN_MASK               0x80

#define ADDR_PSR               0x010 // (wd add 0x4)    // Pattern Set
#define ADDR_PLR               0x014 // (wd add 0x5)    // Pattern Length
#define ADDR_PTR               0x018 // (wd add 0x6)    // Pattern Tap

#define ADDR_CTL               0x01c // (wd add 0x7)    // Control register (PCR)
#define CTL_SIZE                           8

#define CTL_START_OFS                      0
#define CTL_START_MASK                     0x01
#define CTL_STOP_OFS                       1
#define CTL_STOP_MASK                      0x02
#define CTL_RESYNC_OFS                     2
#define CTL_RESYNC_MASK                    0x4
// following bits taken from D2172 EIR
#define CTL_RL_OFS                         3 // Latch data into PRR
#define CTL_RL_MASK                        0x08
#define CTL_LC_OFS                         4 // Latch Bit and Bit Error count values
#define CTL_LC_MASK                        0x10
#define CTL_PS_OFS                         5 // Pattern Select
#define CTL_PS_MASK                        0x20
// CJW.  Does customer want this bit?  Not implemented at this time.
#define CTL_QRSS_OFS                       6 // Zero suppression
#define CTL_QRSS_MASK                      0x40
#define CTL_TL_OFS                         7 // Load pattern generator
#define CTL_TL_MASK                        0x80


#define ADDR_DEBUG 0x020 // (wd add 0x8)    // control loopback, continuous/limited run

#define DEBUG_SIZE                         2
#define DEBUG_LOOPBACK_OFS                 0
#define DEBUG_LOOPBACK_MASK                0x1
#define DEBUG_RUN_CONTROL_OFS              1
#define DEBUG_RUN_CONTROL_MASK             0x2
#define DEBUG_FAR_END_LOOPBACK_OFS         2
#define DEBUG_FAR_END_LOOPBACK_MASK        0x4

// LOOPBACK options
#define EXTERNAL                           0
#define INTERNAL                           1
// RUN_CONTROL options
#define CONTINUOUS                         0
#define LIMITED                            1


#define ADDR_IDLE_CODE         0x024 // (wd add 0x9)    // IDLE Code value, 8 bits

#define ADDR_FRC_ERR           0x028 // (wd add 0xa)    // Force errors on Xmit, rcv blocks
#define FRC_ERR_SIZE                   16
#define FRC_BAD_HEADER_OFS             1
#define FRC_BAD_MSMTCH_OVHD_N_OFS      2
#define FRC_BAD_MSMTCH_OVHD_FIXED_OFS  3
#define FRC_BAD_CMPLMNT_OVHD_N_OFS     4
#define FRC_BAD_CMPLMNT_OVHD_FIXED_OFS 5
//#define FRC_BAD_DATA_CFG_CHAN_OFS      6
//#define FRC_BAD_DATA_UNCFG_CHAN_OFS    7
// following bits taken from D2172 EIR
#define FRC_EIB_OFS                    8
#define FRC_EIB_MASK                   0x700
#define FRC_EIB_SIZE                   3
#define FRC_SBE_OFS                    11
#define FRC_SBE_MASK                   0x800
#define FRC_RINV_OFS                   12
#define FRC_RINV_MASK                  0x1000
#define FRC_TINV_OFS                   13
#define FRC_TINV_MASK                  0x2000
// force yellow alarm in xmit model
#define FRC_YELLOW_ALARM_OFS           14
// force condition in rcv model
#define FRC_RCV_RDI_OFS                15

#define ADDR_FRC_XMIT_JITTER   0x02c // (wd add 0xb)    // Force xmit clock conditions (jitter, Master)
#define FRC_XMIT_JITTER_OFS          0  // Set transmit jitter in clock increments
#define FRC_XMIT_JITTER_SIZE         4  // Valid range 0 - 13(~100ns).
#define FRC_XMIT_MASTER_OFS          5  // Set transmitter as Master,

//currently 8 bits of status
#define ADDR_STATUS            0x030 // (wd add 0xc)    // Collection of status bits:
#define TEST_STATUS_OFS              0    // Test status ACTIVE=1,STANDBY=0
#define TEST_STATUS_MASK             0x1 
#define CLOCK_STATUS_OFS             1    // Receive clock LOCKED=1,UNLOCKED=0
#define CLOCK_STATUS_MASK            0x2  
#define LOS_STATUS_OFS               2    // LOS_INACTIVE=1, LOS_ACTIVE=0
#define LOS_STATUS_MASK              0x4   
#define RDI_STATUS_OFS               3    // RDI_INACTIVE=1, RDI_ACTIVE=0
#define RDI_STATUS_MASK              0x8    
#define SYNC_STATUS_OFS              4    // Pattern sync SYNCED=1, SEARCHING=0
#define SYNC_STATUS_MASK             0x10    
// search on 32 bits
#define DESYNC_STATUS_OFS            5
#define DESYNC_STATUS_MASK           0x20
#define UNLOCKED                     0
#define LOCKED                       1
#define LOS_ACTIVE_STATE             0
#define LOS_INACTIVE                 1
#define RDI_ACTIVE_STATE             0
#define RDI_INACTIVE                 1
#define SEARCHING                    0
#define SYNCED                       1

// ___________________________________________
//     Status counters (R/clear on Write)
//     Address range 16 to 21
// ___________________________________________
#define ADDR_LOS_DETECT_CNT    		0x040 // (wd add 0x10)    // Number of times Loss of Signal condition detected
#define ADDR_LOS_NUMFRMS_ACTIVE    	0x044 // (wd add 0x11)    // Number of frames where LOS is active
#define ADDR_RDI_DETECT_CNT    		0x048 // (wd add 0x12)    // Number of times Remote Defect Indication condition detected
#define ADDR_RDI_NUMFRMS_ACTIVE    	0x04c // (wd add 0x13)    // Number of frames where RDI is active
#define ADDR_NUM_FRAMES_RX     		0x050 // (wd add 0x14)    // Number of frames received during active tests
#define ADDR_NUM_FRAMES_TX     		0x054 // (wd add 0x15)    // Number of frames transmitted during active tests

// ___________________________________________
//     Clock control address parameters (R/W)
//     Address range 22 to 22
//     CJW: Does customer want this data?  Not implemented at this time.
// ___________________________________________
#define ADDR_CDR_PH_ADJ_EDGES    0x058 // (wd add 0x16
#define PH_ADJ_EDGES_OFS               0  // default value of 64
#define PH_ADJ_EDGES_SIZE              10 // allow value up to 512
#define PH_ADJ_EDGES_LOG2_OFS          12  
#define PH_ADJ_EDGES_LOG2_SIZE         3 

// ___________________________________________
//     Clock control/status address parameters (R/clear on write)
//     Address range 23 to 26
//     CJW: Does customer want this data?  Not implemented at this time.
// ___________________________________________
#define ADDR_CDR_MAX_PH_ADJ  0x05c // (wd add 0x17)    // max phase adjustment on clocks
#define MAX_PH_ADJ_SIZE                5

//#define ADDR_CDR_MAX_WANDER         0x060 // (wd add 0x18)    // Max absolute clock shift wander during a test
//#define ADDR_CDR_MIN_EDGE_TO_EDGE   0x064 // (wd add 0x19)    // Minimum edge to edge time on input data
//#define ADDR_CDR_MAX_EDGE_TO_EDGE   0x068 // (wd add 0x1a)    // Maximum edge to edge time on input data

// ___________________________________________
//     BERT Status registers
//     Address range 27 to 29
// ___________________________________________
#define ADDR_PSIR                   0x06c // (wd add 0x1b)    // Pattern Sync Info Register
#define ADDR_PRR                    0x070 // (wd add 0x1c)    // Pattern Receive Register
#define ADDR_PER                    0x074 // (wd add 0x1d)    // Pattern Error Register

// ___________________________________________
//     Overall error counters. (R/Clear on write)
//     Address range 30 to 31
// ___________________________________________
#define ADDR_BAD_FRAMES_RX          0x078 // (wd add 0x1e)    // Found error in any frame
#define ADDR_BAD_FRAMES_PER_SEC     0x07c // (wd add 0x1f)    // Bad frames per second, always updating


// ___________________________________________
//     Data mismatch error counters. (R/Clear on write)
//     Address range 32 to 41
// ___________________________________________
#define ADDR_MSMTCH_ERR_HDR_BITS_L     0x080 // (wd add 0x20)    // Number of mismatch errors on headers, lower 32 bits
#define ADDR_MSMTCH_ERR_HDR_BITS_H     0x084 // (wd add 0x21)    // Number of mismatch errors on headers, upper 8 bits
#define ADDR_MSMTCH_ERR_HDR_FRAMES     0x088 // (wd add 0x22)    // Number of frames with mismatch errors on headers
#define ADDR_MSMTCH_ERR_OV_N_BITS_L    0x08c // (wd add 0x23)    // Number of mismatch errors on overhead N, lower 32 bits
#define ADDR_MSMTCH_ERR_OV_N_BITS_H    0x090 // (wd add 0x24)    // Number of mismatch errors on overhead N, upper 8 bits
#define ADDR_MSMTCH_ERR_OV_N_FRAMES    0x094 // (wd add 0x25)    // Number of frames with mismatch errors on overhead N
#define ADDR_MSMTCH_ERR_OV_FIX_BITS_L  0x098 // (wd add 0x26)    // Number of mismatch errors on overhead fixed, lower 32 bits
#define ADDR_MSMTCH_ERR_OV_FIX_BITS_H  0x09c // (wd add 0x27)    // Number of mismatch errors on overhead fixed, upper 8 bits
#define ADDR_MSMTCH_ERR_OV_FIX_FRAMES  0x0a0 // (wd add 0x28)    // Number of frames with mismatch errors on overhead fixed
//#define ADDR_MSMTCH_ERR_FRAMES         0x0a4 // (wd add 0x29)    // Number of frames with any mismatch errors

// ___________________________________________
//     BERT counters. (R/Clear on read
//     Address range 42 to 44
// ___________________________________________
// data BERT registers
#define ADDR_BIT_COUNT_L               0x0a8 // (wd add 0x2a)    // Number of bits received on used channels, lower 32 bits
#define ADDR_BIT_COUNT_H               0x0ac // (wd add 0x2b)    // Number of bits received on used channels, upper 8 bits
#define ADDR_BIT_ERROR_COUNT           0x0b0 // (wd add 0x2c)    // Number of bit errors received during active tests


// ___________________________________________
//     Complement bit error counters
//     Address range 51 to 63
//     Note: no bit errors on Header.
// ___________________________________________
#define ADDR_CMPLMNT_ERR_OV_N_BITS_L    0x0cc // (wd add 0x33)    // Number of complement bit errors on overhead N, lower 32 bits
#define ADDR_CMPLMNT_ERR_OV_N_BITS_H    0x0d0 // (wd add 0x34)    // Number of complement bit errors on overhead N, upper 8 bits
#define ADDR_CMPLMNT_ERR_OV_N_FRAMES    0x0d4 // (wd add 0x35)    // Number of frames with complement bit errors on overhead N
#define ADDR_CMPLMNT_ERR_OV_FIX_BITS_L  0x0d8 // (wd add 0x36)    // Number of complement bit errors on overhead fixed, lower 32 bits
#define ADDR_CMPLMNT_ERR_OV_FIX_BITS_H  0x0dc // (wd add 0x37)    // Number of complement bit errors on overhead fixed, upper 8 bits
#define ADDR_CMPLMNT_ERR_OV_FIX_FRAMES  0x0e0 // (wd add 0x38)    // Number of frames with complement bit errors on overhead fixed
//#define ADDR_CMPLMNT_ERR_FRAMES               0x0fc // (wd add 0x3f)    // Number of frames with any complement bit errors

// ___________________________________________
#define CTL_RWLB_OFS                   	8 // Receive window latch bit (control register, self clearing)
#define CTL_RWLB_MASK                   0x100

#define RCV_WINDOW_WORD_SIZE            32   // 32 bit words are read from the receive window (msb is first bit received, lsb last)
#define RCV_WINDOW_FRAME_WORDS          8 	 // 8 32 bit words per frame in the receive window (lowest address is first word received, highest last)
#define RCV_WINDOW_NUM_FRAMES           2    // 2 frames in the receive window, order not guaranteed
#define RCV_WINDOW_ADDRESS_RANGE        16   // 512 bits of stuff here total
#define RCV_WINDOW_ADDRESS_RANGE_LOG2   4

#define ADDR_RCV_WINDOW_BASE            0x100


#endif /* C3794_DEF_H */
