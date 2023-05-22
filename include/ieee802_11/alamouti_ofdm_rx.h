/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_ALAMOUTI_OFDM_RX_H
#define INCLUDED_IEEE802_11_ALAMOUTI_OFDM_RX_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API alamouti_ofdm_rx : virtual public gr::block
{
public:
    typedef std::shared_ptr<alamouti_ofdm_rx> sptr;
    static sptr
    make(unsigned int occupied_carriers, unsigned int fft_length, unsigned int cplen);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_ALAMOUTI_OFDM_RX_H */
