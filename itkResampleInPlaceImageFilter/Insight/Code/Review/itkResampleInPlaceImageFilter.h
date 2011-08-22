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
#ifndef __itkResampleInPlaceImageFilter_h
#define __itkResampleInPlaceImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkVersorRigid3DTransform.h"
#include "itkImageDuplicator.h"

namespace itk
{

/** \class ResampleInPlaceImageFilter
 * \brief Resample an image in place.
 *
 * The ResampleInPlaceImageFilter adjusts the physical space representation
 * of the input image without modifying the image values at each voxel
 * location.
 * It uses a rigid transformations to change the physical space
 * mapping.
 * Applying rigid transforms through composition with the image
 * original physical to voxel lattice mapping
 * minimizes the accumulative interpolation errors.
 *
 * \param RigidTransform -- Currently must be a VersorRigid3D
 * \param InputImage -- The image to be duplicated and modified to incorporate the
 * rigid transform
 * \return -- An image with the same voxels values as the input, but with differnt
 * physical space representation affected by the rigid transform.
 * \ingroup GeometricTransforms
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ResampleInPlaceImageFilter :
    public ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ResampleInPlaceImageFilter                    Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro( Self );

  /** Run-time type information (and related methods) */
  itkTypeMacro( ResampleInPlaceImageFilter, ImageToImageFilter );

  /** input/output image typedefs */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::PointType    InputImagePointType;

  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro(SameDimensionCheck,
                  (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  itkConceptMacro(InputConvertibleToOutputCheck,
                  (Concept::Convertible<InputImagePixelType, OutputImagePixelType>));
#endif

  /** Transform typedef */
  typedef VersorRigid3DTransform<double> RigidTransformType;
  typedef typename RigidTransformType::Pointer RigidTransformPointer;

  /** Image duplicator typedef */
  typedef ImageDuplicator<InputImageType> DuplicatorType;
  typedef typename DuplicatorType::Pointer DuplicatorPointer;

  /** Set/Get rigid transform. The default is an identity transform */
  itkSetObjectMacro( RigidTransform, RigidTransformType );
  itkGetObjectMacro( RigidTransform, RigidTransformType );

  /** Set/Get required input image. (A wrapper to this->Set/GetInput()) */
  void SetInputImage( const InputImageType * image );
  const InputImageType * GetInputImage() const;

protected:
  ResampleInPlaceImageFilter();
  ~ResampleInPlaceImageFilter() {};

  void GenerateData();

  void PrintSelf( std::ostream& os, Indent indent ) const;

private:
  ResampleInPlaceImageFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  DuplicatorPointer       m_Duplicator;
  RigidTransformPointer   m_RigidTransform;
  OutputImagePointer      m_OutputImage;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkResampleInPlaceImageFilter.txx"
#endif

#endif
