/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_SAMPLER_IMPL_H
#define INCLUDED_IEEE802_11_OFDM_SAMPLER_IMPL_H

#include <ieee802_11/ofdm_sampler.h>

namespace gr {
namespace ieee802_11 {

class ofdm_sampler_impl : public ofdm_sampler
{
private:
    enum state_t { STATE_NO_SIG, STATE_PREAMBLE, STATE_FRAME };

    state_t d_state;
    unsigned int d_timeout_max;
    unsigned int d_timeout;
    unsigned int d_fft_length;
    unsigned int d_symbol_length;

public:
    ofdm_sampler_impl(unsigned int fft_length,
                      unsigned int symbol_length,
                      unsigned int timeout);
    ~ofdm_sampler_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_SAMPLER_IMPL_H */
