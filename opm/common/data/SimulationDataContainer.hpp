/*
  Copyright 2016 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIMULATION_DATA_CONTAINER_HPP
#define SIMULATION_DATA_CONTAINER_HPP

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace Opm {


    /// The SimulationDataContainer is a simple container to manage
    /// simulation data. The container is instantiated with information
    /// of how many cells, faces and phases are present in the
    /// reservoirmodel. You can then add data to the container by using the
    ///
    ///   registerCellData()
    ///   registerFaceData()
    ///
    /// functions. The container owns and manages the data, but
    /// mutable references are returned with the getCellData() and
    /// getFaceData() methods, and the content will typically be
    /// modified by external scope.


    class SimulationDataContainer {
    public:

	// The constants PRESSURE,TEMPERATURE, SATURATION,
	// FACEPRESSURE and FACEPRESSURE are defined to support
	// compile time checking of the keys used in the data container.
        static const std::string PRESSURE;
        static const std::string TEMPERATURE;
        static const std::string SATURATION;
	static const std::string FACEPRESSURE;
	static const std::string FACEFLUX;
      

        SimulationDataContainer(size_t num_cells, size_t num_faces , size_t num_phases);

        size_t numPhases() const;
        size_t numFaces() const;
        size_t numCells() const;

        bool hasCellData( const std::string& name ) const;

        /// Will register a data vector of size numCells() *
        /// components.
        void registerCellData( const std::string& name , size_t components , double initialValue = 0.0 );
        std::vector<double>& getCellData( const std::string& name );
        const std::vector<double>& getCellData( const std::string& name ) const;

        bool hasFaceData( const std::string& name ) const;
        void registerFaceData( const std::string& name , size_t components , double initialValue = 0.0 );
        std::vector<double>& getFaceData( const std::string& name );
        const std::vector<double>& getFaceData( const std::string& name ) const;

        /// Will return the number of components of the celldata with
        /// name @name:
        ///
        ///    numCellDataComponents( "PRESSURE" )   -> 1
        ///    numCellDataComponents( "SATURATION" ) -> 3
        ///
        /// for a three phase model.
        size_t numCellDataComponents( const std::string& name ) const;
        bool equal(const SimulationDataContainer& other) const;


        /// Will set the values of component nr @component in the
        /// field @key. All the cells in @cells will be set to the
        /// values in @values.
        void setCellDataComponent( const std::string& key , size_t component , const std::vector<int>& cells , const std::vector<double>& values);

        const std::unordered_map<std::string, std::vector<double>>& cellData() const;
        std::unordered_map<std::string, std::vector<double>>& cellData();
    private:
        void  addDefaultFields();

        size_t m_num_cells;
        size_t m_num_faces;
        size_t m_num_phases;

        std::unordered_map< std::string , std::vector<double> > m_cell_data;
        std::unordered_map< std::string , std::vector<double> > m_face_data;
    };
}

#endif
