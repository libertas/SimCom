cmake_minimum_required(VERSION 3.2)

project(SimCom)


set(SRC_LIST SimCom.cpp PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp)
set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

find_package(Threads)

add_compile_options(-std=c++11)

add_library(simcom SHARED ${SRC_LIST})
target_link_libraries (simcom ${CMAKE_THREAD_LIBS_INIT} serial)
add_library(simcom_static STATIC ${SRC_LIST})

add_executable(test_sl ${SRC_LIST})
target_link_libraries (test_sl ${CMAKE_THREAD_LIBS_INIT} serial)
target_compile_definitions(test_sl PRIVATE -D TEST_SERVICE)

add_executable(test_dl ${SRC_LIST})
target_link_libraries (test_dl ${CMAKE_THREAD_LIBS_INIT} serial)
target_compile_definitions(test_dl PRIVATE -D TEST_DATALINK)

add_executable(test_ph ${SRC_LIST})
target_link_libraries (test_ph ${CMAKE_THREAD_LIBS_INIT} serial)
target_compile_definitions(test_ph PRIVATE -D TEST_PHYSICAL)
