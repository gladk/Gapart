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

#include <Eigen/Dense>
#include <memory>
#include <vector>

class particle {
  private:
    Eigen::Vector3d _c;                    // Center of mass
    unsigned long long _id; int _type;     // Particle id, type
    unsigned int _fileId;                  // File number of the particle
    double _rad;                           // Particle radius
    double _d;                             // Density of the particle

  public:
    particle(unsigned long long id, int type, double rad, double dens, Eigen::Vector3d c);
    particle();
    unsigned long long id() {return _id;};
    
    int type() {return _type;};
    void type(int t) {_type = t;};
    
    double rad() {return _rad;};
    void rad(double r) {_rad = r;};
    
    Eigen::Vector3d c() {return _c;}
    void c(Eigen::Vector3d c) {_c=c;}
    
    double vol() { return 4.0/3.0*M_PI*_rad*_rad*_rad;};
    double density() {return _d;};
};
