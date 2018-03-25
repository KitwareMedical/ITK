/*=========================================================================
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
#ifndef itkGiftiMeshIOFactory_h
#define itkGiftiMeshIOFactory_h
#include "ITKIOMeshExport.h"

#include "itkMeshIOBase.h"
#include "itkObjectFactoryBase.h"

namespace itk
{
/** \class GiftiMeshIOFactory
   * \brief Create instances of GiftiMeshIO objects using an object factory.
   * \ingroup ITKIOMesh
   */
class ITKIOMesh_EXPORT GiftiMeshIOFactory:public ObjectFactoryBase
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(GiftiMeshIOFactory);

  /** Standard class type aliases. */
  using Self = GiftiMeshIOFactory;
  using Superclass = ObjectFactoryBase;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Class methods used to interface with the registered factories. */
  const char * GetITKSourceVersion(void) const override;

  const char * GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GiftiMeshIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    GiftiMeshIOFactory::Pointer giftiFactory = GiftiMeshIOFactory::New();

    ObjectFactoryBase::RegisterFactoryInternal(giftiFactory);
  }

protected:
  GiftiMeshIOFactory();
  ~GiftiMeshIOFactory() override;

  void PrintSelf(std::ostream & os, Indent indent) const override;
};
// /////////////////////////////////////////////////////////////////////
} // end namespace itk

#endif
