/**
 *  
 *  This program illustrates the AdaptImageFilter
 *
 *  The example shows how an Accessor can be used to 
 *  convert an Vector image to an image that has
 *  just a single component.
 *
 *  That will allow to pass a component of the
 *  image as input any filter that expects
 *  a float image
 *
 */


#include <itkAdaptImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkVector.h>
#include <itkNthElementPixelAccessor.h>

#include <vnl/vnl_sample.h>


//-------------------------------------
//     Typedefs for convenience
//-------------------------------------
typedef itk::Image< itk::Vector<float, 3>,   2 > myVectorImageType;
typedef itk::ImageRegionIteratorWithIndex< myVectorImageType >  myVectorIteratorType;

typedef itk::NthElementPixelAccessor<float, itk::Vector<float, 3> > myAccessorType;

typedef itk::Image< float,   2 > myImageType;
typedef itk::ImageRegionIteratorWithIndex< myImageType >  myIteratorType;



//-------------------------
//
//   Main code
//
//-------------------------
int main() {


  myVectorImageType::SizeType size;
  size[0] = 2;
  size[1] = 2;

  myVectorImageType::IndexType index;
  index[0] = 0;
  index[1] = 0;

  myVectorImageType::RegionType region;
  region.SetIndex( index );
  region.SetSize(  size  );

  myVectorImageType::Pointer myImage = myVectorImageType::New();


  myImage->SetLargestPossibleRegion( region );
  myImage->SetBufferedRegion( region );
  myImage->SetRequestedRegion( region );
  myImage->Allocate();
  
  myVectorIteratorType  it1( myImage, myImage->GetRequestedRegion() );
  
  // Value to initialize the pixels
  myVectorImageType::PixelType color;
  
  // Initializing all the pixel in the image
  it1.GoToBegin();
  while( !it1.IsAtEnd() )
    {
    color[0] = (float) vnl_sample_uniform(0.0, 1.0);
    color[1] = (float) vnl_sample_uniform(0.0, 1.0);
    color[2] = (float) vnl_sample_uniform(0.0, 1.0);
    it1.Set(color);
    ++it1;
    }

  // Reading the values to verify the image content
  std::cout << "--- Initial image --- " << std::endl;
  it1.GoToBegin();
  while( !it1.IsAtEnd() )
    {
    const myVectorImageType::PixelType c( it1.Get() );
    std::cout << c[0]   << "  ";
    std::cout << c[1] << "  ";
    std::cout << c[2]  << std::endl;
    ++it1;
    }


  bool passed = true;

  // Get the first element
  itk::AdaptImageFilter<myVectorImageType, myImageType, myAccessorType>::Pointer  adaptImage = itk::AdaptImageFilter<myVectorImageType, myImageType, myAccessorType>::New();

  myAccessorType accessor;
  accessor.SetElementNumber(0);

  adaptImage->SetAccessor( accessor );
  adaptImage->SetInput(myImage);
  adaptImage->UpdateLargestPossibleRegion();
 
  myIteratorType  it( adaptImage->GetOutput(), adaptImage->GetOutput()->GetRequestedRegion() );

  std::cout << "--- First component values --- " << std::endl;

  it.GoToBegin();
  it1.GoToBegin();
  while( !it.IsAtEnd() )
  {
  std::cout << it.Get()   << std::endl;
  if (it.Get() != it1.Get()[0])
    {
    passed = false;
    }
    
  ++it;
  ++it1;
  }

  // Get the second element
  accessor.SetElementNumber(1);
  adaptImage->SetAccessor( accessor );
  adaptImage->UpdateLargestPossibleRegion();
 
  it = myIteratorType( adaptImage->GetOutput(), adaptImage->GetOutput()->GetRequestedRegion() );

  std::cout << "--- Second component values --- " << std::endl;

  it.GoToBegin();
  it1.GoToBegin();
  while( !it.IsAtEnd() )
  {
  std::cout << it.Get()   << std::endl;
  if (it.Get() != it1.Get()[1])
    {
    passed = false;
    }
  
  ++it;
  ++it1;
  }

  // Convert to the third component
  accessor.SetElementNumber(2);
  adaptImage->SetAccessor( accessor );
  adaptImage->UpdateLargestPossibleRegion();
 
  it = myIteratorType( adaptImage->GetOutput(), adaptImage->GetOutput()->GetRequestedRegion() );

  std::cout << "--- Third component values --- " << std::endl;

  it.GoToBegin();
  it1.GoToBegin();
  while( !it.IsAtEnd() )
  {
  std::cout << it.Get()   << std::endl;
  if (it.Get() != it1.Get()[2])
    {
    passed = false;
    }
  
  ++it;
  ++it1;
  }

  std::cout << std::endl;
  if (passed)
    {
    std::cout << "AdaptImageFilterTest2 passed" << std::endl;
    return EXIT_SUCCESS;
    }
  else
    {
    std::cout << "AdaptImageFilterTest2 passed" << std::endl;
    return EXIT_FAILURE;
    }
}



