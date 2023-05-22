# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT ieee802_11_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT ieee802_11_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND ieee802_11_python_files
    chunks_to_symbols_python.cc
    constellations_python.cc
    decode_mac_python.cc
    ether_encap_python.cc
    extract_csi_python.cc
    frame_equalizer_python.cc
    mac_python.cc
    mapper_python.cc
    parse_mac_python.cc
    signal_field_python.cc
    sync_long_python.cc
    sync_short_python.cc
    uav_system_python.cc
    my_strobe_python.cc
    ofdm_mapper_python.cc
    ofdm_demapper_python.cc
    alamouti_decode_cc_python.cc
    alamouti_encode_cc_python.cc
    alamouti_ofdm_rx_python.cc
    alamouti_ofdm_tx_cc_python.cc
    ofdm_insert_preamble_tagged_python.cc
    preamble_detector_python.cc
    ofdm_sampler_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(ieee802_11 
   ../..
   gr::ieee802_11
   "${ieee802_11_python_files}")

install(TARGETS ieee802_11_python DESTINATION ${GR_PYTHON_DIR}/ieee802_11 COMPONENT pythonapi)
