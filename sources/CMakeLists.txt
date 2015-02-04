# Create a library called "xbee_osc" which includes the source files
# The extension is already found. Any number of sources could be listed here.
# add_library (xbee_osc BoostSerialPort.cpp)

# target_link_libraries(xbee_osc ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} )

# Make sure the compiler can find include files for our xbee_osc library
# when other libraries or executables link to Hello
# target_include_directories (xbee_osc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(minibee_osc_SRCS
  main.cpp
  MiniBee.cpp
  MiniHive.cpp
  MiniBeeConfig.cpp
  minibee_utils.cpp
  MiniHiveOSC.cpp
  osc/lo_extensions.cpp
  osc/oscin.cpp
)

add_executable( minibee2osc  ${minibee_osc_SRCS})

target_link_libraries( minibee2osc ${LIBXBEEP_LIBRARIES} ${LIBXBEE_LIBRARIES} ${liblo_LIBRARIES} )

install(TARGETS minibee2osc DESTINATION bin)
