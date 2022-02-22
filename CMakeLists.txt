cmake_minimum_required(VERSION 3.12)

project(BlueMarbleCC)

add_executable(BlueMarbleCC main.cpp)

target_include_directories(BlueMarbleCC PRIVATE deps/glm
                                                deps/stb
                                                deps/glfw/include
                                                deps/glew/include)

target_link_directories(BlueMarbleCC PRIVATE deps/glfw/lib-vc2022
                                             deps/glew/lib/Release/x64)

target_link_libraries(BlueMarbleCC PRIVATE glfw3.lib
                                           glew32.lib)

add_custom_command(TARGET BlueMarbleCC POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll")



