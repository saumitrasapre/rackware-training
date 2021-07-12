#include <iostream>
#include <winsock.h>

#define SEPARATOR cout<<"============================================="<<endl
#define PORT 9900   
using namespace std;

struct sockaddr_in srv;

int main()
{
    int nRet = 0;
    //Initialise the WSA variables (for windows)

    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << "WSA Failed to initialise... " << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "WSA initialised... " << endl;
    }
    
    //Initialising the socket
    int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*
        Address_Family =  AF_INET  This corresponds to the internetwork address families like UDP, TCP, etc.
        Socket_Type = SOCK_STREAM This corresponds to the TCP packet type. We can use SOCK_DGRAM for UDP sockets.
        Protocol = IPPROTO_TCP This corresponds to the protocol that we propose to use. Here we use TCP. For UDP, we have IPPROTO_UDP.
    */
    if (nClientSocket < 0)
    {
        cout << "Failed to open the socket... " << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Socket opened successfully... " << endl;
        cout << "Socket Descriptor ID: " << nClientSocket << endl;
    }

    //Initialise the environment for sockaddr structure
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1"); //assign ip address of this machine to s_addr
    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));


    //Connect to server

    nRet = connect(nClientSocket, (const sockaddr*)&srv, sizeof(srv));

    if (nRet < 0)
    {
        cout << "Failed to connect to the server..." << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        //Connected to Server

        char buff[256]  ="";
        recv(nClientSocket, buff, sizeof(buff), 0);
        cout << "SERVER: "<<buff << endl;

        //Now send messages to server
        while (TRUE)
        {
            cout << "Enter message: ";
            fgets(buff, 256, stdin);
            send(nClientSocket, buff, sizeof(buff), 0);
            memset(buff, ' ', 256);
            recv(nClientSocket, buff, sizeof(buff), 0);
            cout << "SERVER: " << buff << endl;

        }
    }




    return 0;
}


