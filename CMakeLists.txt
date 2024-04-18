cmake_minimum_required(VERSION 3.0)

# Definitions for debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
endif()

# Create library target for lambda_engine
add_library(lambda_engine STATIC)

# Set base directory
set(basedir lambda_engine)

# LINUX SDL INCLUSION
if(UNIX AND NOT APPLE)
    # Find SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    # Include directories for SDL dependencies
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

    # Link with SDL libraries
    target_link_libraries(lambda_engine PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
endif()

# WINDOWS SDL INCLUSION
if(WIN32)
    # Add SDL include directory for Windows
    include_directories("path/to/SDL2/include")

    # Link with SDL libraries for Windows
    target_link_libraries(lambda_engine PRIVATE "path/to/SDL2/lib/SDL2.lib" "path/to/SDL2/lib/SDL2main.lib")
    target_link_libraries(lambda_engine PRIVATE "path/to/SDL2_image/lib/SDL2_image.lib")
    target_link_libraries(lambda_engine PRIVATE "path/to/SDL2_ttf/lib/SDL2_ttf.lib")
    target_link_libraries(lambda_engine PRIVATE "path/to/SDL2_mixer/lib/SDL2_mixer.lib")
endif()

# Get a list of directories within basedir
file(GLOB dirs LIST_DIRECTORIES true "${basedir}/*")

# Iterate over the list of directories and add them as subdirectories
foreach(dir ${dirs})
    if(IS_DIRECTORY ${dir})
        message(STATUS "Adding subdirectory: ${dir}")
        add_subdirectory(${dir})

        # List .cpp and .h files in the directory
        file(GLOB_RECURSE SOURCES "${dir}/*.cpp" "${dir}/*.h")

        # Add source files to the library target
        message(STATUS "Adding sources: ${SOURCES}")
        target_sources(lambda_engine PRIVATE ${SOURCES})

        # Include directory for header files
        include_directories(${dir})

        # Install all header files in the directory
        file(GLOB_RECURSE HEADERS "${dir}/*.h")
        foreach(header ${HEADERS})
            get_filename_component(header_path "${header}" DIRECTORY)
            install(FILES "${header}" DESTINATION include)
        endforeach()
    endif()
endforeach()

# Include rapidxml headers
include_directories("${CMAKE_SOURCE_DIR}/rapidxml")
install(FILES "${CMAKE_SOURCE_DIR}/rapidxml/rapidxml.hpp" DESTINATION include)

# Add lambda.h to the library target
target_sources(lambda_engine PRIVATE "${CMAKE_SOURCE_DIR}/lambda_engine/lambda.h")

# Install the library and header files
install(TARGETS lambda_engine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Specify which headers should be installed
install(FILES
    "${CMAKE_SOURCE_DIR}/lambda_engine/lambda.h"
    DESTINATION include
)

