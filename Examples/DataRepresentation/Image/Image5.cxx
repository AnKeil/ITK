/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Image5.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Software Guide : BeginLatex
//
// This example illustrates how to import data in to the \doxygen{Image}.  This
// is particularly useful for interfacing with other software systems that use
// a different data structure for representing images. It is quite common to
// use a contiguous block of memory as buffer for the image pixel data. The
// current example assumes this is the case and uses this block of memory to
// feed data into \doxygen{ImportImageFilter} that will produce an
// \doxygen{Image} as output.  
//
// For the sake of having some fun, we create a synthetic image with a centered
// sphere in a locally allocated buffer and pass this block of memory to the
// \code{ImportImageFilter}.
//
// \index{itk::ImportImageFilter!Instantiation}
// \index{itk::ImportImageFilter!Header}
//
// First, the header file of the \doxygen{ImportImageFilter} class must be
// included.
//
// Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkImage.h"
#include "itkImportImageFilter.h"
// Software Guide : EndCodeSnippet

#include "itkImageFileWriter.h"


int main(int argc, char ** argv)
{

  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  outputImageFile" << std::endl; 
    return 1;
    }
  
  // Software Guide : BeginLatex
  // 
  // We select here the data type to use for representing image pixels.  We
  // assume that the external block of memory uses the same data type to
  // represent the pixels.
  //
  // Software Guide : EndLatex 
  //
  // Software Guide : BeginCodeSnippet 
  typedef unsigned char   PixelType;
  const unsigned int Dimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;
  // Software Guide : EndCodeSnippet 

  
  // Software Guide : BeginLatex
  //
  // The type of the \doxygen{ImportImageFilter} is instantiated in the
  // following line.
  //
  // \index{itk::ImportImageFilter!Instantiation}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::ImportImageFilter< PixelType, Dimension >   ImportFilterType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  A filter object is created using the \code{New()} method and assigning
  //  the result to a \code{SmartPointer}.
  //  
  // \index{itk::ImportImageFilter!Pointer}
  // \index{itk::ImportImageFilter!New()}
  // 
  // Software Guide : EndLatex 
  //
  // Software Guide : BeginCodeSnippet 
  ImportFilterType::Pointer importFilter = ImportFilterType::New();      
  // Software Guide : EndCodeSnippet 
 

  // Software Guide : BeginLatex
  //
  // This filter requires the user to specify the size of the image to be
  // produced as output.  The methods \code{SetRegion()} is used to this end.
  // The image size should match exactly the number of pixels available in the
  // locally allocated buffer. 
  //
  // \index{itk::ImportImageFilter!SetRegion()}
  // \index{itk::ImportImageFilter!New()}
  // \index{itk::ImportImageFilter!New()}
  //
  // Software Guide : EndLatex 
  //
  // Software Guide : BeginCodeSnippet 
  ImportFilterType::SizeType  size;

  size[0]  = 200;  // size along X
  size[1]  = 200;  // size along Y
  size[2]  = 200;  // size along Z

  ImportFilterType::IndexType start;
  start.Fill( 0 );

  ImportFilterType::RegionType region;
  region.SetIndex( start );
  region.SetSize(  size  );

  importFilter->SetRegion( region );
  // Software Guide : EndCodeSnippet 



  //  Software Guide : BeginLatex
  //
  //  The origin of the output image is specified with the \code{SetOrigin()}
  //  method.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  double origin[ Dimension ];

  origin[0] = 0.0;    // X coordinate 
  origin[1] = 0.0;    // Y coordinate
  origin[2] = 0.0;    // Z coordinate

  importFilter->SetOrigin( origin );
  // Software Guide : EndCodeSnippet




  //  Software Guide : BeginLatex
  //
  //  The spacing of the image is passed with the \code{SetSpacing()} method.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  double spacing[ Dimension ];

  spacing[0] = 1.0;    // along X direction 
  spacing[1] = 1.0;    // along Y direction
  spacing[2] = 1.0;    // along Z direction

  importFilter->SetSpacing( spacing );
  // Software Guide : EndCodeSnippet




  //  Software Guide : BeginLatex
  //
  //  We allocate the memory block that will contain the pixel data to be
  //  passed to the \code{ImportImageFilter}. Note that we use exactly the same
  //  size that was promised to the filter with the \code{SetRegion()} method.
  //  In a practical application you may get this buffer from some other
  //  library using a different data structure for representing images.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const unsigned int numberOfPixels =  size[0] * size[1] * size[2];

  PixelType * localBuffer = new PixelType[ numberOfPixels ];
  // Software Guide : EndCodeSnippet


  const double radius = 80.0;


  //  Software Guide : BeginLatex
  //
  //  Here we fill up the buffer with a binary sphere. We use good old-time
  //  \code{for()} loops here... it looks just like FORTAN. Note that ITK does
  //  not use \code{for()} loops in its internal code for accessing pixels. All
  //  the pixel access tasks are performed using \doxygen{ImageIterator}s that
  //  support managing $ND$ images.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const double radius2 = radius * radius;
  PixelType * it = localBuffer;

  for(unsigned int z=0; z < size[2]; z++)
    {
    const double dz = static_cast<double>( z ) - static_cast<double>(size[2])/2.0;
    for(unsigned int y=0; y < size[1]; y++)
      {
      const double dy = static_cast<double>( y ) - static_cast<double>(size[1])/2.0;
      for(unsigned int x=0; x < size[0]; x++)
        {
        const double dx = static_cast<double>( x ) - static_cast<double>(size[0])/2.0;
        const double d2 = dx*dx + dy*dy + dz*dz;
        *it++ = ( d2 < radius2 ) ? 255 : 0;
        }
      }
    }
  // Software Guide : EndCodeSnippet





  //  Software Guide : BeginLatex
  //
  //  The buffer is passed to the \doxygen{ImportImageFilter} with the
  //  \code{SetImportPointer()}. Note that the last argument of this method
  //  specifies who will be responsible for deleting the memory block once it
  //  is no longer in use. A \code{true} value indicates that the
  //  \doxygen{ImportImageFilter} will not try to delete the buffer when its
  //  destructor is called. A \code{false} on the other hand will allow the
  //  filter to delete the memory block.
  //
  //  For the \doxygen{ImportImageFilter} to approriately delete the memory block,
  //  the memory must be allocated with the C++ new operator.  Memory allocated with
  //  with other memory allocation mechanisms, such as the C malloc or calloc,
  //  will not be deleted properly by the \doxygen{ImportImageFilter}.  In other
  //  words, it is the application programmers responsiblility to ensure
  //  that \doxygen{ImportImageFilter} is only given responsibilty to delete the
  //  C++ new operator allocated memory.
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const bool userPromiseToDeleteTheBuffer = false;

  importFilter->SetImportPointer( localBuffer, 
                                  numberOfPixels, 
                                  userPromiseToDeleteTheBuffer );
  // Software Guide : EndCodeSnippet





  //  Software Guide : BeginLatex
  //
  //  Finally we can connect the output of this filter to a pipeline. Here, for
  //  simplicity we just use a writer but it could be any other filter. 
  //
  //  Software Guide : EndLatex 


  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[1] );

  // Software Guide : BeginCodeSnippet
  writer->SetInput(  importFilter->GetOutput()  );
  // Software Guide : EndCodeSnippet


  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & exp ) 
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << exp << std::endl;
    }


  //  Software Guide : BeginLatex
  //
  //  Note that we do not call \code{delete} on the buffer since we pass
  //  \code{false} to the last argument of \code{SetImportPointer()}. Now the
  //  C++ new operator allocated buffer is owned by the \code{ImportImageFilter}.
  //
  //  Software Guide : EndLatex


  return 0;

}

