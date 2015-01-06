# Create a library called "xbee_osc" which includes the source files
# The extension is already found. Any number of sources could be listed here.
# add_library (xbee_osc BoostSerialPort.cpp)

# target_link_libraries(xbee_osc ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} )

# Make sure the compiler can find include files for our xbee_osc library
# when other libraries or executables link to Hello
# target_include_directories (xbee_osc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable( minibee main.cpp MiniBee.cpp MiniHive.cpp MiniBeeConfig.cpp)
target_link_libraries( minibee ${LIBXBEEP_LIBRARIES}  )

install(TARGETS minibee DESTINATION bin)
