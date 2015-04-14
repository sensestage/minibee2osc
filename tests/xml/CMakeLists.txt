# cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
# cmake_policy(VERSION 2.6)

# pkg_check_modules(tinyxml2 REQUIRED tinyxml2)

add_executable(xmlparser xmlparsing.cpp)

# add_dependencies(xmlparser tinyxml2)
# target_link_libraries( minibee2osc sensestage ${LIBXBEEP_LIBRARIES} ${LIBXBEE_LIBRARIES} ${liblo_LIBRARIES} )
target_link_libraries(xmlparser ${TINYXML2_LIBRARY})


add_executable(xmlsearcher xmlsearching.cpp)

# add_dependencies(xmlparser tinyxml2)
# target_link_libraries( minibee2osc sensestage ${LIBXBEEP_LIBRARIES} ${LIBXBEE_LIBRARIES} ${liblo_LIBRARIES} )
target_link_libraries(xmlsearcher ${TINYXML2_LIBRARY})
