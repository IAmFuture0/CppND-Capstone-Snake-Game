#include "snakebot.h"
#include <algorithm>
#include <cmath>
#include <memory>

// constructor
Snakebot::Snakebot(int grid_width, int grid_height):Snake(grid_width, grid_height), _grid_width(grid_width), _grid_height(grid_height){
    _head_x = 31;
    _head_y = 15;
    speed = 0.4f;
    size = 1;
}

// destructor
Snakebot::~Snakebot(){
    // thread barriers
    std::for_each(_snakebotThreads.begin(), _snakebotThreads.end(), [](std::thread &t){
        t.join();
    });
}

// simulate the snakebot and the traffic light at the intersections (multi-threading implementation)
void Snakebot::simulate(std::shared_ptr<bool> running){
    _snakebotThreads.emplace_back(std::thread(&Snakebot::Update, this, running));
    _snakebotThreads.emplace_back(std::thread(&Intersection::cycleThroughPhases, &intersection, running));
}

void Snakebot::Update(std::shared_ptr<bool> running){
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
        int cycleDuration = 100;
        
        // *** ISSUE : THIS while loop slow the ending process of the program *** //
        while(*running){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // compute time difference 
            long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
            
            if(timeSinceLastUpdate >= cycleDuration){
                
                SDL_Point prev_cell{static_cast<int>(_head_x), static_cast<int>(_head_y)};
                
                // whether halting position in front of the trafficlight
                std::vector<std::vector<int>> lightLoc = intersection.getLocation();
                if(std::find(lightLoc.begin(), lightLoc.end(), path[0]) != lightLoc.end()){
                    auto ftrEntryGranted = std::async(&Intersection::addSnakebotToQueue, &intersection);
                    ftrEntryGranted.get();
                }

                // update the snakebot's direction
                UpdateDirection();

                // update the head based on the its direction and speed
                UpdateHead();
                SDL_Point current_cell{static_cast<int>(_head_x), static_cast<int>(_head_y)};
                
                // Update all of the body vector items if the snake head has moved to a new cell.
                if(current_cell.x != prev_cell.x || current_cell.y != prev_cell.y){
                    UpdateBody(current_cell, prev_cell);
                }

                // reset stop watch for next cycle
  	            lastUpdate = std::chrono::system_clock::now();
            }    
        }
        return;
}

void Snakebot::UpdateDirection(){
    std::lock_guard<std::mutex> lock(_mutex);

    if(static_cast<int>(_head_x) == path[0][0] && static_cast<int>(_head_y) == path[0][1]){
        path.erase(path.begin());
    }

    // compute the difference between the place of current node and next target node
    int delta_x = path[0][0] - static_cast<int>(_head_x);
    int delta_y = path[0][1] - static_cast<int>(_head_y);

    // handling the corner case of direction when the snakebot go out of screen
    if(delta_x == -1 || delta_x==31){
        direction = Direction::kLeft;
    }else if(delta_x == 1 || delta_x == -31){
        direction = Direction::kRight;
    }else if(delta_y ==31 || delta_y == -1){
        direction = Direction::kUp;
    }else if(delta_y == -31 || delta_y == 1){
        direction = Direction::kDown;
    }
    return;
}

void Snakebot::UpdateHead(){
    std::lock_guard<std::mutex> lock(_mutex);

    switch (direction) {
        case Direction::kUp:
            _head_y -= speed;
            break;

        case Direction::kDown:
            _head_y += speed;
            break;
        case Direction::kLeft:
            _head_x -= speed;
            break;
        case Direction::kRight:
            _head_x += speed;
            break;
    }

  // Wrap the Snake around to the beginning if going off of the screen.
  _head_x = fmod(_head_x + _grid_width, _grid_width);
  _head_y = fmod(_head_y + _grid_height, _grid_height);
}


void Snakebot::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell){
    std::lock_guard<std::mutex> lock(_mutex);
    
    // Add previous head location to vector
    body.push_back(prev_head_cell);

    if (!growing) {
        // Remove the tail from the vector.
        body.erase(body.begin());
    } else {
        growing = false;
        size++;
  }
  
}

void Snakebot::GrowBody(){growing = true;}

// searching the route by A* search algorithm
void Snakebot::PlanRoute(SDL_Point food){
    std::lock_guard<std::mutex> lock(_mutex);

    path.clear();
    std::unique_ptr<Astar> astar_search(new Astar());
    
    // set the start node and end node
    Node<int>* start_node = astar_search->nodeGrid[static_cast<int>(_head_x)][static_cast<int>(_head_y)];
    astar_search->setStartNode(start_node);
    Node<int> * end_node = astar_search->nodeGrid[food.x][food.y];
    astar_search->setEndNode(end_node);
    astar_search->getStartNode()->visited = true;
    astar_search->getStartNode()->g_value = 0;
    astar_search->getStartNode()->h_value = astar_search->calculateHValue(astar_search->getStartNode());

    // add the start node into the open list
    astar_search->open_list.push_back(astar_search->getStartNode());
    
    Node<int>* currentnode = nullptr;
    while(astar_search->open_list.size()!=0){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // nextnode
        currentnode = astar_search->nextNode();
        
        // if goal is found
        if(currentnode == astar_search->getEndNode()){
            astar_search->constructFinalPath(currentnode);
            StorePath(astar_search->path_found);
            return;
        }else{
            // add neighbors of currentnode 
            astar_search->addNeighbors(currentnode);
        }
    }
    
}

// pass the route to snakebot 
void Snakebot::StorePath(std::vector<Node<int>*> path_found){
    for(auto node:path_found){
        std::vector<int> index{node->getX(), node->getY()};
        path.push_back(index);
    }
    return;
}

// constructor
Astar::Astar(){
    // assign new memory
    for(int i = 0; i<32; i++){
        for(int j = 0; j<32; j++){
            nodeGrid[i][j] = new Node<int>(i, j);
        }   
    }
}

// destructor
Astar::~Astar(){
    // prevent memory leak
    for(int i = 0; i<32; i++){
        for(int j = 0; j<32; j++){
            delete nodeGrid[i][j];
        }
    }
}

// define the calculation of euclidean distance as Heuristic function 
float Astar::calculateHValue(Node<int> *node){
    return pow((endnode->getX()-node->getX()), 2)+ pow((endnode->getY()-node->getY()), 2);
}

// sort the open_list based on the f value in descending order
Node<int>* Astar::nextNode(){
    sort(open_list.begin(), open_list.end(), [](const Node<int>* a, const Node<int>* b){return (a->h_value+a->g_value)>(b->h_value+b->g_value);});
    Node<int>* lowest_cost_node = open_list.back();
    open_list.pop_back();
    return lowest_cost_node;
}

// add unvisited neighbor nods of current node to the open list
void Astar::addNeighbors(Node<int>* currentnode){  
    for(int i = 0; i < 4; i++){
        int x = fmod(currentnode->getX() + delta[i][0] + 32, 32); // grid_width
        int y = fmod(currentnode->getY() + delta[i][1] + 32, 32); // grid_height
        if(nodeGrid[x][y]->visited == false){
            nodeGrid[x][y]->h_value = calculateHValue(nodeGrid[x][y]);
            nodeGrid[x][y]->g_value = currentnode->g_value + 1;
            nodeGrid[x][y]->parent = currentnode;
            nodeGrid[x][y]->visited = true;
            open_list.push_back(nodeGrid[x][y]);
        }
    }
    return;
}

// consturct the path based on the parent node setting in the searching procedure
void Astar::constructFinalPath(Node<int>* currentnode){
    while(currentnode != startnode){
        path_found.push_back(currentnode);
        currentnode = currentnode->parent;
    }
    std::reverse(path_found.begin(), path_found.end());
    return;
}


