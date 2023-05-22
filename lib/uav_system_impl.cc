/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "uav_system_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/crc.hpp>
#include <chrono>
#include <thread>

namespace gr {
namespace ieee802_11 {

uav_system::sptr uav_system::make(std::vector<uint8_t> src, long period_ms, bool master)
{
    return gnuradio::make_block_sptr<uav_system_impl>(src, period_ms, master);
}

/*
 * The private constructor
 */
uav_system_impl::uav_system_impl(std::vector<uint8_t> src, long period_ms, bool master)
    : gr::block(
          "uav_system", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_period_ms(period_ms),
      d_master(master)
{
    message_port_register_out(pmt::mp("phy out0"));
    message_port_register_out(pmt::mp("phy out1"));

    message_port_register_in(pmt::mp("phy in0"));
    message_port_register_in(pmt::mp("phy in1"));
    set_msg_handler(pmt::mp("phy in0"),
                    boost::bind(&uav_system_impl::phy_in0, this, boost::placeholders::_1));
    set_msg_handler(pmt::mp("phy in1"),
                    boost::bind(&uav_system_impl::phy_in1, this, boost::placeholders::_1));

    if (!check_mac(src)) {
        throw std::invalid_argument("wrong mac address size");
    }

    for (int i=0;i<MAC_ADDR_SIZE;++i)
        d_uav_1.mac_addr[i] = src[i];
    
    if (d_master) {
        // send initial token(s)
        initial_token(0 /* token_id */);
    }
}

/* TODO:
    - Check the data whether correctly communicate or not

*/

void uav_system_impl::initial_token(const uint8_t token_id) {
    d_token_1.header.token_id = token_id;
    d_token_1.header.num_uavs = 1;

    // add this uav's initial values into the payload
    for (uint8_t i(0);i<MAC_ADDR_SIZE;++i)
        d_token_1.payload[i] = d_uav_1.mac_addr[i];
    d_token_1.payload[6] = d_uav_1.pos.x;
    d_token_1.payload[7] = d_uav_1.pos.y;
    d_token_1.payload[8] = d_uav_1.pos.z;

    // update token after uav selection
    // source and destination mac addresses are loaded in here
    // TODO:
    //  - after uav selection method add destination mac address
    update_token();

    // add token header
    size_t msg_len{static_cast<size_t>(UAV_SIZE)};

    // make Token frame
    generate_mac_data_frame(msg_len);

    // dict
    pmt::pmt_t d_meta = pmt::make_dict();
    d_meta = pmt::dict_add(d_meta, pmt::mp("init_crc_included"), pmt::PMT_T);

    // blob
    pmt::pmt_t d_msg = pmt::make_blob(d_psdu, static_cast<size_t>(TOKEN_HEADER_SIZE + CRC_SIZE + msg_len));

    // pdu
    std::cout << "init token" << std::endl;
    message_port_pub(pmt::mp("phy out0"), pmt::cons(d_meta, pmt::cons(d_meta, d_msg)));
}

/**
 * Process data messages that come from channel
 * 
 * @param msg data from channel
*/
void uav_system_impl::phy_in0(pmt::pmt_t msg) 
{
    // omit token header 
    // this must be a pair
    // msg -> pmt::cons(pmt::mp("crc_included"), pmt::make_blob(psdu, psdu_len))
    if (!pmt::is_blob(pmt::cdr(msg))) {
        throw std::runtime_error("PMT must be blob");
    }

    // strip token header
    pmt::pmt_t blob(pmt::cdr(msg));
    const char* mpdu = reinterpret_cast<const char*>(pmt::blob_data(blob));
    const char* msdu = &mpdu[TOKEN_HEADER_SIZE];
    
    // update payload 
    // msdu[13] -> num_uavs
    size_t num_uavs{static_cast<size_t>(mpdu[13])};
    num_uavs = update_cache(msdu, num_uavs);

    // update token after uav selection
    update_token();

    // add token header
    size_t msg_len{static_cast<size_t>(num_uavs * UAV_SIZE)};

    if (msg_len > MAX_PAYLOAD_SIZE) {
        throw std::invalid_argument("Frame too large (>1500)");
    }

    // make Token frame
    generate_mac_data_frame(msg_len);

    // dict
    pmt::pmt_t d_meta = pmt::make_dict();
    d_meta = pmt::dict_add(d_meta, pmt::mp("crc_included"), pmt::PMT_T);

    // blob
    size_t psdu_len{static_cast<size_t>(TOKEN_HEADER_SIZE + CRC_SIZE + msg_len)};
    pmt::pmt_t d_msg = pmt::make_blob(d_psdu, psdu_len);

    // pdu
    std::cout << " here in phy in0\n";
    for (uint16_t i(0);i<msg_len;++i)
        std::cout << d_token_1.payload[i] << ", ";
    // d_msg0 = pmt::cons(d_meta, pmt::cons(d_meta, d_msg));
    message_port_pub(pmt::mp("phy out0"), pmt::cons(d_meta, pmt::cons(d_meta, d_msg)));
}

void uav_system_impl::generate_mac_data_frame(int msdu_size)
{
        // copy mac header into psdu
        std::memcpy(d_psdu, &d_token_1.header.token_id, TOKEN_HEADER_SIZE);

        // copy updated payload into psdu
        std::memcpy(d_psdu + TOKEN_HEADER_SIZE, d_token_1.payload, msdu_size);

        // compute and store fcs
        boost::crc_32_type result;
        result.process_bytes(d_psdu, msdu_size + TOKEN_HEADER_SIZE);

        uint32_t fcs = result.checksum();
        std::memcpy(d_psdu + msdu_size + TOKEN_HEADER_SIZE, &fcs, sizeof(uint32_t));
}

void uav_system_impl::update_token() 
{
    for (uint8_t i(0);i<MAC_ADDR_SIZE;++i)
        d_token_1.header.source_mac_addr[i] = d_uav_1.mac_addr[i];

    for (uint8_t i(0);i<MAC_ADDR_SIZE;++i)
        d_token_1.header.dest_mac_addr[i] = d_uav_1.mac_addr[i] + 1;
}

size_t uav_system_impl::update_cache(const char* msdu, size_t num_uavs)
{
    /*
        msdu[0]   -> number of uavs
        msdu[1:6] -> source mac address
        msdu[7:9] -> positions (x, y, z)
    */
    std::memset(d_token_1.payload, 0, MAX_PAYLOAD_SIZE);

    // update the chache of this uav
    d_token_1.header.num_uavs = num_uavs;
    std::memcpy(d_token_1.payload, msdu, num_uavs * UAV_SIZE);

    // if the uav exist in the list, add the uav's new position
    bool not_exist{true};
    for (uint8_t i(0);i<num_uavs*UAV_SIZE;i+=UAV_SIZE) {
        if (d_token_1.payload[i*UAV_SIZE]     == d_uav_1.mac_addr[0] && 
            d_token_1.payload[i*UAV_SIZE + 1] == d_uav_1.mac_addr[1] && 
            d_token_1.payload[i*UAV_SIZE + 2] == d_uav_1.mac_addr[2] && 
            d_token_1.payload[i*UAV_SIZE + 3] == d_uav_1.mac_addr[3] && 
            d_token_1.payload[i*UAV_SIZE + 4] == d_uav_1.mac_addr[4] && 
            d_token_1.payload[i*UAV_SIZE + 5] == d_uav_1.mac_addr[5]) {
            d_token_1.payload[i*UAV_SIZE + 6] =  d_uav_1.pos.x;
            d_token_1.payload[i*UAV_SIZE + 7] =  d_uav_1.pos.y;
            d_token_1.payload[i*UAV_SIZE + 8] =  d_uav_1.pos.z;
            not_exist = false;
            break;
        }
    }

    // if the uav is not exist in the list, include it to the list 
    if (not_exist) {
        d_token_1.header.num_uavs += 1;
        d_token_1.payload[num_uavs * UAV_SIZE]     = d_uav_1.mac_addr[0];
        d_token_1.payload[num_uavs * UAV_SIZE + 1] = d_uav_1.mac_addr[1];
        d_token_1.payload[num_uavs * UAV_SIZE + 2] = d_uav_1.mac_addr[2];
        d_token_1.payload[num_uavs * UAV_SIZE + 3] = d_uav_1.mac_addr[3];
        d_token_1.payload[num_uavs * UAV_SIZE + 4] = d_uav_1.mac_addr[4];
        d_token_1.payload[num_uavs * UAV_SIZE + 5] = d_uav_1.mac_addr[5];
        d_token_1.payload[num_uavs * UAV_SIZE + 6] = d_uav_1.pos.x;
        d_token_1.payload[num_uavs * UAV_SIZE + 7] = d_uav_1.pos.y;
        d_token_1.payload[num_uavs * UAV_SIZE + 8] = d_uav_1.pos.z;
    }

    return static_cast<size_t>(d_token_1.header.num_uavs);
}

/**
 * Process control messages that come from channel
 * 
 * @param msg data from channel
*/
void uav_system_impl::phy_in1(pmt::pmt_t msg)
{
    /** omit token header **/
    // this must be a pair
    // msg -> pmt::cons(pmt::mp("CTS"), pmt::make_blob(psdu, psdu_len))
    if (!pmt::is_blob(pmt::cdr(msg))) {
        throw std::runtime_error("PMT must be blob");
    }

    // strip token header
    pmt::pmt_t blob(pmt::cdr(msg));
    const char* mpdu = reinterpret_cast<const char*>(pmt::blob_data(blob));
    
    // mpdu[0] -> type of the control package

    // TODO:
    uint8_t type = decide(mpdu[0]);



    // create a dict then add
    pmt::pmt_t d_meta = pmt::make_dict();
    pmt::pmt_t d_msg  = pmt::make_blob(0, 0); // temp NULL make_blob
    if (type == CTS_TYPE) {
        d_meta = pmt::dict_add(d_meta, pmt::mp("subtype"), pmt::mp("CTS"));
        d_msg = pmt::make_blob(reinterpret_cast<const char*>(type), sizeof(uint8_t));
    }
    else if (type == RTS_TYPE) {
        d_meta = pmt::dict_add(d_meta, pmt::mp("subtype"), pmt::mp("RTS"));
        d_msg = pmt::make_blob(reinterpret_cast<const char*>(type), sizeof(uint8_t));
    }

    // pdu
    message_port_pub(pmt::mp("phy out1"), pmt::cons(d_meta, d_msg));
    // d_msg1 = pmt::cons(d_meta, d_msg);
}

uint8_t uav_system_impl::decide(uint8_t type)
{
    return CTS_TYPE;
}

/*
 * Our virtual destructor.
 */
uav_system_impl::~uav_system_impl()
{
    stop();
}

bool uav_system_impl::start()
{
    // d_finished = false;
    // d_thread = gr::thread::thread([this] { run(); });

    d_run_uav = true;
    d_uav_thread = gr::thread::thread([&] { run_uav(std::ref(d_uav_1)); });
    return block::start();
}

bool uav_system_impl::stop()
{
    // d_finished = true;
    // d_thread.interrupt();
    // d_thread.join();

    d_run_uav = false;
    d_uav_thread.interrupt();
    d_uav_thread.join();
    return block::stop();
}

void uav_system_impl::run()
{
    while (!d_finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(d_period_ms)));
        if (d_finished) 
            return;

        message_port_pub(pmt::mp("phy out0"), d_msg0);
        message_port_pub(pmt::mp("phy out1"), d_msg1);
    }
}

void uav_system_impl::run_uav(struct UAV& uav)
{
    while (d_run_uav) {
        // update positions
        std::this_thread::sleep_for(std::chrono::milliseconds(80U));
        if (!d_run_uav)
            return;

        uav.pos.x += rand() & 15;
        uav.pos.y += rand() & 15;
        uav.pos.z += rand() & 15;
    }
}

bool uav_system_impl::check_mac(std::vector<uint8_t> mac)
{
    if (mac.size() != MAC_ADDR_SIZE)
        return false;
    return true;
}

} /* namespace ieee802_11 */
} /* namespace gr */
