//---------------------------------------------------------------------------//
/*
  Copyright (c) 2014, Stuart R. Slattery
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  *: Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  *: Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  *: Neither the name of the Oak Ridge National Laboratory nor the
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
 * \brief DTK_MapOperator.cpp
 * \author Stuart R. Slattery
 * \brief Map operator interface.
 */
//---------------------------------------------------------------------------//

#include "DTK_MapOperator.hpp"
#include "DTK_DBC.hpp"

#include <Thyra_MultiVectorStdOps.hpp>

namespace DataTransferKit
{
//---------------------------------------------------------------------------//
// Constructor.
MapOperator::MapOperator()
{ /* ... */ }

//---------------------------------------------------------------------------//
// Destructor.
MapOperator::~MapOperator()
{ /* ... */ }

//---------------------------------------------------------------------------//
// Setup the map operator.
void MapOperator::setup( const Teuchos::RCP<FunctionSpace>& domain_space,
			 const Teuchos::RCP<FunctionSpace>& range_space,
			 const Teuchos::ParameterList>& parameters )
{
    bool not_implemented = true;
    DTK_INSIST( !not_implemented );
}

//---------------------------------------------------------------------------//
// Apply the map operator to data defined on the entities by computing g =
// Minv*(v-A*f).
void MapOperator::apply( 
    const Thyra::MultiVectorBase<double>& domain_dofs,
    const Teuchos::Ptr<Thyra::MultiVectorBase<double> >& range_dofs,
    const double alpha,
    const double beta ) const
{
    DTK_REQUIRE( Teuchos::nonnull(b_coupling_matrix) );
    DTK_REQUIRE( Teuchos::nonnull(domain_dofs) );
    DTK_REQUIRE( Teuchos::nonnull(range_dofs) );
  
    // Make a work vector.
    Teuchos::RCP<Thyra::MultiVectorBase<double> > work = 
	b_forcing_vector->clone_mv();

    // A*f
    b_coupling_matrix->apply( 
	Thyra::NOTRANS, domain_dofs, Teuchos::ptr(work.getRawPtr()), 1.0, 1.0 );

    // v-A*f
    Thyra::Vt_S( Teuchos::ptr(work.getRawPtr()), -1.0 );
    if ( Teuchos::nonnull(b_forcing_vector) )
    {
	Thyra::Vp_V( Teuchos::ptr(work.getRawPtr()), *b_forcing_vector );
    }

    // Minv*(v-A*f)
    if ( Teuchos::nonnull(b_mass_matrix_inv) )
    {
	b_mass_matrix_inv->apply( 
	    Thyra::NOTRANS, *work, range_dofs, 1.0, 1.0 );
    }
    else
    {
	Thyra::assign( range_dofs, *work );
    }

    // g = alpha*g + beta*f
    Thyra::Vt_S( range_dofs, alpha );
    Thyra::scaleUpdate( beta, domain_dofs, range_dofs );
}

//---------------------------------------------------------------------------//

} // end namespace DataTransferKit

//---------------------------------------------------------------------------//
// end DTK_MapOperator.cpp
//---------------------------------------------------------------------------//
