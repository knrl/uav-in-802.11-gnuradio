/* -*- c++ -*- */
/*
 * Copyright 2023 mkaanerol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_IEEE802_11_UAV_SYSTEM_IMPL_H
#define INCLUDED_IEEE802_11_UAV_SYSTEM_IMPL_H

#include <ieee802_11/uav_system.h>
#include <atomic>
#include <vector>

namespace gr {
namespace ieee802_11 {

constexpr uint32_t MAX_PAYLOAD_SIZE = 1500;
constexpr uint8_t MAC_ADDR_SIZE     = 6;
constexpr uint8_t TOKEN_HEADER_SIZE = 14;
constexpr uint8_t CRC_SIZE = 4;
constexpr uint8_t UAV_SIZE = 9;
constexpr uint8_t CTS_TYPE = 1;
constexpr uint8_t RTS_TYPE = 2; 

struct Position {
    uint8_t x;
    uint8_t y;
    uint8_t z;
}__attribute__((packed));

struct UAV {
    uint8_t mac_addr[MAC_ADDR_SIZE];
    struct Position pos;
}__attribute__((packed));

struct TokenHeader { // token header
    uint8_t token_id;
    uint8_t source_mac_addr[MAC_ADDR_SIZE];
    uint8_t dest_mac_addr[MAC_ADDR_SIZE];
    uint8_t num_uavs;
}__attribute__((packed));

struct Token {
    struct TokenHeader header;
    uint8_t payload[MAX_PAYLOAD_SIZE];
};

struct ControlHeader {
    uint8_t type;
};

class uav_system_impl : public uav_system
{
private:
    gr::thread::thread d_uav_thread;
    std::atomic<bool>  d_run_uav;
    bool d_master;
    long d_period_ms;
    pmt::pmt_t d_msg0;
    pmt::pmt_t d_msg1;

    struct UAV   d_uav_1;
    struct Token d_token_1;

    uint8_t d_psdu[MAX_PAYLOAD_SIZE];

    std::atomic<bool>  d_finished;
    gr::thread::thread d_thread;
    void run();

    void run_uav(struct UAV& uav);
    void initial_token(const uint8_t token_id);
    void phy_in0(pmt::pmt_t msg);
    void phy_in1(pmt::pmt_t msg);

    void generate_mac_data_frame(int msdu_size);

public:
    uav_system_impl(std::vector<uint8_t> src, long period_ms, bool master);
    ~uav_system_impl();

    uint8_t decide(uint8_t type);
    void update_token();
    size_t update_cache(const char* msdu, size_t num_uavs);

    bool check_mac(std::vector<uint8_t> mac);
    void set_master(bool master) { d_master = master; };
    bool get_master() const { return d_master; }
    void set_period(long period_ms) { d_period_ms = period_ms; }
    long period() const { return d_period_ms; }

    bool start();
    bool stop();
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_UAV_SYSTEM_IMPL_H */
