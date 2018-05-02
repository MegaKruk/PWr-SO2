#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <ncurses.h>

constexpr int maxNoOfClients = 3;
constexpr int waitingRoomCapacity = 3;
//constexpr int restaurantCapacity = 4;
//constexpr int noOfRazors = 3;
//int noOfClients = 0;

std::thread barber;
//std::thread baker;						// makes cookies, serves them and cleans bloodied razors
std::thread clients[maxNoOfClients];
//std::thread nature;						// creates client threads every few seconds
//std::mutex restaurant[restaurantCapacity];
//std::mutex razors[noOfRazors];			// razors as mutexes
std::mutex waitingRoom[waitingRoomCapacity];// chairs in waiting room as mutexes
std::mutex barberChair;		 				// barber's chair as mutex

std::mutex myMutex; 						// mutex for keeping cout safe
//bool isBloodied[noOfRazors] = { false }; 	// is razor i bloodied
bool stop = false;

void safePrint(std::string str)
{
	myMutex.lock(); 			// Locking cout, so that no other operations will intrupt
	std::cout << std::endl << str << std::endl;
	myMutex.unlock();
}

/*void menuInit()
{
	for (int i = 0; i < noOfRazors; i++)
	{
		myMutex.lock();
		move(i, 0);
		printw("Philosopher[%d] is idling", i);
		move(i + 6, 0);
		printw("razors[%d] is free", i);
		move(i + 12, 0);
		printw("Philosopher[%d] isBloodied 0 times ", i);
		refresh();
		myMutex.unlock();
		//safePrint("Philosopher " + std::to_string(id) + " is thinking " + std::to_string(i/randThink) + " %");
	}
}*/

/*void think(int id)
{
	int randThink = (std::rand() % 30) + 30;
	float progressT = 0.0;
	//safePrint("Philosopher " + std::to_string(id) + " is thinking for " + std::to_string(randThink/10) + " seconds"); 
	for (int i = 1; i <= randThink; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		progressT = (100 * i) / randThink;
		myMutex.lock();
		move(id, 0);
		clrtoeol();
		printw("Philosopher[%d] is thinking:\t%.0f\tpercent done", id, progressT);
		refresh();
		myMutex.unlock();
		//safePrint("Philosopher " + std::to_string(id) + " is thinking " + std::to_string(i/randThink) + " %");
	}
	
	myMutex.lock();
	move(id, 0);
	clrtoeol();
	printw("Philosopher[%d] is idling", id);
	refresh();
	myMutex.unlock();
	//safePrint("Philosopher " + std::to_string(id) + " has stopped thinking");
}*/

/*void putDownRazors(int left, int right)
{
	razors[left].unlock();
	myMutex.lock();
	move(left + 6, 0);
	clrtoeol();
	printw("razor[%d] is free", left);
	refresh();
	myMutex.unlock();

	razors[right].unlock();
	myMutex.lock();
	move(right + 6, 0);
	clrtoeol();
	printw("razor[%d] is free", right);
	refresh();
	myMutex.unlock();
}*/

void workStarted()
{
	// Taking the min id razors first

	// int leftrazors = philID;
	// int rightrazors = (philID + 1) % (noOfRazors);
	//int leftrazors = std::min(philID, (philID + 1) % (noOfRazors));
	//int rightrazors = std::max(philID, (philID + 1) % (noOfRazors));
	safePrint("Sweeney Todd started his work");
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

void butFirstSirIThinkAShave(int clientID)
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
			int i = 0;
			while(i < waitingRoomCapacity)
			{
				if(waitingRoom[i].try_lock())
				{
					safePrint("client[" + std::to_string(clientID) + "] waiting in place " + std::to_string(i));
					break;
				}
				else 
				{
					i++;
					// if i == waitingRoomCapacity leave
				}
			}
			while(!stop) // replace with not dead
			{
				if(i == 0)
				{
					if(barberChair.try_lock())
					{
						waitingRoom[i].unlock();
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
					if(waitingRoom[i-1].try_lock())
					{
						waitingRoom[i].unlock();
						i--;
						safePrint("client[" + std::to_string(clientID) + "] hopped in place " + std::to_string(i));
					}
					else std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}
		}
	}
}

void arrive(int clientID)
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

int main()
{
	srand(time(NULL));
	//initscr();
	//raw();
	//menuInit();

	// creating threads
	barber = std::thread(workStarted);
	for(int i = 0; i < maxNoOfClients; ++i)
	{
		clients[i] = std::thread(arrive, i);
	}
	
	std::cin.get(); 	// pause main thread here, other threads still going
	stop = true;		// this breaks main loop

	// joining threads
	barber.join();
	for (int i = 0; i < maxNoOfClients; ++i)
	{
		clients[i].join();
		//noOfClients--;
		safePrint("client[" + std::to_string(i) + "] left");
		//safePrint("Number of clients in the area: " + std::to_string(noOfClients));
	}

	//endwin();
	return 0;
}
