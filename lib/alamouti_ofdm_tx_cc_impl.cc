/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "alamouti_ofdm_tx_cc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace ieee802_11 {

alamouti_ofdm_tx_cc::sptr alamouti_ofdm_tx_cc::make(int fft_length)
{
    return gnuradio::make_block_sptr<alamouti_ofdm_tx_cc_impl>(fft_length);
}


/*
 * The private constructor
 */
alamouti_ofdm_tx_cc_impl::alamouti_ofdm_tx_cc_impl(int fft_length)
    : gr::block("alamouti_ofdm_tx_cc",
        gr::io_signature::make(1, 1, sizeof(gr_complex)*200),
        gr::io_signature::make(2, 2, sizeof(gr_complex)*fft_length)),
        fft_len(fft_length)
{
    log.open("log_OFDM_alamouti_encoder.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
}

/*
 * Our virtual destructor.
 */
alamouti_ofdm_tx_cc_impl::~alamouti_ofdm_tx_cc_impl() 
{
    log.close();
}

void alamouti_ofdm_tx_cc_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items;
}

int alamouti_ofdm_tx_cc_impl::general_work(int noutput_items,
                                           gr_vector_int& ninput_items,
                                           gr_vector_const_void_star& input_items,
                                           gr_vector_void_star& output_items)
{
    auto in   = static_cast<const gr_complex*>(input_items[0]);
    auto out0 = static_cast<gr_complex*>(output_items[0]);
    auto out1 = static_cast<gr_complex*>(output_items[1]);

	// initialization 
	memset(out0, 0, noutput_items*fft_len*sizeof(gr_complex));
	memset(out1, 0, noutput_items*fft_len*sizeof(gr_complex));

	int occupied_tones = 200;		// by doing this we can eliminate some calculations
	int unoccupied_carriers = fft_len - occupied_tones;
	int zeros_on_left = (int)ceil(unoccupied_carriers/2.0) ;

	  int i = 0, o = 0; // input and output buffer pointers
	// as long as there is space in output buffer,  symbols are processed
	  while(o < noutput_items) {
		// bad item? discard it!
		if (in[i*occupied_tones + occupied_tones/5] == gr_complex(0,0) || 
			in[i*occupied_tones + 4*occupied_tones/5] == gr_complex(0,0)){
			i++;
			continue;
		}
		// this is where alamouti-coding is taking place
	    	for(int j = 0; j < occupied_tones/2 ; j++) {
	       		out0[o*fft_len + zeros_on_left + 2*j]     = in[i*occupied_tones + 2*j];
			out0[o*fft_len + zeros_on_left + 2*j + 1] = -conj(in[i*occupied_tones + 2*j + 1]);
		}
		for(int j = 0; j < occupied_tones/2 ; j++) {
		       out1[o*fft_len +  zeros_on_left + 2*j] 	 = in[i*occupied_tones + 2*j + 1];
		       out1[o*fft_len +  zeros_on_left + 2*j +1] = conj(in[i*occupied_tones + 2*j]);
		}
		i++;
		o++;
	  }
	
	consume_each(i);	// number of consumed input OFDM-symbols
	return o;		// number of produced output OFDM-symbol (bad items => o != i)
}

} /* namespace ieee802_11 */
} /* namespace gr */
