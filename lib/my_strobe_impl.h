/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_MY_STROBE_IMPL_H
#define INCLUDED_IEEE802_11_MY_STROBE_IMPL_H

#include <ieee802_11/my_strobe.h>
#include <atomic>
#include <vector>

namespace gr {
namespace ieee802_11 {

class my_strobe_impl : public my_strobe
{
private:
    gr::thread::thread d_thread;
    std::atomic<bool> d_finished;
    long d_period_ms;
    pmt::pmt_t d_msg;
    const pmt::pmt_t d_port;
    
    void run();

public:
    my_strobe_impl(pmt::pmt_t msg, long period_ms);
    ~my_strobe_impl();

    void set_msg(pmt::pmt_t msg) { d_msg = msg; };
    pmt::pmt_t msg() const { return d_msg; }
    void set_period(long period_ms) { d_period_ms = period_ms; }
    long period() const { return d_period_ms; }

    bool start();
    bool stop();
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_MY_STROBE_IMPL_H */
