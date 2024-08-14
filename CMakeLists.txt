cmake_minimum_required(VERSION 3.16)

project(BufferIssueDemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.6 REQUIRED COMPONENTS Quick Core Multimedia)

qt_standard_project_setup(REQUIRES 6.6)

qt_add_executable(appBufferIssueDemo
    main.cpp
)

qt_add_qml_module(appBufferIssueDemo
    URI BufferIssueDemo
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES audio_file.h audio_file.cpp
        SOURCES audio_player.h audio_player.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBufferIssueDemo PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBufferIssueDemo
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBufferIssueDemo
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Core
    PRIVATE Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appBufferIssueDemo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
