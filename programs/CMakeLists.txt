
set(minibee_osc_SRCS
  main.cpp
)

add_executable( minibee2osc ${minibee_osc_SRCS})

target_link_libraries( minibee2osc sensestage ${LIBXBEEP_LIBRARIES} ${LIBXBEE_LIBRARIES} ${liblo_LIBRARIES} )

install(TARGETS minibee2osc DESTINATION bin)
