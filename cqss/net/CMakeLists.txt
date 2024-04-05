set(TARGET net)

aux_source_directory(. SRCS_DIR)
add_executable(
    ${TARGET}
    ${SRCS_DIR}
)
target_link_libraries(${TARGET} glog::glog absl::strings)

install(
    TARGETS ${TARGET}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_ARCDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# file(GLOB ERRC_HEADER_FILES "*.h" "*.hpp")
# install(FILES ${ERRC_HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${TARGET})
