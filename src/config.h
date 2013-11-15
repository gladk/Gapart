/*
    This file is part of Gapart.
    Gapart is the program to modify particle-pack-files for DEM-simulations.
    Copyright (C) 2013 TU Bergakademie Freiberg, Institute for Mechanics and Fluid Dynamics

    Author: 2013, Anton Gladky <gladky.anton@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gapart is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gapart.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp> 
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <string>
#include <Eigen/Dense>


class configopt {
  private:
    Eigen::Vector3d _c;       // Center point
    
    int _nAt;                 // Number of atom string
    int _nDat;                // Begin of data string
    int _cId;                 // Column of Id
    int _cT;                  // Column of type
    int _cC;                  // Column of center
    int _cR;                  // Column of radius
    int _cD;                  // Column of density
    int _maxC;                // Maximal column number
    void _maxColumnCheck(int, int);         // Check whether the new column is maximal
    std::string _FNameI;       // Input file name of particles
    std::string _FNameO;       // Output file name of particles
    
    
    // Others
    bool  _vtk;                // True, if VTK-file will be created
    bool  _yade;               // True, if YADE-file will be created
    bool  _liggghts;           // True, if LIGGGHTS-file will be created
    double _radFactor;         // Factor for radiueses
    
    // Action
    double _scale;             // If particles need to be scaled;
    int _clearType;            // Set the type of all particles
    double _setDensity;        // Set the density of all particles
    Eigen::Vector3d _move;     // Move particles
    Eigen::Vector3i _layer;    // Particle layers
    Eigen::Vector3d _cutPlus;  // Cut particles, positive direction
    Eigen::Vector3d _cutMinus; // Cut particles, negative direction
    Eigen::Vector3i _cutPlusB; // Set 1, if positive direction should be cutted
    Eigen::Vector3i _cutMinusB;// Set -1, if negative direction should be cutted
    Eigen::Vector3d _markPlus;  // Mark particles, positive direction
    Eigen::Vector3d _markMinus; // Mark particles, negative direction
    Eigen::Vector3i _markPlusB; // Set 1, if positive direction should be marked
    Eigen::Vector3i _markMinusB;// Set -1, if negative direction should be marked
    double _markCylZ;           // Mark particles as a cylinder with given radius along Z-coordinate
    
    
  public:
    configopt(Eigen::Vector3d c, int nAt, int nDat, int cId, int cT, int cC, 
              int cR, int cD, double radFactor, bool vtk, bool yade, bool liggghts, 
              std::string FNameI, std::string FNameO, double scale, int clearType, 
              Eigen::Vector3d move, Eigen::Vector3i layer,
              Eigen::Vector3d cutPlus,  Eigen::Vector3d cutMinus,
              Eigen::Vector3i cutPlusB, Eigen::Vector3i cutMinusB,
                     Eigen::Vector3d markPlus, Eigen::Vector3d markMinus,
                     Eigen::Vector3i markPlusB, Eigen::Vector3i markMinusB,
                     double markCylZ, double setDensity
                     );
              
    Eigen::Vector3d get_c(){return _c;};
    int nAt(){return _nAt;};
    int nDat(){return _nDat;};
    int cId(){return _cId;};
    int cT(){return _cT;};
    int cC(){return _cC;};
    int cR(){return _cR;};
    int cD(){return _cD;};
    int maxC(){return _maxC;};
    
    // Others
    void setVtk() {_vtk=true;}
    void unSetVtk() {_vtk=false;}
    bool Vtk() {return _vtk;}
    bool YADE() {return _yade;}
    bool LIGGGHTS() {return _liggghts;}
    std::string FNameI() {return _FNameI;}
    std::string FNameO() {return _FNameO;}
    double scale() {return _scale;}
    int clearType() {return _clearType;}
    double setDensity() {return _setDensity;}
    Eigen::Vector3d move() {return _move;}
    Eigen::Vector3i layer() {return _layer;}
    double radFactor() {return _radFactor;}
    Eigen::Vector3d cutPlus() {return _cutPlus;}
    Eigen::Vector3i cutPlusB() {return _cutPlusB;}
    Eigen::Vector3d cutMinus() {return _cutMinus;}
    Eigen::Vector3i cutMinusB() {return _cutMinusB;}
    Eigen::Vector3d markPlus() {return _markPlus;}
    Eigen::Vector3i markPlusB() {return _markPlusB;}
    Eigen::Vector3d markMinus() {return _markMinus;}
    Eigen::Vector3i markMinusB() {return _markMinusB;}
    double markCylZ() {return _markCylZ;}
};
