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

#include "export.h"
#include <boost/foreach.hpp>

exportclass::exportclass(std::shared_ptr<configopt> cfg, std::shared_ptr <particleRow> particleRow) {
  _cfg = cfg;
  _particleRow = particleRow;
};

void exportclass::LiggghtsIN() {
  stringstream ss;
  //================================================
  
  std::string _fileNameOut;
  _fileNameOut = _cfg->FNameO();
  _fileNameOut += ".liggghts";
  
  ofstream Lig (_fileNameOut.c_str());
  Lig << "LIGGGHTS Description\n\n";
  Lig << _particleRow->sizeUpd() << " atoms\n";
  Lig << "1 atom types\n\n";       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!
  Lig << _particleRow->centerUpd()(0) - _particleRow->extendsUpd()(0) << " " << _particleRow->centerUpd()(0) + _particleRow->extendsUpd()(0) << " xlo xhi\n";
  Lig << _particleRow->centerUpd()(1) - _particleRow->extendsUpd()(1) << " " << _particleRow->centerUpd()(1) + _particleRow->extendsUpd()(1) << " ylo yhi\n";
  Lig << _particleRow->centerUpd()(2) - _particleRow->extendsUpd()(2) << " " << _particleRow->centerUpd()(2) + _particleRow->extendsUpd()(2) << " zlo zhi\n\nAtoms\n\n";
  
  for (unsigned long long z = 0; z<_particleRow->sizeUpd(); z++) {
    std::shared_ptr<particle> p = _particleRow->getPartUpd(z);
    Lig << p->id() << " " << p->type() << " " << p->rad()*2.0 << " " << p->density() << " " << p->c()(0) << " " << p->c()(1)<< " " << p->c()(2) <<'\n';
  }
  
  Lig <<"\nVelocities\n\n";
  
  for (unsigned long long z = 0; z<_particleRow->sizeUpd(); z++) {
    std::shared_ptr<particle> p = _particleRow->getPartUpd(z);
    Lig << p->id() << " 0.0 0.0 0.0 0.0 0.0 0.0\n";
  }
  Lig.close();
};

void exportclass::VTK() {
  ofstream fileVTK;
  std::string _fileNameVTU;

  _fileNameVTU =  _cfg->FNameO();
  _fileNameVTU += ".vtu";

  
  //Export Particles
  vtkSmartPointer<vtkPoints>  spheresPos = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> spheresCells = vtkSmartPointer<vtkCellArray>::New();

  vtkSmartPointer<vtkDoubleArray> radii = vtkSmartPointer<vtkDoubleArray>::New();
  radii->SetNumberOfComponents(1);
  radii->SetName("radii");

  vtkSmartPointer<vtkDoubleArray> density = vtkSmartPointer<vtkDoubleArray>::New();
  density->SetNumberOfComponents(1);
  density->SetName("density");
  
  vtkSmartPointer<vtkIntArray> spheresId = vtkSmartPointer<vtkIntArray>::New();
  spheresId->SetNumberOfComponents(1);
  spheresId->SetName("id");

  vtkSmartPointer<vtkIntArray> spheresType = vtkSmartPointer<vtkIntArray>::New();
  spheresType->SetNumberOfComponents(1);
  spheresType->SetName("type");
  
  
  vtkSmartPointer<vtkUnstructuredGrid> spheresUg = vtkSmartPointer<vtkUnstructuredGrid>::New();
  
  
    
    for (unsigned long long z = 0; z<_particleRow->sizeUpd(); z++) {
      std::shared_ptr<particle> partTemp = _particleRow->getPartUpd(z);
      
      
      vtkIdType pid[1];
      pid[0] = spheresPos->InsertNextPoint(partTemp->c()[0], partTemp->c()[1], partTemp->c()[2]);
      radii->InsertNextValue(partTemp->rad());
      density->InsertNextValue(partTemp->density());
      spheresId->InsertNextValue(partTemp->id());
      spheresType->InsertNextValue(partTemp->type());
      spheresCells->InsertNextCell(1,pid);
    }
    
    
    spheresUg->SetPoints(spheresPos);
    spheresUg->SetCells(VTK_VERTEX, spheresCells);
    spheresUg->GetPointData()->AddArray(radii);
    spheresUg->GetPointData()->AddArray(density);
    spheresUg->GetPointData()->AddArray(spheresId);
    spheresUg->GetPointData()->AddArray(spheresType);
    
  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetDataModeToAscii();
  writer->SetInput(spheresUg);
  
  writer->SetFileName(_fileNameVTU.c_str());
  writer->Write();
};

