#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <ncurses.h>
#include <algorithm>

//constexpr int maxNoOfClients = 8;
constexpr int waitingRoomCapacity = 3;
constexpr int bakeryCapacity = 4;

class FleetStreet
{
private:

public:
    bool stop, amIDead, amIFull;
    int myName, myName2, myPos, myPos2, meat, meatPies, uniqueID, money;

    std::vector<int> clientsIDs;
	std::vector<int> priorityList;
	std::vector<int> barberShopStatus;			// 0-barber's chair, 1-3 lounge
	std::vector<int> bakeryStatus;				// 0 serving pies, 1-3 bakery queue

    FleetStreet();
    void barberFunction();
    void bakerFunction();
    void butFirstSirIThinkAShave(int);
    void theWorstPiesInLondon(int);
    void arrive(int);
    void startSimulation();
    void createClients();
    void changeGUI();
    ~FleetStreet();
};