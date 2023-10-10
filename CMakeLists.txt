cmake_minimum_required(VERSION 3.0)

project(osbadapples LANGUAGES C)

add_executable(osbadapples main.c)

install(TARGETS osbadapples RUNTIME DESTINATION bin)
