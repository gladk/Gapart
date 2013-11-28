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

#include "bulk.h"
#include "export.h"

bulk::bulk(std::shared_ptr<configopt> cfg) {
  _cfg = cfg;
  _particleNum = 0;
  _particleLoaded = std::shared_ptr <particleRow> (new particleRow());
  
  loadParticles();
  _particleLoaded->calculateSizeSrc();
  
  if (_particleLoaded->size() > 0){
    _particleLoaded->createUpdArray();
    std::cout<<"Creating new array "<<std::endl;
  }
  
  if (_cfg->scale() != 1.0) {
    std::cout<<"Scaling particle with coefficient: "<<_cfg->scale()<<std::endl;
    _particleLoaded->scale(_cfg->scale());
  }
  
  if (_cfg->layer()(0)>1 or _cfg->layer()(1)>1 or _cfg->layer()(2)>1) {
    std::cout<<"Creating layers of particles along xyz-axis layers: ["<<  _cfg->layer()(0) <<", "<<  _cfg->layer()(1) <<", "<<  _cfg->layer()(2) <<"] with the shift ["<<  _cfg->move()(0) <<", "<<  _cfg->move()(1) <<", "<<  _cfg->move()(2) <<"]"<<std::endl;
    _particleLoaded->createLayers(_cfg->layer(), _cfg->move());
  } else  if (_cfg->move() != Eigen::Vector3d::Zero()) {
    std::cout<<"Moving particles with the shift: ["<< _cfg->move()(0) <<", "<< _cfg->move()(1) <<", "<< _cfg->move()(2) <<"]"<<std::endl;
    _particleLoaded->move(_cfg->move());
  }
  
    
  if (_cfg->copyRotate()(0)>1 or _cfg->copyRotate()(1)>1 or _cfg->copyRotate()(2)>1) {
    std::cout<<"Creating copyRotate-layers of particles along xyz-axis layers: ["<<  _cfg->copyRotate()(0) <<", "<<  _cfg->copyRotate()(1) <<", "<<  _cfg->copyRotate()(2) <<"]"<<std::endl;
    _particleLoaded->createCopyRotate(_cfg->copyRotate());
  } else if (_cfg->rotate() != Eigen::Vector3d::Zero()) {
    std::cout<<"Rotating particles with the angles: ["<< _cfg->rotate()(0) <<", "<< _cfg->rotate()(1) <<", "<< _cfg->rotate()(2) <<"]"<<std::endl;
    _particleLoaded->rotate(_cfg->rotate());
  }
  
  
  if (_cfg->cutPlusB().norm()!=0 ) {
    _particleLoaded->cutParticlesXYZ(_cfg->cutPlusB(), _cfg->cutPlus(), _cfg->cutCylZ());
  }
  
  if (_cfg->cutMinusB().norm()!=0 ) {
    _particleLoaded->cutParticlesXYZ(_cfg->cutMinusB(), _cfg->cutMinus(), _cfg->cutCylZ());
  }
  
  if (_cfg->markPlusB().norm()!=0 ) {
    _particleLoaded->markParticlesXYZ(_cfg->markPlusB(), _cfg->markPlus(),_cfg->clearType(), _cfg->markCylZ());
  }
  
  if (_cfg->markMinusB().norm()!=0 ) {
    _particleLoaded->markParticlesXYZ(_cfg->markMinusB(), _cfg->markMinus(),_cfg->clearType(), _cfg->markCylZ());
  }
  
  // We clear particle types only, if cut or mark functions are not enabled
    
  if (_cfg->clearType() >= 0 and _cfg->markPlusB().norm()==0 and _cfg->markMinusB().norm()==0 and _cfg->markCylZ()<=0.0) {
    std::cout<<"Setting type of particles: "<<_cfg->clearType()<<std::endl;
    _particleLoaded->type(_cfg->clearType());
  }

  if (_cfg->setDensity() >= 0 ) {
    std::cout<<"Setting density of particles: "<<_cfg->setDensity()<<std::endl;
    _particleLoaded->density(_cfg->setDensity());
  }

  if (_cfg->markCylZ()>0.0 and _cfg->markPlusB().norm()==0 and _cfg->markMinusB().norm()==0 ) {
    _particleLoaded->markParticlesCylZ(_cfg->markCylZ(), _cfg->clearType());
  }
  
  std::shared_ptr <exportclass> exp (new exportclass(_cfg, _particleLoaded));
  
  if (_cfg->Vtk())  {
    std::cout<<"Creating vtk-file "<<std::endl;
    exp->VTK();
  }
  
  if (_cfg->YADE())  {
    std::cout<<"Creating yade-file "<<std::endl;
    exp->YADE();
  }
  
  exp->LiggghtsIN();
};


void bulk::loadParticles() {
  std::ifstream _file;
  _file.open(_cfg->FNameI());
  
  std::string  line;
  int curLine = 1;
  std::vector <std::shared_ptr<particle> > tmpPartVector;
  bool breakLoading = false;
  unsigned long long pId=0;
  
  while(std::getline(_file, line) and not(breakLoading)) {  
    std::stringstream linestream(line);
    std::string data;
    
    if (linestream.rdbuf()->in_avail() != 0 and curLine>=_cfg->nDat()) {  //Check, whether the line is not empty
      int valInt;
      double valD;
      double pR=0.0, pD=2000.0;
      int pT=1;
      Eigen::Vector3d pC;
      if (curLine>=_cfg->nDat()) {
        for (int i=1; i<=_cfg->maxC(); i++) {
          if (i==_cfg->cId()) {
            linestream >> pId;
          } else if (i==_cfg->cT()) {
            linestream >> pT;
            //std::cerr<<pT<<std::endl;
          } else if (i==_cfg->cC()) {
            linestream >> pC[0];
            linestream >> pC[1];
            linestream >> pC[2];
            i+=2;
            //std::cerr<<pC<<std::endl<<std::endl;
          } else if (i==_cfg->cR()) {
            linestream >> pR;
            //std::cerr<<pR<<std::endl;
          } else if (i==_cfg->cD()) {
            linestream >> pD;
            //std::cerr<<pD<<std::endl;
          } else {
            linestream >> valD;
          }
        }
        if (_cfg->cId()<0) {
          pId++;
        }
        std::shared_ptr<particle> tmpParticle ( new particle (pId, pT, pR*_cfg->radFactor(), pD, pC));
        _particleLoaded->addP(tmpParticle);
      } else if (curLine == _cfg->nAt()) {
        linestream >> valInt;
        std::cout<<"Expected particles "<<valInt<<"; ";
      } 
    } else if (linestream.rdbuf()->in_avail()==0 and curLine>_cfg->nDat()+1) {  //Check, whether the line is not empty
      breakLoading = true;
    }
    curLine++;
  };
  std::cout<<_particleLoaded->size()<<" particles added"<<std::endl;
};
