//
// Created by megakruk on 03.05.18.
//

#include "Interface.h"

std::mutex myMutex;

Interface::Interface() { /*safePrint("UI created");*/ }

void Interface::safePrint(std::string str)
{
    myMutex.lock(); 	// Locking cout, so that no other operations will intrupt
    std::cout << std::endl << str << std::endl;
    myMutex.unlock();
}

Interface::~Interface() { /*safePrint("UI deleted");*/ }