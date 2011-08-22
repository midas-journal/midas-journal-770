option( ITK_USE_REVIEW "Build projects under review." ON )
if( ITK_USE_REVIEW )
  add_subdirectory( Review )
endif( ITK_USE_REVIEW )