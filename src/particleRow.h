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
#include <boost/foreach.hpp>
#include "particle.h"

struct calculatedSizes {
  Eigen::Vector3d center;
  Eigen::Vector3d extends;
  std::vector <std::shared_ptr<particle> > minP;
  std::vector <std::shared_ptr<particle> > maxP;
};

class particleRow {
  private: 
    std::vector <std::shared_ptr<particle> > _allPart;
    std::vector <std::shared_ptr<particle> > _allPartUpd;
    bool _sizeCalculate, _sizeCalculateUpd;
    Eigen::Vector3d _center, _extends, _centerUpd, _extendsUpd;
    std::vector <std::shared_ptr<particle> > _minP, _maxP, _minPUpd, _maxPUpd;
    std::vector <int> _types;
  
  public:
    particleRow();
    void addP(std::shared_ptr<particle> );
    calculatedSizes calculateSize(std::vector <std::shared_ptr<particle> > &  calcPart);
    void calculateSizeSrc();
    void calculateSizeUpd();
    void scale(double scaleFactor);
    void move(Eigen::Vector3d shift);
    void rotate(Eigen::Vector3d angles);
    void type(int setType);
    void density(double setDensity);
    void createUpdArray();
    void createLayers(Eigen::Vector3i numL, Eigen::Vector3d shiftL);
    void cutParticlesXYZ(Eigen::Vector3i cutBool, Eigen::Vector3d cutLength);
    void markParticlesXYZ(Eigen::Vector3i markBool, Eigen::Vector3d markLength, int setType, double markCylZ);
    void markParticlesCylZ(double markCylZ, int setType);
    unsigned long long size() {return _allPart.size();}; 
    std::vector <int> types(); 
    unsigned long long sizeUpd() {return _allPartUpd.size();};
    std::shared_ptr<particle> getPart(unsigned long long id);
    std::shared_ptr<particle> getPartUpd(unsigned long long id);
    Eigen::Vector3d center();
    Eigen::Vector3d extends();
    Eigen::Vector3d centerUpd();
    Eigen::Vector3d extendsUpd();
    bool sizeCalculate() {return _sizeCalculate;}
    bool sizeCalculateUpd() {return _sizeCalculateUpd;}
    bool markCylZDo(std::shared_ptr<particle> p, double & markCylZ);
};
