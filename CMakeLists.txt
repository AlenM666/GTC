cmake_minimum_required(VERSION 3.25...4.4 FATAL_ERROR)

# ==============================================================================
# Project
# ==============================================================================

project(
    GTC
    VERSION 0.0.1
    DESCRIPTION "GTC"
    LANGUAGES C CXX
)

# ==============================================================================
# Policies / global project configuration
# ==============================================================================

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

# Generate compile_commands.json for clangd / Neovim / tooling.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use modern CMake behavior.
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_EXTENSIONS OFF)

# C++ standard.
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only require C11 for raylib / C-side dependencies.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Prevent accidental in-source builds.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
        "In-source builds are not supported.\n"
        "Use: cmake --preset linux-dev"
    )
endif()

# ==============================================================================
# Build options
# ==============================================================================

option(MYGAME_BUILD_TESTS
    "Build tests"
    OFF
)

option(MYGAME_BUILD_TOOLS
    "Build developer tools"
    OFF
)

option(MYGAME_ENABLE_ASAN
    "Enable AddressSanitizer"
    OFF
)

option(MYGAME_ENABLE_UBSAN
    "Enable UndefinedBehaviorSanitizer"
    OFF
)

option(MYGAME_ENABLE_TSAN
    "Enable ThreadSanitizer"
    OFF
)

option(MYGAME_ENABLE_MSAN
    "Enable MemorySanitizer where supported"
    OFF
)

option(MYGAME_ENABLE_LTO
    "Enable Link Time Optimization"
    ON
)

option(MYGAME_ENABLE_NATIVE_ARCH
    "Optimize for the host CPU (-march=native)"
    OFF
)

option(MYGAME_ENABLE_PGO_GENERATE
    "Generate profile-guided optimization data"
    OFF
)

option(MYGAME_ENABLE_PGO_USE
    "Use previously generated profile-guided optimization data"
    OFF
)

option(MYGAME_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors"
    OFF
)

option(MYGAME_ENABLE_WERROR_RELEASE
    "Treat warnings as errors in Release"
    ON
)

option(MYGAME_ENABLE_UNITY_BUILD
    "Enable CMake unity builds"
    OFF
)

option(MYGAME_USE_CCACHE
    "Use ccache when available"
    ON
)

option(MYGAME_USE_SCCACHE
    "Use sccache when available"
    OFF
)

option(MYGAME_INSTALL
    "Enable install/package rules"
    ON
)

option(MYGAME_FETCH_RAYLIB
    "Fetch raylib automatically when no suitable system package exists"
    ON
)

option(MYGAME_RAYLIB_SHARED
    "Build raylib as a shared library when fetching it"
    OFF
)

set(MYGAME_RAYLIB_VERSION
    "6.0"
    CACHE STRING
    "Raylib version to use when fetched"
)

# ==============================================================================
# Output directories
# ==============================================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/lib"
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/lib"
)

# Multi-config generators such as Visual Studio need per-config directories.
foreach(CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)

    string(TOUPPER "${CONFIG_TYPE}" CONFIG_UPPER)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_BINARY_DIR}/bin/${CONFIG_TYPE}"
    )

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_BINARY_DIR}/lib/${CONFIG_TYPE}"
    )

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_BINARY_DIR}/lib/${CONFIG_TYPE}"
    )

endforeach()

# ==============================================================================
# Build configuration defaults
# ==============================================================================

# Single-config generators need a build type.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

    set(
        CMAKE_BUILD_TYPE
        RelWithDebInfo
        CACHE STRING
        "Build type"
        FORCE
    )

    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
            Debug
            RelWithDebInfo
            Release
            MinSizeRel
    )

endif()

# ==============================================================================
# Developer tools: ccache / sccache
# ==============================================================================

if(MYGAME_USE_CCACHE AND MYGAME_USE_SCCACHE)

    message(FATAL_ERROR
        "MYGAME_USE_CCACHE and MYGAME_USE_SCCACHE cannot both be enabled."
    )

endif()

if(MYGAME_USE_CCACHE)

    find_program(MYGAME_CCACHE_PROGRAM ccache)

    if(MYGAME_CCACHE_PROGRAM)

        message(STATUS "Using ccache: ${MYGAME_CCACHE_PROGRAM}")

        set(CMAKE_C_COMPILER_LAUNCHER
            "${MYGAME_CCACHE_PROGRAM}"
        )

        set(CMAKE_CXX_COMPILER_LAUNCHER
            "${MYGAME_CCACHE_PROGRAM}"
        )

    else()

        message(STATUS "ccache not found; continuing without it.")

    endif()

elseif(MYGAME_USE_SCCACHE)

    find_program(MYGAME_SCCACHE_PROGRAM sccache)

    if(MYGAME_SCCACHE_PROGRAM)

        message(STATUS "Using sccache: ${MYGAME_SCCACHE_PROGRAM}")

        set(CMAKE_C_COMPILER_LAUNCHER
            "${MYGAME_SCCACHE_PROGRAM}"
        )

        set(CMAKE_CXX_COMPILER_LAUNCHER
            "${MYGAME_SCCACHE_PROGRAM}"
        )

    else()

        message(STATUS "sccache not found; continuing without it.")

    endif()

endif()

# ==============================================================================
# Platform detection
# ==============================================================================

if(WIN32)

    set(MYGAME_PLATFORM "Windows")

elseif(UNIX AND NOT APPLE)

    set(MYGAME_PLATFORM "Linux")

elseif(APPLE)

    set(MYGAME_PLATFORM "macOS")

else()

    set(MYGAME_PLATFORM "${CMAKE_SYSTEM_NAME}")

endif()

message(STATUS "Game platform: ${MYGAME_PLATFORM}")

# ==============================================================================
# Raylib dependency
#
# First try to use an installed raylib package.
# If unavailable, fetch the pinned version automatically.
# ==============================================================================

set(MYGAME_RAYLIB_FROM_FETCHCONTENT OFF)

find_package(
    raylib ${MYGAME_RAYLIB_VERSION}
    CONFIG
    QUIET
)

if(TARGET raylib)

    message(STATUS
        "Using installed raylib target."
    )

elseif(MYGAME_FETCH_RAYLIB)

    message(STATUS
        "System raylib ${MYGAME_RAYLIB_VERSION} not found."
    )

    message(STATUS
        "Fetching raylib ${MYGAME_RAYLIB_VERSION}..."
    )

    include(FetchContent)

    # --------------------------------------------------------------------------
    # Configure raylib before bringing it into this project.
    # --------------------------------------------------------------------------

    set(BUILD_EXAMPLES OFF CACHE BOOL
        "Disable raylib examples"
        FORCE
    )

    set(BUILD_GAMES OFF CACHE BOOL
        "Disable raylib example games"
        FORCE
    )

    set(BUILD_SHARED_LIBS
        ${MYGAME_RAYLIB_SHARED}
        CACHE BOOL
        "Build raylib shared"
        FORCE
    )

    set(CUSTOMIZE_BUILD OFF CACHE BOOL
        "Do not expose extra raylib customization"
        FORCE
    )

    set(USE_AUDIO ON CACHE BOOL
        "Enable raylib audio"
        FORCE
    )

    set(WITH_PIC OFF CACHE BOOL
        "Position independent raylib"
        FORCE
    )

    # For Linux desktop, use the normal desktop backend.
    if(UNIX AND NOT APPLE AND NOT WIN32)

        set(PLATFORM "Desktop" CACHE STRING
            "raylib platform"
            FORCE
        )

    endif()

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY
            https://github.com/raysan5/raylib.git

        GIT_TAG
            ${MYGAME_RAYLIB_VERSION}

        GIT_SHALLOW
            TRUE

        EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(raylib)

    if(NOT TARGET raylib)

        message(FATAL_ERROR
            "raylib was fetched but the raylib target was not created."
        )

    endif()

    set(MYGAME_RAYLIB_FROM_FETCHCONTENT ON)

else()

    message(FATAL_ERROR
        "raylib ${MYGAME_RAYLIB_VERSION} was not found.\n"
        "Install raylib or configure with -DMYGAME_FETCH_RAYLIB=ON."
    )

endif()

# ==============================================================================
# Project warning target
# ==============================================================================

add_library(mygame_warnings INTERFACE)

if(MSVC)

    target_compile_options(
        mygame_warnings
        INTERFACE

        /W4
        /permissive-
        /EHsc
    )

    if(MYGAME_WARNINGS_AS_ERRORS)

        target_compile_options(
            mygame_warnings
            INTERFACE
            /WX
        )

    endif()

else()

    target_compile_options(
        mygame_warnings
        INTERFACE

        -Wall
        -Wextra
        -Wpedantic

        -Wshadow
        -Wconversion
        -Wsign-conversion

        -Wcast-align
        -Wcast-qual

        -Wformat=2
        -Wundef

        -Wdouble-promotion

        -Wnon-virtual-dtor
        -Woverloaded-virtual

        -Wold-style-cast
        -Wzero-as-null-pointer-constant

        -Wnull-dereference
        -Wimplicit-fallthrough

        -Wmissing-declarations
        -Wswitch-enum
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

        target_compile_options(
            mygame_warnings
            INTERFACE

            -Wextra-semi
            -Wcomma
            -Wconditional-uninitialized
            -Wloop-analysis
        )

    endif()

    if(MYGAME_WARNINGS_AS_ERRORS)

        target_compile_options(
            mygame_warnings
            INTERFACE
            -Werror
        )

    endif()

endif()

# ==============================================================================
# Sanitizers
# ==============================================================================

if(NOT MSVC)

    set(MYGAME_SANITIZER_FLAGS "")

    if(MYGAME_ENABLE_ASAN)

        list(APPEND
            MYGAME_SANITIZER_FLAGS
            -fsanitize=address
            -fno-omit-frame-pointer
        )

    endif()

    if(MYGAME_ENABLE_UBSAN)

        list(APPEND
            MYGAME_SANITIZER_FLAGS
            -fsanitize=undefined
        )

    endif()

    if(MYGAME_ENABLE_TSAN)

        list(APPEND
            MYGAME_SANITIZER_FLAGS
            -fsanitize=thread
        )

    endif()

    if(MYGAME_ENABLE_MSAN)

        list(APPEND
            MYGAME_SANITIZER_FLAGS
            -fsanitize=memory
            -fno-omit-frame-pointer
        )

    endif()

    if(MYGAME_SANITIZER_FLAGS)

        target_compile_options(
            mygame_warnings
            INTERFACE
            ${MYGAME_SANITIZER_FLAGS}
        )

        target_link_options(
            mygame_warnings
            INTERFACE
            ${MYGAME_SANITIZER_FLAGS}
        )

    endif()

endif()

# ==============================================================================
# Explicit game source files
#
# IMPORTANT:
# Do NOT use file(GLOB_RECURSE ...) here.
# Add new source files explicitly.
# ==============================================================================

set(MYGAME_SOURCES

    # --------------------------------------------------------------------------
    # Application
    # --------------------------------------------------------------------------

    src/main.cpp

    # --------------------------------------------------------------------------
    # Core
    # --------------------------------------------------------------------------

    src/core/application/application.cpp
    src/core/window/window.cpp
    src/core/time/time.cpp


    # --------------------------------------------------------------------------
    # Player
    # --------------------------------------------------------------------------

    # src/game/player/player.cpp

    # --------------------------------------------------------------------------
    # Rendering
    # --------------------------------------------------------------------------

    # src/rendering/renderer.cpp
    # src/rendering/camera.cpp

    # --------------------------------------------------------------------------
    # World
    # --------------------------------------------------------------------------

    # src/world/world.cpp

    # --------------------------------------------------------------------------
    # Audio
    # --------------------------------------------------------------------------

    # src/audio/audio_manager.cpp

    # --------------------------------------------------------------------------
    # Input
    # --------------------------------------------------------------------------

    # src/input/input_manager.cpp
)

set(MYGAME_HEADERS
    src/core/application/application.hpp
    src/core/window/window.hpp
    src/core/time/time.hpp
)

# ==============================================================================
# Main executable
# ==============================================================================

add_executable(
    ${PROJECT_NAME}

    ${MYGAME_SOURCES}
    ${MYGAME_HEADERS}
)

# Keep headers visible to IDEs without compiling them separately.
set_source_files_properties(
    ${MYGAME_HEADERS}
    PROPERTIES
    HEADER_FILE_ONLY TRUE
)

# ==============================================================================
# Source groups for IDEs
# ==============================================================================

source_group(
    TREE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX
        "Source Files"
    FILES
        ${MYGAME_SOURCES}
        ${MYGAME_HEADERS}
)

# ==============================================================================
# Include directories
# ==============================================================================

target_include_directories(
    ${PROJECT_NAME}

    PRIVATE

        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# ==============================================================================
# Compile definitions
# ==============================================================================

target_compile_definitions(
    ${PROJECT_NAME}

    PRIVATE

        MYGAME_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        MYGAME_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        MYGAME_VERSION_PATCH=${PROJECT_VERSION_PATCH}

        # Build configuration.
        $<$<CONFIG:Debug>:MYGAME_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:MYGAME_DEV>
        $<$<CONFIG:Release>:MYGAME_RELEASE>

        # Platform.
        $<$<PLATFORM_ID:Linux>:MYGAME_PLATFORM_LINUX>
        $<$<PLATFORM_ID:Windows>:MYGAME_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Darwin>:MYGAME_PLATFORM_MACOS>

        # Architecture.
        # $<$<CMAKE_SIZEOF_VOID_P:8>:MYGAME_64BIT>
)

# ==============================================================================
# Link dependencies
# ==============================================================================

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE

        raylib
        mygame_warnings
)

# ==============================================================================
# Modern compile features
# ==============================================================================

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
        cxx_std_23
)

# ==============================================================================
# Per-configuration compiler flags
# ==============================================================================

if(MSVC)

    # --------------------------------------------------------------------------
    # Debug
    # --------------------------------------------------------------------------

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
    )

    # --------------------------------------------------------------------------
    # Development
    # --------------------------------------------------------------------------

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:RelWithDebInfo>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/Zi>
    )

    # --------------------------------------------------------------------------
    # Release
    # --------------------------------------------------------------------------

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/Oi>
        $<$<CONFIG:Release>:/Ot>
        $<$<CONFIG:Release>:/GL>
    )

    target_link_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:Release>:/LTCG>
    )

else()

    # --------------------------------------------------------------------------
    # Debug
    # --------------------------------------------------------------------------

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g3>
    )

    # --------------------------------------------------------------------------
    # Development
    # --------------------------------------------------------------------------

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        # Excellent balance between optimization and debugging.
        $<$<CONFIG:RelWithDebInfo>:-O2>

        # Keep stack traces usable.
        $<$<CONFIG:RelWithDebInfo>:-fno-omit-frame-pointer>
    )

    # --------------------------------------------------------------------------
    # Release
    # --------------------------------------------------------------------------

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:Release>:-O3>

        # Better optimization opportunities.
        $<$<CONFIG:Release>:-fstrict-aliasing>

        # Remove unnecessary frame pointers.
        $<$<CONFIG:Release>:-fomit-frame-pointer>

        # Put functions/data into separate sections so the linker
        # can remove unreachable sections.
        $<$<CONFIG:Release>:-ffunction-sections>
        $<$<CONFIG:Release>:-fdata-sections>
    )

    # --------------------------------------------------------------------------
    # GNU / Clang linker optimizations
    # --------------------------------------------------------------------------

    target_link_options(
        ${PROJECT_NAME}
        PRIVATE

        $<$<CONFIG:Release>:
            -Wl,--gc-sections
        >
    )

endif()

# ==============================================================================
# Native architecture optimization
# ==============================================================================

if(MYGAME_ENABLE_NATIVE_ARCH)

    if(MSVC)

        message(WARNING
            "MYGAME_ENABLE_NATIVE_ARCH is not supported through MSVC flags here."
        )

    else()

        target_compile_options(
            ${PROJECT_NAME}
            PRIVATE

            -march=native
            -mtune=native
        )

    endif()

endif()

# ==============================================================================
# Clang-specific tuning
# ==============================================================================

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE

        # Keep strict alias analysis enabled in optimized builds.
        $<$<CONFIG:Release>:-fstrict-vtable-pointers>
    )

endif()

# ==============================================================================
# Link Time Optimization / IPO
# ==============================================================================

if(MYGAME_ENABLE_LTO)

    include(CheckIPOSupported)

    check_ipo_supported(
        RESULT MYGAME_IPO_SUPPORTED
        OUTPUT MYGAME_IPO_ERROR
    )

    if(MYGAME_IPO_SUPPORTED)

        message(STATUS "LTO/IPO: ENABLED")

        set_property(
            TARGET
                ${PROJECT_NAME}
            PROPERTY
                INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
        )

        set_property(
            TARGET
                ${PROJECT_NAME}
            PROPERTY
                INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )

    else()

        message(WARNING
            "LTO/IPO requested but not supported:\n"
            "${MYGAME_IPO_ERROR}"
        )

    endif()

else()

    message(STATUS "LTO/IPO: DISABLED")

endif()

# ==============================================================================
# Profile Guided Optimization
# ==============================================================================

if(MYGAME_ENABLE_PGO_GENERATE AND MYGAME_ENABLE_PGO_USE)

    message(FATAL_ERROR
        "PGO_GENERATE and PGO_USE cannot be enabled simultaneously."
    )

endif()

if(NOT MSVC)

    if(MYGAME_ENABLE_PGO_GENERATE)

        target_compile_options(
            ${PROJECT_NAME}
            PRIVATE

            -fprofile-generate
        )

        target_link_options(
            ${PROJECT_NAME}
            PRIVATE

            -fprofile-generate
        )

    endif()

    if(MYGAME_ENABLE_PGO_USE)

        target_compile_options(
            ${PROJECT_NAME}
            PRIVATE

            -fprofile-use
            -fprofile-correction
        )

        target_link_options(
            ${PROJECT_NAME}
            PRIVATE

            -fprofile-use
        )

    endif()

endif()

# ==============================================================================
# Unity build support
# ==============================================================================

if(MYGAME_ENABLE_UNITY_BUILD)

    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            UNITY_BUILD ON
    )

    message(STATUS "Unity build: ENABLED")

else()

    message(STATUS "Unity build: DISABLED")

endif()

# ==============================================================================
# Windows configuration
# ==============================================================================

if(WIN32)

    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE

            NOMINMAX
            WIN32_LEAN_AND_MEAN
    )

    # Release build behaves as a GUI application instead of opening
    # a console window.
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES

            WIN32_EXECUTABLE
                $<$<CONFIG:Release>:TRUE>
    )

endif()

# ==============================================================================
# Asset handling
# ==============================================================================

set(MYGAME_ASSET_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

set(MYGAME_RUNTIME_ASSET_DIR
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

if(EXISTS "${MYGAME_ASSET_SOURCE_DIR}")

    add_custom_command(
        TARGET
            ${PROJECT_NAME}
        POST_BUILD

        COMMAND
            ${CMAKE_COMMAND}
            -E copy_directory

            "${MYGAME_ASSET_SOURCE_DIR}"

            "${MYGAME_RUNTIME_ASSET_DIR}"

        COMMENT
            "Copying game assets..."
    )

else()

    message(STATUS
        "No assets directory found; skipping asset staging."
    )

endif()

# ==============================================================================
# Compile-time asset path
#
# Runtime code should normally resolve paths relative to the executable
# rather than depending on this absolute development path.
# ==============================================================================

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE

        MYGAME_SOURCE_ASSET_DIR="${MYGAME_ASSET_SOURCE_DIR}"
)

# ==============================================================================
# Developer run target
# ==============================================================================

add_custom_target(
    run
    COMMAND
        "$<TARGET_FILE:${PROJECT_NAME}>"
    DEPENDS
        ${PROJECT_NAME}
    WORKING_DIRECTORY
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    USES_TERMINAL
)

# ==============================================================================
# Dependency information
# ==============================================================================

if(MYGAME_RAYLIB_FROM_FETCHCONTENT)

    message(STATUS
        "Raylib source: FetchContent"
    )

else()

    message(STATUS
        "Raylib source: system installation"
    )

endif()

# ==============================================================================
# Install
# ==============================================================================

if(MYGAME_INSTALL)

    include(GNUInstallDirs)

    # --------------------------------------------------------------------------
    # Executable
    # --------------------------------------------------------------------------

    install(
        TARGETS
            ${PROJECT_NAME}

        RUNTIME
            DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # --------------------------------------------------------------------------
    # Assets
    # --------------------------------------------------------------------------

    if(EXISTS "${MYGAME_ASSET_SOURCE_DIR}")

        install(
            DIRECTORY
                "${MYGAME_ASSET_SOURCE_DIR}/"

            DESTINATION
                "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/assets"
        )

    endif()

endif()

# ==============================================================================
# Tests
# ==============================================================================

if(MYGAME_BUILD_TESTS)

    enable_testing()

    add_subdirectory(tests)

endif()

# ==============================================================================
# Developer tools
# ==============================================================================

if(MYGAME_BUILD_TOOLS)

    add_subdirectory(tools)

endif()

# ==============================================================================
# Status summary
# ==============================================================================

message("")
message("============================================================")
message(" ${PROJECT_NAME} ${PROJECT_VERSION}")
message("============================================================")
message(" Platform:              ${MYGAME_PLATFORM}")
message(" System:                ${CMAKE_SYSTEM_NAME}")
message(" Architecture:          ${CMAKE_SYSTEM_PROCESSOR}")
message(" C compiler:            ${CMAKE_C_COMPILER_ID}")
message(" C++ compiler:          ${CMAKE_CXX_COMPILER_ID}")
message(" C++ compiler path:     ${CMAKE_CXX_COMPILER}")
message(" C++ standard:          C++${CMAKE_CXX_STANDARD}")
message(" Build type:            ${CMAKE_BUILD_TYPE}")
message(" Raylib:                ${MYGAME_RAYLIB_VERSION}")
message(" Raylib FetchContent:   ${MYGAME_RAYLIB_FROM_FETCHCONTENT}")
message(" LTO/IPO:               ${MYGAME_ENABLE_LTO}")
message(" Native CPU tuning:     ${MYGAME_ENABLE_NATIVE_ARCH}")
message(" PGO generate:          ${MYGAME_ENABLE_PGO_GENERATE}")
message(" PGO use:               ${MYGAME_ENABLE_PGO_USE}")
message(" Unity build:           ${MYGAME_ENABLE_UNITY_BUILD}")
message(" AddressSanitizer:      ${MYGAME_ENABLE_ASAN}")
message(" UndefinedSanitizer:    ${MYGAME_ENABLE_UBSAN}")
message(" ThreadSanitizer:       ${MYGAME_ENABLE_TSAN}")
message("============================================================")
message("")

