/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_ALAMOUTI_ENCODE_CC_H
#define INCLUDED_IEEE802_11_ALAMOUTI_ENCODE_CC_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API alamouti_encode_cc : virtual public gr::block
{
public:
    typedef std::shared_ptr<alamouti_encode_cc> sptr;
    static sptr make();
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_ALAMOUTI_ENCODE_CC_H */
