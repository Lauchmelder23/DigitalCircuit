cmake_minimum_required(VERSION 3.10)

project(DigitalCircuit)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE INC "include/*.hpp")
file(GLOB_RECURSE UI "ui/*.ui")

qt_wrap_ui(UI_INC ${UI})
qt_add_resources(RESOURCES ui/assets/resources.qrc)

message(STATUS ${RESOURCES})

add_executable(DigitalCircuit
	${RESOURCES} ${SRC} ${INC} ${UI_INC}
)

target_include_directories(DigitalCircuit PRIVATE
	include
)

target_link_libraries(DigitalCircuit PRIVATE
	Qt5::Widgets
)

if(WIN32)
	add_custom_command(TARGET DigitalCircuit POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:DigitalCircuit>
	)
endif()