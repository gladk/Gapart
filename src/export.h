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


#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkZLibDataCompressor.h>
#include <vtkTriangle.h>
#include <vtkLine.h>
#include <vtkQuad.h>
#include <vtkBitArray.h>


#include "particleRow.h"
#include "config.h"


using namespace std;
class exportclass {
  private:
    std::shared_ptr <configopt> _cfg;
    std::shared_ptr <particleRow> _particleRow;
    
  public:
    exportclass(std::shared_ptr<configopt>, std::shared_ptr <particleRow>);
    void VTK();
    void LiggghtsIN();
    void YADE();
};
