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

#include "SplineInterpretation.h"


// CONSTRUCTOR
OpenInfraPlatform::UserInterface::SplineInterpretation::SplineInterpretation()
{}

// PUBLIC FUNCTIONS
void OpenInfraPlatform::UserInterface::SplineInterpretation::convertSketchToAlignment()
{
	// obtain control points from sketch or from file
	// (from file is from past sketch or from [external] test data)
	// ToDo:
	// std::vector<carve::geom::vector<3>> controlPoints = ...

	// TEMPORARY constant test data
	std::vector<carve::geom::vector<3>> controlPoints;
	controlPoints.push_back(carve::geom::VECTOR(0., 2., 0.));
	controlPoints.push_back(carve::geom::VECTOR(1., 5., 0.));
	controlPoints.push_back(carve::geom::VECTOR(3., 4., 0.));
	controlPoints.push_back(carve::geom::VECTOR(3., 1., 0.));
	controlPoints.push_back(carve::geom::VECTOR(5., 0., 0.));
	controlPoints.push_back(carve::geom::VECTOR(7., 1., 0.));
	controlPoints.push_back(carve::geom::VECTOR(8., 4., 0.));
	controlPoints.push_back(carve::geom::VECTOR(10., 4., 0.));
	const int order = 4;
	const std::vector<double> knotArray = obtainKnotArrayOpenUniform(controlPoints.size(), order);
}

// PRIVATE FUNCTIONS
std::vector<double> OpenInfraPlatform::UserInterface::SplineInterpretation::obtainKnotArrayOpenUniform(const size_t nPoints, const int order)
{
	std::vector<double> knotArray;
	// .resize sets all vector values to 0
	knotArray.resize(nPoints + order);

	// set values from vector position order+1 to nPoints (positon according to mathematical one-based vector)
	for (size_t i = order; i < nPoints; i++)
		knotArray[i] = (i + 1) - order;

	// set values form vector positon nPoints+1 to end (positon according to mathematical one-based vector)
	for (size_t i = nPoints; i < nPoints + order; i++)
		knotArray[i] = nPoints - order + 1;

	return knotArray;
}