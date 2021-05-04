###########################################################################
## This file is part of the minilockcpp distribution 
## (https://github.com/mrom1/minilockcpp).
## Copyright (c) 2021 mrom1.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, version 3.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program. If not, see <http://www.gnu.org/licenses/>.
###########################################################################




###########################################################################
## Find 3rd party packages
###########################################################################
find_package(Qt5 COMPONENTS Widgets Qml Core Quick REQUIRED)
find_package (Threads REQUIRED)


###########################################################################
## Build minilock GUI
###########################################################################
add_definitions(${Qt5Widgets_DEFINITIONS} 
    ${QtQml_DEFINITIONS} 
    ${Qt5Quick_DEFINITIONS})
qt5_add_resources(QT_RESOURCES qml.qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(minilock main.cpp 
    QMinilock.cpp QMinilock.h 
    QMinilockWorker.h 
    ${QT_RESOURCES})

target_include_directories(minilock PRIVATE 
    ${Qt5Widgets_INCLUDE_DIRS} 
    ${QtQml_INCLUDE_DIRS} 
    ${MINILOCK_INCLUDES})
    
target_link_libraries(minilock minilockcpp
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Threads::Threads)
