/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_ALAMOUTI_DECODE_CC_IMPL_H
#define INCLUDED_IEEE802_11_ALAMOUTI_DECODE_CC_IMPL_H

#include <ieee802_11/alamouti_decode_cc.h>
#include <fstream>

namespace gr {
namespace ieee802_11 {

class alamouti_decode_cc_impl : public alamouti_decode_cc
{
private:
    int payload_length;		// number of payload symbols between two consequent pilot 
    int pilot_length;		// number of pilot symbols transmited by each antenna

    gr_complex pilot_symbol[4];	// pilot symbols

    int symbols_recieved;		
    int pilots_recieved;
    int state;
    //Channel State Information
    gr_complex h00;
    gr_complex h01;
    gr_complex h10;
    gr_complex h11;
    gr_complex norm;

    // receivrd pilots
    gr_complex rcvd_pilot_rx0;
    gr_complex rcvd_pilot_rx1;
    std::fstream log;
    void recieve_pilot (int &consumed_inputs,const int noutput_items,
            const gr_complex *in0, const gr_complex *in1) ;

public:
    alamouti_decode_cc_impl();
    ~alamouti_decode_cc_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_ALAMOUTI_DECODE_CC_IMPL_H */
