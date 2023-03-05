# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required (VERSION 3.5)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif ()

# Set the project name
project (geo_encode)

# Generate compile_commands.json
set (CMAKE_EXPORT_COMPILE_COMMANDS True)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    main.c
    geo_hash.c
    geo_country.c
    math_lib.c
    cJSON.c
    time/timestamp.c
    log/log.c
)

# Add an executable with the above sources
add_executable(geo_encode ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(geo_encode
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)
