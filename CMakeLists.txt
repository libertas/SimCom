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
