/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_ALAMOUTI_ENCODE_CC_IMPL_H
#define INCLUDED_IEEE802_11_ALAMOUTI_ENCODE_CC_IMPL_H

#include <ieee802_11/alamouti_encode_cc.h>
#include <fstream>

namespace gr {
namespace ieee802_11 {

class alamouti_encode_cc_impl : public alamouti_encode_cc
{
private:
	int payload_length;		// number of payload symbols between two consequent pilot 
	int pilot_length;		// number of pilot symbols transmited by each antenna
	int num_of_packs; 		// number of packets between consequent preambles
	int preamble_length;		// number of preamble symbols

	gr_complex pilot_symbol[4];	// pilot symbols
	int preamble[64];		// preamble sequence (GOLD)

	// following parameters indicte the state of the transmitter {PILOT,PREAMBLE,PAYLOAD}
	int packets_sent;		
	int symbols_sent;
	int pilots_sent;
	int preambles_sent;
	int state;			
	std::fstream log;	// discriptor of log file
	
	void send_pilot(int &generated_outputs, const int noutput_items,
				gr_complex *out0,  gr_complex *out1);
	void send_preamble(int &generated_outputs, const int noutput_items,
				gr_complex *out0,  gr_complex *out1);

public:
    alamouti_encode_cc_impl();
    ~alamouti_encode_cc_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_ALAMOUTI_ENCODE_CC_IMPL_H */
