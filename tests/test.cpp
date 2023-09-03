#include <iostream>
// #include <string>

using namespace std;

int main()
{
  string tempstr = "11111";
  tempstr = "32222";

  tempstr[2] = 'A';

  cout << tempstr << "\n";

  if(tempstr[2] == 'B')
    tempstr[3] = 'B';
  else
    tempstr[5] = 'G';

int num1 = 44444;
  if(tempstr == "11111")
  {
    int num333 = 555555;
    tempstr[555] = 'd';
  }

tempstr[7] = 'Q';
  if(tempstr == "aaaba")
  {

  }

  int *tempnum1;
  tempnum1 = &num1;
}


/*
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"struct.std::__1::__compressed_pair_elem" }
%"struct.std::__1::__compressed_pair_elem" = type { %"struct.std::__1::basic_string<char>::__rep" }
%"struct.std::__1::basic_string<char>::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char>::__long" }
%"struct.std::__1::basic_string<char>::__long" = type { i8*, i64, i64 }
%"struct.std::__1::__default_init_tag" = type { i8 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::__compressed_pair_elem.0" = type { i8 }
%"class.std::__1::allocator" = type { i8 }
%"struct.std::__1::__non_trivial_if" = type { i8 }

%"struct.std::__1::basic_string<char>::__short" = type { [23 x i8], %struct.anon }
%struct.anon = type { i8 }

*/