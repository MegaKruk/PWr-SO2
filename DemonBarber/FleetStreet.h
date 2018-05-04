//
// Created by megakruk on 03.05.18.
//

#ifndef DEMONBARBER_FLEETSTREET_H
#define DEMONBARBER_FLEETSTREET_H

#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>

constexpr int maxNoOfClients = 3;
constexpr int waitingRoomCapacity = 3;

class FleetStreet
{
private:

public:
    bool stop = false;

    FleetStreet();
    void workStarted();
    void butFirstSirIThinkAShave(int);
    void arrive(int);
    void startSimulation();
    ~FleetStreet();
};


#endif //DEMONBARBER_FLEETSTREET_H
