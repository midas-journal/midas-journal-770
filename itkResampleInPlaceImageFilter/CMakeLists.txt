project( itkResampleInPlaceImageFilter )
cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION 2.8 )

FIND_PACKAGE( ITK REQUIRED )
if(ITK_FOUND)
  include( ${ITK_USE_FILE} )
endif( ITK_FOUND )

add_subdirectory( Insight )