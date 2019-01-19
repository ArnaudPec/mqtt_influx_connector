cmake_minimum_required(VERSION 2.8.9)
project(mqtt_client)

find_package(PkgConfig REQUIRED)

pkg_check_modules(MOSQUITTO libmosquitto REQUIRED)
pkg_check_modules(JSON json-c REQUIRED)
pkg_check_modules(CURL libcurl REQUIRED)

set(INFLUX_DB_INCLUDE_DIRS "/usr/local/include/influxdb")
set(INFLUX_DB_LIBRARY_DIRS "/usr/local/lib")
set(INFLUX_DB_LIBRARIES "-linfluxdb -lcurl")

include_directories(${MOSQUITTO_INCLUDE_DIRS}
                    ${JSON_INCLUDE_DIRS}
                    ${INFLUX_DB_INCLUDE_DIRS})

link_directories(${MOSQUITTO_LIBRARY_DIRS}
                 ${JSON_LIBRARY_DIRS}
                 ${INFLUX_DB_LIBRARY_DIRS})

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_parser.c
    )

add_executable(main ${SRCS})

target_link_libraries(main ${MOSQUITTO_LIBRARIES}
                           ${JSON_LIBRARIES}
                           ${INFLUX_DB_LIBRARIES})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DIST_DIR "${CMAKE_PROJECT_NAME}")
add_custom_target(dist
                  COMMAND rm -rf ${DIST_DIR}
                  COMMAND mkdir ${DIST_DIR}
                  COMMAND cp -r ${CMAKE_SOURCE_DIR}/* ${DIST_DIR} || true
                  COMMAND rm -rf ${DIST_DIR}/build
                  COMMAND mkdir ${DIST_DIR}/build
                  COMMAND tar --exclude="*~" --exclude="._*" -zcvf ${DIST_DIR}.tar.gz ${DIST_DIR}
                  COMMAND rm -rf  ${DIST_DIR}
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})


add_custom_target(cppcheck
                  COMMAND /usr/bin/cppcheck
                  --enable=all
                  --std=c89
                  --inconclusive
                  --library=gnu,posix
                  ${SRCS})
