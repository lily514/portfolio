/*
 * This file is part of DynSuite
 *
 * DynSuite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * DynSuite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHNATABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DynSuite. If not, see <http://www.gnu.org/licenses/>.

 * Copyright 2014-2017 Lily Carlson
 */

#include <string>
#include <vector>

#include "PackmolShape.h"
#include "Cube.h"
#include "Logger.h"
#include "Vector3.h"
#include "StringUtility.h"

using std::string;
using std::vector;


Cube::Cube( vector<double> constraints) :
	PackmolShape::PackmolShape(constraints),
	constraints(constraints) {
	if (constraints.size() != expectedConstraints) {
		throw SizeMismatchError(expectedConstraints, constraints.size());
	}
	xmin = constraints[0] - 0.1;
	ymin = constraints[1] - 0.1;
	zmin = constraints[2] -0.1;
	d = constraints[3] + 0.2; //add tolerance

}

	
Cube::~Cube(){
}

void Cube::debug_abc(double a, double b, double c) {
                Logger::log(Logger::Level::INFO, "a:" + std::to_string(a));
                Logger::log(Logger::Level::INFO, "b:" + std::to_string(b));
                Logger::log(Logger::Level::INFO, "c:" + std::to_string(c));

}


void Cube::reflection(AtomicStructure& molecule, Vector3 transform_value, double t_remaining){
}

bool Cube::edgeDetect(AtomicStructure& molecule, vector<Vector3> delta, double timestep) {
	molecule.update_positions(delta);
	Vector3 com = molecule.center_of_mass();
	//previous positions
	vector<Vector3> old = molecule.get_history(1);	
	Logger::log(Logger::Level::INFO, "--- Edge Detect ---");
	bool inBounds = true;
	if ( com.x > xmin + d) {
		Logger::log(Logger::Level::INFO, "OUT OF BOUNDS xmax");
		inBounds = false;
	}
	if ( com.x < xmin) {
		Logger::log(Logger::Level::INFO, "OUT OF BOUNDS xmin");
		inBounds = false;
	}
	if ( com.y > ymin + d) {
		Logger::log(Logger::Level::INFO, "OUT OF BOUNDS ymax");
		inBounds = false;
	}
	if ( com.y < ymin) {
		Logger::log(Logger::Level::INFO, "OUT OF BOUNDS ymin");
		inBounds = false;	
	}
	if (com.z > zmin + d) {
		Logger::log(Logger::Level::INFO, "OUT OF BOUNDS zmax");
		inBounds = false;	
	}
	if (com.z < zmin) {
		Logger::log(Logger::Level::INFO, "OUT OF BOUNDS zmin");
		inBounds = false;	
	}
	else {
		Logger::log(Logger::Level::INFO, "In bounds");
	}

	if (!inBounds){
		Vector3 v_com = Vector3();
		vector<Vector3> v_prior = molecule.get_velocity();
		
		float total_mass = 0.0;
        for (size_t i = 0; i < molecule.size(); i++) {
                float atomic_mass = molecule[i].getElement().mass();
                total_mass += atomic_mass;
                v_com += atomic_mass * v_prior[i];
        }
        v_com *= 1.0/total_mass;

        vector<Vector3> v_new;
        for (size_t i = 0; i < molecule.size(); i++) {
                v_new.push_back(v_prior[i] - v_com);
        }
        molecule.set_velocity(v_new);

        vector<Vector3> negated;
        for (Vector3 d : delta){
        	negated.push_back(-1.0 * d);
        }
        molecule.update_positions(negated);
        molecule.apply_velocity(timestep);	
	}
		
return inBounds;
}

