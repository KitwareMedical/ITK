/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkBlobSpatialObject.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __itkBlobSpatialObject_txx
#define __itkBlobSpatialObject_txx

#include "itkBlobSpatialObject.h" 

#include <itkNumericTraits.h>

namespace itk  
{

/** Constructor */
template< unsigned int TDimension >
BlobSpatialObject< TDimension > 
::BlobSpatialObject()  
{ 
  m_Dimension = TDimension;
  m_TypeName = "BlobSpatialObject";
  m_Property->SetRed(1); 
  m_Property->SetGreen(0); 
  m_Property->SetBlue(0); 
  m_Property->SetAlpha(1); 
} 

/** Destructor */ 
template< unsigned int TDimension >
BlobSpatialObject< TDimension > 
::~BlobSpatialObject()
{ 
} 
 
/** Get the list of points that defines the blob */
template< unsigned int TDimension >
typename BlobSpatialObject< TDimension > ::PointListType &  
BlobSpatialObject< TDimension > 
::GetPoints() 
{ 
  itkDebugMacro( "Getting BlobPoint list" );
  return m_Points;
} 

/** Get the list of points which are defining the blob */
template< unsigned int TDimension >
const typename BlobSpatialObject< TDimension > ::PointListType &  
BlobSpatialObject< TDimension > 
::GetPoints() const
{ 
  itkDebugMacro( "Getting BlobPoint list" );
  return m_Points;
} 

/** Set the points which are defining the Blob structure */
template< unsigned int TDimension >
void  
BlobSpatialObject< TDimension > 
::SetPoints( PointListType & points )  
{
  // in this function, passing a null pointer as argument will
  // just clear the list...
  m_Points.clear();
        
  typename PointListType::iterator it,end;
  it = points.begin();    
  end = points.end();
  while(it != end)
    {
    m_Points.push_back(*it);
    it++;
    }
  
  this->Modified();
} 
 
/** Print the blob spatial object */
template< unsigned int TDimension >
void  
BlobSpatialObject< TDimension >  
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "BlobSpatialObject(" << this << ")" << std::endl; 
  os << indent << "ID: " << m_Id << std::endl; 
  os << indent << "nb of points: "<< static_cast<unsigned long>( m_Points.size() ) << std::endl;
  Superclass::PrintSelf( os, indent ); 
} 
  
/** Compute the bounds of the blob */ 
template< unsigned int TDimension >
bool 
BlobSpatialObject< TDimension > 
::ComputeBoundingBox() const
{ 
  itkDebugMacro( "Computing blob bounding box" );
  bool ret = false;

  if( this->GetMTime() > m_BoundsMTime )
    {
    ret = Superclass::ComputeBoundingBox();

    if( m_BoundingBoxChildrenName.empty() || strstr(typeid(Self).name(), m_BoundingBoxChildrenName.c_str()) )
      {
      typename PointListType::const_iterator it  = m_Points.begin();
      typename PointListType::const_iterator end = m_Points.end();
  
      if(it == end)
        {
        return ret;
        }
      else
        {
        if(!ret)
          {
          m_Bounds->SetMinimum((*it).GetPosition());
          m_Bounds->SetMaximum((*it).GetPosition());
          it++;
          }
        while(it!= end) 
          {  
          m_Bounds->ConsiderPoint((*it).GetPosition());
          it++;
          }
        ret = true;
        }
      }

    m_BoundsMTime = this->GetMTime();
    }

  return ret;
} 

/** Test if the given point is inside the blob
 *  Note: ComputeBoundingBox should be called before. */
template< unsigned int TDimension >
bool 
BlobSpatialObject< TDimension > 
::IsInside( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking the point [" << point << "] is inside the blob" );

  if( name == NULL || strstr(typeid(Self).name(), name) )
    {
    typename PointListType::const_iterator it = m_Points.begin();
    typename PointListType::const_iterator itEnd = m_Points.end();
    
    const TransformType * giT = GetWorldToIndexTransform();
    PointType transformedPoint = giT->TransformPoint(point);
  
    if( m_Bounds->IsInside(transformedPoint) )
      {
      while(it != itEnd)
        {
        typename PointType::VectorType difference = 
          transformedPoint - it->GetPosition();
        if(difference[0] <= 0.5 || difference[1] <= 0.5 || difference[2] <= 0.5)
          {
          return true;
          }
        it++;
        }
      }
    }

  return Superclass::IsInside(point, depth, name);
} 

/** Return true if the blob is evaluable at a given point 
 *  i.e if the point is defined in the points list        */
template< unsigned int TDimension >
bool
BlobSpatialObject< TDimension > 
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking if the blob is evaluable at " << point );
  return IsInside(point, depth, name);
}


/** Return 1 if the point is in the points list */
template< unsigned int TDimension >
bool
BlobSpatialObject< TDimension > 
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name ) const
{
  itkDebugMacro( "Getting the value of the blob at " << point );
  if( IsInside(point, 0, name) )
    {
    value = 1;
    return true;
    }
  else
    {
    if( Superclass::IsEvaluableAt(point, depth, name) )
      {
      Superclass::ValueAt(point, value, depth, name);
      return true;
      }
    else
      {
      value = 0;
      return false;
      }
    }
}

} // end namespace itk 

#endif
