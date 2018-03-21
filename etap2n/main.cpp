#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <ncurses.h>

constexpr int noOfPhils = 5; 
std::thread philosphers[noOfPhils]; 
std::mutex fork[noOfPhils]; 	//forks as mutexes
std::mutex cout_mutex; 			// mutex for keeping cout safe
int ate[5] = {0}; 				// Counting which philosopher ate how many times
bool stop = false;

void safePrint(std::string str)
{
    cout_mutex.lock(); 			// Locking cout, so that no other operations will intrupt
    std::cout << std::endl << str << std::endl; // old
    cout_mutex.unlock(); 
}

void menuInit()
{
	for(int i = 0; i < noOfPhils; i++)
    {
    	move(i, 0);
    	printw("Philosopher[%d] idling", i);
    	refresh();
    	move(i + 6, 0);
    	printw("Fork[%d] is free", i);
    	refresh();
    	//safePrint("Philosopher " + std::to_string(id) + " is thinking " + std::to_string(i/randThink) + " %");
	}
}

void think(int id)
{
	int randThink = (std::rand() % 30) + 30;
	float progressT = 0.0;
    //safePrint("Philosopher " + std::to_string(id) + " is thinking for " + std::to_string(randThink/10) + " seconds"); 
    for(int i = 1; i <= randThink; i++)
    {
    	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    	move(id, 0);
    	progressT = (100*i)/randThink;
    	clrtoeol();
    	printw("Philosopher[%d] is thinking:\t%.0f\tpercent done", id, progressT);
    	refresh();
    	//safePrint("Philosopher " + std::to_string(id) + " is thinking " + std::to_string(i/randThink) + " %");
	}
    //safePrint("Philosopher " + std::to_string(id) + " has stopped thinking");
}

// Philosopher[id] is trying to eat
bool eat(int id, int left, int right) 
{ 
	// Philosopher[id] trying to pick up left fork, keeps going until success
    while(1) if (fork[left].try_lock()) 
    { 
		//safePrint("Philosopher " + std::to_string(id) + " got the fork " + std::to_string(left));
		move(left + 6, 0);
		clrtoeol();
    	printw("Fork[%d] is taken", left);
    	refresh();
        if (fork[right].try_lock()) 
        {
        	move(right + 6, 0);
			clrtoeol();
	    	printw("Fork[%d] is taken", right);
	    	refresh();
            // Philosopher[id] trying to pick up right fork
            //safePrint("Philosopher " + std::to_string(id) + " got the fork " + std::to_string(right)); 
            int randEat = (std::rand() % 30) + 20;
			float progressE = 0.0;
		    //safePrint("Philosopher " + std::to_string(id) + " is thinking for " + std::to_string(randThink/10) + " seconds"); 
		    for(int i = 1; i <= randEat; i++)
		    {
		    	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		    	move(id, 0);
		    	progressE = (100*i)/randEat;
		    	clrtoeol();
		    	printw("Philosopher[%d] is eating:\t%.0f\tpercent done", id, progressE);
		    	refresh();
		    	//safePrint("Philosopher " + std::to_string(id) + " is thinking " + std::to_string(i/randThink) + " %");
			}
            //safePrint("Philosopher " + std::to_string(id) + " has stopped eating");
            return true;
        } 
        else 
        {
            // Drop first fork if can't pick up other to prevent deadlock and starvation
            fork[left].unlock();
            move(left + 6, 0);
			clrtoeol();
	    	printw("Fork[%d] is free", left);
	    	refresh();
        }
    }
}

void putDownForks(int left, int right) 
{
    fork[left].unlock(); 
    move(left + 6, 0);
	clrtoeol();
	printw("Fork[%d] is free", left);
	refresh();

    fork[right].unlock();
    move(right + 6, 0);
	clrtoeol();
	printw("Fork[%d] is free", right);
	refresh(); 
}

void dinner_started(int philID) 
{
    // Taking the min id fork first

	// int leftFork = philID;
	// int rightFork = (philID + 1) % (noOfPhils);
    int leftFork = std::min(philID, (philID + 1) % (noOfPhils));
    int rightFork = std::max(philID, (philID + 1) % (noOfPhils));

    while (!stop) 
    { 
    	// Philosopher[philID] is trying to eat
        if (eat(philID, leftFork, rightFork)) 
        { 
            putDownForks(leftFork, rightFork);
            ate[philID]++;	//counting how many times Philosopher[philID] ate
            think(philID);
        }
    }
}

int main() 
{
	srand(time(NULL));
	initscr();
	menuInit();
	raw();
/*
initscr();			
printw("Hello World !!!");	
refresh();			
getch();			
endwin();
*/
    // creating threads
    for (int i = 0; i < noOfPhils; ++i) 
    	philosphers[i] = std::thread(dinner_started, i);

    std::cin.get(); 	// pause main thread here, philosophers still going
	stop = true;		// this breaks philosophers' main loop

	// joining threads
	for (int i = 0; i < noOfPhils; ++i) 
		philosphers[i].join();

	// how many times philosophers ate
    //for (int i = 0; i < noOfPhils; ++i) 
    //	safePrint("Philosopher[" + std::to_string(i) + "] ate " + std::to_string(ate[i]) + " times");
    endwin();
}