cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 11)

project(QuickViewOnFp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_library(${PROJECT_NAME} STATIC
    "fboquickview.cpp"
    "fboquickwindow.cpp"
    "QuickViewOnFpRenderMedium.cpp"
#    "QuickViewOnFpDisplay.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    FpDisplay
    Qt5::Quick
)