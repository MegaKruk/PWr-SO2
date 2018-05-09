#include "FleetStreet.h"

std::vector<int> clientsIDs;
std::vector<int> waitingRoomStatus;
std::vector<std::thread> clients;
std::thread clientCreator;
std::thread barber;
std::thread GUI;
std::mutex waitingRoom[waitingRoomCapacity];// chairs in waiting room as mutexes
std::mutex barberChair;		 				// barber's chair as mutex
std::mutex myMutex; 						// mutex for keeping cout safe
int myName;
int myPos;
//std::thread::id myPID;


FleetStreet::FleetStreet()
{
	stop = false;
	amIDead = false;
	uniqueID = 0;
	waitingRoomStatus.resize(waitingRoomCapacity);
	for(int i = 0; i < waitingRoomCapacity; i++)
	{
		waitingRoomStatus[i] = -1;
	}
}

void FleetStreet::safePrint(std::string str)	// useful for debugging
{
	myMutex.lock();
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
				int randWait1 = (std::rand() % 1) + 30;
				float progressT1 = 0.0;
				for (int i = 1; i <= randWait1; i++)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					progressT1 = (100 * i) / randWait1;
					myMutex.lock();
					move(4, 0);
					clrtoeol();
					printw("Sweeney Todd is sleeping in his chair\t\t%.0f\t%%", progressT1);
					refresh();
					myMutex.unlock();
				}
				barberChair.unlock();
			}
			else isEmpty = false;
		}
		if(barberChair.try_lock()) { barberChair.unlock(); }
		else
		{
			
			int randWait3 = (std::rand() % 1) + 20;
			float progressT3 = 0.0;
			for (int j = 1; j <= randWait3; j++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				progressT3 = (100 * j) / randWait3;
				myMutex.lock();
				move(4, 0);
				clrtoeol();
				printw("Sweeney Todd is shaving Client[%d]\t\t%.0f\t%%", myName, progressT3);
				refresh();
				myMutex.unlock();

			}
			//kill
			myMutex.lock();
			amIDead = true;
			clients[myPos].join();
			clients.erase(clients.begin() + myPos);
			clientsIDs.erase(clientsIDs.begin() + myPos);
			move(9, 0);
			clrtoeol();
			printw("Sweeney Todd has killed client[%d]", myName);
			refresh();
			myMutex.unlock();
			barberChair.unlock();
			/*******************delete later****************/
			//std::this_thread::sleep_for(std::chrono::milliseconds(234));
			//wrPointer = waitingRoomCapacity - 1;
		}
	}
}

void FleetStreet::butFirstSirIThinkAShave(int clientID)
{
	int wrPointer = waitingRoomCapacity - 1;
	while(!stop)
	{
		if(waitingRoom[wrPointer].try_lock())
		{
			myMutex.lock();
			waitingRoomStatus[wrPointer] = clientID;
			myMutex.unlock();
			break;
		}
		else 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			continue; // replace with leave
		}
	}
	while(!stop) // replace with not dead
	{
		if(wrPointer == 0)
		{
			if(barberChair.try_lock())
			{
				myMutex.lock();
				waitingRoomStatus[0] = -1;
				myName = clientID;
				myPos = std::find(clientsIDs.begin(), clientsIDs.end(), clientID) - clientsIDs.begin();
				//myPID = clients[myPos].get_id();
				myMutex.unlock();
				waitingRoom[0].unlock();
				while(!amIDead)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
				amIDead = false;
				break;
				//std::this_thread::sleep_for(std::chrono::milliseconds(4444));
				//wrPointer = waitingRoomCapacity - 1;
				/*myMutex.lock();
				waitingRoomStatus[0] = -1;
				myMutex.unlock();
				waitingRoom[0].unlock();
				int randWait3 = (std::rand() % 1) + 20;
				float progressT3 = 0.0;
				for (int j = 1; j <= randWait3; j++)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					progressT3 = (100 * j) / randWait3;
					myMutex.lock();
					move(4, 0);
					clrtoeol();
					printw("Sweeney Todd is shaving Client[%d]\t\t%.0f\t%%", clientID, progressT3);
					refresh();
					myMutex.unlock();
				}
				barberChair.unlock();
				/*******************delete later****************/
				//std::this_thread::sleep_for(std::chrono::milliseconds(234));
				//wrPointer = waitingRoomCapacity - 1;
				//die here
			}
			else std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		else 
		{
			if(waitingRoom[wrPointer-1].try_lock())
			{
				myMutex.lock();
				waitingRoomStatus[wrPointer] = -1;
				waitingRoomStatus[wrPointer-1] = clientID;
				myMutex.unlock();
				waitingRoom[wrPointer].unlock();
				wrPointer--;
			}
			else std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
}

void FleetStreet::arrive(int clientID)
{
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
			int randWait0 = (std::rand() % 1) + 7;
			float progressT0 = 0.0;
			for (int i = 1; i <= randWait0; i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				progressT0 = (100 * i) / randWait0;
				myMutex.lock();
				move(1, 0);
				clrtoeol();
				printw("Next Client is coming to Fleet Street\t\t%.0f\t%%", progressT0);
				refresh();
				myMutex.unlock();
			}
			myMutex.lock();
			clients.push_back(std::thread(&FleetStreet::arrive, this, uniqueID));
			clientsIDs.push_back(uniqueID);		  
			move(1, 0);
			clrtoeol();
			printw("Next Client is coming to Fleet Street\t\t%.0f\t%%", 0);
			refresh();
			myMutex.unlock();
			uniqueID++;
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void FleetStreet::changeGUI()
{
	while(!stop)
	{
		myMutex.lock();
		for (int i = 0; i < waitingRoomCapacity; i++)
		{
			move(i+5, 0);
			clrtoeol();
			if(waitingRoomStatus[i] == -1)
				printw("Lounge[%d]:\tempty", i);
			else
				printw("Lounge[%d]:\tClient[%d]", i, waitingRoomStatus[i]);
			refresh();
		}
		myMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void FleetStreet::startSimulation()
{
	initscr();
	cbreak();
	//raw();
	//menuinit();

	// creating threads
	barber = std::thread(&FleetStreet::workStarted, this);
	clientCreator = std::thread(&FleetStreet::createClients, this);
	GUI = std::thread(&FleetStreet::changeGUI, this);

	std::cin.get(); 	// pause main thread here, other threads still going
	stop = true;		// this breaks main loop

	// joining threads
	clientCreator.join();
	barber.join();
	GUI.join();
	for (int i = 0; i < clients.size(); ++i)
	{
		clients[i].join();
		// pop
	}
	endwin();
}

FleetStreet::~FleetStreet() {  }
