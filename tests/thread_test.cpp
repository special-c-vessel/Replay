#include <iostream>
#include <thread>

using std::thread;

using namespace std;


void func1() {
  for (int i = 0; i < 10; i++) {
    std::cout << "쓰레드 111 작동중! \n";
  }

  cout << "func1\n";

}

void func2() {
  for (int i = 0; i < 11; i++) {
    std::cout << "쓰레드 22222 작동중! \n";
  }
  
}

// void func3() {
//   for (int i = 0; i < 12; i++) {
//     std::cout << "쓰레드 3333333333 작동중! \n";
//   }
// }

// int func4()
// {
//   cout << "func4 \n";

//   return 0;
// }

int main() {
  thread t1(func1);
  thread t2(func2);
  // thread t3(func3);
  // thread t4(func4);
  // thread t5(func4);

  // func1();

  // std::thread::id childThreadID2222 =  t1.get_id();

  // cout << "aaaaaaaaaaaaaaaaa" << childThreadID2222 << "bbbbbbbbbb\n";
  // printf("%d", childThreadID2222);
  t1.join();
  t2.join();
  // t3.join();
  // t4.join();
  // t5.join();

  // cout << thread.get_id() << "\n";;
  
  return 0;
}