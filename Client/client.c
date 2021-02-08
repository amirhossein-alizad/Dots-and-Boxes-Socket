#include "game.h"

int main(int argc, char *argv[])
{
    siginterrupt(SIGALRM, 1);
    fd_set readFDs;
    int broadcastFD;
    struct sockaddr_in addressOfSocket;
    if( (broadcastFD = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        write(2, "fail to open socket\n", 21);
        return 0 ;
    }
    addressOfSocket.sin_family = AF_INET;
    addressOfSocket.sin_port = htons(atoi(argv[1]));
    addressOfSocket.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(broadcastFD, (struct sockaddr *) &addressOfSocket, sizeof(addressOfSocket)) < 0)
    {
      write(1, "failed to connect", 18);
      exit(EXIT_FAILURE);
    }
    int port;
    char in[250];
    recv(broadcastFD, in, sizeof(in), 0);
    write(1, in, strlen(in));
    char gp;
    write(1, "enter group:\n", 14);
    read(0, &gp, 1);
    int group = atoi(&gp);
    char buff2[250];
    send(broadcastFD, &gp, 1, 0);

    while(1)
    {
      recv(broadcastFD, buff2, sizeof(buff2), 0);
      if(atoi(buff2) != 0)
        break;
      write(1, buff2, strlen(buff2));
    }
    char inp[250];
    read(0, inp, sizeof(inp));
    write(1, "PORT:", 6);
    write(1, buff2, strlen(buff2));
    write(1,"\n", 1);

    port = atoi(buff2);
    char cturn;
    int turn;
    recv(broadcastFD, &cturn, sizeof(cturn), 0);
    turn = atoi(&cturn);
    int gamefd;
    struct sockaddr_in game_addr;
    game_addr.sin_family = AF_INET;
    game_addr.sin_port = htons(port);
    game_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    gamefd = socket(AF_INET, SOCK_DGRAM, 0);
    if(gamefd < 0)
    {
      write(1,"SOCKET FAILED\n",15);
      exit(EXIT_FAILURE);
    }
    int opt = 1, broadcast = 1, opt2 = 1, opt3 = 1;
    setsockopt(gamefd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(gamefd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    setsockopt(gamefd, SOL_SOCKET, SO_REUSEADDR, &opt2, sizeof(opt2));
    if(bind(gamefd, (struct sockaddr *) &game_addr, sizeof(game_addr)) < 0)
    {
      write(1,"BIND FAILED\n",13);
      exit(EXIT_FAILURE);
    }

    int game_turn = 0;
    game_turn = 0;
    int flag = 0;
    int count = 0;
    init();
    while(1)
    {
      char buff[1024], tempbuff[1024];
      memset(buff, 0, sizeof(buff));
      if(game_turn == turn)
      {
        signal(SIGALRM, sigHandler);
        alarm(20);
        write(1, "YOUR TURN\n", 11);
        read(0, buff, 1024);
        alarm(0);
        if(sendto(gamefd, buff, strlen(buff), 0, (struct sockaddr *) &game_addr, sizeof(game_addr)) != strlen(buff)){write(1, "failed to send\n", 15);}
        recv(gamefd, buff, sizeof(buff), 0);
        buff[5] = '\0';
      }
      else
      {
         write(1, "NOT YOUR TURN\n", 15);
         int n;
         if((n = recv(gamefd, buff, sizeof(buff), 0)) < 0)
         {
           write(1, "failed to receive\n", 18);
         }
         buff[5] = '\0';
      }
      memset(tempbuff, 0, sizeof(tempbuff));
      strncpy(tempbuff, buff, 1024);
      shapes(tempbuff);
      flag = name(tempbuff, &game_turn, turn, group);
      newmap(group + 1);
      if(game_turn < group -1)
      {
        if (flag == 0)
        {
          game_turn++;
        }
      }
      else
      {
        if (flag == 0)
        {
          game_turn = 0;
        }
      }
      winner(group);
    }
}
