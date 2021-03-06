//---------------------------------------------------------------------------//
/*
  Copyright (c) 2012, Stuart R. Slattery
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  *: Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  *: Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  *: Neither the name of the University of Wisconsin - Madison nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//---------------------------------------------------------------------------//
/*!
 * \brief DTK_STKMeshManager.cpp
 * \author Stuart R. Slattery
 * \brief High-level manager for STK mesh.
 */
//---------------------------------------------------------------------------//

#include "DTK_STKMeshManager.hpp"
#include "DTK_STKMeshEntityPredicates.hpp"
#include "DTK_STKMeshNodalShapeFunction.hpp"
#include "DTK_STKMeshEntitySet.hpp"
#include "DTK_STKMeshEntityLocalMap.hpp"
#include "DTK_DBC.hpp"

namespace DataTransferKit
{
//---------------------------------------------------------------------------//
//! Default constructor.
STKMeshManager::STKMeshManager(
    const Teuchos::RCP<stk::mesh::BulkData>& bulk_data,
    const EntityType entity_type,
    const BasisType basis_type )
{
    Teuchos::RCP<EntitySelector> entity_selector = 
	Teuchos::rcp( new EntitySelector(entity_type) );

    createFunctionSpace( bulk_data, basis_type, entity_selector );

    DTK_ENSURE( Teuchos::nonnull(d_function_space) );
}

//---------------------------------------------------------------------------//
//! Part name constructor.
STKMeshManager::STKMeshManager(
    const Teuchos::RCP<stk::mesh::BulkData>& bulk_data,
    const Teuchos::Array<std::string>& part_names,
    const EntityType entity_type,
    const BasisType basis_type )
{
    STKPartNamePredicate pred( part_names, bulk_data );
    Teuchos::RCP<EntitySelector> entity_selector =
	Teuchos::rcp( new EntitySelector(entity_type,pred.getFunction()) );

    createFunctionSpace( bulk_data, basis_type, entity_selector );

    DTK_ENSURE( Teuchos::nonnull(d_function_space) );
}

//---------------------------------------------------------------------------//
//! Part vector constructor.
STKMeshManager::STKMeshManager(
    const Teuchos::RCP<stk::mesh::BulkData>& bulk_data,
    const stk::mesh::PartVector& parts,
    const EntityType entity_type,
    const BasisType basis_type )
{
    STKPartVectorPredicate pred( parts );
    Teuchos::RCP<EntitySelector> entity_selector =
	Teuchos::rcp( new EntitySelector(entity_type,pred.getFunction()) );

    createFunctionSpace( bulk_data, basis_type, entity_selector );
}

//---------------------------------------------------------------------------//
//! Selector constructor.
STKMeshManager::STKMeshManager(
    const Teuchos::RCP<stk::mesh::BulkData>& bulk_data,
    const stk::mesh::Selector& selector,
    const EntityType entity_type,
    const BasisType basis_type )
{
    STKSelectorPredicate pred( selector );
    Teuchos::RCP<EntitySelector> entity_selector =
	Teuchos::rcp( new EntitySelector(entity_type,pred.getFunction()) );

    createFunctionSpace( bulk_data, basis_type, entity_selector );
}

//---------------------------------------------------------------------------//
// Destructor.
STKMeshManager::~STKMeshManager()
{ /* ... */ }

//---------------------------------------------------------------------------//
// Get the function space over which the mesh and its fields are defined. 
Teuchos::RCP<FunctionSpace> STKMeshManager::functionSpace() const
{
    return d_function_space;
}

//---------------------------------------------------------------------------//
// Create the function space.
void STKMeshManager::createFunctionSpace( 
    const Teuchos::RCP<stk::mesh::BulkData>& bulk_data,
    const BasisType basis_type,
    const Teuchos::RCP<EntitySelector>& entity_selector )
{
    Teuchos::RCP<EntitySet> entity_set = 
	Teuchos::rcp( new STKMeshEntitySet(bulk_data) );
    
    Teuchos::RCP<EntityLocalMap> local_map =
	Teuchos::rcp( new STKMeshEntityLocalMap(bulk_data) );

    Teuchos::RCP<EntityShapeFunction> shape_function;
    switch( basis_type )
    {
	case BASIS_TYPE_GRADIENT:
	    shape_function = 
		Teuchos::rcp( new STKMeshNodalShapeFunction(bulk_data) );
	    break;

	default:
	    bool bad_basis_type = true;
	    DTK_INSIST( !bad_basis_type );
	    break;
    }
    DTK_CHECK( Teuchos::nonnull(shape_function) );

    d_function_space = Teuchos::rcp( 
	new FunctionSpace(entity_set,entity_selector,local_map,shape_function) );

    DTK_ENSURE( Teuchos::nonnull(d_function_space) );
}

//---------------------------------------------------------------------------//

} // end namespace DataTransferKit

//---------------------------------------------------------------------------//
// end DTK_STKMeshManager.cpp
//---------------------------------------------------------------------------//
