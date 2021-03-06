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
 * \brief DTK_EntityCenteredDOFVector.hpp
 * \author Stuart R. Slattery
 * \brief Entity-centered DOF vector.
 */
//---------------------------------------------------------------------------//

#ifndef DTK_ENTITYCENTEREDDOFVECTOR_HPP
#define DTK_ENTITYCENTEREDDOFVECTOR_HPP

#include <DTK_Entity.hpp>

#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayView.hpp>
#include <Teuchos_Comm.hpp>

#include <Tpetra_MultiVector.hpp>

namespace DataTransferKit
{
//---------------------------------------------------------------------------//
/*!
  \class EntityCenteredDOFVector
  \brief Entity-centered DOF vector.

  Tools for constructing Tpetra vectors for data bound to entity-centered
  shape functions.
*/
//---------------------------------------------------------------------------//
class EntityCenteredDOFVector
{
  public:

    /*!
     * \brief Constructor.
     */
    EntityCenteredDOFVector()
    { /* ... */ }

    /*!
     * \brief Destructor.
     */
    ~EntityCenteredDOFVector()
    { /* ... */ }

    /*!
     * \brief Given a set of entity ids and DOF data bound to the center of
     * those entites, build a Tpetra vector and copy the data into the
     * vector.
     * \param comm The communicator the DOFs are defined over.
     * \param entities The entities over which the DOFs are defined.
     * \param dof_data Reference counted array of the DOF data for the given
     * entity ids.
     * \param lda Single vector length. Should be the same as the size of
     * entity_ids.
     * \param num_vectors The number of vectors in the multivector.
     */
    template<class Scalar>
    static Teuchos::RCP<Tpetra::MultiVector<Scalar,int,std::size_t> > 
    pullTpetraMultiVectorFromEntitiesAndView(
	const Teuchos::RCP<const Teuchos::Comm<int> >& comm,
	const Teuchos::ArrayView<Entity>& entities,
	const int field_dim,
	const Teuchos::ArrayView<Scalar>& dof_data );

    /*!
     * \brief Given a Tpetra vector created by pulling from a view, push back
     * to the same view.
    */
    template<class Scalar>
    static void pushTpetraMultiVectorToEntitiesAndView(
	const Tpetra::MultiVector<Scalar,int,std::size_t>& vector,
	Teuchos::ArrayView<Scalar>&& dof_data );
};

//---------------------------------------------------------------------------//

} // end namespace DataTransferKit

//---------------------------------------------------------------------------//
// Template includes.
//---------------------------------------------------------------------------//

#include "DTK_EntityCenteredDOFVector_impl.hpp"

//---------------------------------------------------------------------------//

#endif // end DTK_ENTITYCENTEREDDOFVECTOR_HPP

//---------------------------------------------------------------------------//
// end DTK_EntityCenteredDOFVector.hpp
//---------------------------------------------------------------------------//
