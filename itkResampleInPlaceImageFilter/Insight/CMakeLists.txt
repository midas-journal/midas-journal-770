project( Insight )
# Dispatch the build into the proper subdirectories.
add_subdirectory( Code )

option( BUILD_TESTING "Build testing for the project." ON )
if( BUILD_TESTING )
  add_subdirectory( Testing )
endif( BUILD_TESTING )
