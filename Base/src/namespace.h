﻿/*
    Copyright (c) 2021 Technical University of Munich
    Chair of Computational Modeling and Simulation.

    TUM Open Infra Platform is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    TUM Open Infra Platform is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef OpenInfraPlatform_Base_namespace_a5ae066c_4434_4e69_9238_ed179dc9fbdb_g
#define OpenInfraPlatform_Base_namespace_a5ae066c_4434_4e69_9238_ed179dc9fbdb_g
#if !defined OIP_NAMESPACE_OPENINFRAPLATFORM_BASE_BEGIN
#define OIP_NAMESPACE_OPENINFRAPLATFORM_BASE_BEGIN \
namespace OpenInfraPlatform { \
namespace Base { 
#endif
#if !defined OIP_NAMESPACE_OPENINFRAPLATFORM_BASE_END
#define OIP_NAMESPACE_OPENINFRAPLATFORM_BASE_END \
}\
}
#endif
#if !defined EMBED_BASE_INTO_OIP_NAMESPACE
#define EMBED_BASE_INTO_OIP_NAMESPACE(X) \
namespace oip { \
using OpenInfraPlatform::Base::X; \
}
#endif
#endif // end define OpenInfraPlatform_Base_namespace_a5ae066c_4434_4e69_9238_ed179dc9fbdb_g
