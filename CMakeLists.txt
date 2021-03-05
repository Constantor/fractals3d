cmake_minimum_required(VERSION 3.17)
project(fractals3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(DEBUG True)
set(DEBUG False)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # ?

set(INSTALL_DIR "./")

set(QT_VERSION 6)
set(QT_LIB_PREF Qt${QT_VERSION}::)

set(REQUIRED_LIBS Core Gui Widgets OpenGL OpenGLWidgets)
foreach(lib ${REQUIRED_LIBS})
	set(REQUIRED_LIBS_QUALIFIED "${REQUIRED_LIBS_QUALIFIED};${QT_LIB_PREF}${lib}")
endforeach()

add_compile_options(-Wall -Wextra -O3 -ffast-math)
if (UNIX AND DEBUG)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif (UNIX AND DEBUG)

include_directories(headers)

add_executable(${PROJECT_NAME}
        main.cpp mainwindow.cpp mainwindow.hpp mainwindow.ui fractalwidget.hpp
        fractalwidget.cpp fractaldata.hpp fractaldata.cpp
        src/GeometryEngine.cpp headers/GeometryEngine.hpp
        src/FractalPoint.cpp headers/FractalPoint.hpp
        src/Complex3D.cpp headers/Complex3D.hpp
        src/Fractal3D.cpp headers/Fractal3D.hpp
)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBS_QUALIFIED})

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        )
#if (NOT CMAKE_PREFIX_PATH)
#    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
#            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
#endif (NOT CMAKE_PREFIX_PATH)

if(WIN32)
	foreach(lib ${REQUIRED_LIBS_QUALIFIED})
		message(STATUS "Added ${lib}")
		add_custom_command(
			TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			    $<TARGET_FILE:${lib}>
				$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
    endforeach()

	set(CMAKE_USR_DIR "${Qt${QT_VERSION}_DIR}/../../..")
	message(STATUS ${CMAKE_USR_DIR}/plugins/platforms/qwindows.dll)

	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		    ${CMAKE_USR_DIR}/plugins/platforms/qwindows.dll
			$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/qwindows.dll
	)
endif()

# Resources
set(shaders_resource_files
        "fshader.glsl"
        "vshader.glsl"
)

qt6_add_resources(fractals3d "shaders" PREFIX "/" FILES ${shaders_resource_files})
set(textures_resource_files "cube.png")

qt6_add_resources(fractals3d "textures" PREFIX "/" FILES ${textures_resource_files})
# End resources

install(TARGETS fractals3d
        RUNTIME DESTINATION "${INSTALL_DIR}"
        BUNDLE DESTINATION "${INSTALL_DIR}"
        LIBRARY DESTINATION "${INSTALL_DIR}"
)
