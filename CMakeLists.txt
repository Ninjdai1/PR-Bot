	# Minimum CMake version required, we'll just use the latest version.
	cmake_minimum_required(VERSION 3.30)
	# Project name, version and description
    project(prbot VERSION 1.0 DESCRIPTION "PR Bot")
	 
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
	
    # Un-comment to compile with debug sybols
    #set(CMAKE_BUILD_TYPE Debug)

    ADD_LIBRARY(RHHUtils
        src/utils/misc.cpp
        src/utils/github.cpp
        src/utils/regex.cpp
    )
	add_executable(${PROJECT_NAME}
	    src/main.cpp
	)
	 
	find_package(DPP REQUIRED)
	 
	target_link_libraries(${PROJECT_NAME} 
	    ${DPP_LIBRARIES}
        RHHUtils
    )

	target_include_directories(${PROJECT_NAME} PRIVATE
	    ${DPP_INCLUDE_DIR}
	)
	 
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    CXX_STANDARD 17
	    CXX_STANDARD_REQUIRED ON
	)
