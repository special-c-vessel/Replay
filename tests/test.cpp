#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int globalllll = 55555;

void testFunc111()
{
  int a = 1;
  int b = 2;

  int c = a + b;
}

// void testFunc222()
// {
//   int aa = 1;
//   int bb = 2;

//   int cc = aa + bb;
// }

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

class QQQQQ {
  public:
  int a123 = 30;
  int b1123 = 40;
};

int main()
{
  VA.push_back(x11111);
  VA.push_back(x11111);
  VA.push_back(x22222);

  int * ptr1111 = x11111.get_x_ptr();
  int ptr2222 = x11111.get_x_ref();

  int num1 = 10;
  int num2 = 20;
  int num3 = num1 + num2;

  long long int num4 = 111111;
  float ffffstatic = 112.2414;
  double vvvv = 525.124;
  bool isisis = true;

  float arrTest[50];
  long long int arrTest2[50][50];

  int ResetArr[50] = {11, 22, 33, 44, 55};

  arrTest[30] = 12;
  arrTest2[25][25] = 55555;

  int arrTest1[100];

  int num333 = 5;
  int * ptrarrTest = &num333;
  int ** pptrarrTest = &ptrarrTest;
  int *** ppptrarrTest = &pptrarrTest;


  int kkk = 11111;
  **pptrarrTest = 22222;
  ***ppptrarrTest = 55555;

  string str1111 = "hello world\n";
  str1111 = "faewfefe";
  cout << str1111 << "\n";

  string str2222 = "hello world\n";
  str2222 = "change value\n";
  int lll = 123123;

  str2222[5] = 'e';

  vector<int> vvvvv;
  vector<long long int> vvvvv22;
  vector<float> fffff;
  vector<string> sssss;

  vvvvv.push_back(55555);
  vvvvv.push_back(66);
  vvvvv.push_back(77);

  vvvvv22.push_back(12345);
  vvvvv22.push_back(3456);
  vvvvv22.push_back(56789);


  fffff.push_back(111.24124);

  sssss.push_back("vector string test");

  string tempvectorStringTest = sssss[0];
  string compareStringTest = "asdfgzxcvbn\n\n\n";

  

  return 0;
}

