#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <random>
#include <string>
#include <thread>

using namespace std;

int my_sock;
struct sockaddr_in serv_addr;
int thread_Cnt = 0;
// timeWait();

int str_len;

int main(int argc, char *argv[]) {
  char message[100];
  
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(-50, 50);

  pthread_t t0;
  pthread_t t1;

  char tempstr[200] = "temp str value";

  if (argc != 3) {
    printf("%s <IP> <PORT>\n", argv[0]);
    exit(1);
  }
  my_sock = socket(PF_INET, SOCK_STREAM, 0); // 1번
  if (my_sock == -1)
    printf("socket error \n");
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  // pthread_create(&t0, NULL, timeCount, NULL);

  int cnt = 0;

  while (1) {
    // connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {

      str_len = read(my_sock, message, sizeof(message)); // 3번

      int randomNum = rand() % 99;

      if (str_len == -1)
        printf("read error\n");

      if(message[0] == '9')
        break;

      int tempnum;

      if (message[0] >= '2')
        tempnum = message[0] - '0';
      else
        tempnum = 0;

      int result = randomNum / tempnum;

      cout << "result : " << result << "\n";
    }
  }
  close(my_sock); // 4번
  printf("close client\n");

  return 0;
}