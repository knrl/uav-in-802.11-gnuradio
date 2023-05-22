/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_UAV_SYSTEM_H
#define INCLUDED_IEEE802_11_UAV_SYSTEM_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

/*!
 * \brief This block represent a uav system with its own data link layer
 * \ingroup ieee802_11
 *
 */
class IEEE802_11_API uav_system : virtual public gr::block
{
public:
    typedef std::shared_ptr<uav_system> sptr;
    static sptr make(std::vector<uint8_t> src, long period_ms, bool master);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_UAV_SYSTEM_H */
