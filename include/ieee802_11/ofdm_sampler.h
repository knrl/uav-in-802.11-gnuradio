/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_SAMPLER_H
#define INCLUDED_IEEE802_11_OFDM_SAMPLER_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API ofdm_sampler : virtual public gr::block
{
public:
    typedef std::shared_ptr<ofdm_sampler> sptr;
    static sptr make(unsigned int fft_length,
                     unsigned int symbol_length,
                     unsigned int timeout = 1000);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_SAMPLER_H */
