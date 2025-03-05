cmake_minimum_required(VERSION 3.0)

project(worms2d VERSION 0.0.1)

set(AUTOUIC ON)
set(AUTOMOC ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer 2.6.3 REQUIRED)
find_package(SDL2pp REQUIRED)
find_package(spdlog REQUIRED)
find_package(box2d REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Multimedia)
find_package(yaml-cpp REQUIRED)
find_package(GTest REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${SDL2PP_INCLUDE_DIRS})
include_directories(${SPDLOG_INCLUDE_DIRS})
include_directories(${BOX2D_INCLUDE_DIRS})
include_directories(${yaml-cpp_INCLUDE_DIRS})
include_directories(${GTest_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -pedantic-errors -O0 -ggdb -DDEBUG -fno-inline -D _POSIX_C_SOURCE=200809L")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TARGET_DIR ../target)
set(QT_RES client/resources/images.qrc)

add_definitions(-DDATA_PATH="${PROJECT_SOURCE_DIR}/sprites")
add_definitions(-DMEDIA_PATH="${PROJECT_SOURCE_DIR}/client/resources/sounds")
add_definitions(-DFONT_PATH="${PROJECT_SOURCE_DIR}/client/resources/font")
add_definitions(-DYAML_PATH="${PROJECT_SOURCE_DIR}/yaml")
include_directories(${SDL2PP_INCLUDE_DIRS})

file(GLOB COMMON_SRC common/*.cpp)
file(GLOB CLIENT_SRC client/*.cpp client/main.cpp)
file(GLOB CLIENT_H client/*.h)
file(GLOB SERVER_SRC server/*.cpp server/main.cpp)
file(GLOB UI_SRC client/*.ui)

set(QT_CLIENT_SRC ${CLIENT_SRC}
        client/TeamResourcesHolder.h
        client/TeamResourcesHolder.cpp)
qt_wrap_ui(QT_CLIENT_SRC ${UI_SRC})
qt_wrap_cpp(QT_CLIENT_SRC ${CLIENT_H})
qt_add_resources(QT_RES client/resources/images.qrc)
qt_add_resources(QT_RES client/resources/sounds.qrc)
qt_add_resources(QT_RES client/resources/cheat_images.qrc)

add_executable(client_exe ${COMMON_SRC} ${QT_CLIENT_SRC} ${QT_RES})

target_include_directories(client_exe PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(client_exe PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(client_exe PUBLIC ${SDL2_MIXER_INCLUDE_DIRS})

target_link_libraries(client_exe PUBLIC Qt6::Core)
target_link_libraries(client_exe PUBLIC Qt6::Widgets)
target_link_libraries(client_exe PUBLIC Qt6::Gui)
target_link_libraries(client_exe PUBLIC Qt6::Multimedia)
target_link_libraries(client_exe PRIVATE spdlog::spdlog)
target_link_libraries(client_exe PUBLIC SDL2pp::SDL2pp)
target_link_libraries(client_exe PUBLIC SDL2)
target_link_libraries(client_exe PUBLIC SDL2_image)
target_link_libraries(client_exe PUBLIC SDL2_mixer)
target_link_libraries(client_exe PUBLIC ${SDL2PP_LIBRARIES})
target_link_libraries(client_exe PUBLIC yaml-cpp)


add_executable(server_exe ${COMMON_SRC} ${SERVER_SRC})

target_link_libraries(server_exe PRIVATE spdlog::spdlog)
target_link_libraries(server_exe PUBLIC box2d)
target_link_libraries(server_exe PUBLIC yaml-cpp)
target_link_libraries(server_exe ${GTest_LIBRARIES})

set_target_properties(client_exe server_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})
set_target_properties(client_exe PROPERTIES OUTPUT_NAME worms2d)
set_target_properties(server_exe PROPERTIES OUTPUT_NAME worms2dsv)

enable_testing()

add_executable(
  worms2d_test
  ${COMMON_SRC} client/cprotocol.cpp server/sprotocol.cpp test/protocol_test.cpp
)

target_link_libraries(
  worms2d_test
  box2d
  GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(worms2d_test)
