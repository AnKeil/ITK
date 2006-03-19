/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCentralDifferenceImageFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCentralDifferenceImageFunction_h
#define __itkCentralDifferenceImageFunction_h

#include "itkImageFunction.h"
#include "itkCovariantVector.h"
#include "itkImageBase.h"

namespace itk
{

/**
 * \class CentralDifferenceImageFunction
 * \brief Calculate the derivative by central differencing.
 *
 * This class is templated over the input image type and
 * the coordinate representation type (e.g. float or double).
 *
 * Possible improvements:
 * - the use of Neighborhood operators may improve efficiency.
 *
 * \ingroup ImageFunctions
 */
template <
  class TInputImage, 
  class TCoordRep = float >
class ITK_EXPORT CentralDifferenceImageFunction :
  public ImageFunction< TInputImage, 
                        CovariantVector<double, \
                        ::itk::GetImageDimension<TInputImage>::ImageDimension>, 
                        TCoordRep >
{
public:
  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard class typedefs. */
  typedef CentralDifferenceImageFunction Self;
  typedef ImageFunction<TInputImage,
                        CovariantVector<double, 
                        itkGetStaticConstMacro(ImageDimension)>,
                        TCoordRep>       Superclass;
  typedef SmartPointer<Self>             Pointer;
  typedef SmartPointer<const Self>       ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(CentralDifferenceImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage InputImageType;

  /** OutputType typdef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;
  
  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Point typedef support. */
  typedef typename Superclass::PointType PointType;

  /** Evalulate the image derivative by central differencing at specified index.
   *
   *  No bounds checking is done.
   *  The point is assume to lie within the image buffer.
   *
   *  ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual OutputType EvaluateAtIndex( const IndexType& index ) const;
  
  /** Evalulate the image derivative by central differencing at non-integer 
   *  positions.
   *
   *  No bounds checking is done.
   *  The point is assume to lie within the image buffer.
   *
   *  ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
   virtual OutputType Evaluate( const PointType& point ) const
    { 
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return this->EvaluateAtIndex( index ); 
    }
  virtual OutputType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    return this->EvaluateAtIndex( index ); 
    }
  
protected:
  CentralDifferenceImageFunction();
  ~CentralDifferenceImageFunction(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  CentralDifferenceImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCentralDifferenceImageFunction.txx"
#endif

#endif
