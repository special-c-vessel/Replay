#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

class B {
  public:
  int x;
  int y;
  B() : x(-99), y(-88) {}  
};

class AAAAA {
  public:
  int xxxxx;
  int yyyyy;
  string classString;
  B z;
  AAAAA() : xxxxx(99), yyyyy(-1) {}

  void inc_x(int zz111) {
    xxxxx += zz111;
  }

  void dec_x(int zz222) {
    xxxxx -= zz222;
  }

  void inc_y(int zz333) {
    yyyyy += zz333;
  }

  void dec_y(int zz444) {
    yyyyy -= zz444;
  }

  int* get_x_ptr() {
    return &(this->xxxxx);
  }
  int& get_x_ref() {
    return this->xxxxx;
  }
};

int main()
{
  vector<AAAAA> VA;

  //VA.push_back(x11111);
  //VA.push_back(x11111);
  //VA.push_back(x22222);

  vector<int> vvvvv;
  vector<long long int> vvvvv22;
  vector<float> fffff;
  vector<string> sssss;

  vvvvv.push_back(55555);

  vvvvv22.push_back(12345);
  vvvvv22.push_back(3456);

  vvvvv.push_back(66);
  vvvvv.push_back(77);

  vvvvv22.push_back(56789);

  fffff.push_back(111.24124);

  sssss.push_back("vector string test");

  return 0;
}