#ifndef INSDL_DELAY
#define INSDL_DELAY

#include <iostream>
#include <chrono>
#include <thread>

namespace insdl {

/**
 * @brief Pauses the execution of the program for a specified number of milliseconds
 *
 * @param msec Number of milliseconds to delay
 */
void delayMs(int msec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

/**
 * @brief Pauses the execution of the program for a specified number of seconds
 * 
 * @param sec Number of seconds to delay
 */
void delaySec(int sec)
{
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

/**
 * @brief Pauses the execution of the program for a specified number of minutes
 * 
 * @param min Number of minutes to delay
 */
void delayMin(int min)
{
    std::this_thread::sleep_for(std::chrono::minutes(min));
}

/**
 * @brief Pauses the execution of the program for a specified number of hours
 * 
 * @param hour Number of hours to delay
 */
void delayHour(int hour)
{
    std::this_thread::sleep_for(std::chrono::hours(hour));
}

} // namespace insdl

#endif
