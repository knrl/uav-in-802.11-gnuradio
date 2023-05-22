/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "my_strobe_impl.h"
#include <gnuradio/io_signature.h>
#include <chrono>
#include <thread>

namespace gr {
namespace ieee802_11 {

#pragma message("set the following appropriately and remove this warning")
using input_type = float;
#pragma message("set the following appropriately and remove this warning")
using output_type = float;
my_strobe::sptr my_strobe::make(pmt::pmt_t msg, long period_ms)
{
    return gnuradio::make_block_sptr<my_strobe_impl>(msg, period_ms);
}

/*
 * The private constructor
 */
my_strobe_impl::my_strobe_impl(pmt::pmt_t msg, long period_ms)
    : gr::block("my_strobe",
        gr::io_signature::make(0,0,0), gr::io_signature::make(0,0,0)),
        d_finished(false),
        d_period_ms(period_ms),
        d_msg(msg),
        d_port(pmt::mp("strobe"))
    {
        message_port_register_out(d_port);
        message_port_register_in(pmt::mp("set_msg"));
        set_msg_handler(pmt::mp("set_msg"), boost::bind(&my_strobe_impl::set_msg, this, boost::placeholders::_1));        
}

/*
 * Our virtual destructor.
 */
my_strobe_impl::~my_strobe_impl() {}

bool my_strobe_impl::start() {
    d_finished = false;
    d_thread = gr::thread::thread([this] { run(); });
    return block::start();
}

bool my_strobe_impl::stop() {
    d_finished = true;
    d_thread.interrupt();
    d_thread.join();
    return block::stop();
}

void my_strobe_impl::run() {
    while (!d_finished) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<long>(d_period_ms)));
    
    if (d_finished) 
        return;
    
    message_port_pub(d_port, d_msg);
    }
}

} /* namespace ieee802_11 */
} /* namespace gr */
