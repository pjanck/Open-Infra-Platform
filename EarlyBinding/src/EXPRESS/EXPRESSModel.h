/*
    This file is part of Expresso, a simple early binding generator for EXPRESS.
	Copyright (c) 2016 Technical University of Munich
	Chair of Computational Modeling and Simulation.

    BlueFramework is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    BlueFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#ifndef OpenInfraPlatform_EarlyBinding_EXPRESSModel_795d0e6a_aa9f_4804_a9ff_67beadbd82c2_h
#define OpenInfraPlatform_EarlyBinding_EXPRESSModel_795d0e6a_aa9f_4804_a9ff_67beadbd82c2_h

#include "EXPRESSEntity.h"

#include <string>
#include <map>
#include <memory>
#include <algorithm>

OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_BEGIN


OIP_EARLYBINDING_EXTERN
class
OIP_EARLYBINDING_API_EXPRESS
EXPRESSModel 
{
public:
	EXPRESSModel(const std::string &schema);

	~EXPRESSModel() ;

	//const bool Save(const std::string &filename) const {
	//	//TODO Implement
	//	return true;
	//};

	const std::string schema;
	std::map<size_t, std::shared_ptr<EXPRESSEntity>> entities;
};

OIP_NAMESPACE_OPENINFRAPLATFORM_EARLYBINDING_END

EMBED_EARLYBINDING_INTO_OIP_NAMESPACE(EXPRESSModel);

#endif // end define OpenInfraPlatform_EarlyBinding_EXPRESSModel_795d0e6a_aa9f_4804_a9ff_67beadbd82c2_h
