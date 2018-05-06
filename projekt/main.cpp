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

/*int main()	//old
{
	srand(time(NULL));
	//initscr();
	//cbreak(); //raw();
	//menuInit();

	// creating threads
	barber = std::thread(workStarted);
	clientCreator = std::thread(createClients);
	
	std::cin.get(); 	// pause main thread here, other threads still going
	stop = true;		// this breaks main loop

	// joining threads
	clientCreator.join();
	barber.join();
	for (int i = 0; i < clients.size(); ++i)
	{
		clients[i].join();
		safePrint("client[" + std::to_string(i) + "] left");
	}

	//endwin();
	return 0;
}*/