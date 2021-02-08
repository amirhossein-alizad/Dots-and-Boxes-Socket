#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define SERVER_PORT 8000
#define SERVER_ADDR "127.0.0.1"

int clientSockets[9] = {0};
fd_set readFDs;
struct sockaddr_in serverAddr;
int listenfd;
int activity;
int two_players[2] = {-1}, three_players[3] = {-1}, four_players[4] = {-1};
int len2 = 0, len3 = 0, len4 = 0;
int port_count = 1;

int rand_p()
{
  srand(time(0));
  return (rand() % 1000) + 9000;
}

static char *itoa_simple_helper(char *dest, int i) {
  if (i <= -10) {
    dest = itoa_simple_helper(dest, i/10);
  }
  *dest++ = '0' - i%10;
  return dest;
}

char *itoa_simple(char *dest, int i) {
  char *s = dest;
  if (i < 0) {
    *s++ = '-';
  } else {
    i = -i;
  }
  *itoa_simple_helper(s, i) = '\0';
  return dest;
}
void handleIncomingConnections()
{
    int newSocket;
    int addrlen;
    struct sockaddr_in address;
	  memset(&address, 0, sizeof(address));
    addrlen = sizeof(address);
    if (FD_ISSET(listenfd, &readFDs))
    {
        if ((newSocket= accept(listenfd,
                (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            write(1, "accept", 7);
            exit(EXIT_FAILURE);
        }
        char* addr = inet_ntoa(address.sin_addr);
        char port[256]; itoa_simple(port, ntohs(address.sin_port));
        char*msg = "$YOU ARE NOW CONNECTED\n";
        write(1, "New Connection, address :", 26);
        write(1,"127.0.0.1:", 11);
        write(1, ":", 1);
        write(1, port, strlen(port));
        write(1, "\n", 1);
        send(newSocket, msg, 24, 0);
        write(1, "Connected message sent to client. \n", 36);
        int i;
        for (i = 0; i < 9; i++)
        {
            if( clientSockets[i] == 0 )
            {
                clientSockets[i] = newSocket;
                write(1, "Adding to list of sockets as ", 30);
                char msg[250]; itoa_simple(msg, i);
                write(1, msg, strlen(msg));
                write(1, "\n" , 1);
                break;
            }
        }
    }
}

void handleIncomingInformation()
{
  for (int i = 0; i < 9; i++)
  {
    if (FD_ISSET( clientSockets[i] , &readFDs))
    {
      char gp;
      recv(clientSockets[i], &gp, 1, 0);
      if (gp == '2')
      {
          write(1, "SOCKET NUMBER ", 15);
          char msg[250]; itoa_simple(msg, i);
          write(1, msg, strlen(msg));
          write(1, "WANTS TO PLAY IN A GROUP OF 2\n", 31);
          two_players[len2] = clientSockets[i];
          if (len2 == 1)
          {
            char* x = malloc(sizeof(char));
            itoa_simple(x, (rand_p()));
            send(two_players[0], x, sizeof(x), 0);
            send(two_players[1], x, sizeof(x), 0);
            send(two_players[0], "0", 1, 0);
            send(two_players[1], "1", 1, 0);
            two_players[0] = -1;
            two_players[1] = -1;
            len2 = 0;
          }
          else
          {
            send(two_players[0], "wait for one more player!\n", 28, 0);
            len2++;
          }
      }
      else if(gp == '3')
      {
        write(1, "SOCKET NUMBER ", 15);
        char msg[250]; itoa_simple(msg, i);
        write(1, msg, strlen(msg));
        write(1, "WANTS TO PLAY IN A GROUP OF 3\n", 31);
        three_players[len3] = clientSockets[i];
        if (len3 == 1)
        {

          send(three_players[0], "wait for one more player!\n", 28, 0);
          send(three_players[1], "wait for one more player!\n", 28, 0);
          len3++;
        }
        else if (len3 == 2)
        {
          char * x = malloc(sizeof(char));
          itoa_simple(x, (rand_p()));
          send(three_players[0], x, sizeof(x), 0);
          send(three_players[1], x, sizeof(x), 0);
          send(three_players[2], x, sizeof(x), 0);
          send(three_players[0], "0", 1, 0);
          send(three_players[1], "1", 1, 0);
          send(three_players[2], "2", 1, 0);
          three_players[0] = -1;
          three_players[2] = -1;
          three_players[2] = -1;
          len3 = 0;
        }
        else
        {
          send(three_players[0], "wait for two more players!\n", 28, 0);
          len3++;
        }
      }
      else
      {
        write(1, "SOCKET NUMBER ", 15);
        char msg[250]; itoa_simple(msg, i);
        write(1, msg, strlen(msg));
        write(1, "WANTS TO PLAY IN A GROUP OF 4\n", 31);
        four_players[len4] = clientSockets[i];
        if (len4 == 1)
        {

          send(four_players[0], "wait for two more players!\n", 28, 0);
          send(four_players[1], "wait for two more players!\n", 28, 0);
          len4 += 1;
        }
        else if (len4 == 2)
        {
          send(four_players[0], "wait for one more player!\n", 28, 0);
          send(four_players[1], "wait for one more player!\n", 28, 0);
          send(four_players[2], "wait for one more player!\n", 28, 0);
          len4 += 1;
        }
        else if(len4 == 3)
        {
          char * x = malloc(sizeof(char));
          itoa_simple(x, (rand_p()));
          send(four_players[0], x, sizeof(x), 0);
          send(four_players[1], x, sizeof(x), 0);
          send(four_players[2], x, sizeof(x), 0);
          send(four_players[3], x, sizeof(x), 0);
          send(four_players[0], "0", 1, 0);
          send(four_players[1], "1", 1, 0);
          send(four_players[2], "2", 1, 0);
          send(four_players[3], "3", 1, 0);
          four_players[0] = -1;
          four_players[1] = -1;
          four_players[2] = -1;
          four_players[3] = -1;
          len4 = 0;
        }
        else
        {
          send(four_players[0], "wait for three more players!\n", 31, 0);
          len4 += 1;
        }
      }
      clientSockets[i] = 0;
    }
  }
}
