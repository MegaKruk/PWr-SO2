//
// Created by megakruk on 03.05.18.
//

#ifndef DEMONBARBER_INTERFACE_H
#define DEMONBARBER_INTERFACE_H

#include <iostream>
#include <mutex>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <ncurses.h>

class Interface
{
private:

public:
    Interface();
    void safePrint(std::string);
    ~Interface();
};


#endif //DEMONBARBER_INTERFACE_H
