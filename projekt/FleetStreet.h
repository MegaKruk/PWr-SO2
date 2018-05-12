#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <ncurses.h>
#include <algorithm>

constexpr int maxNoOfClients = 4;
constexpr int waitingRoomCapacity = 3;
constexpr int bakeryCapacity = 4;

class FleetStreet
{
private:

public:
    bool stop, amIDead;
    int myName, myPos, meat, meatPies, uniqueID;

    std::vector<int> clientsIDs;
	std::vector<int> priorityList;
	std::vector<int> barberShopStatus;			// 0-barber's chair, 1-3 lounge
	std::vector<int> bakeryStatus;				// 0 serving pies, 1-3 bakery queue

    FleetStreet();
    void safePrint(std::string);
    void barberFunction();
    void bakerFunction();
    void butFirstSirIThinkAShave(int);
    void arrive(int);
    void startSimulation();
    void createClients();
    void changeGUI();
    ~FleetStreet();
};