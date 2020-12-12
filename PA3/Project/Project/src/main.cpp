/// \file
/// This is the main file for the project Traffic Simulation.
/// \author Yuzhong Shen (orginator)
/// \author Thomas Laverghetta
/// \version 2.0
/// \date 12/10/2020
///


#include <stdlib.h>
#include "TrafficSimulation.h"

/// Main function
int main(int argc, char** argv)
{	
	TrafficSimulation carSim;

	carSim.SetWindowPosition(100, 100);
	carSim.SetWindowSize(1300, 800);
	carSim.SetTitle("Traffic Simulation");

	carSim.InitializeSimulation(argc, argv);
	carSim.Run();
	system("pause");
}