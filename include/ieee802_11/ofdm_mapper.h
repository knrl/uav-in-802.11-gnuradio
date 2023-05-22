/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_MAPPER_H
#define INCLUDED_IEEE802_11_OFDM_MAPPER_H

#include <gnuradio/tagged_stream_block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API ofdm_mapper : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<ofdm_mapper> sptr;
    static sptr make(const std::vector<gr_complex>& constellation,
                     unsigned int occupied_carriers,
                     unsigned int fft_length);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_MAPPER_H */
