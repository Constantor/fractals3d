cmake_minimum_required(VERSION 3.19.3)
project(fractals3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(DEBUG True)
set(DEBUG False)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # ?

set(INSTALL_DIR "./")

set(QT_VERSION 6)

set(REQUIRED_LIBS Core Gui Widgets 3DCore 3DRender Concurrent 3DInput 3DExtras OpenGL OpenGLWidgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets Qt6::3DCore Qt6::3DRender Qt6::Concurrent Qt6::3DInput Qt6::3DExtras Qt6::OpenGL Qt6::OpenGLWidgets)

add_compile_options(-Wall -Wextra -O3 -ffast-math)
if (UNIX AND DEBUG)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif (UNIX AND DEBUG)

include_directories(headers)

add_executable(${PROJECT_NAME}
        main.cpp
        mainwindow.cpp mainwidget.h mainwindow.ui
        mainwidget.cpp mainwidget.h
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
