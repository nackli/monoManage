cmake_minimum_required(VERSION 3.16)
project(monoDevCtrl VERSION 2.5.0)


if(CMAKE_CROSSCOMPILING)
    message("CMAKE_CROSSCOMPILING")
    include_directories("~/work/mono-6.12/install-aarch/include/mono-2.0")
    link_directories("~/work/mono-6.12/install-aarch/lib")
    link_directories("~/work/libzmq/aarch64-install/lib")
else()
    link_directories("/usr/lib")
    include_directories("/usr/include/mono-2.0")
endif()

include_directories("../libzmq/include")
include_directories("./rapidjson/include")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -O3 -fPIC -g")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -fPIC  -g")

file(GLOB  DEV_SOURCES_LIB ./*.cpp)
add_library(monoDevCtrl STATIC ${DEV_SOURCES_LIB})


add_executable(${PROJECT_NAME}_test ${DEV_SOURCES_LIB})
target_link_libraries(${PROJECT_NAME}_test -lmono-2.0 zmq)

#target_link_libraries(TestMono -lmono-2.0)
#target_link_libraries(TestMono ./TestMonoExe/TestMonoDll.so)
#target_link_libraries(CompentDev ComponentCore)

#if(MSVC)
#    add_custom_command(TARGET CompentDev POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/config.yml" "${CMAKE_CURRENT_BINARY_DIR}/config.yml")
#    set_target_properties(CompentDev PROPERTIES COMPILE_FLAGS "/EHsc")
#else()
#    add_custom_command(TARGET CompentDev POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/config.yml" "${CMAKE_BINARY_DIR}/config.yml")
#endif()
# if(CMAKE_CROSSCOMPILING )
#     set_target_properties(monoDevCtrl PROPERTIES 
#             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
# else()
#     set_target_properties(monoDevCtrl PROPERTIES 
#             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../linux_bin)
# endif() 

#set_target_properties(TestMono PROPERTIES OUTPUT_NAME TestMonoExe)
#set_target_properties(CompentDev PROPERTIES ENABLE_EXPORTS True)
