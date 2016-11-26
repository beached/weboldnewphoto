cmake_minimum_required( VERSION 2.8.12 )
project( weboldnewphoto )

include( ExternalProject )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR} )

find_package( Wt REQUIRED )

set( Boost_USE_STATIC_LIBS OFF ) 
set( Boost_USE_MULTITHREADED ON )  
set( Boost_USE_STATIC_RUNTIME OFF ) 

find_package( Boost 1.58.0 REQUIRED COMPONENTS system filesystem regex unit_test_framework program_options iostreams )

find_package( FreeImage REQUIRED )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 )
else( )
	find_package( Threads REQUIRED )
	if( THREADS_HAVE_PTHREAD_ARG )
		set( COMPILER_SPECifIC_LIBS, "-lpthread" )
	endif( )

	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
		add_compile_options(-std=c++14 -fopenmp -Weverything -Wno-c++98-compat -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)		
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options( -std=c++14 -g -Wall -fopenmp -Wno-deprecated-declarations )
	endif( )

endif( )

externalproject_add(
	lib_grayscale_filter_prj
	GIT_REPOSITORY "https://github.com/beached/lib_grayscale_filter.git"
	GIT_TAG "master"
	SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/lib_grayscale_filter"
	INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

externalproject_add( 
	header_libraries_prj
	GIT_REPOSITORY "https://github.com/beached/header_libraries.git"
	GIT_TAG "master"
	SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/header_libraries"
	INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
include_directories( "./include" )
link_directories( "${CMAKE_BINARY_DIR}/install/lib" )

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( SYSTEM ${FREEIMAGE_INCLUDE_DIRS} )
link_directories( ${FREEIMAGE_LIBRARIES} )

set( HEADER_FOLDER "include" )

set( HEADER_FILES
	${HEADER_FOLDER}/autolevelfilter.h
	${HEADER_FOLDER}/channelcombiner.h
	${HEADER_FOLDER}/imagefilter.h
	${HEADER_FOLDER}/itypes.h
	${HEADER_FOLDER}/largepixeldata.h
	#${HEADER_FOLDER}/opencvimage.h
	${HEADER_FOLDER}/weboldnewphoto.h
	${HEADER_FOLDER}/wgenericvirtualimage.h
	${HEADER_FOLDER}/whelpers.h
)

set( SOURCE_FILES
	autolevelfilter.cpp
	channelcombiner.cpp
	largepixeldata.cpp
	main.cpp
	#opencvimage.cpp
	weboldnewphoto.cpp
	wgenericvirtualimage.cpp
	whelpers.cpp
)


set( WT_CONNECTOR "wthttp" CACHE STRING "Connector used (wthttp or wtfcgi)" )

add_executable( weboldnewphoto ${HEADER_FILES} ${SOURCE_FILES} )
add_dependencies( weboldnewphoto header_libraries_prj lib_grayscale_filter_prj )

target_link_libraries( ${PROJECT_NAME} grayscale_filter ${Wt_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_LIBRARIES} ${FREEIMAGE_LIBRARIES} )

