#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
  int num1 = 5;
  string str3333 = "abcdefghijk";
  str3333 = "12345";
  
  str3333[13] = 'e';

  if(str3333 == "a")
  {
    cout << "str is a";
  }

  return 0;
}

