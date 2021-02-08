#include "server.h"
int main(int argc, char *argv[])
{
  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    write(1, "socket() failed", 16);
    exit(EXIT_FAILURE);
  }
  int opt = 1;
  if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
  {
    write(1, "setsockopt() failed", 20);
    exit(EXIT_FAILURE);
  }
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (bind(listenfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
  {
    write(1, "bind() failed", 14);
    exit(EXIT_FAILURE);
  }
  if(listen(listenfd, 9) < 0)
  {
    write(1, "listen() failed", 16);
    exit(EXIT_FAILURE);
  }
  while(1)
  {
        FD_ZERO(&readFDs);
        FD_SET(listenfd, &readFDs);
        int highestFileDescriptor = 9;
        for (int  i = 0 ; i < 9 ; i++)
        {
            if(clientSockets[i] > 0)
                FD_SET( clientSockets[i], &readFDs);

            if(clientSockets[i] > highestFileDescriptor)
                highestFileDescriptor = clientSockets[i];

        }
        activity = select( highestFileDescriptor+1, &readFDs, NULL , NULL , NULL);
        if ((activity < 0)  &&  (errno!=EINTR))
        {
            write(1, "select error", 13);
        }
        handleIncomingConnections();
        handleIncomingInformation();
  }
}
