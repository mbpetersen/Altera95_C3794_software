	//all header information configured by UI:
	//		Stream selected by UI (initially set to 0, 1 stream)
	//
	//		Set LOAD_PERCENT by Stream -> called when User diddles:    FPS%  OR   Frame_size
	//		if(GIGE10)		set_generator_10G_load(packet_length, stream_percent);
	//		else				set_generator_tse_load(packet_length, stream_percent);
	//
	//		Set NUM_PACKETS for the Stream -> called when user diddles:  Test-duration-secs,  FPS%  OR   Frame_size
	//
	//	.....as frame types are changed - GEN and MON headers are cleared (by current stream setting)
	//  GEN:
	// 			set GEN_CONFIG_________
	//				if((traffic_gen_config&RNDLEN_ON) == RNDLEN_ON) 	SETGEN_PKTLENGTH_BY_STREAM(9600);
	//				else 		SETGEN_PKTLENGTH_BY_STREAM(packet_length)			// packet_length = traffic_gen_pkt_length OR if RNDLEN_ON
	//				SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(header_length)		// header_length = HDRLEN_VLAN;
	// 			set GEN_CONFIG2________
	//				SET_TRAFFIC_GEN_LENOFS_BY_STREAM(header_len-2)		// header_len = (header_length/0x10000);
	//			 set GEN_CONFIG3________
	//				SET_TRAFFIC_GEN_ILEN_BY_STREAM(header_len);
	//
	//			GEN header content is set
	//			Set dest & src MAC addresses to BOTH GEN & MON,  then
	//			remaining HDRx content detailed by the UI for that frame type,   then
	//			LAST HDR element is the payload length written into the length field if appropriate for the Frame Type.
	//				(packet_length-header_len-4)   // payload length = packet-length-header_len-4 for FCS,  *0x10000 for upper word of HDR or directly for lower word
	//
	//	MON:
	//			Clear MON MASK reg's
	//			Enable the MON for the selected Stream (number)
	//				write_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL, (header_length/0x100)+0x0001); // enable selected stream
	//			Set the MON HDRx content we're interested in "finding" and counting
	//			Set the MON MSKx to match the content above
	//
	//		*** NOTE could be easier to always set the MON to the GEN HDR's and only set MASK to "filter" those specific parts of the HDR we want to "find"!
	//