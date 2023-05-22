/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_PREAMBLE_DETECTOR_IMPL_H
#define INCLUDED_IEEE802_11_PREAMBLE_DETECTOR_IMPL_H

#include <ieee802_11/preamble_detector.h>
#include <fstream>

namespace gr {
namespace ieee802_11 {

class preamble_detector_impl : public preamble_detector
{
private:
	//block parameters
	int preamble_length; 	//in symbols
	int packet_length; 	//in symbols : pilots + payloads
	int num_of_packets;	//between consequent preambles
	int samp_per_symb;	//samples per symbol

	bool preamble_mode;	// state of system
	int samples_sent;
	std::fstream log;
	int transient_counter;	// counts the transient samples (these samples are discarde)
	int preambles_received;
	int history;		// history of system [in samples]

public:
    preamble_detector_impl();
    ~preamble_detector_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_PREAMBLE_DETECTOR_IMPL_H */
