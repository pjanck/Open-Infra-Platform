/*
	Copyright (c) 2018 Technical University of Munich
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
#ifndef FACECONVERTER_H
#define FACECONVERTER_H

#include "CarveHeaders.h"
#include <memory>
#include "ConverterBase.h"
#include "CurveConverter.h"
#include "PlacementConverter.h"
#include "GeomUtils.h"
#include "GeometryInputData.h"
#include "PlacementConverter.h"
#include "BlueFramework/Core/Diagnostics/log.h"

namespace OpenInfraPlatform {
	namespace Core {
		namespace IfcGeometryConverter {
			class ItemData;

			/*! \brief Converter functionality for \c IfcSurfaces's subtypes.
			*
			* This class includes converter functions for
			* \c IfcBoundedSurface and its subtypes,
			* \c IfcElementarySurface and its subtypes and
			* \c IfcSweptSurface.
			*
			* \param IfcEntityTypesT The IFC version templates
			*/

			template <
				class IfcEntityTypesT
			>
				class FaceConverterT : public ConverterBaseT<IfcEntityTypesT>
			{
			public:

				//! Constructor
				FaceConverterT(std::shared_ptr<GeometrySettings> geomSettings,
					std::shared_ptr<UnitConverter<IfcEntityTypesT>> unitConverter,
					std::shared_ptr<PlacementConverterT<IfcEntityTypesT>> pc,
					std::shared_ptr<CurveConverterT<IfcEntityTypesT>> cc)
					:
					ConverterBaseT<IfcEntityTypesT>(geomSettings, unitConverter),
					placementConverter(pc),
					curveConverter(cc)
				{
				}

				//! Virtual destructor
				virtual ~FaceConverterT()
				{
				}

				/*! \brief Converts an \c IfcSurface to an array of segments to be rendered on screen.
				 *
				 * \param[in] surface		The \c IfcSurface to be converted.
				 * \param[in] pos			The relative location of the origin of the representation's coordinate system within the geometric context.
				 * \param[out] itemData		A pointer to be filled with the relevant data.
				 *
				 * \note Calls the other overload FaceConverterT::convertIfcSurface with corresponding input parameters. Adds their result to itemData.
				 */
				void convertIfcSurface(
					const EXPRESSReference<typename IfcEntityTypesT::IfcSurface>& surface,
					const carve::math::Matrix& pos,
					std::shared_ptr<ItemData>& itemData
				) const throw(...)
				{
					std::shared_ptr<carve::input::PolylineSetData> polyline =
						convertIfcSurface(surface, pos);
					if (polyline->getVertexCount() > 1) 
						itemData->polylines.push_back(polyline);
				}

				/*! \brief Converts \c IfcSurface to a according to subtype.
					\param		surface	\c IfcSurface entity to be interpreted.
					\param		pos
					\return		polylineData
					\note		The \c IfcSurface subtypes are \c IfcBoundedSurface,  \c IfcElementarySurface and  \c IfcSweptSurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					// (1/3) IfcBoundedSurface SUBTYPE of IfcSurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcBoundedSurface>()) 
					{
						return convertIfcBoundedSurface(surface.as<typename IfcEntityTypesT::IfcBoundedSurface>(),
							pos);
					}

					// (2/3) IfcElementarySurface SUBTYPE of IfcSurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcElementarySurface>()) 
					{
						return convertIfcElementarySurface(surface.as<typename IfcEntityTypesT::IfcElementarySurface>(),
							pos);
					}

					// (3/3) IfcSweptSurface SUBTYPE of IfcSurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcSweptSurface>())  
					{
						return convertIfcSweptSurface(surface.as<typename IfcEntityTypesT::IfcSweptSurface>(),
							pos);
					}

					// the rest we do not support
					throw oip::UnhandledException(surface);
				}

				//--------------------------------------------------------------------------------------------
				// IfcBoundedSurface
				//--------------------------------------------------------------------------------------------

						/*! \brief  Converts \c IfcBoundedSurface by calling the conversion function according to subtype.
						\param		surface	\c IfcBoundedSurface entity to be interpreted.
						\param		pos
						\return		polylineData
						\note		The \c IfcBoundedSurface subtypes are: \c IfcBSplineSurface, \c IfcCurveBoundedPlane, \c IfcCurveBoundedSurface and \c IfcRectangularTrimmedSurface.
						*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcBoundedSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcBoundedSurface>& surface,
					const carve::math::Matrix& pos)  const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					// (1/4) IfcBSplineSurface SUBTYPE of IfcBoundedSurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcBSplineSurface>()) 
					{
						return convertIfcBSplineSurface(
							surface.as<typename IfcEntityTypesT::IfcBSplineSurface>(),
							pos);
					}

					// (2/4) IfcCurveBoundedPlane SUBTYPE OF IfcBoundedSurface.
					else if (surface.isOfType<typename IfcEntityTypesT::IfcCurveBoundedPlane>()) 
					{
						return convertIfcCurveBoundedPlane(
							surface.as<typename IfcEntityTypesT::IfcCurveBoundedPlane>(),
							pos);
					}

					// (3/4) IfcCurveBoundedSurface SUBTYPE of IfcBoundedSurface.
					else if (surface.isOfType<typename IfcEntityTypesT::IfcCurveBoundedSurface>()) 
					{
						return convertIfcCurveBoundedSurface(
							surface.as<typename IfcEntityTypesT::IfcCurveBoundedSurface>(),
							pos);
					}

					// (4/4) IfcRectangularTrimmedSurface SUBTYPE of IfcBoundedSurface.
					else if (surface.isOfType<typename IfcEntityTypesT::IfcRectangularTrimmedSurface>())  
					{
						return convertIfcRectangularTrimmedSurface(
							surface.as<typename IfcEntityTypesT::IfcRectangularTrimmedSurface>(),
							pos);
					}

					// the rest we do not support
					throw oip::UnhandledException(surface);
				}

				//--------------------------------------------------------------------------------------------
				// IfcElementarySurface
				//--------------------------------------------------------------------------------------------

						/*! \brief  Converts \c IfcElementarySurface by calling the conversion function according to subtype.
						\param		surface	\c IfcElementarySurface entity to be interpreted.
						\param		pos
						\return		polylineData
						\note		The \c IfcElementarySurface subtypes are \c IfcCylindricalSurface, \c IfcPlane, \c IfcSphericalSurface and \c IfcToroidalSurface.
						*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcElementarySurface(const EXPRESSReference<typename IfcEntityTypesT::IfcElementarySurface>& surface,
					const carve::math::Matrix& pos)  const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					// (1/4) IfcCylindricalSurface SUBTYPE of IfcElementarySurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcCylindricalSurface>()) 
					{
						return convertIfcCylindricalSurface(
							surface.as<typename IfcEntityTypesT::IfcCylindricalSurface>(),
							pos);
					}

					// (2/4) IfcPlane SUBTYPE of IfcElementarySurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcPlane>()) 
					{
						return convertIfcPlane(
							surface.as<typename IfcEntityTypesT::IfcPlane>(),
							pos);
					}

					// (3/4) IfcSphericalSurface SUBTYPE of IfcElementarySurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcSphericalSurface>())
					{
						return convertIfcSphericalSurface(
							surface.as<typename IfcEntityTypesT::IfcSphericalSurface>(),
							pos);
					}

					// (4/4) IfcToroidalSurface SUBTYPE of IfcElementarySurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcToroidalSurface >()) 
					{
						return convertIfcToroidalSurface(
							surface.as<typename IfcEntityTypesT::IfcToroidalSurface>(),
							pos);
					}

					// the rest we do not support
					throw oip::UnhandledException(surface);
				}

				//--------------------------------------------------------------------------------------------
				// IfcSweptSurface
				//--------------------------------------------------------------------------------------------

						/*! \brief  Converts \c IfcSweptSurface by calling the conversion function according to subtype.
						\param		surface	\c IfcSweptSurface entity to be interpreted.
						\param		pos
						\return		polylineData
						\note		The \c IfcSweptSurface subtypes are \c IfcSurfaceOfLinearExtrusion and \c IfcSurfaceOfRevolution.
						*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcSweptSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcSweptSurface>& surface,
					const carve::math::Matrix& pos)  const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					// (1/2) IfcSurfaceOfLinearExtrusion SUBTYPE of IfcSweptSurface
					if (surface.isOfType<typename IfcEntityTypesT::IfcSurfaceOfLinearExtrusion>()) 
					{
						return convertIfcSurfaceOfLinearExtrusion(
							surface.as<typename IfcEntityTypesT::IfcSurfaceOfLinearExtrusion>(),
							pos);
					}

					// (2/2) IfcSurfaceOfRevolution SUBTYPE of IfcSweptSurface
					else if (surface.isOfType<typename IfcEntityTypesT::IfcSurfaceOfRevolution>()) 
					{
						return convertIfcSurfaceOfRevolution(
							surface.as<typename IfcEntityTypesT::IfcSurfaceOfRevolution>(),
							pos);
					}

					// the rest we do not support
					throw oip::UnhandledException(surface);
				}

				//--------------------------------------------------------------------------------------------
				// Conversion functions
				//--------------------------------------------------------------------------------------------

					/*! \brief  Converts \c IfcBSplineSurface to ...
						\param	surface	\c IfcBSplineSurface entity to be interpreted.
						\param	pos
						\return	polylineData
						\note	The \c IfcBSplineSurface is a subtype of \c IfcBoundedSurface.
					*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcBSplineSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcBSplineSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcBSplineSurface

					if(surface.isOfType<typename IfcEntityTypesT::IfcBSplineSurfaceWithKnots>()) const throw(...) 
					{

					}
					else 
					{
						// Get attributes 1-4.
						int uDegree = surface->UDegree;
						int vDegree = surface->VDegree;

						auto controlPointList = surface->ControlPointsList;
						for (auto& itControlPointList : controlPointList) {
							std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcCartesianPoint>()> vectorControlPointList;
							std::shared_ptr<ItemData> input_data_cpl_set(new ItemData);

							vectorControlPointList.resize(itControlPointList.size());
							std::transform(itControlPointList.begin(),
								itControlPointList.end(),
								vectorControlPointList.begin(),
								[](EXPRESSReference<typename IfcEntityTypesT::IfcCartesianPoint> it) { return it.lock(); });

						}

						// TO DO: Add enum PLANE_SURF, CYLINDRICAL_SURF, CONICAL_SURF, SPHERICAL_SURF, TOROIDAL_SURF, SURF_OF_REVOLUTION, RULED_SURF, GENERALISED_CONE, QUADRIC_SURF, SURF_OF_LINEAR_EXTRUSION, UNSPECIFIED
						typename IfcEntityTypesT::IfcBSplineSurfaceForm surfaceForm = surface->SurfaceForm;

						// Get attributes 5-7. For information only.
						typename IfcEntityTypesT::IfcLogical uClosed = surface->UClosed;
						typename IfcEntityTypesT::IfcLogical vClosed = surface->VClosed;
						typename IfcEntityTypesT::IfcLogical selfIntersect = surface->SelfIntersect;
					}
					*/

					// return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);

				}

				/*! \brief  Converts \c IfcCurveBoundedPlane to ...
					\param	surface	\c IfcCurveBoundedPlane entity to be interpreted.
					\param	pos
					\return	polylineData
					\note	The \c IfcCurveBoundedPlane is a subtype of \c IfcBoundedSurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcCurveBoundedPlane(const EXPRESSReference<typename IfcEntityTypesT::IfcCurveBoundedPlane>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcCurveBoundedPlane.

					// Get basis surface, outer boundary and inner boundaries.
					EXPRESSReference<typename IfcEntityTypesT::IfcPlane>& basisSurface = surface->BasisSurface;

					if(basisSurface) 
					{

					// Get basis surface position.
					EXPRESSReference<typename IfcEntityTypesT::IfcAxis2Placement3D>& basisSurfacePlacement = basisSurface->Position;

					if(basisSurfacePlacement) 
					{
						surfaceMatrix = pos * placementConverter->convertIfcAxis2Placement3D(basisSurfacePlacement);
					}

					}

					EXPRESSReference<typename IfcEntityTypesT::IfcCurve>& outerBoundary = surface->OuterBoundary;

					if(outerBoundary) 
					{
					}

					std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcCurve>> vInnerBoundaries;
					vInnerBoundaries.resize(surface->InnerBoundaries.size());

					std::transform(
						surface->InnerBoundaries.begin(),
						surface->InnerBoundaries.end(),
						vInnerBoundaries.begin(),
						[](auto& it) { return it.lock(); }
						);

					for(unsigned int i = 0; i < vInnerBoundaries.size(); ++i) 
					{
						EXPRESSReference<typename IfcEntityTypesT::IfcCurve>& innerCurve = vInnerBoundaries[i];
					}

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcCurveBoundedSurface to ...
				\param	surface	\c IfcCurveBoundedSurface entity to be interpreted.
				\param	pos
				\return	polylineData
				\note	The \c IfcCurveBoundedSurface is a subtype of \c IfcBoundedSurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcCurveBoundedSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcCurveBoundedSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcCurveBoundedSurface.

					// Get basis surface, boundaries and implicit outer.
					EXPRESSReference<typename IfcEntityTypesT::IfcSurface> basisSurface = surface->BasisSurface;
					EXPRESSReference<typename IfcEntityTypesT::IfcBoundaryCurve> boundaries = surface->Boundaries;
					std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcBoundaryCurve>> vBoundaries;

					vBoundaries.resize(surface->Boundaries.size());
					std::transform(
						surface->Boundaries.begin(),
						surface->Boundaries.end(),
						vBoundaries.begin(),
						[](auto& it)
						{ return it.lock(); });
					EXPRESSReference<typename IfcEntityTypesT::IfcCartesianPoint> controlPointList = surface->ControlPointsList;

					bool implicitOuter = surface->ImplicitOuter;

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcRectangularTrimmedSurface to ...
				\param	surface	\c IfcRectangularTrimmedSurface entity to be interpreted.
				\param	pos
				\return	polylineData
				\note	The \c IfcRectangularTrimmedSurface is a subtype of \c IfcBoundedSurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcRectangularTrimmedSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcRectangularTrimmedSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcRectangularTrimmedSurface.

					EXPRESSReference<typename IfcEntityTypesT::IfcSurface>& basisSurface = surface->BasisSurface;
					if(basisSurface) 
					{
						convertIfcSurface(basisSurface, pos, polylineData);
					}

					// Get attributes 1-7.
					double = surface->m_U1;
					double v1 = surface->m_V1;
					double u2 = surface->m_U2;
					double v2 = surface->m_V2;
					bool uSense = *(surface->m_Usense);
					bool vSense = *(surface->m_Vsense);

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcCylindricalSurface to ...
				\param	surface	\c IfcCylindricalSurface entity to be interpreted.
				\param	pos
				\return	polylineData
				\note	The \c IfcCylindricalSurface is a subtype of \c IfcElementarySurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcCylindricalSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcCylindricalSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcCylindricalSurface.

					// Get radius.
					double cylindricalRadius = surface->Radius;

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcPlane to ...
				\param	surface	\c IfcPlane entity to be interpreted.
				\param	pos
				\return	polylineData
				\note	The \c IfcPlane is a subtype of \c IfcElementarySurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcPlane(const EXPRESSReference<typename IfcEntityTypesT::IfcPlane>& surface,
					const carve::math::Matrix& pos)  const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					// Get surface position.					
					carve::math::Matrix surfaceMatrix  = pos * placementConverter->convertIfcAxis2Placement3D(surface->Position);

					// 1-----0	create big rectangular plane
					// |	 |	^ y
					// |	 |	|
					// 2-----3	---> x

					std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();

					double planeSpan = HALF_SPACE_BOX_SIZE;
					polylineData->beginPolyline();
					polylineData->addVertex(surfaceMatrix * carve::geom::VECTOR(planeSpan, planeSpan, 0.0));
					polylineData->addVertex(surfaceMatrix * carve::geom::VECTOR(-planeSpan, planeSpan, 0.0));
					polylineData->addVertex(surfaceMatrix * carve::geom::VECTOR(-planeSpan, -planeSpan, 0.0));
					polylineData->addVertex(surfaceMatrix * carve::geom::VECTOR(planeSpan, -planeSpan, 0.0));

					polylineData->addPolylineIndex(0);
					polylineData->addPolylineIndex(1);
					polylineData->addPolylineIndex(2);
					polylineData->addPolylineIndex(3);

					return polylineData;

				}

				/*! \brief  Converts \c IfcSphericalSurface to ...
				\param	surface	\c IfcSphericalSurface entity to be interpreted.
				\param	pos
				\return	polylineData
				\note	The \c IfcSphericalSurface is a subtype of \c IfcElementarySurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcSphericalSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcSphericalSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcSphericalPlane.

					// Get radius.
					double sphericalRadius = surface->Radius;

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcToroidalSurface to ...
				\param	surface	\c IfcToroidalSurface entity to be interpreted.
				\param	pos
				\param	polylineData
				\note	The \c IfcToroidalSurface is a subtype of \c IfcElementarySurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData>  convertIfcToroidalSurface(const EXPRESSReference<typename IfcEntityTypesT::IfcToroidalSurface>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcToroidalSurface.

						// Get major and minor radius.
						double majorRadius = surface->MajorRadius;
						double minorRadius = surface->MinorRadius;

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcSurfaceOfLinearExtrusion to ...
				\param	surface	\c IfcSurfaceOfLinearExtrusion entity to be interpreted.
				\param	pos
				\param	polylineData
				\note	The \c IfcSurfaceOfLinearExtrusion is a subtype of \c IfcSweptSurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcSurfaceOfLinearExtrusion(const EXPRESSReference<typename IfcEntityTypesT::IfcSurfaceOfLinearExtrusion>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcSurfaceOfLinearExtrusion.

					// Get swept curve and position.
					EXPRESSReference<typename IfcEntityTypesT::IfcProfileDef> sweptSurfaceProfile = surface->SweptCurve;
					EXPRESSReference<typename IfcEntityTypesT::IfcAxis2Placement3D> sweptSurfacePlacement = nullptr;

					if (surface->Position) 
					{
						EXPRESSReference<typename IfcEntityTypesT::IfcAxis2Placement3D> ref = surface->Position;
						sweptSurfacePlacement;
					}
					else 
					{
						BLUE_LOG(warning) << "#" << swept_surface->getId() << " IfcSweptSurface without placement found.";
					}

					carve::math::Matrix surfaceMatrix(pos);
					if (sweptSurfacePlacement) 
					{
						surfaceMatrix = pos * placementConverter->convertIfcAxis2Placement3D(sweptSurfacePlacement);
					}


					// Get extrude direction and depth.
					EXPRESSReference<typename IfcEntityTypesT::IfcDirection>& linearExtrusionDirection = surface->ExtrudedDirection;
					double linearExtrusionDepth = surface->Depth;

					*/

					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				/*! \brief  Converts \c IfcSurfaceOfRevolution to ...
				\param	surface	\c IfcSurfaceOfRevolution entity to be interpreted.
				\param	pos
				\return	polylineData
				\note	The \c IfcSurfaceOfRevolution is a subtype of \c IfcSweptSurface.
				*/

				std::shared_ptr<carve::input::PolylineSetData> convertIfcSurfaceOfRevolution(const EXPRESSReference<typename IfcEntityTypesT::IfcSurfaceOfRevolution>& surface,
					const carve::math::Matrix& pos) const throw(...) 
				{
					if (surface.expired()) {
						throw oip::ReferenceExpiredException(surface);
					}
					/* TO DO: Finish implementation of convertIfcSurfaceOfRevolution.

						// Get axis position.
						EXPRESSReference<typename IfcEntityTypesT::IfcAxis1Placement>& axisPosition =
							surface->AxisPosition;

					*/
					//return std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
					throw oip::UnhandledException(surface);
				}

				//------------------------------------------------------------------------------------------------------------
				// FaceConverter functions:
				// convertIfcFaceList, convertIfcFace, convert3DPointsTo2D, triangulateFace, convertIfcCartesianPoint2DVector
				//------------------------------------------------------------------------------------------------------------

						/*! \brief  Converts \c IfcFace to a polygon and adds it to the carve PolyhedronData vector.
						\param	faces \c IfcFace entity to be interpreted.
						\param	pos
						\param	itemData
						\return polygon carve polygon
						\note The \c IfcFaceList can be an open or closed shell.
						*/

				void convertIfcFaceList(const std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcFace>>& faces,
					const carve::math::Matrix& pos,
					std::shared_ptr<ItemData> itemData)  const throw(...) 
				{
					// Carve polygon of the converted face list
					std::shared_ptr<carve::input::PolyhedronData> polygon(new carve::input::PolyhedronData());

					// Contains polygon indices of vertices (x,y,z converted to string)
					std::map<std::string, uint32_t> polygonIndices;

					// Loop through all faces
					for (auto it = faces.cbegin(); it != faces.cend(); ++it) 
					{ //TODO Stefan
						EXPRESSReference<typename IfcEntityTypesT::IfcFace> face = (*it);

						if (face.expired()) {
							throw oip::ReferenceExpiredException(face);
						}

						if (!convertIfcFace(face, pos, polygon, polygonIndices)) 
						{
							throw oip::InconsistentGeometryException(face, "IFC Face conversion failed with these faces");
						}
					}

					// IfcFaceList can be a closed or open shell, so let the calling function decide where to put it
					itemData->open_or_closed_polyhedrons.push_back(polygon);
				}

				/*! \brief  Converts \c IfcFace to a vector of face vertices.
				\param	face \c IfcFace entity to be interpreted.
				\param	pos
				\param	polygon
				\param	polygonIndices
				\return conversionFailed true/false
				\note	At the end, the calculated and merged face vertices are handed over to the \c triangulateFace function.
				*/

				bool convertIfcFace(const EXPRESSReference<typename IfcEntityTypesT::IfcFace>& face,
					const carve::math::Matrix& pos,
					std::shared_ptr<carve::input::PolyhedronData> polygon,
					std::map<std::string, uint32_t>& polygonIndices)  const throw(...) 
				{
					if (face.expired()) {
						throw oip::ReferenceExpiredException(face);
					}
					// Indicates if conversion has failed
					bool conversionFailed = false;

					// Id of face in step file
					const uint32_t faceID = face->getId();

					// To triangulate the mesh, carve needs 2D polygons, we collect the data in 2D and 3D for every bound
					std::vector<std::vector<carve::geom2d::P2>> faceVertices2D;
					std::vector<std::vector<carve::geom::vector<3>>> faceVertices3D;

					// Save polygon indices of merged vertices
					std::map<uint32_t, uint32_t> mergedIndices;
					std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcFaceBound>> modBounds;
					modBounds.reserve(2);
					bool faceLoopReversed = false;

					// Loop through all boundary definitions
					int boundID = -1;

					// If polygon has more than 3 vertices, then we have to project polygon into 2D, so that carve can triangulate the mesh
					ProjectionPlane plane = UNDEFINED;

					// As carve expects outer boundary of face to be at first index, outer boundary has index 0 and inner boundary has index 1
					for (const auto& bound : face->Bounds)
					{
						if (bound.isOfType<typename IfcEntityTypesT::IfcFaceOuterBound>())
						{
							modBounds.insert(modBounds.begin(), bound);
						}
						else 
						{
							modBounds.push_back(bound);
						}
					}
					modBounds.shrink_to_fit();

					for (const auto& bound : modBounds) 
					{
						boundID++;

						//	IfcLoop <- IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop
						const EXPRESSReference<typename IfcEntityTypesT::IfcLoop>& loop = bound->Bound;
						bool polyOrientation = bound->Orientation;

						if (!loop) {
							throw oip::InconsistentModellingException(face, " No valid loop");
						}
						
						// Collect all vertices of the current loop
						std::vector<carve::geom::vector<3>> loopVertices3D;
						curveConverter->convertIfcLoop(loop, loopVertices3D);

						for (auto& vertex : loopVertices3D) 
						{
							vertex = pos * vertex;
						}

						if (loopVertices3D.size() < 3) 
						{
							throw oip::InconsistentGeometryException(loop, " Number of vertices < 3");
						}
						

						// Check for orientation and reverse vertices order if FALSE
						if (!polyOrientation) 
						{
							std::reverse(loopVertices3D.begin(), loopVertices3D.end());
						}

						//	3 Vertices Triangle
						if (loopVertices3D.size() == 3) 
						{
							std::vector<uint32_t> triangleIndices;
							triangleIndices.reserve(3);

							int pointID = -1;
							for (const auto& vertex3D : loopVertices3D) 
							{
								pointID++;

								// apply global transformation to vertex
								const carve::geom::vector<3> v = vertex3D;

								// set string id and search for existing vertex in polygon
								std::stringstream vertexString;
								vertexString << v.x << " " << v.y << " " << v.z;
								auto itFound = polygonIndices.find(vertexString.str());

								uint32_t index = 0;
								if (itFound != polygonIndices.end()) 
								{
									index = itFound->second;
								}
								else 
								{
									index = polygon->addVertex(v);
									polygonIndices[vertexString.str()] = index;
								}

								triangleIndices.push_back(index);
								mergedIndices[pointID] = index;
							}
							polygon->addFace(triangleIndices.at(0), triangleIndices.at(1), triangleIndices.at(2));
						}

						//	> 3 Vertices Triangle					
						std::vector<carve::geom2d::P2> loopVertices2D;

						if (!convert3DPointsTo2D(boundID, plane, loopVertices2D, loopVertices3D, faceLoopReversed)) 
						{
							throw oip::InconsistentGeometryException(face, "loop could not be projected");
						}

						if (loopVertices2D.size() < 3) 
						{
							throw oip::InconsistentGeometryException(face, "loopVertices2D.size() < 3");
						}

						// push back vertices to all faceVertices
						faceVertices2D.push_back(loopVertices2D);
						faceVertices3D.push_back(loopVertices3D);
					}

					// If no faceVertices were collected, no carve operations are required
					if (faceVertices2D.empty()) 
					{
						throw oip::InconsistentGeometryException(face, "no faceVertices were collected"); 
					}

					// Result after incorporating holes in polygons if defined
					std::vector<std::pair<size_t, size_t>> incorporatedIndices;

					// merged vertices after incorporating of holes
					std::vector<carve::geom2d::P2> mergedVertices2D;
					std::vector<carve::geom::vector<3>> mergedVertices3D;

					try 
					{
						incorporatedIndices = carve::triangulate::incorporateHolesIntoPolygon(faceVertices2D);

						for (const auto& incorpIndex : incorporatedIndices) 
						{
							size_t loopIndex = incorpIndex.first;
							size_t vertexIndex = incorpIndex.second;

							carve::geom2d::P2& point2D = faceVertices2D[loopIndex][vertexIndex];
							carve::geom::vector<3>& point3D = faceVertices3D[loopIndex][vertexIndex];

							// add vertices to merged results
							mergedVertices2D.push_back(point2D);
							mergedVertices3D.push_back(point3D);
						}

					}
					catch (const carve::exception& e) // catch carve error if holes cannot be incorporated
					{
						throw oip::InconsistentGeometryException(face, "carve::triangulate::incorporateHolesIntoPolygon failed");
					}

					triangulateFace(mergedVertices2D, mergedVertices3D, faceLoopReversed, polygon, polygonIndices);
					return !conversionFailed;
				}

				/*! \brief  Converts 3D points to 2D.
				\param	boundID
				\param	plane The projection plane
				\param	loopvertices2D
				\param	loopVertices3D
				\param faceLoopReversed
				\return
				\note
				*/

				bool convert3DPointsTo2D(const int boundID,
					ProjectionPlane& plane,
					std::vector<carve::geom2d::P2>& loopVertices2D,
					std::vector<carve::geom::vector<3>>& loopVertices3D,
					bool& faceLoopReversed)  const throw(...)
				{
					// Compute normal of polygon
					carve::geom::vector<3> normal = GeomUtils::computePolygonNormal(loopVertices3D);

					if (boundID == 0) 
					{
						const double nx = std::abs(normal.x);
						const double ny = std::abs(normal.y);
						const double nz = std::abs(normal.z);

						const double nMax = std::max(std::max(nx, ny), nz);

						if (nMax == nx) 
						{
							plane = ProjectionPlane::YZ_PLANE;
						}
						else if (nMax == ny) 
						{
							plane = ProjectionPlane::XZ_PLANE;
						}
						else if (nMax == nz) 
						{
							plane = ProjectionPlane::XY_PLANE;
						}
						else 
						{
							throw oip::InconsistentGeometryException( "It is not possible to define plane in other dimension");
						}
					}

					// Now collect all vertices in 2D
					for (const auto& vertex : loopVertices3D) 
					{
						if (plane == ProjectionPlane::YZ_PLANE) 
						{
							loopVertices2D.push_back(carve::geom::VECTOR(vertex.y, vertex.z));
						}

						else if (plane == ProjectionPlane::XZ_PLANE) 
						{
							loopVertices2D.push_back(carve::geom::VECTOR(vertex.x, vertex.z));
						}

						else if (plane == ProjectionPlane::XY_PLANE) 
						{
							loopVertices2D.push_back(carve::geom::VECTOR(vertex.x, vertex.y));
						}
						else 
						{
							throw oip::InconsistentGeometryException("plane is undefined");
						}
					}

					// Check winding order of 2D polygon
					carve::geom3d::Vector normal2D = GeomUtils::computePolygon2DNormal(loopVertices2D);

					if (boundID == 0) 
					{
						if (normal2D.z < 0) 
						{
							std::reverse(loopVertices2D.begin(), loopVertices2D.end());
							std::reverse(loopVertices3D.begin(), loopVertices3D.end());
							faceLoopReversed = true;
						}
					}
					else 
					{
						if (normal2D.z > 0) 
						{
							std::reverse(loopVertices2D.begin(), loopVertices2D.end());
							std::reverse(loopVertices3D.begin(), loopVertices3D.end());
							// faceLoopReversed = true;
						}
					}
					return true;
				}

				/*! \brief Triangulates merged 2D and 3D vertices to faces.
				\param	faceVertices2D to be triangulated using carve
				\param	faceVertices3D to be added to the triangulation
				\param	faceLoopReversed to adapt order of adding vertices of a new face
				\return	polygon to compare triangulated vertices with existing ones in polygon, and only add them if they're new
				\param	polygonIndices
				*/

				void triangulateFace(const std::vector<carve::geom::vector<2>>& faceVertices2D,
					const std::vector<carve::geom::vector<3>>& faceVertices3D,
					const bool faceLoopReversed,
					std::shared_ptr<carve::input::PolyhedronData> polygon,
					std::map<std::string, uint32_t>& polygonIndices)  const throw(...) 
					{

					// indices after carve triangulation of merged vertices
					std::vector<carve::triangulate::tri_idx> triangulatedIndices;
					std::map<uint32_t, uint32_t> mergedIndices;

					// triangulate 2D polygon and improve triangulation by carve
					carve::triangulate::triangulate(faceVertices2D, triangulatedIndices);
					carve::triangulate::improve(faceVertices2D, triangulatedIndices);

					// add new vertices to polygon or get index of existing vertex
					for (uint32_t i = 0; i < faceVertices3D.size(); ++i) 
					{
						const carve::geom::vector<3>& v = faceVertices3D[i];

						// set string id and search for existing vertex in polygon
						std::stringstream vertexString;
						vertexString << v.x << " " << v.y << " " << v.z;

						auto itFound = polygonIndices.find(vertexString.str());
						uint32_t index = 0;

						if (itFound != polygonIndices.end()) 
						{
							index = itFound->second;
						}
						else 
						{
							index = polygon->addVertex(v);
							polygonIndices[vertexString.str()] = index;
						}
						mergedIndices[i] = index;
					}

					// go through triangulated result and add new faces to polygon
					for (const auto& triangle : triangulatedIndices) 
					{
						const uint32_t i0 = triangle.a;
						const uint32_t i1 = triangle.b;
						const uint32_t i2 = triangle.c;

						const uint32_t v0 = mergedIndices[i0];
						const uint32_t v1 = mergedIndices[i1];
						const uint32_t v2 = mergedIndices[i2];

						if (faceLoopReversed)
							polygon->addFace(v0, v2, v1);
						else
							polygon->addFace(v0, v1, v2);
					}

				}

				/*! \brief Converts \c IfcCartesianPoint to a 2D vector.
				\param	faceVertices2D to be triangulated using carve
				\param	faceVertices3D to be added to the triangulation
				*/

				std::vector<std::vector<carve::geom::vector<3>>> convertIfcCartesianPoint2DVector(
					const std::vector<std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcCartesianPoint>>>& points2D)  const throw(...)
				{
					if (points2D.expired()) {
						throw oip::ReferenceExpiredException(points2D);
					}
					std::vector<std::vector<carve::geom::vector<3>>> loop2D = std::vector<std::vector<carve::geom::vector<3>>>();
					const double lengthFactor = UnitConvert()->getLengthInMeterFactor();
					const uint32_t numPointsY = points2D.size();
					loop2D.reserve(numPointsY);

					for (unsigned int j = 0; j < numPointsY; ++j) //TODO Stefan
					{
						const uint32_t numPointsX = points2D[j].size();
						const std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcCartesianPoint>>& points = points2D[j];

						for (unsigned int i = 0; i < numPointsX; ++i) 
						{
							/*const std::vector<EXPRESSReference<typename IfcEntityTypesT::IfcLengthMeasure>>& coords =
								points[i]->m_Coordinates;*/
							const std::vector<double>& coords = points[i]->Coordinates;
							if (coords.size() > 2) 
							{
								/*double x = coords[0]->m_value * lengthFactor;
								double y = coords[1]->m_value * lengthFactor;
								double z = coords[2]->m_value * lengthFactor;
								*/
								double x = coords[0] * lengthFactor;
								double y = coords[1] * lengthFactor;
								double z = coords[2] * lengthFactor;
								loop2D[j].push_back(carve::geom::VECTOR(x, y, z));
							}
							else if (coords.size() > 1) 
							{
								double x = coords[0] * lengthFactor;
								double y = coords[1] * lengthFactor;

								loop2D[j].push_back(carve::geom::VECTOR(x, y, 0.0));
							}
							else 
							{
								throw oip::InconsistentGeometryException(points2D, "ifc_pt->Coordinates.size() != 2");
							}
						}
					}
					return loop2D;
				}
				
				/*! \internal Still to refactor */
				void convertIfcFaceBasedSurfaceModel(
					const EXPRESSReference<typename IfcEntityTypesT::IfcFaceBasedSurfaceModel>& surface_model,
					const carve::math::Matrix& pos,
					std::shared_ptr<ItemData>& itemData
				) const throw(...)
				{
					auto& vec_face_sets = surface_model->FbsmFaces;

					for (auto& it_face_sets : vec_face_sets) 
					{
						std::shared_ptr<ItemData> input_data_face_set(new ItemData);
						// convert
						convertIfcFaceList(it_face_sets->CfsFaces, pos, input_data_face_set);
						// copy the data
						std::copy(input_data_face_set->open_or_closed_polyhedrons.begin(),
							input_data_face_set->open_or_closed_polyhedrons.end(),
							std::back_inserter(itemData->open_polyhedrons));
					}

				}
				
				/*! \internal Still to refactor */
				void convertIfcShellBasedSurfaceModel(
					const EXPRESSReference<typename IfcEntityTypesT::IfcShellBasedSurfaceModel>& surface_model,
					const carve::math::Matrix& pos,
					std::shared_ptr<ItemData>& itemData
				) const throw(...) 
				{
					//auto vec_shells = shell_based_surface_model->SbsmBoundary;
					for (auto& it_shells : surface_model->SbsmBoundary) 
					{
						std::vector<oip::EXPRESSReference<typename IfcEntityTypesT::IfcFace>> vec_shells;
						std::shared_ptr<ItemData> input_data_shells_set(new ItemData);

						switch (it_shells.which()) 
						{
						case 0:
							vec_shells.resize(it_shells.get<0>()->CfsFaces.size());
							std::transform(it_shells.get<0>()->CfsFaces.begin(), it_shells.get<0>()->CfsFaces.end(), vec_shells.begin(), [](auto& it) {return it; });
							break;
						case 1:
							vec_shells.resize(it_shells.get<1>()->CfsFaces.size());
							std::transform(it_shells.get<1>()->CfsFaces.begin(), it_shells.get<1>()->CfsFaces.end(), vec_shells.begin(), [](auto& it) {return it; });
							break;
						}

						convertIfcFaceList(vec_shells, pos, input_data_shells_set);
						std::copy(input_data_shells_set->open_or_closed_polyhedrons.begin(),
							input_data_shells_set->open_or_closed_polyhedrons.end(),
							std::back_inserter(itemData->closed_polyhedrons));

					}
				}


				/*! \internal Still to refactor */
				void convertIfcTessellatedItem(
					const EXPRESSReference<typename IfcEntityTypesT::IfcTessellatedItem>& tessItem,
					const carve::math::Matrix& pos,
					std::shared_ptr<ItemData>& itemData
					) const throw(...) 
				{
					if (tessItem.as<typename IfcEntityTypesT::IfcTriangulatedFaceSet>())
					{
						std::shared_ptr<carve::input::PolyhedronData> polygon(new carve::input::PolyhedronData());
						auto& faceSet = tessItem.as<typename IfcEntityTypesT::IfcTriangulatedFaceSet>();

						double length_factor = UnitConvert()->getLengthInMeterFactor();

						// obtain vertices from coordinates list and add them to the new polygon
						for (const auto& point : faceSet->Coordinates->CoordList)
						{
							carve::geom::vector<3> vertex =
								carve::geom::VECTOR(point[0],
									point[1],
									point[2]) * length_factor;

							// apply transformation
							vertex = pos * vertex;

							polygon->addVertex(vertex);
						}

						auto& coordinatesIndices = faceSet->CoordIndex;
						auto& pnIndices = faceSet->PnIndex; // optional
						
						std::vector<int> flags;
						if (tessItem.isOfType<typename IfcEntityTypesT::IfcTriangulatedIrregularNetwork>())
						{
							auto tin = tessItem.as<typename IfcEntityTypesT::IfcTriangulatedIrregularNetwork>();
							flags.resize(tin->Flags.size());
							std::transform(tin->Flags.begin(), tin->Flags.end(), flags.begin(), [](auto& it) { return (int)it; });
						}

						// read coordinates index list and create faces
						int i = 0;
						for (auto& indices : coordinatesIndices)
						{
							if (indices.size() < 3)
							{
								throw oip::InconsistentModellingException(tessItem, "invalid size of coordIndex attribute.");
							}
							
							// determine vertices' indices
							size_t i0, i1, i2;

							if (pnIndices)
							{
								i0 = pnIndices.get()[indices[0] - 1] - 1;
								i1 = pnIndices.get()[indices[1] - 1] - 1;
								i2 = pnIndices.get()[indices[2] - 1] - 1;
							}
							else
							{
								i0 = indices[0] - 1;
								i1 = indices[1] - 1;
								i2 = indices[2] - 1;
							}

							// account for flags, if there
							if (!flags.empty())
							{
								// skip void triangles
								if (flags[i++] < 0)
									continue;
								// add break line
								else
								{
									if (flags[i-1] & 1) // first flag set
									{
										std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
										polylineData->beginPolyline();
										polylineData->addVertex(polygon->getVertex(i0));
										polylineData->addPolylineIndex(0);
										polylineData->addVertex(polygon->getVertex(i1));
										polylineData->addPolylineIndex(1);
										itemData->polylines.push_back(polylineData);
									}
									if (flags[i-1] & 2) // second flag set
									{
										std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
										polylineData->beginPolyline();
										polylineData->addVertex(polygon->getVertex(i1));
										polylineData->addPolylineIndex(0);
										polylineData->addVertex(polygon->getVertex(i2));
										polylineData->addPolylineIndex(1);
										itemData->polylines.push_back(polylineData);
									}
									if (flags[i-1] & 4) // third flag set
									{
										std::shared_ptr<carve::input::PolylineSetData> polylineData = std::make_shared<carve::input::PolylineSetData>();
										polylineData->beginPolyline();
										polylineData->addVertex(polygon->getVertex(i0));
										polylineData->addPolylineIndex(0);
										polylineData->addVertex(polygon->getVertex(i2));
										polylineData->addPolylineIndex(1);
										itemData->polylines.push_back(polylineData);
									}
								}
							}

							polygon->addFace(i0, i1, i2);
						}

						itemData->open_or_closed_polyhedrons.push_back(polygon);
						return;
					}

					if (tessItem.template isOfType<typename IfcEntityTypesT::IfcPolygonalFaceSet>())
					{
						std::shared_ptr<carve::input::PolyhedronData> polygon(new carve::input::PolyhedronData());
						auto faceSet = tessItem.template as<typename IfcEntityTypesT::IfcPolygonalFaceSet>();

						double length_factor = UnitConvert()->getLengthInMeterFactor();

						// obtain vertices from coordinates list and add them to the new polygon
						for (const auto& point : faceSet->Coordinates->CoordList)
						{
							carve::geom::vector<3> vertex =
								carve::geom::VECTOR(point[0],
									point[1],
									point[2]) * length_factor;

							// apply transformation
							vertex = pos * vertex;

							polygon->addVertex(vertex);
						}

						auto& faces = faceSet->Faces;
						auto& pnIndices = faceSet->PnIndex; // optional

						for( auto& face : faces )
						{
							// determine vertices' indices
							std::vector<size_t> indices(face->CoordIndex.size());
							std::transform(std::begin(face->CoordIndex), std::end(face->CoordIndex), std::begin(indices),
								[&pnIndices](auto& el) -> size_t { return pnIndices ? pnIndices.get()[el - 1] - 1 : el - 1; });
							polygon->addFace(std::begin(indices), std::end(indices));

							if (face.template isOfType<typename IfcEntityTypesT::IfcIndexedPolygonalFaceWithVoids>())
								BLUE_LOG(warning) << "Ignoring voids at " << face->getErrorLog();
						}

						itemData->open_or_closed_polyhedrons.push_back(polygon);
						return;
					}

					throw oip::UnhandledException(tessItem);
				}

				protected:

				std::shared_ptr<PlacementConverterT<IfcEntityTypesT>> placementConverter;
				std::shared_ptr<CurveConverterT<IfcEntityTypesT>> curveConverter;

			};
		}
	}
}
#endif






