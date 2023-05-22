/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_OFDM_DEMAPPER_H
#define INCLUDED_IEEE802_11_OFDM_DEMAPPER_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API ofdm_demapper : virtual public gr::block
{
public:
    typedef std::shared_ptr<ofdm_demapper> sptr;
    static sptr make(unsigned int occupied_carriers);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_OFDM_DEMAPPER_H */
