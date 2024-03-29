/* =========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/* =========================================================================
 *  Created by Wei Lu on 10/14/10.
 *  Copyright 2010 The University of Iowa
 *=========================================================================*/

#include "itkResampleInPlaceImageFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkVersorRigid3DTransform.h"
#include "itkImageRegionConstIterator.h"

#include <iostream>


// return true if it fails the validation
inline bool Validate( double input, double desired, double tolerance )
{
  return ( abs( input - desired ) > tolerance * abs( desired ) );
}

int itkResampleInPlaceImageFilterTest( int argc, char * argv[] )
{
  // Simple parameter check
  if ( argc < 2 )
    {
    std::cerr << "Wrong arguments!" << std::endl;
    std::cerr << "Usage: ./" << argv[0] << " inputImage baselineImage" << std::endl;
    exit( -1 );
    }

  bool result = false; // test result default = no failure
  double tol = 1.e-3; // tolerance

  // Image, filter, transform typedef
  const unsigned int LocalImageDimension = 3;
  typedef short PixelType;

  typedef itk::Image< PixelType, LocalImageDimension > ImageType;
  typedef ImageType::Pointer                           ImagePointer;
  typedef ImageType::PointType                         ImagePointType;
  typedef ImageType::DirectionType                     ImageDirectionType;
  typedef ImageType::SpacingType                       ImageSpacingType;

  typedef itk::ImageRegionConstIterator< ImageType >   ImageConstIterator;
  typedef itk::ImageFileReader< ImageType >            ReaderType;
  typedef itk::VersorRigid3DTransform< double >        TransformType;

  typedef itk::ResampleInPlaceImageFilter< ImageType, ImageType > FilterType;

  // Read in input test image
  ImagePointer inputImage;
  {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[1] );
    try
    {
      reader->Update();
    }
    catch ( itk::ExceptionObject & err )
    {
      std::cerr << " Error while reading image file(s) with ITK:\n "
      << err << std::endl;
    }
    inputImage = reader->GetOutput();
  }

  // Set up transforms
  itk::Vector<double, 3> rotationAxis;
  rotationAxis.Fill( 0. );
  rotationAxis[0] = 1.;
  double rotationAngle = .5; // in rad
  itk::Vector<double, 3> translation;
  translation.Fill( 0. );
  translation[1] = 300.; // in mm along P-axis
  TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();
  transform->SetRotation( rotationAxis, rotationAngle );
  transform->Translate( translation, true );

  // Set up the resample filter
  FilterType::Pointer filter = FilterType::New();
  filter->SetInputImage( inputImage );
  filter->SetRigidTransform( transform );
  filter->Update();
  ImagePointer outputImage = filter->GetOutput();

  // Get image info
  ImagePointType origin = outputImage->GetOrigin();
  ImageDirectionType direction = outputImage->GetDirection();
  ImageSpacingType spacing = outputImage->GetSpacing();

  // Read in baseline image
  ImagePointer baselineImage;
  {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[2] );
    try
    {
      reader->Update();
    }
    catch ( itk::ExceptionObject & err )
    {
      std::cerr << " Error while reading image file(s) with ITK:\n "
      << err << std::endl;
    }
    baselineImage = reader->GetOutput();
  }
  ImagePointType origin_d = baselineImage->GetOrigin();
  ImageDirectionType direction_d = baselineImage->GetDirection();
  ImageSpacingType spacing_d = baselineImage->GetSpacing();

  // Image info validation
  for ( unsigned int i = 0; i < LocalImageDimension; ++i )
    {
      result = ( result || Validate( origin[i], origin_d[i], tol ) );
      result = ( result || Validate( spacing[i], spacing_d[i], tol ) );
      for ( unsigned int j = 0; j < LocalImageDimension; ++j )
        {
        result = ( result || Validate( direction(i,j), direction_d(i,j), tol ) );
        }
    }

  // Voxel contents validation
  ImageConstIterator it1( outputImage, outputImage->GetRequestedRegion() );
  ImageConstIterator it2( baselineImage, baselineImage->GetRequestedRegion() );
  it1.GoToBegin();
  it2.GoToBegin();
  while ( !it1.IsAtEnd() )
    {
    result = ( result || Validate( it1.Get(), it2.Get(), tol ) );
    ++it1;
    ++it2;
    }

  return result;
}
