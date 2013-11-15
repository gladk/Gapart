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

#include "config.h"

namespace fs = boost::filesystem;

configopt::configopt(Eigen::Vector3d c, int nAt, int nDat, int cId, int cT, int cC, 
                     int cR, int cD, double radFactor, bool vtk, bool yade, bool liggghts, 
                     std::string FNameI, std::string FNameO, double scale, 
                     int clearType, Eigen::Vector3d move, Eigen::Vector3d rotate, Eigen::Vector3i layer,
                     Eigen::Vector3i copyRotate,
                     Eigen::Vector3d cutPlus, Eigen::Vector3d cutMinus,
                     Eigen::Vector3i cutPlusB, Eigen::Vector3i cutMinusB,
                     Eigen::Vector3d markPlus, Eigen::Vector3d markMinus,
                     Eigen::Vector3i markPlusB, Eigen::Vector3i markMinusB,
                     double markCylZ, double setDensity
                     ) {
  
  _maxC = -1;
  
  _c = c;
  
  _nAt = nAt;
  _nDat = nDat;
  
  _cId = cId; _maxColumnCheck(_cId, 0);
  _cT = cT; _maxColumnCheck(_cT, 0);
  _cC = cC; _maxColumnCheck(_cC, 2);
  _cR = cR; _maxColumnCheck(_cR, 0);
  _cD = cD; _maxColumnCheck(_cD, 0);
  _vtk = vtk;
  _yade = yade;
  _liggghts = liggghts;
  _FNameI = FNameI;
  _FNameO = FNameO;
  _scale = scale;
  _clearType = clearType;
  _move = move;
  _rotate = rotate;
  _layer = layer;
  _copyRotate = copyRotate;
  _radFactor = radFactor;
  _cutPlus = cutPlus;
  _cutMinus = cutMinus;
  _cutPlusB = cutPlusB;
  _cutMinusB = cutMinusB;
  _markPlus = markPlus;
  _markMinus = markMinus;
  _markPlusB = markPlusB;
  _markMinusB = markMinusB;
  _markCylZ = markCylZ;
  _setDensity = setDensity;
};

void configopt::_maxColumnCheck(int col, int addN) {
  if (col+addN > _maxC) {
    _maxC = col+addN;
  }
};
