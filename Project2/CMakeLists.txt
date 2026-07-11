cmake_minimum_required(VERSION 3.21)

project(WaitLessLiftMore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Qml QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(WaitLessLiftMore
    main.cpp
)

set_source_files_properties(Project2/Constants.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(WaitLessLiftMore
    URI Project2
    VERSION 1.0
    QML_FILES
        Project2Content/App.qml
        Project2Content/Screen01.qml
        Project2/Constants.qml
    SOURCES
        BackendService.cpp
        BackendService.h
        src/data/GymRecord.h
        src/data/loader.cpp
        src/data/loader.h
        src/structures/hashmap.cpp
        src/structures/hashmap.h
        src/structures/heap.cpp
        src/structures/heap.h
)

qt_add_resources(WaitLessLiftMore "application_configuration"
    PREFIX "/"
    FILES qtquickcontrols2.conf
)

target_link_libraries(WaitLessLiftMore
    PRIVATE
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
)

set_target_properties(WaitLessLiftMore PROPERTIES
    WIN32_EXECUTABLE TRUE
)
