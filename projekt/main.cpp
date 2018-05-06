#include <iostream>
#include "FleetStreet.h"

int main()
{
	srand(time(NULL));

	FleetStreet *simulation = new FleetStreet;
	simulation->startSimulation();
	delete simulation;

	return 0;
}