cmake_minimum_required(VERSION 3.14)
project(libusb_sandbox VERSION 1.0 LANGUAGES CXX C)

include(CMakePrintHelpers)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
cmake_print_variables(LIBUSB_LIBRARIES LIBUSB_INCLUDE_DIRS LIBUSB_LINK_LIBRARIES)

add_executable(listdevs listdevs.c)

target_include_directories(listdevs PRIVATE 
    ${LIBUSB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(listdevs PRIVATE ${LIBUSB_LINK_LIBRARIES} )
