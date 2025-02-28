cmake_minimum_required(VERSION 3.5) #最低的CMake版本要求

project(RegistrationProj LANGUAGES CXX) # 定义项目名称，项目语言C++

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 自动选择 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 目录
set(Qt5_DIR "D:/QT/5.15.18/msvc2019_64/lib/cmake/Qt5")
find_package(Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets REQUIRED)

# OpenCV 目录
set(OpenCV_DIR "E:/Opencv/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

set(QRC_SOURCE_FILES Resource.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

if(ANDROID)
  add_library(RegistrationProj SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(RegistrationProj
    main.cpp
    ${QRC_FILES}
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    detection.cpp
    detection.h
  )
endif()

target_link_libraries(RegistrationProj PRIVATE Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets ${OpenCV_LIBS})
