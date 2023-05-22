/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_MY_STROBE_H
#define INCLUDED_IEEE802_11_MY_STROBE_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief <+description of block+>
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API my_strobe : virtual public gr::block
{
public:
    typedef std::shared_ptr<my_strobe> sptr;
    static sptr make(pmt::pmt_t msg, long period_ms);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_MY_STROBE_H */
