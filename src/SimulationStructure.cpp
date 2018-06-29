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

 * Copyright 2014-2018 Lily Carlson
 */

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


#include "SimulationStructure.h"
#include "Atom.h"
#include "Exceptions.h"
#include "Logger.h"
#include "StringUtility.h"
#include "tinyxmlUtility.h"

using std::endl;
using std::exception;
using std::ios_base;
using std::out_of_range;

using StringUtility::tokenize;
using StringUtility::split;

using tinyxmlUtility::getFirstChildHandle;
using tinyxmlUtility::handleToDoubleTuple;
using tinyxmlUtility::handleToInt;
using tinyxmlUtility::handleToUnsigned;
using tinyxmlUtility::handleToUnsignedVector;


SimulationStructure::SimulationStructure() {

}

SimulationStructure::SimulationStructure(const vector<AtomicStructure>& atomicStructures) {
	add(atomicStructures);

}     

SimulationStructure::SimulationStructure(const SimulationStructure& other){
	add(other.structures);
	charge = other.get_charge();
	spinstate = other.get_spinstate();

}

SimulationStructure::~SimulationStructure() {

}

void SimulationStructure::saveToXML(string filename) {
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement* simStructureElement = generateXML("SimulationStructure");
	doc.LinkEndChild( simStructureElement );

	doc.SaveFile(filename.c_str());
}

TiXmlElement* SimulationStructure::generateXML(string handleName) {
	TiXmlElement* simStructureElement = new TiXmlElement(handleName.c_str());

	TiXmlElement* structuresElement = new TiXmlElement("Structures");
	for(unsigned i = 0; i < structures.size(); i++) {
		structuresElement->LinkEndChild( structures.at(i).generateXML("AtomicStructure") );
	}
	simStructureElement->LinkEndChild( structuresElement );

	TiXmlElement* chargeElement = new TiXmlElement("Charge");
	chargeElement->LinkEndChild( new TiXmlText( std::to_string(charge).c_str() ) );
	simStructureElement->LinkEndChild( chargeElement );

	TiXmlElement* spinStateElement = new TiXmlElement("SpinState");
	spinStateElement->LinkEndChild( new TiXmlText( std::to_string(spinstate).c_str() ) );
	simStructureElement->LinkEndChild( spinStateElement );
		
	return simStructureElement;
}

void SimulationStructure::loadFromXML(string filename) {
	TiXmlDocument doc;

	if(!doc.LoadFile(filename.c_str())) {
		Logger::log(Logger::Level::SEVERE, "Error loading from xml file: " + string(doc.ErrorDesc()));
		throw ios_base::failure("TinyXML reported the following error: " + string(doc.ErrorDesc()));
		return;
	}

	TiXmlHandle hDoc(&doc);
	
	TiXmlHandle simStructureHandle = hDoc.FirstChild("SimulationStructure");
	parseXML(simStructureHandle);
}

void SimulationStructure::parseXML(TiXmlHandle& simStructureHandle) {
	TiXmlHandle structuresHandle = getFirstChildHandle(simStructureHandle, "Structures");
	TiXmlHandle structureHandle = getFirstChildHandle(structuresHandle, "AtomicStructure");
	int structureIndex = 1;
	while (structureHandle.ToNode()) {
		AtomicStructure newAtomicStructure;
		newAtomicStructure.parseXML(structureHandle);
		structures.push_back(newAtomicStructure);
		structureHandle = structuresHandle.Child("AtomicStructure", structureIndex++);
	}

	TiXmlHandle chargeHandle = getFirstChildHandle(structureHandle, "Charge");
	charge = handleToInt(chargeHandle);

	TiXmlHandle spinStateHandle = getFirstChildHandle(structureHandle, "SpinState");
	spinstate = handleToInt(spinStateHandle);

}

void SimulationStructure::add(const AtomicStructure& atomicStructure) {
	structures.push_back(atomicStructure);
	set_charge(charge);
}

void SimulationStructure::add(const string& structure) {
// TODO
	 Logger::log(Logger::Level::SEVERE, "simulationStructure.add(const string& structure not implemented");
/*	vector<string> lines = split(structure, "\n");
	size_t numlines = lines.size();
	for (size_t i=0; i<numlines; ++i) {
		atoms.push_back(Atom(lines[i]));
	}
*/
	set_charge(charge);
}

void SimulationStructure::add(const vector<AtomicStructure>& new_structures) {
	size_t size = new_structures.size();
	for (size_t i=0; i<size; ++i) {
		structures.push_back(new_structures[i]);
	}
	set_spinstate(spinstate);
	set_charge(charge);
}

void SimulationStructure::set_spinstate(int spinstate) {
	this->spinstate = spinstate;
}
        
void SimulationStructure::set_charge(int charge) {
	this->charge = charge;
}

unsigned int SimulationStructure::getAtomCount() {
	unsigned int size = 0;
	for(AtomicStructure& a : structures){
		size += a.size();
	}
	return size;
	
}

const AtomicStructure& SimulationStructure::operator [] (size_t index) const {
        return structures[index];
}

AtomicStructure& SimulationStructure::operator [] (size_t index) {
        return structures[index];
}

ostream& operator << (ostream& out, const SimulationStructure& self) {
        out << self.get_charge() << '\t' << self.get_spinstate() << endl;
        size_t size = self.size();
        for (size_t i=0; i<size; ++i) {
                out << self[i];
        }
        return out;
}

istream& operator >> (istream& in, SimulationStructure& self) {
        self = SimulationStructure();
        string line;

        // Electronic State
        getline(in, line);
        vector<int> electronic_state;
        try {
                electronic_state = tokenize<int>(line);
        } catch (exception& e) {
                throw runtime_error("Unexpected format for electronic state");
        }
        if (electronic_state.size() != 2) {
                throw runtime_error("Unexpected format for electronic state");
        }
        self.set_charge(electronic_state[0]);
        self.set_spinstate(electronic_state[1]);

        while (getline(in, line)) {
                if (line == "") break; // quit on blank line
                self.add(AtomicStructure(line));
        }
        return in;
}


string to_string(const SimulationStructure& simulation_structure) {
	ostringstream result;
	result << simulation_structure;
	return result.str();
}

void SimulationStructure::save_history() {
	for(AtomicStructure& atomicStructure: structures) {
		atomicStructure.save_history();
	}
	//history[history_step % ATOMIC_STRUCTURE_HISTORY_SIZE] = get_positions();
	//++history_step;

}

void SimulationStructure::save_timestep(unsigned timestep) {
	for(AtomicStructure& atomicStructure: structures) {
		atomicStructure.setCurrentTimestep(timestep);
	}
	//history[history_step % ATOMIC_STRUCTURE_HISTORY_SIZE] = get_positions();
	//++history_step;

}

vector<AtomicStructure> SimulationStructure::getStructures(){
	return structures;
}

string SimulationStructure::toXYZFile() {
	stringstream ss;
	ss << this->getAtomCount() << endl;
	ss << endl;
	for (size_t i = 0; i < size(); ++i) {
		ss << structures[i].toXYZFile();
	}
	return ss.str();
}

bool SimulationStructure::equals(SimulationStructure const& other) const {
	return this->structures == other.structures &&
	       this->charge == other.charge &&
	       this->spinstate == other.spinstate;
}
