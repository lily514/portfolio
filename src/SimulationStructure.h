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

#ifndef SIMULATION_STRUCTURE_H_INCLUDED
#define SIMULATION_STRUCTURE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Atom.h"
#include "Serializable.h"
#include "tinyxml.h"
#include "AtomicStructure.h"

using std::istream;
using std::ostream;
using std::string;
using std::vector;

#define SIMULATION_STRUCTURE_HISTORY_SIZE 2

/**
 * Structure that represents the entire simulation with a vector of atomicStructures (molecules)
 */
class SimulationStructure : public Equatable<SimulationStructure>, public Serializable {

private:

	/**
	 * Tracks each AtomicStructure in the simulation 
	 */
    vector<AtomicStructure> structures;

    /**
	 * Tracks the charge value of the electronic state of the simulation
	 */
    int charge = 0;

	/**
	 * Tracks the spin state value of the electronic state of the simulation
	 */
    int spinstate = 0;


public:

	/**
	 * Default constructur
	 */
    SimulationStructure();

	/**
	 * Constructor for the SimulationStructure given a vector of AtomicStructures
	 * @param structures Vector of AtomicStructures in the simulation
	 */
     SimulationStructure(const vector<AtomicStructure>& structures);

	/**
	 *
	 */
     SimulationStructure(const SimulationStructure& other);

	/**
	 * Default destructor
	 */
     ~SimulationStructure();
        
	/**
	 * Returns the number of structures contained in the Simulation
	 * @return The number of structures contained in the Simulation
	 */
    inline size_t size() const {return structures.size();}	
 
	/**
	 * Returns the charge of the AtomicStructure
	 * @return The charge of the AtomicStructure
	 */
    inline int get_charge() const {return charge;}

	/**
	 * Returns the spin state of the AtomicStructure
	 * @return The spin state of the AtomicStructure
	 */
    inline int get_spinstate() const {return spinstate;}
    
    /**
     * Counts all the atoms
	 * @return The number of total atoms in the simulation
     */
	unsigned int getAtomCount();

	/**
	 * Sets the spinstate for the simulation
	 * @param spinstate
	 */
    void set_spinstate(int spinstate);

    /**
     * Setter for the charge of the simulation
     * @param charge
     */
    void set_charge(int charge);

	/**
	 * Adds a single AtomicStructure to structures
	 * @param structure Single AtomicStructure object to be added to the simulation
	 */
    void add(const AtomicStructure& structure);

	/**
	 * Adds a single AtomicStructure to structures from a string
 	 * @param structure String representing the AtomicStructure to be added to the simulation
	 */
    void add(const string& structure);

	/**
	 * Adds a vector of AtomicStructures to structures
	 * @param structure Vector of AtomicStructures to be added to the simulation
	 */
    void add(const vector<AtomicStructure>& structures);

	/**
	 * Adds the structures of another simulationStructure into this structure
	 * @param simulationStructure The simulationStructure to be combined with this one
	 */
    void add(const SimulationStructure& simulationStructure);

	/**
	 * Saves the history of each atomicStructure in the simulation
	 */
	void save_history();

	/**
	 * TODO: DOCS
	 */
	void save_timestep(unsigned timestep);

	/**
	 * TODO: get_history() of AtomicStructures
	 */
	//	const vector<Vector3>& get_history(int steps_back) const;
       
	/**
	 * Saves the class instance to an xml file
	 * @param filename The name of the xml file to save to
	 */ 	
	void saveToXML(string filename);

	/**
	 * Serializes the class instane to an xml representation
	 * @param handleName The name to give to the xml element being generated
	 * @returns An xml representation of the class instance
	 */
	TiXmlElement* generateXML(string handleName);

	/**
	 * Lods the class instance from an xml file
	 * @param filename The name of the xml file to load from
	 */
	void loadFromXML(string filename);

	/**
	 * Deserializes the class instance from an xml representation
	 * @param atomicStructureHandle The xml representation to deserialize the class instance from
	 */
	void parseXML(TiXmlHandle& atomicStructureHandle);
 
	/**
	 * Uses the insertion operator for the class to create a string representation of the class instance
	 * @return A string that represents the class instance
	 */
     friend string to_string(const AtomicStructure&);


    /**
     * Returns a particular atom as indicated by the index value
     * @param index The index of the atom to return
     * @return A particular atom as indicated by the index value
     */
    const AtomicStructure& operator [] (size_t index) const;

    /**
     * Returns a particular atom as indicated by the index value
     * @param index The index of the atom to return
     * @return A particular atom as indicated by the index value
     */
    AtomicStructure& operator [] (size_t index);

	/**
	 * Stream insertion operator for AtomicStructure class
	 * @param out The ostream instance being inserted to
	 * @param self The AtomicStructure instance being inserted
	 */
    friend ostream& operator << (ostream& out, const SimulationStructure& self);

	/**
	 * Stream extraction operator for AtomicStructure class
	 * @param in The istream instance being extracted from
	 * @param self The AtomicStructure instance being extracted
	 */
    friend istream& operator >> (istream& in, SimulationStructure& self);
	
	/**
	 * Returns reference to the vector of structures in the simulation
	 * @return The vector of all AtomicStructures in the simulation
	 */
	vector<AtomicStructure> getStructures();
	
	/**
	 * Returns a string that represents the atomic structure in a format suitable for an xyz file
	 * @return A string that represents the atomic structure in a format suitable for an xyz file
	 */
	string toXYZFile();

	/**
	 * From Equatable interface, checks if this simulationStructure is equal to another
	 * @param other SimulationStructure to be compared
	 * @return True if the structures are equal
	 */

	bool equals(SimulationStructure const& other) const;
};

#endif // SIMULATION_STRUCTURE_H_INCLUDED
