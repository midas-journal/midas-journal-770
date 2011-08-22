project( ITKTesting )

set( ITK_DATA_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Data )

if( BUILD_TESTING )
  enable_testing()
endif( BUILD_TESTING )

add_subdirectory( Code )