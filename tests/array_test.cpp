#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int main()
{
  float arrTest[50];
  long long int arrTest2[50][50];

  int ResetArr[50] = {11, 22, 33, 44, 55};

  arrTest[30] = 12.4f;
  arrTest2[25][25] = 55555;

  arrTest2[0][1] = 1;
  arrTest2[1][2] = 2;
  arrTest2[3][4] = 3;

  arrTest2[1][2] = 222;

  int arrTest1[100];
  
  return 0;
}