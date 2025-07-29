cmake_minimum_required(VERSION 3.28)
project(alien_boogie)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/alien.cpp
    src/audio_capture.cpp
    src/score_label.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
	SFML::Graphics
	SFML::Audio
)

# This command copies the asset dir to the output for relative filepath goodness
add_custom_command(TARGET alien_boogie POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:alien_boogie>/assets
)