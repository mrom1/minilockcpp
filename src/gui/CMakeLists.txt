######################################################################################
## Find 3rd party packages
######################################################################################
find_package(Qt5 COMPONENTS Widgets Qml Core Quick REQUIRED)
find_package (Threads REQUIRED)


######################################################################################
## Build minilock GUI
######################################################################################
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
qt5_add_resources(QT_RESOURCES qml.qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(minilock main.cpp QMinilock.cpp QMinilock.h QMinilockWorker.h ${QT_RESOURCES})
target_include_directories(minilock PRIVATE ${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS} ${MINILOCK_INCLUDES})
target_link_libraries(minilock minilockcpp
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Threads::Threads)
