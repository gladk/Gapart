/*
    This file is part of Gapart.
    Gapart is the programm to modify particle-pack-files for DEM-simulations.
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

#include "config.h"
#include "particleRow.h"

class bulk {
  private:
    std::shared_ptr<configopt> _cfg;
    unsigned long long  _particleNum;
    std::shared_ptr <particleRow> _particleLoaded;
  public:
    bulk(std::shared_ptr<configopt>);
    void loadParticles();
};
