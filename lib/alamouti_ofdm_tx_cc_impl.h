/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_ALAMOUTI_OFDM_TX_CC_IMPL_H
#define INCLUDED_IEEE802_11_ALAMOUTI_OFDM_TX_CC_IMPL_H

#include <ieee802_11/alamouti_ofdm_tx_cc.h>
#include <fstream>

namespace gr {
namespace ieee802_11 {

class alamouti_ofdm_tx_cc_impl : public alamouti_ofdm_tx_cc
{
    friend alamouti_ofdm_tx_cc::sptr make(int fft_length);
private:
    int fft_len;
    std::fstream log;
    std::fstream log2;

public:
    alamouti_ofdm_tx_cc_impl(int fft_length);
    ~alamouti_ofdm_tx_cc_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_ALAMOUTI_OFDM_TX_CC_IMPL_H */
