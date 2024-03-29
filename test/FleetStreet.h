#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <ncurses.h>

constexpr int maxNoOfClients = 3;
constexpr int waitingRoomCapacity = 3;

class FleetStreet
{
private:

public:
    bool stop;
    int uniqueID;

    FleetStreet();
    void safePrint(std::string);
    void workStarted();
    void butFirstSirIThinkAShave(int);
    void arrive(int);
    void startSimulation();
    void createClients();
    ~FleetStreet();
};