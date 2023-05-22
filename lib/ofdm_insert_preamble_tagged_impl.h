/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_INSERT_PREAMBLE_TAGGED_IMPL_H
#define INCLUDED_IEEE802_11_OFDM_INSERT_PREAMBLE_TAGGED_IMPL_H

#include <ieee802_11/ofdm_insert_preamble_tagged.h>
#include <fstream>

namespace gr {
namespace ieee802_11 {

class ofdm_insert_preamble_tagged_impl : public ofdm_insert_preamble_tagged
{
private:
    int fft_len;
    std::vector<std::vector<gr_complex>> preamble_symbols;
    std::fstream log;

protected:
    int calculate_output_stream_length(const gr_vector_int& ninput_items);

public:
    ofdm_insert_preamble_tagged_impl(int fft_length, const std::vector<std::vector<gr_complex>>& preamble);
    ~ofdm_insert_preamble_tagged_impl();

	void add_time_tag(const double delay_sec);

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_int& ninput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_INSERT_PREAMBLE_TAGGED_IMPL_H */
