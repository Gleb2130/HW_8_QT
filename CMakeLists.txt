cmake_minimum_required(VERSION 3.20)

project(HW_8_QT)
set(CMAKE_CXX_STANDARD 20)

set(Qt_DIR "C:/Qt/6.8.0/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCES
    main.cpp
    HW_8_QT.cpp
    Player.cpp
    Question.cpp
)

set(HEADERS
    HW_8_QT.h
    Player.h
    Question.h
    resource.h
)

set(RESOURCES
    HW_8_QT.qrc
)

set(UI_FILES
    HW_8_QT.ui
)
qt6_wrap_ui(UI_GENERATED ${UI_FILES})
qt6_add_resources(RESOURCE_GENERATED ${RESOURCES})

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_GENERATED}
    ${RESOURCE_GENERATED}
)

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)
