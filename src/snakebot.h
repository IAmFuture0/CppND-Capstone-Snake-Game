#ifndef SNAKEBOT_H
#define SNAKEBOT_H
#include <thread>
#include <vector>
#include "SDL.h"
#include "intersection.h"
#include <future>
#include <memory>
#include "snake.h"
#include <mutex>

template <typename T>

class Node{
    public:
        Node(int x, int y):_x(x),_y(y){};
        Node(){};
        int g_value;
        float h_value;
        bool visited{false};
        Node* parent;

        // getter and setter
        T getX(){return _x;};
        T getY(){return _y;};
        void setX(T &x){_x = x;};
        void setY(T &y){_y = y;};

        // operator
        bool operator==(const Node& other){
            return (this->_x == other._x) && (this->_y == other._y);
        }
        bool operator!=(const Node& other){
            return (this->_x != other._x) || (this->_y !=other._y);
        }
        
    private:
        T _x;
        T _y;
};

class Astar{
    public:
        Astar();
        ~Astar();
        std::vector<Node<int>*> open_list;
        std::vector<Node<int>*> path_found;
        Node<int>* nextNode();
        Node<int>* nodeGrid[32][32];

        // methonds
        float calculateHValue(Node<int>* node);
        void addNeighbors(Node<int>* currentnode);
        void constructFinalPath(Node<int>* currentnode);

        // getter / setter
        Node<int>* getStartNode(){return startnode;};    
        Node<int>* getEndNode(){return endnode;};
        void setStartNode(Node<int>* node){startnode = node;};
        void setEndNode(Node<int>* node){endnode = node;};

    private:
        const int delta[4][2]{{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
        Node<int>* startnode=nullptr;
        Node<int>* endnode=nullptr;
};

class Snakebot : public Snake{
    public:
        enum class Direction { kUp, kDown, kLeft, kRight };
        Snakebot(int grid_width, int grid_height);
        ~Snakebot();
        void simulate();
        void PlanRoute(SDL_Point food);
        void StorePath(std::vector<Node<int>*> path_found); 
        void Update();
        void GrowBody();

        std::vector<SDL_Point> body;
        std::vector<std::thread> _snakebotThreads;
        Intersection intersection;
        std::vector<std::vector<int>> getPath() const {return path;};
        float _head_x;
        float _head_y;

    private:
        std::vector<std::vector<int>> path;
        Direction direction=Direction::kUp;
        void UpdateDirection() override;
        void UpdateHead();
        void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
        int _grid_width;
        int _grid_height;
        bool growing{false};
        float speed;
        int size;
        std::mutex _mutex;
};

#endif