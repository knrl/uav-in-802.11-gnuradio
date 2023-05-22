/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_MAPPER_IMPL_H
#define INCLUDED_IEEE802_11_OFDM_MAPPER_IMPL_H

#include <ieee802_11/ofdm_mapper.h>
#include <vector>
#include <fstream>

namespace gr {
namespace ieee802_11 {

class ofdm_mapper_impl : public ofdm_mapper
{
private:
	static const int HEADERBYTELEN   = 4;
  	static const int HEADERCONSTPOINTS = HEADERBYTELEN*8/2; // The header is qpsk modulated
    int fft_len;
	int occupied_tones;
	int bits_per_symbol;
	std::vector<gr_complex> constell;
	std::vector<int> subcarrier_map;

	std::fstream log;
	unsigned int make_header(unsigned int bytes_per_packet);

protected:
    int calculate_output_stream_length(const gr_vector_int& ninput_items);

public:
    ofdm_mapper_impl(const std::vector<gr_complex>& constellation,
                     unsigned int occupied_carriers,
                     unsigned int fft_length);
    ~ofdm_mapper_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_int& ninput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_MAPPER_IMPL_H */
