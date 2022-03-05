cmake_minimum_required(VERSION 3.11)

set (CMAKE_CXX_STANDARD 11)

project(QuickViewOnFp
        VERSION 0.0.1
        LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

include(cmake/CPM.cmake)

CPMFindPackage(
  NAME FrontPanelIO
  GITHUB_REPOSITORY dliess/FrontPanelIO
  GIT_TAG master
)
if(FrontPanelIO_ADDED)
  add_library(FrontPanelIO::FpBase ALIAS FpBase)
  add_library(FrontPanelIO::FpGfx ALIAS FpGfx)
  add_library(FrontPanelIO::FpDisplay ALIAS FpDisplay)
endif()

set(TARGET_NAME ${PROJECT_NAME})

set(SOURCES
    "fboquickview.cpp"
    "fboquickwindow.cpp"
    "QuickViewOnFpRenderMedium.cpp"
    #"QuickViewOnFpDisplay.cpp"
)

set(HEADERS
    "fboquickview.h"
    "QuickViewOnFpRenderMedium.h"
    "fboquickwindow.h"
    #"QuickViewOnFpDisplay.h"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME}
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>"
)

target_link_libraries(${TARGET_NAME}
  PUBLIC
    FrontPanelIO::FpDisplay
    Qt5::Quick
)

set_target_properties(${TARGET_NAME} PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

install(TARGETS ${TARGET_NAME}  EXPORT ${PROJECT_NAME}-targets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}-targets
  NAMESPACE  ${PROJECT_NAME}::
  DESTINATION  "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

configure_file(cmake/${PROJECT_NAME}Config.cmake.in cmake/${PROJECT_NAME}Config.cmake @ONLY)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config-version.cmake" COMPATIBILITY SameMajorVersion)
install(FILES 
           "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
           "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config-version.cmake" 
        DESTINATION 
           "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")