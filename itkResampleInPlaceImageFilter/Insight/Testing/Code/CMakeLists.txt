project( itkResampleInPlaceImageFilterTest )

set( ITK_REVIEW_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Insight/Code/Review )
include_directories( ${ITK_REVIEW_SOURCE_DIR} )

add_executable( itkResampleInPlaceImageFilterTest itkBasicFiltersTests.cxx )

target_link_libraries( itkResampleInPlaceImageFilterTest ITKIO ITKBasicFilters )

add_test( itkResampleInPlaceImageFilterTest itkResampleInPlaceImageFilterTest
    itkResampleInPlaceImageFilterTest
    ${ITK_DATA_ROOT}/Input/itkResampleInPlaceImageFilterTest.nii.gz
    ${ITK_DATA_ROOT}/Baseline/Review/itkResampleInPlaceImageFilterTest.nii.gz
)
