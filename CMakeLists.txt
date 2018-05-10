cmake_minimum_required(VERSION 3.1)

if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

project(SimplyCpp)

set (CMAKE_CXX_STANDARD 14)


find_package(wxWidgets COMPONENTS base core stc aui REQUIRED)

include( "${wxWidgets_USE_FILE}" )

add_executable(
    ${PROJECT_NAME}
    src/stdafx.h
    src/stdafx.cpp
    src/UI/Utils.h
    src/UI/Application.h
    src/UI/Application.cpp
    src/UI/EditorWidget.h
    src/UI/EditorWidget.cpp
    src/UI/ErrorList.h
    src/UI/ErrorList.cpp
    src/UI/MainFrame.h
    src/UI/MainFrame.cpp
    src/UI/ProjectExplorer.h
    src/UI/ProjectExplorer.cpp
    src/UI/TerminalWidget.h
    src/UI/TerminalWidget.cpp
    src/Core/Config.h
    src/Core/Config.cpp
    src/Core/Project.h
    src/Core/Project.cpp
    src/Core/SourceFile.h
    src/Core/SourceFile.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    ${wxWidgets_LIBRARIES}
    )
