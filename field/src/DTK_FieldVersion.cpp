//---------------------------------------------------------------------------//
/*!
 * \file DTK_FieldVersion.cpp
 * \author Stuart R. Slattery
 * \brief DataTransferKit fieldversion definition.
 */
//---------------------------------------------------------------------------//

#include "DTK_FieldVersion.hpp"
#include "Trilinos_version.h"

namespace DataTransferKit
{

std::string DataTransferKit_FieldVersion()
{ 
  return("DataTransferKitField built against Trilinos " TRILINOS_VERSION_STRING); 
}

} // end namespace DataTransferKit

//---------------------------------------------------------------------------//
// end DTK_FieldVersion.cpp
//---------------------------------------------------------------------------//
