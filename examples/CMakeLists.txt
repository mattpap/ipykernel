include_directories(${ipykernel_SOURCE_DIR}/src)

add_executable(echo echo.c)
target_link_libraries(echo ipykernel ${LIBS})
