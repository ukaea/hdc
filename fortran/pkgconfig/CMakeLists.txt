set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
configure_file("fhdc.pc.in" "${CMAKE_BINARY_DIR}/lib/pkgconfig/fhdc.pc" @ONLY)

install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/pkgconfig DESTINATION lib)
