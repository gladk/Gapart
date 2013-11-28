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

#include "particleRow.h"
#include <iostream>
#include <Eigen/Geometry>

particleRow::particleRow() {
   _allPart.erase(_allPart.begin(),_allPart.end());
   _allPartUpd.erase(_allPartUpd.begin(),_allPartUpd.end());
   _sizeCalculate = false;
   _sizeCalculateUpd = false;
   _center = Eigen::Vector3d::Zero();
   _extends = Eigen::Vector3d::Zero();
   _centerUpd = Eigen::Vector3d::Zero();
   _extendsUpd = Eigen::Vector3d::Zero();
   _minP.resize(3);
   _maxP.resize(3);
   _minPUpd.resize(3);
   _maxPUpd.resize(3);
};

void particleRow::addP(std::shared_ptr<particle> part ) {
  _allPart.push_back(part);
  _sizeCalculate = false;
  if (not(std::find(_types.begin(), _types.end(), part->type()) != _types.end())) {
    _types.push_back(part->type());
  }
};

void particleRow::createUpdArray() {
  if (_allPart.size()>0) {
    _allPartUpd.erase(_allPartUpd.begin(),_allPartUpd.end());
    BOOST_FOREACH(std::shared_ptr<particle> p, _allPart) {
      std::shared_ptr<particle> tmpParticle ( new particle (p->id(), p->type(), p->rad(), p->density(), p->c()));
      _allPartUpd.push_back(tmpParticle);
    }
  }
  _sizeCalculateUpd = false;
};

std::shared_ptr<particle> particleRow::getPart(unsigned long long id) {
  if (id < _allPart.size()) {
    return _allPart[id];
  } else {
    std::cout << "Illegal access to vector element!\n"; 
    exit (EXIT_FAILURE);
  }
};

std::shared_ptr<particle> particleRow::getPartUpd(unsigned long long id) {
  if (id < _allPartUpd.size()) {
    return _allPartUpd[id];
  } else {
    std::cout << "Illegal access to vector element!\n"; 
    exit (EXIT_FAILURE);
  }
};

void particleRow::calculateSizeSrc() {
  calculatedSizes tmpSizes = calculateSize(_allPart);
  _center = tmpSizes.center;
  _extends = tmpSizes.extends;
  _minP = tmpSizes.minP;
  _maxP = tmpSizes.maxP;
  _sizeCalculate = true;
}

void particleRow::calculateSizeUpd() {
  calculatedSizes tmpSizes = calculateSize(_allPartUpd);
  _centerUpd = tmpSizes.center;
  _extendsUpd = tmpSizes.extends;
  _minPUpd = tmpSizes.minP;
  _maxPUpd = tmpSizes.maxP;
  _sizeCalculateUpd = true;
}
  
Eigen::Vector3d particleRow::center() {
  if (not (_sizeCalculate)) calculateSizeSrc();
  return _center;
}
  
Eigen::Vector3d particleRow::extends() {
  if (not (_sizeCalculate)) calculateSizeSrc();
  return _extends;
}

Eigen::Vector3d particleRow::centerUpd() {
  if (not (_sizeCalculateUpd)) calculateSizeUpd();
  return _centerUpd;
}
  
Eigen::Vector3d particleRow::extendsUpd() {
  if (not (_sizeCalculateUpd)) calculateSizeUpd();
  return _extendsUpd;
}

calculatedSizes particleRow::calculateSize(std::vector <std::shared_ptr<particle> > &  calcPart) {
  Eigen::Vector3d minS = calcPart[0]->c() - Eigen::Vector3d(calcPart[0]->rad(),calcPart[0]->rad(),calcPart[0]->rad());
  Eigen::Vector3d maxS = calcPart[0]->c() + Eigen::Vector3d(calcPart[0]->rad(),calcPart[0]->rad(),calcPart[0]->rad());
  std::vector <std::shared_ptr<particle> > minP; minP.resize(3);
  std::vector <std::shared_ptr<particle> > maxP; maxP.resize(3);
  
  for (unsigned short z=0; z<3; z++) {
    minP[z] = calcPart[0];
    maxP[z] = calcPart[0];
  }
  
  BOOST_FOREACH(std::shared_ptr<particle> p, calcPart) {
    Eigen::Vector3d minStmp = p->c() - Eigen::Vector3d(calcPart[0]->rad(),calcPart[0]->rad(),calcPart[0]->rad());
    Eigen::Vector3d maxStmp = p->c() + Eigen::Vector3d(calcPart[0]->rad(),calcPart[0]->rad(),calcPart[0]->rad());
    
    for (unsigned short z=0; z<3; z++) {
      if (minStmp[z] < minS[z]) {
        minS[z] = minStmp[z];
        minP[z] = p;
      };
      
      if (maxStmp[z] > maxS[z]) {
        maxS[z] = maxStmp[z];
        maxP[z] = p;
      };
    }
  }
 
 calculatedSizes tmpSizes;
 tmpSizes.extends = (maxS - minS)/2.0;
 tmpSizes.center = minS + tmpSizes.extends;
 tmpSizes.minP = minP;
 tmpSizes.maxP = maxP;
 return tmpSizes;
};

void particleRow::scale(double scaleFactor) {
  if (_allPart.size()>0) {
    BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
      p->c((p->c() - _center) * scaleFactor + _center);
      p->rad(p->rad()*scaleFactor);
    }
  }
  _sizeCalculateUpd = false;
};

void particleRow::move(Eigen::Vector3d shift) {
  if (_allPart.size()>0) {
    BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
      p->c(p->c() + shift);
    }
  }
  _sizeCalculateUpd = false;
};

void particleRow::rotate(Eigen::Vector3d angles) {
  Eigen::Quaterniond qx = Eigen::Quaterniond(Eigen::AngleAxisd(angles(0)/360.0*2.0*M_PI, Eigen::Vector3d::UnitX()));
  Eigen::Quaterniond qy = Eigen::Quaterniond(Eigen::AngleAxisd(angles(1)/360.0*2.0*M_PI, Eigen::Vector3d::UnitY()));
  Eigen::Quaterniond qz = Eigen::Quaterniond(Eigen::AngleAxisd(angles(2)/360.0*2.0*M_PI, Eigen::Vector3d::UnitZ()));
   
  if (_allPart.size()>0) {
    BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
      Eigen::Vector3d c = p->c();
      if (angles(0)!= 0.0) c = qx*c;
      if (angles(1)!= 0.0) c = qy*c;
      if (angles(2)!= 0.0) c = qz*c;
      p->c(c);
    }
  }
  _sizeCalculateUpd = false;
};

void particleRow::type(int setType) {
  if (_allPart.size()>0) {
    BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
      p->type(setType);
    }
  }
  _sizeCalculateUpd = false;
  _types.clear();
  _types.push_back(setType);
};

void particleRow::density(double density) {
  if (_allPart.size()>0) {
    BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
      p->density(density);
    }
  }
};

void particleRow::createLayers(Eigen::Vector3i numL, Eigen::Vector3d shiftL){
  for (unsigned short int d = 0; d < 3; d++) {
    if (numL(d) > 1) {
      calculatedSizes tmpSizes = calculateSize(_allPartUpd);
      std::vector <std::shared_ptr<particle> > allPartUpdNew;
      BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
        for (int z = 0; z < numL(d); z++) {
            Eigen::Vector3d newPosParticle;
            
            if (d==0) {
              newPosParticle = Eigen::Vector3d((tmpSizes.extends(d)*z*2 + shiftL(d)*z),0.0,0.0);
            } else if (d==1) {
              newPosParticle = Eigen::Vector3d(0.0,(tmpSizes.extends(d)*z*2 + shiftL(d)*z),0.0);
            } else if (d==2) {
              newPosParticle = Eigen::Vector3d(0.0,0.0,(tmpSizes.extends(d)*z*2 + shiftL(d)*z));
            }
            
            std::shared_ptr<particle> tmpParticle (new particle (p->id() + _allPartUpd.size()*z, p->type(), p->rad(), 
                          p->density(), p->c() + newPosParticle));

            
          allPartUpdNew.push_back(tmpParticle);
        }
      }
      _allPartUpd = allPartUpdNew;
    }
    _sizeCalculateUpd = false;
  }
};

void particleRow::createCopyRotate(Eigen::Vector3i copyRotate) {
  Eigen::Quaterniond q = Eigen::Quaterniond::Identity();
  for (unsigned short int d = 0; d < 3; d++) {
    if (copyRotate(d) > 1) {
      std::vector <std::shared_ptr<particle> > allPartUpdNew;
      BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
        for (int z = 0; z < copyRotate(d); z++) {
            Eigen::Vector3d newPosParticle;
            if (d==0) q = Eigen::Quaterniond(Eigen::AngleAxisd(2.0*M_PI/copyRotate(d)*z, Eigen::Vector3d::UnitX()));
            if (d==1) q = Eigen::Quaterniond(Eigen::AngleAxisd(2.0*M_PI/copyRotate(d)*z, Eigen::Vector3d::UnitY()));
            if (d==2) q = Eigen::Quaterniond(Eigen::AngleAxisd(2.0*M_PI/copyRotate(d)*z, Eigen::Vector3d::UnitZ()));
            newPosParticle = q*p->c();
            std::shared_ptr<particle> tmpParticle (new particle (p->id() + _allPartUpd.size()*z, p->type(), p->rad(), 
                          p->density(), newPosParticle));
            
          allPartUpdNew.push_back(tmpParticle);
        }
      }
      _allPartUpd = allPartUpdNew;
    }
    _sizeCalculateUpd = false;
  }
};

void particleRow::cutParticlesXYZ(Eigen::Vector3i cutBool, Eigen::Vector3d cutLength, double cutCylZ) {
  std::vector <std::shared_ptr<particle> > allPartUpdNew;
  unsigned long long tmpId = 0;
  for (unsigned short int d = 0; d < 3; d++) {
    if (cutBool(d) != 0.0) {
      BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
        if (cutBool(d)>0.0 and ((p->c()(d)) < cutLength(d)) 
          and ((cutCylZ>0.0 and not(inCylZ(p, cutCylZ))) or (cutCylZ<=0.0))
          ) {
          allPartUpdNew.push_back(p);
          _sizeCalculateUpd = false;
        } else if (cutBool(d)<0.0 and ((p->c()(d)) > cutLength(d))
          and ((cutCylZ>0.0 and not(inCylZ(p, cutCylZ))) or (cutCylZ<=0.0))
        ) {
          allPartUpdNew.push_back(p);
          _sizeCalculateUpd = false;
        } else {
          tmpId++;
        }
      }
    }
  }
  std::cout<<"Removed "<<tmpId<< " particles"<<std::endl;
  _allPartUpd = allPartUpdNew;
};

void particleRow::markParticlesXYZ(Eigen::Vector3i markBool, Eigen::Vector3d markLength, int setType, double markCylZ) {
  unsigned long long tmpId = 0;
  for (unsigned short int d = 0; d < 3; d++) {
    if (markBool(d) != 0.0) {
      BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
        if (markBool(d)>0.0 and ((p->c()(d)) >= markLength(d)) and inCylZ(p, markCylZ) ) {
          p->type(setType);
          _sizeCalculateUpd = false;
          tmpId++;
        } else if (markBool(d)<0.0 and ((p->c()(d)) <= markLength(d)) and inCylZ(p, markCylZ) ) {
          p->type(setType);
          _sizeCalculateUpd = false;
          tmpId++;
        }
      }
    }
  }
  std::cout<<"Marked "<<tmpId<< " particles"<<std::endl;
};

void particleRow::markParticlesCylZ(double markCylZ, int setType) {
  unsigned long long tmpId = 0;
  BOOST_FOREACH(std::shared_ptr<particle> p, _allPartUpd) {
    if (Eigen::Vector2d(p->c()(0),p->c()(1)).norm() <  markCylZ) {
      p->type(setType);
      _sizeCalculateUpd = false;
      tmpId++;
    }
  }
  std::cout<<"Marked "<<tmpId<< " particles"<<std::endl;
};

bool particleRow::inCylZ(std::shared_ptr<particle> p, double & markCylZ) {
  if ( (markCylZ>0.0 and (Eigen::Vector2d(p->c()(0),p->c()(1)).norm() <  markCylZ)) or
       (markCylZ<=0.0)) 
     {
       return true;
     } else {
       return false;
     }
};

std::vector <int> particleRow::types()  {
  return _types;
};

