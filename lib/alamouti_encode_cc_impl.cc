/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "alamouti_encode_cc_impl.h"
#include <gnuradio/io_signature.h>
#include <stdio.h>
#include <fstream>

#define PREAMBLE 3
#define PILOT1 1
#define PILOT2 2
#define DATA 0

namespace gr {
namespace ieee802_11 {

alamouti_encode_cc::sptr alamouti_encode_cc::make()
{
    return gnuradio::make_block_sptr<alamouti_encode_cc_impl>();
}

/*
 * The private constructor
 */
alamouti_encode_cc_impl::alamouti_encode_cc_impl()
    : gr::block("alamouti_encode_cc",
        gr::io_signature::make(1, 1, sizeof(gr_complex)),
        gr::io_signature::make(2, 2, sizeof(gr_complex)))
{
    //TODO: make these available in GRC
    payload_length = 4*512; 	// in symbols
    pilot_length = 128; 		// each antenna
    preamble_length = 64;		// in symbols
    num_of_packs = 32;		// by seting this parameter to -1, preamble will be sent just once

    state = PREAMBLE;		//in the begining TX should send preamble 
    pilot_symbol[0] = gr_complex(1,-1);//constelleation dependent
    pilot_symbol[1] = gr_complex(-1,1);
    pilot_symbol[2] = gr_complex(1,1);
    pilot_symbol[3] = gr_complex(-1,-1);

    // initialization
    symbols_sent   = 0;
    pilots_sent    = 0;
    preambles_sent = 0;
    packets_sent   = 0;

    // GOLD Sequence for preamble
    int temp[64] = {1 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 
            1 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 1 , 1 , 1 , 
            0 , 0 , 0 , 0 , 1 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 , 0 , 0 , 1 , 
            1 , 1 , 0 , 1 , 1 , 1 , 1};
    for (int i = 0; i<64; i++)
        preamble[i] = 2*temp[i] - 1;

    //output buffer is always multiple of 2
    set_output_multiple(2);
    log.open("log_encoder.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
}

/*
 * Our virtual destructor.
 */
alamouti_encode_cc_impl::~alamouti_encode_cc_impl() 
{
    log.close();
}

void alamouti_encode_cc_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items;	// in alamouti algorithm every 2k number of inputs
    // produce 2k number of outputs
}

void alamouti_encode_cc_impl::send_preamble(int &generated_outputs, const int noutput_items,
                gr_complex *out0,  gr_complex *out1)
{
    // as long as we have space in output buffer we send symbols
    for (; generated_outputs < noutput_items; generated_outputs++){
        if (preambles_sent == preamble_length){
            state = PILOT1;	// now it is time to send pilots
            preambles_sent = 0;
            break;
        }
        //  sequence of 1 and -1 is converted to 1+j1 and -1-j1
        //  both antennas send the preamble at the same time
        out0[generated_outputs] = gr_complex(preamble[preambles_sent],preamble[preambles_sent]);
        out1[generated_outputs] = gr_complex(preamble[preambles_sent],preamble[preambles_sent]);

        preambles_sent++;
    }
    return;
}

void alamouti_encode_cc_impl::send_pilot(int &generated_outputs, const int noutput_items,
                gr_complex *out0,  gr_complex *out1)
{
    // as long as we have space in output buffer we send symbols
    for (; generated_outputs < noutput_items; generated_outputs++){

        // if a set pilots has been transmitted, state must be changed
        if (pilots_sent == pilot_length){
            if (state == PILOT2)
                state = DATA;
            if (state == PILOT1) 
                state = PILOT2;

            pilots_sent = 0;
            break;
        }

        // while TX1 is sending pilots, TX2 is silent (and vise versa)
        if (state == PILOT1){
            out0[generated_outputs] = pilot_symbol[pilots_sent%4] ;
            out1[generated_outputs] = 0;
        } 
        if (state == PILOT2) {
            out0[generated_outputs] = 0;
            out1[generated_outputs] = pilot_symbol[pilots_sent%4];
        }
        pilots_sent++;
    }
    return;
}

int alamouti_encode_cc_impl::general_work(int noutput_items,
                                          gr_vector_int& ninput_items,
                                          gr_vector_const_void_star& input_items,
                                          gr_vector_void_star& output_items)
{
    auto in   = static_cast<const gr_complex*>(input_items[0]);
    auto out0 = static_cast<gr_complex*>(output_items[0]);
    auto out1 = static_cast<gr_complex*>(output_items[1]);

	// initialization
	int generated_outputs = 0;
	int consumed_inputs = 0;
	
	if (state==PREAMBLE){ //in preamble state
		send_preamble(generated_outputs,noutput_items,out0, out1);
	}

	if (state==PILOT1){ //in pilot state
		//sending pilots of Tx1
		send_pilot(generated_outputs,noutput_items,out0, out1);
	}

		//sending pilots of Tx2
	if (state == PILOT2){
		send_pilot(generated_outputs,noutput_items,out0, out1);
		// we have not consumed any inputs yet
	}

        // sending payload;
	if (state == DATA){
		// as long as we have space in output buffer we send symbols
		for(; generated_outputs < noutput_items; generated_outputs+=2) {

			if (symbols_sent == payload_length){// we need to send pilots again
				state = PILOT1;
				symbols_sent = 0;
				packets_sent++;
				if (packets_sent == num_of_packs && num_of_packs!=-1){
					state = PREAMBLE;
					packets_sent = 0;
				}
				break; // in next function callback pilot symbols will be sent
			}
			// STBC coding is here
			int i = consumed_inputs; // index of current input in buffer
			int o = generated_outputs; // index of current output in buffer

			out0[o] = in[i] ;
			out1[o] = in[i+1];
	
			out0[o+1] = -conj(in[i+1]) ;
			out1[o+1] = conj(in[i]);

			consumed_inputs += 2;
			symbols_sent +=2;//total payload symbols
	      	}
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed_inputs);

        // Tell runtime system how many output items we produced.
        return generated_outputs;
}

} /* namespace ieee802_11 */
} /* namespace gr */
