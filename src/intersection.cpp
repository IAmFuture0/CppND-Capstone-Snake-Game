#include "intersection.h"
void Intersection::waitForGreen(){
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if(getPhase() == TrafficLightPhase::green) return;
    }
}
Intersection::Intersection(){
    _currentPhase = TrafficLightPhase::red;
}

Intersection::~Intersection(){
    for_each(_intersectionThreads.begin(), _intersectionThreads.end(), [](std::thread &t){
        t.join();
    });
}

bool Intersection::TrafficLightIsGreen() const{
    
    if(getPhase() == TrafficLightPhase::green){
        return true;
    }else{
        return false;
    }
}

void Intersection::cycleThroughPhases(){
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    int cycleDuration = 1500;
    // *** ISSUE : THIS while loop slow the ending process of the program *** //
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if(timeSinceLastUpdate >= cycleDuration){
            if(_currentPhase == TrafficLightPhase::red) _currentPhase = TrafficLightPhase::green;
            else if(_currentPhase == TrafficLightPhase::green) _currentPhase = TrafficLightPhase::red;
            
            // reset stop watch for next cycle
  	        lastUpdate = std::chrono::system_clock::now();
        }
    }
    return;
}

void Intersection::addSnakebotToQueue(){
    if(!TrafficLightIsGreen()){
        waitForGreen();
    }
    return;
}