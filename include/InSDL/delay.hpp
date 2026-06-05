#ifndef INSDL_DELAY
#define INSDL_DELAY

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace this_thread;
using namespace chrono;

/**
 * @brief Pauses the execution of the program for a specified number of milliseconds
 * 
 * @param msec Number of milliseconds to delay
 */
void delayms(int msec)
{
    sleep_for(milliseconds(msec));
}

/**
 * @brief Pauses the execution of the program for a specified number of seconds
 * 
 * @param sec Number of seconds to delay
 */
void delaysc(int sec)
{
    sleep_for(seconds(sec));
}

/**
 * @brief Pauses the execution of the program for a specified number of minutes
 * 
 * @param min Number of minutes to delay
 */
void delaymn(int min)
{
    sleep_for(minutes(min));
}

/**
 * @brief Pauses the execution of the program for a specified number of hours
 * 
 * @param hour Number of hours to delay
 */
void delayhr(int hour)
{
    sleep_for(hours(hour));
}

#endif