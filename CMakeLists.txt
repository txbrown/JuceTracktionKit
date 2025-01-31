cmake_minimum_required(VERSION 3.15)
project(tracktion_static)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build (Debug/Release)" FORCE)
endif()

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure build-specific flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG=1" CACHE STRING "Debug flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG=1" CACHE STRING "Release flags" FORCE)

# Add required definitions before anything else
add_compile_definitions(
    JUCE_MODAL_LOOPS_PERMITTED=1)

# Disable JUCE examples and extras before including anything
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "Build JUCE examples" FORCE)
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "Build JUCE extras" FORCE)

# Override the original CMakeLists.txt behavior
# This will prevent add_subdirectory(examples) from being executed
set(TRACKTION_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)

# Disable testing
macro(enable_testing)
    # Do nothing
endmacro()

# Create a function to override add_subdirectory
function(add_subdirectory)
    if(NOT "${ARGV0}" STREQUAL "examples")
        _add_subdirectory(${ARGV})
    endif()
endfunction()

# Add the original tracktion_engine project
add_subdirectory(tracktion_engine)

# Create our static library
add_library(tracktion_static STATIC)

# Set output names based on configuration
set_target_properties(tracktion_static PROPERTIES 
    OUTPUT_NAME_DEBUG "tracktion_static"
    OUTPUT_NAME_RELEASE "tracktion_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")

# Link against the tracktion modules
target_link_libraries(tracktion_static
    PRIVATE
        tracktion::tracktion_core
        tracktion::tracktion_engine
        tracktion::tracktion_graph)

# Add necessary compile definitions
target_compile_definitions(tracktion_static
    PUBLIC
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        TRACKTION_ENABLE_TIMESTRETCH_SOUNDTOUCH=1
        JUCE_PLUGINHOST_VST3=1
        JUCE_PLUGINHOST_AU=1
        TRACKTION_ENABLE_MIDI_RECORDING=1
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_STRICT_REFCOUNTEDPOINTER=0
    $<$<CONFIG:Debug>:JUCE_DEBUG=1>
    $<$<CONFIG:Release>:JUCE_DEBUG=0>)

# Install rules with configuration-specific paths
install(TARGETS tracktion_static
    CONFIGURATIONS Debug Release
    ARCHIVE 
        DESTINATION lib/$<CONFIG>
        COMPONENT libraries)