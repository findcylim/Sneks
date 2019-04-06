/* Start Header****************************************************************/
/*!
\file RTTIHelper.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for RTTI type referencing

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once

#ifndef RTTI_HELPER
#define RTTI_HELPER

#include <typeindex>

template<typename T>
std::type_index getTypeIndex()
{
	return std::type_index(typeid(T));
}

#endif