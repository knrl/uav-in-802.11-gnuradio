/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "ofdm_sampler_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/expj.h>
#include <cstdio>

namespace gr {
namespace ieee802_11 {

ofdm_sampler::sptr ofdm_sampler::make(unsigned int fft_length,
                                      unsigned int symbol_length,
                                      unsigned int timeout)
{
    return gnuradio::make_block_sptr<ofdm_sampler_impl>(fft_length, symbol_length, timeout);
}

/*
 * The private constructor
 */
ofdm_sampler_impl::ofdm_sampler_impl(unsigned int fft_length,
                                     unsigned int symbol_length,
                                     unsigned int timeout)
    : gr::block("ofdm_sampler",
            io_signature::make2(2, 2, sizeof(gr_complex), sizeof(char)),
            io_signature::make2(2, 2, sizeof(gr_complex) * fft_length, sizeof(char) * fft_length)),
      d_state(STATE_NO_SIG),
      d_timeout_max(timeout),
      d_fft_length(fft_length),
      d_symbol_length(symbol_length)
{
    set_relative_rate(1.0 / (double)fft_length); // buffer allocator hint
}

/*
 * Our virtual destructor.
 */
ofdm_sampler_impl::~ofdm_sampler_impl() {}

void ofdm_sampler_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    // FIXME do we need more
    // int nreqd  = (noutput_items-1) * d_symbol_length + d_fft_length;
    int nreqd = d_symbol_length + d_fft_length;
    unsigned ninputs = ninput_items_required.size();
    for (unsigned i = 0; i < ninputs; i++)
        ninput_items_required[i] = nreqd;
}

int ofdm_sampler_impl::general_work(int noutput_items,
                                    gr_vector_int& ninput_items,
                                    gr_vector_const_void_star& input_items,
                                    gr_vector_void_star& output_items)
{
    auto iptr = static_cast<const gr_complex*>(input_items[0]);
    auto trigger = static_cast<const char*>(input_items[1]);

    auto optr = static_cast<gr_complex*>(output_items[0]);
    auto outsig = static_cast<char*>(output_items[1]);

    // FIXME: we only process a single OFDM symbol at a time; after the preamble, we can
    // process a few at a time as long as we always look out for the next preamble.

    unsigned int index = d_fft_length; // start one fft length into the input so we can
                                       // always look back this far

    outsig[0] = 0; // set output to no signal by default

    // Search for a preamble trigger signal during the next symbol length
    while ((d_state != STATE_PREAMBLE) && (index <= (d_symbol_length + d_fft_length))) {
        if (trigger[index]) {
            outsig[0] = 1; // tell the next block there is a preamble coming
            d_state = STATE_PREAMBLE;
        } else
            index++;
    }

    unsigned int i, pos, ret;
    switch (d_state) {
    case (STATE_PREAMBLE):
        // When we found a preamble trigger, get it and set the symbol boundary here
        for (i = (index - d_fft_length + 1); i <= index; i++) {
            *optr++ = iptr[i];
        }

        d_timeout = d_timeout_max; // tell the system to expect at least this many symbols
                                   // for a frame
        d_state = STATE_FRAME;
        consume_each(index - d_fft_length +
                     1); // consume up to one fft_length away to keep the history
        ret = 1;
        break;

    case (STATE_FRAME):
        // use this state when we have processed a preamble and are getting the rest of
        // the frames
        // FIXME: we could also have a power squelch system here to enter STATE_NO_SIG if
        // no power is received

        // skip over fft length history and cyclic prefix
        pos = d_symbol_length; // keeps track of where we are in the input buffer
        while (pos < d_symbol_length + d_fft_length) {
            *optr++ = iptr[pos++];
        }

        if (d_timeout-- == 0) {
            printf("TIMEOUT\n");
            d_state = STATE_NO_SIG;
        }

        consume_each(
            d_symbol_length); // jump up by 1 fft length and the cyclic prefix length
        ret = 1;
        break;

    case (STATE_NO_SIG):
    default:
        consume_each(index - d_fft_length); // consume everything we've gone through so
                                            // far leaving the fft length history
        ret = 0;
        break;
    }

    return ret;
}

} /* namespace ieee802_11 */
} /* namespace gr */
