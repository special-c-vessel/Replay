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

void testFunc222()
{
  int a = 111;
  int b = 222;

  int c = a + b;
}

int main()
{
  int num1 = 10;

  testFunc111();

  num1 = 222;

  testFunc222();
  
  int num2 = 20;
  int num3 = num1 + num2;

  long long int num4 = 111111;
  float ffffstatic = 112.2414;
  double vvvv = 525.124;
  bool isisis = true;

  float arrTest[50];
  long long int arrTest2[50][50];

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

  string str1111 = "hello world";
  str1111 = "faewfefe";
  cout << str1111 << "\n";

  string str2222 = "hello world";
  str2222 = "change value";
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

  num1 = 333;

  sssss.push_back("vector string test");

  string tempvectorStringTest = sssss[0];
  string compareStringTest = "asdfgzxcvbnxw";

  return 0;
}

