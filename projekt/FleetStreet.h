#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <ncurses.h>
#include <algorithm>

constexpr int maxNoOfClients = 3;
constexpr int waitingRoomCapacity = 3;

class FleetStreet
{
private:

public:
    bool stop;
    bool amIDead;
    int uniqueID;

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