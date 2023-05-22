/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_INSERT_PREAMBLE_TAGGED_H
#define INCLUDED_IEEE802_11_OFDM_INSERT_PREAMBLE_TAGGED_H

#include <gnuradio/tagged_stream_block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API ofdm_insert_preamble_tagged : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<ofdm_insert_preamble_tagged> sptr;
    static sptr make(int fft_length, const std::vector<std::vector<gr_complex>>& preamble);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_INSERT_PREAMBLE_TAGGED_H */
