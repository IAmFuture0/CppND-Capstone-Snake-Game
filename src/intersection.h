#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "SDL.h"
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <iostream>

class Snakebot;

enum TrafficLightPhase{
  red,
  green,
};

class Intersection{
  public:
    // Traffic Light
    void waitForGreen();
     
    bool TrafficLightIsGreen() const;
    
    // Intersection
    Intersection();
    ~Intersection();
    void cycleThroughPhases();
    void addSnakebotToQueue();
    
    std::vector<std::vector<int>> getLocation() const{return intersectionLocation;}
    TrafficLightPhase getPhase() const {return _currentPhase;}

  private:
    TrafficLightPhase _currentPhase; 
    std::vector<std::thread> _intersectionThreads;
    const std::vector<std::vector<int>> intersectionLocation{{7, 7}, {7, 15}, {7, 23}, {15, 7}, {15, 15}, {15, 23}, {23, 7}, {23, 15}, {23, 23}};
};

#endif