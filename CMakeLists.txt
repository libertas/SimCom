cmake_minimum_required(VERSION 3.2)

project(SimCom)

set(MAIN_FILE SimCom.cpp)
set(SRC_LIST ${MAIN_FILE} PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp)

if(DEFINED ROOT_DIR)
  set(CMAKE_EXE_LINKER_FLAGS "--sysroot ${ROOT_DIR}")
  set(CMAKE_SHARED_LINKER_FLAGS "--sysroot ${ROOT_DIR}")
endif()

find_package(Threads)
find_package(Qt5SerialPort REQUIRED)

add_compile_options(-std=c++11)
link_directories(${LINK_DIR})

add_library(simcom_static STATIC ${SRC_LIST})
add_library(simcom SHARED ${SRC_LIST})
target_link_libraries (simcom ${CMAKE_THREAD_LIBS_INIT})
qt5_use_modules(simcom_static SerialPort)
qt5_use_modules(simcom SerialPort)

add_executable(test_sl ${MAIN_FILE})
target_link_libraries(test_sl ${CMAKE_THREAD_LIBS_INIT} simcom_static)
target_compile_definitions(test_sl PRIVATE -D TEST_SERVICE)
qt5_use_modules(test_sl SerialPort)

add_executable(test_dl ${MAIN_FILE})
target_link_libraries(test_dl ${CMAKE_THREAD_LIBS_INIT} simcom_static)
target_compile_definitions(test_dl PRIVATE -D TEST_DATALINK)
qt5_use_modules(test_dl SerialPort)

add_executable(test_ph ${MAIN_FILE})
target_link_libraries(test_ph ${CMAKE_THREAD_LIBS_INIT} simcom_static)
target_compile_definitions(test_ph PRIVATE -D TEST_PHYSICAL)
qt5_use_modules(test_ph SerialPort)
