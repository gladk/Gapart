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

#include "main.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

int main(int ac, char* av[])
{ 
  std::cout<<"\n\
Gapart\n\
Copyright (C) 2013 TU Bergakademie Freiberg\nInstitute for Mechanics and Fluid Dynamics\n\
This program comes with ABSOLUTELY NO WARRANTY.\n\
"<<std::endl;
  
  string particlesFileNameI, particlesFileNameO;
  typeParticleFile curTypeParticleFile = liggghtsDump1;
  
  bool setVtk = false;
  bool setYADE = false;
  bool setLIGGGHTS = false;
  double setScale = 1.0;
  int setClearType = -1;
  double setDensity = -1;
  Eigen::Vector3d setMove = Eigen::Vector3d::Zero();
  Eigen::Vector3d setRotate = Eigen::Vector3d::Zero();
  Eigen::Vector3i setLayer = Eigen::Vector3i::Zero();
  Eigen::Vector3i setCopyRotate = Eigen::Vector3i::Zero();
  Eigen::Vector3d setCutPlus = Eigen::Vector3d::Zero();
  Eigen::Vector3d setCutMinus = Eigen::Vector3d::Zero();
  Eigen::Vector3i setCutPlusB = Eigen::Vector3i::Zero();
  Eigen::Vector3i setCutMinusB = Eigen::Vector3i::Zero();
  Eigen::Vector3d setMarkPlus = Eigen::Vector3d::Zero();
  Eigen::Vector3d setMarkMinus = Eigen::Vector3d::Zero();
  Eigen::Vector3i setMarkPlusB = Eigen::Vector3i::Zero();
  Eigen::Vector3i setMarkMinusB = Eigen::Vector3i::Zero();
  Eigen::Vector3i setMirror = Eigen::Vector3i::Zero();
  double setMarkCylZ = 0.0;
  double setCutCylZ = 0.0;
  
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("input,i", po::value<string>(), "input file name")
      ("output,o", po::value<string>(), "output file name")
      ("vtk,v", "create VTK-file, OFF by default")
      ("yade,y", "create YADE-file, OFF by default")
      ("liggghts,l", "create LIGGGHTS-file, OFF by default")
      ("type,t", po::value<string>()->default_value("liggghts-dump1"), "type of particle input file, liggghts-dump1 by default. Possible values: liggghts-dump1, liggghts-in1, utwente1")
      ("scale,s", po::value<double>()->default_value(1.0), "scale particles including distance from center of bulk")
      ("clear,c", po::value<int>()->default_value(-1), "clear types of particles, set them 1, if nothing more detected. This parameter is used to set types for mark-options. In this case particles will not be cleared")
      ("density,d", po::value<double>()->default_value(-1), "set density of all particles")
      ("moveX", po::value<double>()->default_value(0.0), "move particles along X-coord, ignored if layer* are defined")
      ("moveY", po::value<double>()->default_value(0.0), "move particles along Y-coord, ignored if layer* are defined")
      ("moveZ", po::value<double>()->default_value(0.0), "move particles along Z-coord, ignored if layer* are defined")
      ("rotateX", po::value<double>()->default_value(0.0), "rotate particles around X-coord [degrees], ignored if copyRotate* are defined")
      ("rotateY", po::value<double>()->default_value(0.0), "rotate particles around Y-coord [degrees], ignored if copyRotate* are defined")
      ("rotateZ", po::value<double>()->default_value(0.0), "rotate particles around Z-coord [degrees], ignored if copyRotate* are defined")
      ("layerX", po::value<int>()->default_value(1), "particle layers along X-axis, shift between layers can be specified in moveX")
      ("layerY", po::value<int>()->default_value(1), "particle layers along Y-axis, shift between layers can be specified in moveY")
      ("layerZ", po::value<int>()->default_value(1), "particle layers along Z-axis, shift between layers can be specified in moveZ")
      ("copyRotateX", po::value<int>()->default_value(1), "particle will be rotated and copied around X-axis")
      ("copyRotateY", po::value<int>()->default_value(1), "particle will be rotated and copied around Y-axis")
      ("copyRotateZ", po::value<int>()->default_value(1), "particle will be rotated and copied around Z-axis")
      ("cutX+", po::value<double>(), "cut particles by X-coordinate in positive direction, e.g. all particles, where Xi>X")
      ("cutY+", po::value<double>(), "cut particles by Y-coordinate in positive direction, e.g. all particles, where Yi>Y")
      ("cutZ+", po::value<double>(), "cut particles by Z-coordinate in positive direction, e.g. all particles, where Zi>Z")
      ("cutX-", po::value<double>(), "cut particles by X-coordinate in negative direction, e.g. all particles, where Xi<X")
      ("cutY-", po::value<double>(), "cut particles by Y-coordinate in negative direction, e.g. all particles, where Yi<Y")
      ("cutZ-", po::value<double>(), "cut particles by Z-coordinate in negative direction, e.g. all particles, where Zi<Z")
      ("markX+", po::value<double>(), "mark particles by X-coordinate in positive direction, e.g. all particles, where Xi>X")
      ("markY+", po::value<double>(), "mark particles by Y-coordinate in positive direction, e.g. all particles, where Yi>Y")
      ("markZ+", po::value<double>(), "mark particles by Z-coordinate in positive direction, e.g. all particles, where Zi>Z")
      ("markX-", po::value<double>(), "mark particles by X-coordinate in negative direction, e.g. all particles, where Xi<X")
      ("markY-", po::value<double>(), "mark particles by Y-coordinate in negative direction, e.g. all particles, where Yi<Y")
      ("markZ-", po::value<double>(), "mark particles by Z-coordinate in negative direction, e.g. all particles, where Zi<Z")
      ("markCylZ", po::value<double>(), "mark particles as a cylinder with given radius along Z-coordinate. This option can be combined with mark*- options")
      ("cutCylZ",  po::value<double>(), "cut particles as a cylinder with given radius along Z-coordinate. This option can be combined with cut*- options. If -c option is used, the only particles with given type will be affected by this operation")
      ("mirrorXY", "mirror particles about XY-plane")
      ("mirrorYZ", "mirror particles about YZ-plane")
      ("mirrorXZ", "mirror particles about Xz-plane")
      ("copyMirrorXY", "copy and mirror particles about XY-plane")
      ("copyMirrorYZ", "copy and mirror particles about YZ-plane")
      ("copyMirrorXZ", "copy and mirror particles about Xz-plane")
    ;
    
    po::positional_options_description p;
    p.add("input", -1);
    po::variables_map vm;        
    po::store(po::command_line_parser(ac, av).
    options(desc).positional(p).run(), vm);
    po::notify(vm);  
    
    if (vm.count("help")) {
      cout << desc << std::endl;
      return 0;
    }
    
    if (vm.count("vtk")) {
      cout << "VTK-file will be created" << std::endl;
      setVtk = true;
    } else {
      cout << "VTK-file will NOT be created" << std::endl;
      setVtk = false;
    }
    
    if (vm.count("yade")) {
      cout << "YADE-file will be created" << std::endl;
      setYADE = true;
    } else {
      cout << "YADE-file will NOT be created" << std::endl;
      setYADE = false;
    }
    
    if (vm.count("liggghts"))  {
      cout << "LIGGGHTS-file will be created" << std::endl;
      setLIGGGHTS = true;
    } else {
      cout << "LIGGGHTS-file will NOT be created" << std::endl;
      setLIGGGHTS = false;
    }
    
    if (vm.count("input")) {
      cout << "particles input file is: " << vm["input"].as<string>() << std::endl;
    } else {
      cout << "particles input file is required, use `-i` option for that or `--help` for help.\n"; 
      exit (EXIT_FAILURE);
    }

    if (vm.count("type")) {
      cout << "type of particle file is: " << vm["type"].as<string>() << std::endl;
      if (vm["type"].as<string>()=="liggghts-dump1") {
        curTypeParticleFile = liggghtsDump1;
      } else if (vm["type"].as<string>()=="liggghts-in1") {
        curTypeParticleFile = liggghtsIn1;
      } else if (vm["type"].as<string>()=="utwente1") {
        curTypeParticleFile = utwente1;
      } else {
        cout << "type of particle file is has unknown type, use `-t` option for that or `--help` for help.\n"; 
        exit (EXIT_FAILURE);
      }
    } else {
      cout << "the default type of particle file is set: liggghts-dump1"<< std::endl;
      curTypeParticleFile = liggghtsDump1;
    }
    
    particlesFileNameI = vm["input"].as<string>();
    if (vm.count("output")) {
      particlesFileNameO  = vm["output"].as<string>();
    } else {
      particlesFileNameO = particlesFileNameI;
    }
    
    if (vm.count("scale"))  {
      setScale = vm["scale"].as<double>();
      if (setScale<=0.0) {
        cout << "Scale cannot be 0 or less.\n"; 
        exit (EXIT_FAILURE);
      }
    } else {
      setScale = 1.0;
    }
    
    if (vm.count("moveX") or vm.count("moveY") or vm.count("moveZ"))  {      
      if (vm["moveX"].as<double>()!=0.0 or vm["moveY"].as<double>()!= 0.0 or vm["moveZ"].as<double>()!= 0.0 ) {
        setMove = Eigen::Vector3d(vm["moveX"].as<double>(), vm["moveY"].as<double>(), vm["moveZ"].as<double>());
        cout << "particles will be moved with the shift: ["<< setMove(0) <<", "<< setMove(1) <<", "<< setMove(2) <<"]"<<std::endl;
      }
    }
    
    if (vm.count("rotateX") or vm.count("rotateY") or vm.count("rotateZ"))  {      
      if (vm["rotateX"].as<double>()!=0.0 or vm["rotateY"].as<double>()!= 0.0 or vm["rotateZ"].as<double>()!= 0.0 ) {
        setRotate = Eigen::Vector3d(vm["rotateX"].as<double>(), vm["rotateY"].as<double>(), vm["rotateZ"].as<double>());
        cout << "particles will be rotated with the shift: ["<< setRotate(0) <<", "<< setRotate(1) <<", "<< setRotate(2) <<"]"<<std::endl;
      }
    }
    
    if ((vm["layerX"].as<int>()>1) or (vm["layerY"].as<int>()>1) or (vm["layerZ"].as<int>()>1)) {
      setLayer = Eigen::Vector3i(vm["layerX"].as<int>(), vm["layerY"].as<int>(), vm["layerZ"].as<int>());
      cout << "number of particle layers: ["<< setLayer(0) <<", "<< setLayer(1) <<", "<< setLayer(2) <<"]"<<std::endl;
    }
    
    if ((vm["copyRotateX"].as<int>()>1) or (vm["copyRotateY"].as<int>()>1) or (vm["copyRotateZ"].as<int>()>1)) {
      setCopyRotate = Eigen::Vector3i(vm["copyRotateX"].as<int>(), vm["copyRotateY"].as<int>(), vm["copyRotateZ"].as<int>());
      cout << "number of particle CopyRotate-layers: ["<< setCopyRotate(0) <<", "<< setCopyRotate(1) <<", "<< setCopyRotate(2) <<"]"<<std::endl;
    }
  
  
    if (vm.count("cutX+")) { setCutPlusB(0) = 1; setCutPlus(0) =  vm["cutX+"].as<double>(); cout<< "particles will be cutted in positive direction X: "<<setCutPlus(0)<<std::endl;}
    if (vm.count("cutY+")) { setCutPlusB(1) = 1; setCutPlus(1) =  vm["cutY+"].as<double>(); cout << "particles will be cutted in positive direction Y: "<<setCutPlus(1)<<std::endl;}
    if (vm.count("cutZ+")) { setCutPlusB(2) = 1; setCutPlus(2) =  vm["cutZ+"].as<double>(); cout << "particles will be cutted in positive direction Z: "<<setCutPlus(2)<<std::endl;}
    if (vm.count("cutX-")) { setCutMinusB(0) = -1; setCutMinus(0) =  vm["cutX-"].as<double>(); cout << "particles will be cutted in negative direction X: "<<setCutMinus(0)<<std::endl;}
    if (vm.count("cutY-")) { setCutMinusB(1) = -1; setCutMinus(1) =  vm["cutY-"].as<double>(); cout << "particles will be cutted in negative direction Y: "<<setCutMinus(1)<<std::endl;}
    if (vm.count("cutZ-")) { setCutMinusB(2) = -1; setCutMinus(2) =  vm["cutZ-"].as<double>(); cout << "particles will be cutted in negative direction Z: "<<setCutMinus(2)<<std::endl;}
    
  
    if (vm.count("markX+")) { setMarkPlusB(0) = 1; setMarkPlus(0) =  vm["markX+"].as<double>(); cout<< "particles will be marked in positive direction X: "<<setMarkPlus(0)<<std::endl;}
    if (vm.count("markY+")) { setMarkPlusB(1) = 1; setMarkPlus(1) =  vm["markY+"].as<double>(); cout << "particles will be marked in positive direction Y: "<<setMarkPlus(1)<<std::endl;}
    if (vm.count("markZ+")) { setMarkPlusB(2) = 1; setMarkPlus(2) =  vm["markZ+"].as<double>(); cout << "particles will be marked in positive direction Z: "<<setMarkPlus(2)<<std::endl;}
    if (vm.count("markX-")) { setMarkMinusB(0) = -1; setMarkMinus(0) =  vm["markX-"].as<double>(); cout << "particles will be marked in negative direction X: "<<setMarkMinus(0)<<std::endl;}
    if (vm.count("markY-")) { setMarkMinusB(1) = -1; setMarkMinus(1) =  vm["markY-"].as<double>(); cout << "particles will be marked in negative direction Y: "<<setMarkMinus(1)<<std::endl;}
    if (vm.count("markZ-")) { setMarkMinusB(2) = -1; setMarkMinus(2) =  vm["markZ-"].as<double>(); cout << "particles will be marked in negative direction Z: "<<setMarkMinus(2)<<std::endl;}
    
    
    if (vm.count("clear"))  {
      setClearType = vm["clear"].as<int>();
      if (setClearType>=0) {
        cout << "all particles will have type: "<< setClearType<<std::endl;
      }
    }
    
    if (vm.count("density"))  {
      setDensity = vm["density"].as<double>();
      if (setDensity>=0) {
        cout << "all particles will have density: "<< setDensity<<std::endl;
      }
    }

    if (vm.count("markCylZ"))  {
      cout << "particles will be marked as a cylinder with given radius "<< vm["markCylZ"].as<double>() << " along Z-coordinate" << std::endl;
      setMarkCylZ = vm["markCylZ"].as<double>();
    }

    if (vm.count("cutCylZ"))  {
      cout << "particles will be cutted as a cylinder with given radius "<< vm["cutCylZ"].as<double>() << " along Z-coordinate" << std::endl;
      setCutCylZ = vm["cutCylZ"].as<double>();
    }
    
    if (vm.count("mirrorXY"))  { cout << "particles will be mirrored about XY-plane" << std::endl; setMirror(0) = 1;}
    if (vm.count("mirrorYZ"))  { cout << "particles will be mirrored about YZ-plane" << std::endl; setMirror(1) = 1;}
    if (vm.count("mirrorXZ"))  { cout << "particles will be mirrored about ZZ-plane" << std::endl; setMirror(2) = 1;}
    
    if (vm.count("copyMirrorXY"))  { cout << "particles will be mirrored and copied about XY-plane" << std::endl; setMirror(0) = -1;}
    if (vm.count("copyMirrorYZ"))  { cout << "particles will be mirrored and copied about YZ-plane" << std::endl; setMirror(1) = -1;}
    if (vm.count("copyMirrorXZ"))  { cout << "particles will be mirrored and copied about ZZ-plane" << std::endl; setMirror(2) = -1;}
    
  }
  catch(exception& e) {
      cerr << "error: " << e.what() << std::endl;
      exit (EXIT_FAILURE);
  }
  catch(...) {
      cerr << "Exception of unknown type!\n";
  }
  

  //=====================================================
  std::vector< fs::path > filesParticle;
  
  fs::path particle_path( particlesFileNameI );
  fs::path particle_dir = particle_path.parent_path();
  fs::path particle_filesmask = particle_path.filename();
  
  if (particle_dir== "") {
    particle_dir = "./";
  }
  
  if (not(fs::is_directory(particle_dir))) {
    std::cerr<<"The Directory "<<particle_dir.string()<<" does not exists. Exiting."<<std::endl;
    exit (EXIT_FAILURE);
  } else {
    fs::directory_iterator end_itr; // Default ctor yields past-the-end
    const boost::regex my_filter( particle_filesmask.string() );
    for( boost::filesystem::directory_iterator i( particle_dir.string() ); i != end_itr; ++i )
    {
      // Skip if not a file
      if( !boost::filesystem::is_regular_file( i->status() ) ) continue;
      boost::smatch what;
      // Skip if no match
      if( !boost::regex_match(  i->path().filename().string(), what, my_filter ) ) continue;
      // File matches, store it
      if (fs::is_regular_file(i->path())) {
        filesParticle.push_back(i->path());
      }
    }
  }
  
  if (filesParticle.size()<1) {
    std::cerr<<"The file "<<particlesFileNameI<<" does not exists. Exiting."<<std::endl;
    exit (EXIT_FAILURE);
  }
  
  std::shared_ptr<configopt> configParams (new configopt(Eigen::Vector3d::Zero(), 
    4, 10, 1, 2, 4, 19, 27, 1.0, setVtk, setYADE, setLIGGGHTS, particlesFileNameI, 
    particlesFileNameO, setScale, setClearType, setMove, setRotate, setLayer, 
    setCopyRotate, setCutPlus, setCutMinus, setCutPlusB, setCutMinusB, setMarkPlus, 
    setMarkMinus, setMarkPlusB, setMarkMinusB, setMarkCylZ, setDensity, setCutCylZ,
    setMirror));
  
  if (curTypeParticleFile == liggghtsDump1) {
    configParams->changeLoadFieldsNumbs(Eigen::Vector3d::Zero(), 4, 10, 1, 2, 4, 19, 27, 1.0);
  } else if (curTypeParticleFile == liggghtsIn1) {
    configParams->changeLoadFieldsNumbs(Eigen::Vector3d::Zero(), -1, 12, 1, 2, 5, 3, 4, 0.5);
  } else if (curTypeParticleFile == utwente1) {
    configParams->changeLoadFieldsNumbs(Eigen::Vector3d::Zero(), -1, 15, -1, -1, 1, 7, -1, 1.0);
  } else {
    std::cerr<<"Unknown type of particle file."<<std::endl;
    exit (EXIT_FAILURE);
  }
  
  std::shared_ptr<bulk> bulkNew (new bulk(configParams));
  //=====================================================
  
  return 0;
}
