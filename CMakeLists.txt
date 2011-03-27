cmake_minimum_required(VERSION 2.6)
project(decKreator)

find_package(KDE4 REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
include(${QT_USE_FILE})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(QT_USE_QTXML TRUE)

include_directories(${KDE4_INCLUDES} ${QT4_INCLUDES})
add_definitions( ${KDE4_DEFINITIONS} )
set(deckreatorSources main.cpp ImageWindow.cpp BrowseWidget.cpp)
kde4_add_executable(deckreator ${deckreatorSources})
target_link_libraries(deckreator ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KDEIO_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KDECORE_LIBS})
INSTALL(FILES custom-deck.svg DESTINATION ${DATA_INSTALL_DIR}/deckreator)
INSTALL(TARGETS deckreator DESTINATION bin)
