cmake_minimum_required(VERSION 3.2)

project(SimCom)


set(MAIN_FILE SimCom.cpp)
set(SRC_LIST ${MAIN_FILE} PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp)

find_package(Threads)

add_compile_options(-std=c++11)

add_library(simcom_static STATIC ${SRC_LIST})
add_library(simcom SHARED ${SRC_LIST})
target_link_libraries (simcom ${CMAKE_THREAD_LIBS_INIT} serial)

add_executable(test_sl ${MAIN_FILE})
target_link_libraries(test_sl ${CMAKE_THREAD_LIBS_INIT} simcom_static serial)
target_compile_definitions(test_sl PRIVATE -D TEST_SERVICE)

add_executable(test_dl ${MAIN_FILE})
target_link_libraries(test_dl ${CMAKE_THREAD_LIBS_INIT} simcom_static serial)
target_compile_definitions(test_dl PRIVATE -D TEST_DATALINK)

add_executable(test_ph ${MAIN_FILE})
target_link_libraries(test_ph ${CMAKE_THREAD_LIBS_INIT} simcom_static serial)
target_compile_definitions(test_ph PRIVATE -D TEST_PHYSICAL)
