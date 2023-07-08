#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <thread>
#include <random>

using namespace std;

int my_sock;
struct sockaddr_in serv_addr;
char message[100];

int thread_Cnt = 0;
// timeWait();

int str_len;

void testfunc()
{
    printf("It is work\n");
    return;
}

void error(char *msg)
{
    //fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void * timeCount(void *a)
{
    int cnt2 = 0;
    while (1)
    {
        sleep(1); 
        cnt2 ++;

        if(cnt2 % 2 == 0)
            thread_Cnt = 1;
        else
            thread_Cnt = 0;
        //cout << "cnt : " << cnt2 << "\n";
    }

    close(my_sock); // 4번
    printf("close client\n");

    return NULL;
}

void * exeTestFunc(void *a)
{
    while (1)
    {
        char tempstr[50];
        const char *boot = "boot";

        scanf("%s", tempstr);
        sleep(1);

        if (!strcmp(tempstr, boot))
        //if(message[0] == '1')
        {
            printf("입력된 값: %s\n", tempstr);
            printf("성공한 예제\n");
            testfunc();
        }
        else
        {
            printf("실패한 예제 aaa \n");
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-5, 5);
    
    pthread_t t0;
    pthread_t t1;

    if (argc != 3)
    {
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

    //pthread_create(&t0, NULL, timeCount, NULL);
    
    int cnt = 0;

    while (1)
    {
        // connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        {
            str_len = read(my_sock, message, sizeof(message)); // 3번
            int randomNum = dis(gen);
            int userInput;

            cout << "Input number: ";
            cin >> userInput;

            if(userInput == -1)
                break;

            if (str_len == -1)
                printf("read error\n");
            
            // cout << cnt++ << "  서버에서 : " << message << "\n";
            // cout << "userInput: " << userInput << "\n";
            // cout << "thread_Cnt: " << thread_Cnt << "\n";
            // cout << "random : " << randomNum << "\n";

            int tempnum;
            if(message[0] == '1')
                tempnum = 1;
            else  
                tempnum = 0;
            
            int divNum = randomNum * tempnum;
        
            int result =  (userInput + randomNum) / divNum;
            
            cout << "result : " << result << "\n";
            cout << "normaly worked!!\n\n";
        }
    }
    close(my_sock); // 4번
    printf("close client\n");

    return 0;
}
