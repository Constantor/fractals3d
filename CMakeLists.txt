cmake_minimum_required(VERSION 3.19.3)
project(fractals3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(DEBUG True)
set(CMAKE_BUILD_TYPE Release)
set(DEBUG False)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # ?

set(INSTALL_DIR "./")

set(QT_VERSION 6)
set(QT_LIB_PREF Qt${QT_VERSION}::)

set(REQUIRED_LIBS Core Gui Widgets OpenGL OpenGLWidgets)
set(REQUIRED_LIBS_QUALIFIED ${QT_LIB_PREF}Core ${QT_LIB_PREF}Gui ${QT_LIB_PREF}Widgets ${QT_LIB_PREF}OpenGL ${QT_LIB_PREF}OpenGLWidgets)

add_compile_options(-Wall -Wextra -O3 -ffast-math)
if(UNIX AND DEBUG)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif(UNIX AND DEBUG)

include_directories(headers)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/ui")

add_executable(${PROJECT_NAME}
		main.cpp
		headers/MainWindow.hpp src/MainWindow.cpp ui/mainwindow.ui
		headers/FractalWidget.hpp src/FractalWidget.cpp
		headers/FractalData.hpp src/FractalData.cpp
		headers/GeometryEngine.hpp src/GeometryEngine.cpp
		headers/FractalPoint.hpp src/FractalPoint.cpp
		headers/Complex3D.hpp src/Complex3D.cpp
		headers/Fractal3D.hpp src/Fractal3D.cpp
		headers/Recorder.hpp src/Recorder.cpp
		)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBS_QUALIFIED})

set_target_properties(${PROJECT_NAME} PROPERTIES
		WIN32_EXECUTABLE TRUE
		MACOSX_BUNDLE TRUE
		AUTOMOC TRUE # for color picker
		)
#if (NOT CMAKE_PREFIX_PATH)
#    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
#            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
#endif (NOT CMAKE_PREFIX_PATH)

# Resources
set(shaders_resource_files
		"fshader.glsl"
		"vshader.glsl"
		)

qt_add_resources(fractals3d "shaders" PREFIX "/" FILES ${shaders_resource_files})
# End resources

install(TARGETS fractals3d
		RUNTIME DESTINATION "${INSTALL_DIR}"
		BUNDLE DESTINATION "${INSTALL_DIR}"
		LIBRARY DESTINATION "${INSTALL_DIR}"
		)
