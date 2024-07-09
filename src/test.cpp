#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
class A{
  public:
  A();
};

class Ba : public A{
  public:
    Ba();
    Ba(int a, int b):A(){

    };
  private:
    int c;
    int d;
};
int main(){
  int a=3, b=5;
  Ba ba(a, b);
}
