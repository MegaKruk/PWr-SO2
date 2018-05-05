#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <ncurses.h>
#include <vector>
#include "FleetStreet.h"

std::vector<int> clientsIDs;
std::vector<std::thread> clients;
std::thread clientCreator;
std::thread barber;
std::mutex waitingRoom[waitingRoomCapacity];// chairs in waiting room as mutexes
std::mutex barberChair;		 				// barber's chair as mutex
std::mutex myMutex; 						// mutex for keeping cout safe

FleetStreet::FleetStreet()
{
    stop = false;
    uniqueID = 0;
}

void FleetStreet::safePrint(std::string str)
{
	myMutex.lock(); 			// Locking cout, so that no other operations will intrupt
	std::cout << std::endl << str << std::endl;
	myMutex.unlock();
}

void FleetStreet::workStarted()
{
	while (!stop)
	{
		// barber is checking if waiting room is empty
		bool isEmpty = true;
		for(int i = 0; i < waitingRoomCapacity; i++)
		{
			if(waitingRoom[i].try_lock())
			{
				waitingRoom[i].unlock();
			}
			else isEmpty = false;
		}
		if(isEmpty)
		{
			// if so, then barber is trying to sleep
			if (barberChair.try_lock())
			{
				safePrint("Sweeney Todd started sleeping in his chair");
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				safePrint("Sweeney Todd stopped sleeping in his chair");
				barberChair.unlock();
			}
			else isEmpty = false;
		}
	}
}

void FleetStreet::butFirstSirIThinkAShave(int clientID)
{
	while(!stop)
	{
		//safePrint("client[" + std::to_string(clientID) + "] decided to have a shave");
		if(barberChair.try_lock())
		{
			safePrint("Sweeney Todd started shaving client[" + std::to_string(clientID) + "]");
			std::this_thread::sleep_for(std::chrono::milliseconds(1100));
			safePrint("Sweeney Todd finished shaving client[" + std::to_string(clientID) + "]");
			barberChair.unlock();
			/*******************delete later****************/
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		}
		else
		{ 
			int wrPointer = 0;
			while(wrPointer < waitingRoomCapacity)
			{
				if(waitingRoom[wrPointer].try_lock())
				{
					safePrint("client[" + std::to_string(clientID) + "] waiting in place " + std::to_string(wrPointer));
					break;
				}
				else 
				{
					wrPointer++;
					// if wrPointer == waitingRoomCapacity leave
				}
			}
			while(!stop) // replace with not dead
			{
				if(wrPointer == 0)
				{
					if(barberChair.try_lock())
					{
						waitingRoom[wrPointer].unlock();
						safePrint("Sweeney Todd started shaving client[" + std::to_string(clientID) + "]");
						std::this_thread::sleep_for(std::chrono::milliseconds(1100));
						safePrint("Sweeney Todd finished shaving client[" + std::to_string(clientID) + "]");
						barberChair.unlock();
						/*******************delete later****************/
						std::this_thread::sleep_for(std::chrono::milliseconds(2000));
						break;//and die
					}
					else std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
				else 
				{
					if(waitingRoom[wrPointer-1].try_lock())
					{
						waitingRoom[wrPointer].unlock();
						wrPointer--;
						safePrint("client[" + std::to_string(clientID) + "] hopped in place " + std::to_string(wrPointer));
					}
					else std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}
	}
}

void FleetStreet::arrive(int clientID)
{
	//noOfClients++;
	safePrint("client[" + std::to_string(clientID) + "] arrived");
	//safePrint("Number of clients in the area: " + std::to_string(noOfClients));

	int randDecision = (std::rand() % 100) + 1;
	if(randDecision < 50)
		butFirstSirIThinkAShave(clientID);
	else
		butFirstSirIThinkAShave(clientID);
}

void FleetStreet::createClients()
{
    while(!stop)
    {
        if(clients.size() < maxNoOfClients)
        {
            safePrint("Next client is coming to Fleet Street");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            clients.push_back(std::thread(&FleetStreet::arrive, this, uniqueID));
            clientsIDs.push_back(uniqueID);          
			uniqueID++;
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void FleetStreet::startSimulation()
{
    //initscr();
    //cbreak();
    //raw();
    //menuinit();

    // creating threads
    barber = std::thread(&FleetStreet::workStarted, this);
    clientCreator = std::thread(&FleetStreet::createClients, this);
    /*for(int i = 0; i < maxNoOfClients; ++i)
    {
        clients[i] = std::thread(&FleetStreet::arrive, this, i);
    }*/

    std::cin.get(); 	// pause main thread here, other threads still going
    stop = true;		// this breaks main loop

    // joining threads
    clientCreator.join();
    barber.join();
    for (int i = 0; i < clients.size(); ++i)
    {
        clients[i].join();
        //pop
        //myUI->safePrint("client[" + std::to_string(i) + "] left");
        //myUI->clientStatus("");
    }
    //endwin();
}

FleetStreet::~FleetStreet() {  }

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