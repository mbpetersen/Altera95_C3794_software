/*************************************************************************
*		Results & Statistics											 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
//	    		   GUISYS		MEMORY MAP
//	TSE0		0x1000-0x13FF
//	ETH0 GEN	0x1A00-0x1A3F
//	ETH0 MON	0x1AA0-0x1ABF
//	TSE1		0x0400-0x07FF
//	CPU DBG		0x0800-0x0FFF
//  ST_MUX		0x0040-0x005F

#include "gigle.h"
#include "extvars.h"

void snap_tse_a_frame_counters()
{
	unsigned int tx_frame_count = IORD_ALTERA_TSEMAC_A_FRAMES_TX_OK(TRIPLE_SPEED_ETHERNET_0_BASE);
	D(1, BUG("SNAPPER    aFramesTransmittedOK\t\t %0u\n",tx_frame_count));

	unsigned int rx_frame_count = IORD_ALTERA_TSEMAC_A_FRAMES_RX_OK(TRIPLE_SPEED_ETHERNET_0_BASE);
	D(1, BUG("SNAPPER    aFramesReceivedOK\t\t %0u\n", rx_frame_count));

	//SaveBytesLong(ConfigStat, MAC_FRAME_TX_OK3_ptr, tx_frame_count);
	//SaveBytesLong(ConfigStat, MAC_FRAME_RX_OK3_ptr, rx_frame_count);
	SaveBytesLong(ConfigStat, MAC_SNAP_TX_FRAME_CNT03_ptr, tx_frame_count);
	SaveBytesLong(ConfigStat, MAC_SNAP_RX_FRAME_CNT03_ptr, rx_frame_count);
}
//==============================================================================
void read_tse_stat_counters(unsigned int base)
{
unsigned int tmpInt=0;

	// NEED to check max values of stat's AND how & when to clear!  (octet stat's constantly turning neg - even when unsigned long!!??)
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){
		D(1, BUG("\n===============================\nPort-A: TSE MAC Statistics Counters\n===============================\n"));
		}
	//else{ ...stop continuous debug output in B modes!
	//	D(1, BUG("\n===============================\nPort-B: TSE MAC Statistics Counters\n===============================\n"));
	//	}
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("     Name\t\t\t\t Read Value\n"));
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("     ----\t\t\t\t ----------\n"));

	tmpInt = IORD_ALTERA_TSEMAC_A_FRAMES_TX_OK(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_FRAME_TX_OK03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB, MAC_FRAME_TX_OK03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aFramesTransmittedOK\t\t %0u\n",tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_A_FRAMES_RX_OK(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_FRAME_RX_OK03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_FRAME_RX_OK03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aFramesReceivedOK\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_A_FRAME_CHECK_SEQ_ERRS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_FRAME_FCS_ERRS03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_FRAME_FCS_ERRS03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aFrameCheckSequenceErrors\t\t %0u\n",  tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_A_ALIGNMENT_ERRS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ALIGNMENT_ERRS03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ALIGNMENT_ERRS03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aAlignmentErrors\t\t\t %0u\n", tmpInt));

	// these are 64 bits...
//	D(1, BUG("    aOctetsTransmittedOK\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(TRIPLE_SPEED_ETHERNET_1_BASE));
	tmpInt = IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK_MSB(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_OCTETS_TX_OK13_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_OCTETS_TX_OK13_ptr, tmpInt);
	tmpInt = IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_OCTETS_TX_OK03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_OCTETS_TX_OK03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aOctetsTransmittedOK_64\t\t %f\n", getOctetsTransmittedOK_64(base)));

//	//	D(1, BUG("    aOctetsReceivedOK\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(TRIPLE_SPEED_ETHERNET_1_BASE));
	tmpInt = IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK_MSB(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_OCTETS_RX_OK13_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_OCTETS_RX_OK13_ptr, tmpInt);
	tmpInt = IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_OCTETS_RX_OK03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_OCTETS_RX_OK03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aOctetsReceivedOK_64\t\t %f\n", getOctetsReceivedOK_64(base)));

	tmpInt = IORD_ALTERA_TSEMAC_A_TX_PAUSE_MAC_CTRL_FRAMES(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_PAUSE_FRAME_TX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_PAUSE_FRAME_TX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aTxPAUSEMACCtrlFrames\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_A_RX_PAUSE_MAC_CTRL_FRAMES(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_PAUSE_FRAME_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_PAUSE_FRAME_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    aRxPAUSEMACCtrlFrames\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_IN_ERRORS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ERR_FRAMES_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ERR_FRAMES_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifInErrors\t\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_OUT_ERRORS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ERR_FRAMES_TX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ERR_FRAMES_TX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifOutErrors\t\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_IN_UCAST_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_UNICAST_FRAMES_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_UNICAST_FRAMES_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifInUcastPkts\t\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_IN_MULTICAST_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_MULTICAST_FRAMES_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_MULTICAST_FRAMES_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifInMulticastPkts\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_IN_BROADCAST_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_BROADCAST_FRAMES_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_BROADCAST_FRAMES_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifInBroadcastPkts\t\t %0u\n", tmpInt));

	//not used by our MAC
	//D(1, BUG("    ifOutDiscards\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_OUT_DISCARDS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_OUT_DISCARDS(TRIPLE_SPEED_ETHERNET_1_BASE));

	tmpInt = IORD_ALTERA_TSEMAC_IF_OUT_UCAST_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_UNICAST_FRAMES_TX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_UNICAST_FRAMES_TX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifOutUcastPkts\t\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_OUT_MULTICAST_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_MULTICAST_FRAMES_TX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_MULTICAST_FRAMES_TX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifOutMulticastPkts\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_IF_OUT_BROADCAST_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_BROADCAST_FRAMES_TX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_BROADCAST_FRAMES_TX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    ifOutBroadcastPkts\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_DROP_EVENTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ETHER_STATS_DROP03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ETHER_STATS_DROP03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsDropEvents\t\t %0u\n", tmpInt));

	// this is 64 bits...
//	D(1, BUG("    etherStatsOctets\t\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE));
	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS_MSB(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ETHER_STATS_OCTETS13_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ETHER_STATS_OCTETS13_ptr, tmpInt);
	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ETHER_STATS_OCTETS03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ETHER_STATS_OCTETS03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsOctets_64\t\t %f\n", getEtherStatsOctets_64(base)));


	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_ETHER_STATS_PACKETS03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_ETHER_STATS_PACKETS03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts\t\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_UNDERSIZE_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_UNDERSIZED_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_UNDERSIZED_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsUndersizePkts\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_OVERSIZE_PKTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_OVERSIZED_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_OVERSIZED_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsOversizePkts\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_64_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_64_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_64_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts64Octets\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_65_TO_127_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_65_127_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_65_127_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts65to127Octets\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_128_TO_255_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_128_255_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_128_255_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts128to255Octets\t %0u\n", tmpInt);

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_256_TO_511_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_256_511_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_256_511_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts256to511Octets\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_512_TO_1023_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_512_1023_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_512_1023_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts512to1023Octets\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_1024_TO_1518_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_1024_1518_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_1024_1518_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts1024to1518Octets\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_1519_TO_X_OCTETS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_1519_FLEN_PACKETS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_1519_FLEN_PACKETS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsPkts1519toXOctets\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_JABBERS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_JABBERS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_JABBERS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsJabbers\t\t %0u\n", tmpInt));

	tmpInt = IORD_ALTERA_TSEMAC_ETHER_STATS_FRAGMENTS(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	SaveBytesLong(ConfigStat, MAC_FRAGMENTS_RX03_ptr, tmpInt);
	else	SaveBytesLong(ConfigStatB,  MAC_FRAGMENTS_RX03_ptr, tmpInt);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	D(1, BUG("    etherStatsFragments\t\t %0u\n", tmpInt));

}

//======================================================================================
// DEBUG - Collect BOTH TSE MAC Statistics from counters and display both in one table
//======================================================================================
void read_tse_stat_counters_debug()
{
unsigned int tmpInt=0;

	// NEED to check max values of stat's AND how & when to clear!  (octet stat's constantly turning neg - even when unsigned long!!??)
	D(1, BUG("\n=============================================================================\nTSE MAC Statistics Counters\t\tTSE-0\t\tTSE-1\n=============================================================================\n"));
	D(1, BUG("     Name\t\t\t\t Read Value\n"));
	D(1, BUG("     ----\t\t\t\t ----------\n"));

	tmpInt = IORD_ALTERA_TSEMAC_A_FRAMES_TX_OK(TRIPLE_SPEED_ETHERNET_0_BASE);
	D(1, BUG("    aFramesTransmittedOK\t\t %0u\t\t %0u\n",tmpInt,IORD_ALTERA_TSEMAC_A_FRAMES_TX_OK(TRIPLE_SPEED_ETHERNET_1_BASE)));
	SaveBytesLong(ConfigStat, MAC_FRAME_TX_OK03_ptr, IORD_ALTERA_TSEMAC_A_FRAMES_TX_OK(TRIPLE_SPEED_ETHERNET_0_BASE)));

	D(1, BUG("    aFramesReceivedOK\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_A_FRAMES_RX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_A_FRAMES_RX_OK(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    aFrameCheckSequenceErrors\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_A_FRAME_CHECK_SEQ_ERRS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_A_FRAME_CHECK_SEQ_ERRS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    aAlignmentErrors\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_A_ALIGNMENT_ERRS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_A_ALIGNMENT_ERRS(TRIPLE_SPEED_ETHERNET_1_BASE)));

	// these are 64 bits...
//	D(1, BUG("    aOctetsTransmittedOK\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(TRIPLE_SPEED_ETHERNET_1_BASE));
	D(1, BUG("    aOctetsTransmittedOK_64\t\t %f\t %f\n", getOctetsTransmittedOK_64(TRIPLE_SPEED_ETHERNET_0_BASE),getOctetsTransmittedOK_64(TRIPLE_SPEED_ETHERNET_1_BASE)));
//	D(1, BUG("    aOctetsReceivedOK\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(TRIPLE_SPEED_ETHERNET_1_BASE));
	D(1, BUG("    aOctetsReceivedOK_64\t\t %f\t %f\n", getOctetsReceivedOK_64(TRIPLE_SPEED_ETHERNET_0_BASE),getOctetsReceivedOK_64(TRIPLE_SPEED_ETHERNET_1_BASE)));

	D(1, BUG("    aTxPAUSEMACCtrlFrames\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_A_TX_PAUSE_MAC_CTRL_FRAMES(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_A_TX_PAUSE_MAC_CTRL_FRAMES(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    aRxPAUSEMACCtrlFrames\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_A_RX_PAUSE_MAC_CTRL_FRAMES(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_A_RX_PAUSE_MAC_CTRL_FRAMES(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifInErrors\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_IN_ERRORS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_IN_ERRORS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifOutErrors\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_OUT_ERRORS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_OUT_ERRORS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifInUcastPkts\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_IN_UCAST_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_IN_UCAST_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifInMulticastPkts\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_IN_MULTICAST_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_IN_MULTICAST_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifInBroadcastPkts\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_IN_BROADCAST_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_IN_BROADCAST_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	//not used by our MAC
	//D(1, BUG("    ifOutDiscards\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_OUT_DISCARDS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_OUT_DISCARDS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifOutUcastPkts\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_OUT_UCAST_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_OUT_UCAST_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifOutMulticastPkts\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_OUT_MULTICAST_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_OUT_MULTICAST_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    ifOutBroadcastPkts\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_IF_OUT_BROADCAST_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_IF_OUT_BROADCAST_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsDropEvents\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_DROP_EVENTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_DROP_EVENTS(TRIPLE_SPEED_ETHERNET_1_BASE)));

	// this is 64 bits...
//	D(1, BUG("    etherStatsOctets\t\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE));
	D(1, BUG("    etherStatsOctets_64\t\t %f\t %f\n", getEtherStatsOctets_64(TRIPLE_SPEED_ETHERNET_0_BASE),getEtherStatsOctets_64(TRIPLE_SPEED_ETHERNET_1_BASE)));

	D(1, BUG("    etherStatsPkts\t\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsUndersizePkts\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_UNDERSIZE_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_UNDERSIZE_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsOversizePkts\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_OVERSIZE_PKTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_OVERSIZE_PKTS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts64Octets\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_64_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_64_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts65to127Octets\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_65_TO_127_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_65_TO_127_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts128to255Octets\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_128_TO_255_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_128_TO_255_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts256to511Octets\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_256_TO_511_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_256_TO_511_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts512to1023Octets\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_512_TO_1023_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_512_TO_1023_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts1024to1518Octets\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_1024_TO_1518_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_1024_TO_1518_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsPkts1519toXOctets\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_1519_TO_X_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_PKTS_1519_TO_X_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsJabbers\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_JABBERS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_JABBERS(TRIPLE_SPEED_ETHERNET_1_BASE)));
	D(1, BUG("    etherStatsFragments\t\t %0u\t\t %0u\n", IORD_ALTERA_TSEMAC_ETHER_STATS_FRAGMENTS(TRIPLE_SPEED_ETHERNET_0_BASE), IORD_ALTERA_TSEMAC_ETHER_STATS_FRAGMENTS(TRIPLE_SPEED_ETHERNET_1_BASE)));

}


//==============================================================================
//	Read & Decode MAC Stat counters that are 64-bit long
//==============================================================================
//D(1, BUG("    aOctetsTransmittedOK\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(TRIPLE_SPEED_ETHERNET_1_BASE));
//D(1, BUG("    aOctetsReceivedOK\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(TRIPLE_SPEED_ETHERNET_1_BASE));
//D(1, BUG("    etherStatsOctets\t\t\t %0lu\t %0lu\n", (unsigned long) IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(TRIPLE_SPEED_ETHERNET_0_BASE), (unsigned long) IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(TRIPLE_SPEED_ETHERNET_1_BASE));
//========================================================
double getOctetsTransmittedOK_64(unsigned int base)	// 64bits
{
    double bytes=0;
    bytes = ((double) (unsigned long)IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK_MSB(base)) * 4294967296.0;
    bytes += (double) (unsigned long)IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK(base);
    return bytes;
}
//========================================================
double getOctetsReceivedOK_64(unsigned int base)	// 64bits
{
    double bytes=0;
    bytes = ((double)(unsigned long)IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK_MSB(base)) * 4294967296.0;
    bytes += (double)(unsigned long)IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK(base);
    return bytes;
}
//========================================================
double getEtherStatsOctets_64(unsigned int base)	// 64bits
{
    double bytes=0;
    bytes = ((double)(unsigned long)IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS_MSB(base)) * 4294967296.0;
    bytes += (double)(unsigned long)IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS(base);
    return bytes;
}

/*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, USA.    *
* All rights reserved.                                                   *
*************************************************************************/
