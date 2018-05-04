//
// Created by megakruk on 03.05.18.
//

#include "FleetStreet.h"
#include "Interface.h"

std::mutex waitingRoom[waitingRoomCapacity];    // chairs in waiting room as mutexes
std::mutex barberChair;		 				    // barber's chair as mutex

std::thread barber;
std::thread clients[maxNoOfClients];

Interface *myUI = new Interface;

FleetStreet::FleetStreet() { /*myUI->safePrint("Fleet street created");*/ }

void FleetStreet::workStarted()
{
    myUI->safePrint("Sweeney Todd started his work");
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
                myUI->safePrint("Sweeney Todd started sleeping in his chair");
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                myUI->safePrint("Sweeney Todd stopped sleeping in his chair");
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
            myUI->safePrint("Sweeney Todd started shaving client[" + std::to_string(clientID) + "]");
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
            myUI->safePrint("Sweeney Todd finished shaving client[" + std::to_string(clientID) + "]");
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
                    myUI->safePrint("client[" + std::to_string(clientID) + "] waiting in place " + std::to_string(i));
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
                        myUI->safePrint("Sweeney Todd started shaving client[" + std::to_string(clientID) + "]");
                        std::this_thread::sleep_for(std::chrono::milliseconds(1100));
                        myUI->safePrint("Sweeney Todd finished shaving client[" + std::to_string(clientID) + "]");
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
                        myUI->safePrint("client[" + std::to_string(clientID) + "] hopped in place " + std::to_string(i));
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
    myUI->safePrint("client[" + std::to_string(clientID) + "] arrived");
    //safePrint("Number of clients in the area: " + std::to_string(noOfClients));

    int randDecision = (std::rand() % 100) + 1;
    if(randDecision < 50)
        butFirstSirIThinkAShave(clientID);
    else
        butFirstSirIThinkAShave(clientID);
}

void FleetStreet::startSimulation()
{
    //initscr();
    //raw();
    //menmyUInit();

    // creating threads
    barber = std::thread(&FleetStreet::workStarted, this);
    for(int i = 0; i < maxNoOfClients; ++i)
    {
        clients[i] = std::thread(&FleetStreet::arrive, this, i);
    }

    std::cin.get(); 	// pause main thread here, other threads still going
    stop = true;		// this breaks main loop

    // joining threads
    barber.join();
    for (int i = 0; i < maxNoOfClients; ++i)
    {
        clients[i].join();
        //noOfClients--;
        myUI->safePrint("client[" + std::to_string(i) + "] left");
        //safePrint("Number of clients in the area: " + std::to_string(noOfClients));
    }
    //endwin();
    delete myUI;
}

FleetStreet::~FleetStreet() { /*myUI->safePrint("Fleet street deleted");*/ }