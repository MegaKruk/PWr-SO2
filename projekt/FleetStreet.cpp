#include "FleetStreet.h"

std::vector<int> clientsIDs;
std::vector<int> barberShopStatus;
std::vector<std::thread> clients;
std::thread clientCreator;
std::thread barber;
std::thread GUI;
std::mutex waitingRoom[waitingRoomCapacity];// chairs in waiting room as mutexes
std::mutex barberChair;		 				// barber's chair as mutex
std::mutex chute;							// chute as mutex
std::mutex myMutex; 						// mutex for keeping cout and some other operations safe
int myName;
int myPos;
int meat;
//std::thread::id myPID;
// 0 title, 1 client creator, 2 events, 4 title, 5 Sweeney, 6 Lovett, 8 title, 9 chair, 10-12 lounge, 14 title, 15 chute, 16-19 restaurant 

FleetStreet::FleetStreet()
{
	stop = false;
	amIDead = false;
	uniqueID = 0;
	meat = 0;
	barberShopStatus.resize(waitingRoomCapacity + 1);
	for(int i = 0; i < barberShopStatus.size(); i++)
	{
		barberShopStatus[i] = -1;
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
				myMutex.lock();
				barberShopStatus[0] = -2;
				myMutex.unlock();
				int randWait1 = (std::rand() % 1) + 30;
				float progressT1 = 0.0;
				for (int i = 1; i <= randWait1; i++)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					progressT1 = (100 * i) / randWait1;
					myMutex.lock();
					move(5, 0);
					clrtoeol();
					printw("Sweeney Todd is sleeping in his chair\t\t\t%.0f\t%%", progressT1);
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
				move(5, 0);
				clrtoeol();
				printw("Sweeney Todd is shaving Client[%d]\t\t\t%.0f\t%%", myName, progressT3);
				refresh();
				myMutex.unlock();
			}
			//kill
			myMutex.lock();
			amIDead = true;
			clients[myPos].join();
			clients.erase(clients.begin() + myPos);
			clientsIDs.erase(clientsIDs.begin() + myPos);
			move(2, 0);
			clrtoeol();
			printw("Events: Sweeney Todd has killed Client[%d]", myName);
			refresh();
			myMutex.unlock();

			int randWait4 = (std::rand() % 1) + 10;
			float progressT4 = 0.0;
			chute.lock();
			for (int j = 1; j <= randWait4; j++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				progressT4 = (100 * j) / randWait4;
				myMutex.lock();
				meat++;
				move(5, 0);
				clrtoeol();
				printw("Sweeney Todd is sending Client[%d] down the chute\t%.0f\t%%", myName, progressT4);
				refresh();
				myMutex.unlock();
			}
			myMutex.lock();
			barberShopStatus[0] = -1;
			myMutex.unlock();
			chute.unlock();
			barberChair.unlock();
		}
	}
}

void FleetStreet::butFirstSirIThinkAShave(int clientID)
{
	myMutex.lock();
	move(2, 0);
	clrtoeol();
	printw("Events: Client[%d] decided to have a shave", clientID);
	refresh();
	myMutex.unlock();
	int wrPointer = waitingRoomCapacity - 1;
	while(!stop)
	{
		if(waitingRoom[wrPointer].try_lock())
		{
			myMutex.lock();
			barberShopStatus[wrPointer + 1] = clientID;
			myMutex.unlock();
			break;
		}
		else 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			continue; // replace with leave
		}
	}
	while(!stop)
	{
		if(wrPointer == 0)
		{
			if(barberChair.try_lock())
			{
				myMutex.lock();
				barberShopStatus[1] = -1;
				barberShopStatus[0] = clientID;
				myName = clientID;
				myPos = std::find(clientsIDs.begin(), clientsIDs.end(), clientID) - clientsIDs.begin();
				myMutex.unlock();
				waitingRoom[0].unlock();
				while(!amIDead)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
				amIDead = false;
				break;
			}
			else std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		else 
		{
			if(waitingRoom[wrPointer-1].try_lock())
			{
				myMutex.lock();
				barberShopStatus[wrPointer + 1] = -1;
				barberShopStatus[wrPointer] = clientID;
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
				printw("Next Client is coming to Fleet Street\t\t\t%.0f\t%%", progressT0);
				refresh();
				myMutex.unlock();
			}
			myMutex.lock();
			clients.push_back(std::thread(&FleetStreet::arrive, this, uniqueID));
			clientsIDs.push_back(uniqueID);		  
			move(1, 0);
			clrtoeol();
			printw("Next Client is coming to Fleet Street\t\t\t%.0f\t%%", 0);
			refresh();
			myMutex.unlock();
			uniqueID++;
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void FleetStreet::changeGUI()
{
	while(!stop)
	{
		myMutex.lock();
		move(9, 0);
		clrtoeol();
		if(barberShopStatus[0] == -1)
			printw("Barber's chair:\t\t\tempty");
		else if(barberShopStatus[0] == -2)
			printw("Barber's chair:\t\t\tSweeney Todd");
		else
			printw("Barber's chair:\t\t\tClient[%d]", barberShopStatus[0]);

		for (int i = 1; i < barberShopStatus.size(); i++)
		{
			move(i+9, 0);
			clrtoeol();
			if(barberShopStatus[i] == -1)
				printw("Lounge[%d]:\t\t\tempty", i - 1);
			else
				printw("Lounge[%d]:\t\t\tClient[%d]", i - 1, barberShopStatus[i]);
			//refresh();
		}
		move(15, 0);
		clrtoeol();
		printw("Edible meat in the chute:\t%d\tkg", meat);
		refresh();
		myMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void FleetStreet::startSimulation()
{
	initscr();	// try putting in constructor
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
	endwin();	// try putting in destructor
}

FleetStreet::~FleetStreet() {  }
