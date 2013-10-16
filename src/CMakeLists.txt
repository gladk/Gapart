#===========================================================
ADD_EXECUTABLE(gapart main.cpp particle.cpp particleRow.cpp config.cpp bulk.cpp export.cpp)
TARGET_LINK_LIBRARIES(gapart ${Boost_LIBRARIES} vtkHybrid)
INSTALL(TARGETS gapart DESTINATION ${CMAKE_INSTALL_BINDIR})
#===========================================================
