# Install script for directory: /home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ieee802_11" TYPE FILE FILES
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/api.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/chunks_to_symbols.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/constellations.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/decode_mac.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/ether_encap.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/extract_csi.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/frame_equalizer.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/mac.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/mapper.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/parse_mac.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/signal_field.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/sync_long.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/sync_short.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/uav_system.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/my_strobe.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/ofdm_mapper.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/ofdm_demapper.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/alamouti_decode_cc.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/alamouti_encode_cc.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/alamouti_ofdm_rx.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/alamouti_ofdm_tx_cc.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/ofdm_insert_preamble_tagged.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/preamble_detector.h"
    "/home/kaan/Documents/local-works/gnuradio_practices/gr-ieee802-11/include/ieee802_11/ofdm_sampler.h"
    )
endif()

