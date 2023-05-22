#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gnuradio::gnuradio-ieee802_11" for configuration "Release"
set_property(TARGET gnuradio::gnuradio-ieee802_11 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gnuradio::gnuradio-ieee802_11 PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libgnuradio-ieee802_11.so.ga1e1f53"
  IMPORTED_SONAME_RELEASE "libgnuradio-ieee802_11.so.1.1.0git"
  )

list(APPEND _cmake_import_check_targets gnuradio::gnuradio-ieee802_11 )
list(APPEND _cmake_import_check_files_for_gnuradio::gnuradio-ieee802_11 "${_IMPORT_PREFIX}/lib/libgnuradio-ieee802_11.so.ga1e1f53" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
