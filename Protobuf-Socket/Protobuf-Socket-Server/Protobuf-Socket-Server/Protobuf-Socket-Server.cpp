#include <iostream>
#include <winsock.h>
#include <cstring>

#define SEPARATOR cout<<"============================================="<<endl
#define PORT 9900
using namespace std;

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;
int nClientArr[5];


void processNewMessage(int& nClientSocket)
{
    cout << "Processing new message from client ID: "<<nClientSocket << endl;
    char buf[256] = "";
    int nRet = recv(nClientSocket, buf, 256, 0);
    if (nRet < 0)
    {
        cout << "Error receiving message from client... Closing socket" << endl;
        closesocket(nClientSocket);
        for (int i = 0; i < 5; i++)
        {
            if (nClientArr[i] == nClientSocket)
            {
                nClientArr[i] = 0;
                break;
            }
        }
    }
    else
    {
        cout << "CLIENT: " << buf << endl;
        cout << "Appending 'Hello' to input... and sending..." << endl;
        string output = "Hello " + (string)buf;
        char* toClient = &output[0];
        send(nClientSocket, toClient, 256, 0);
        memset(buf, ' ', 256);
        SEPARATOR;
    }
}



void processNewRequest(int &nSocket)
{
    bool flag = FALSE;
    int cliIndex;

    //New connection request
    if (FD_ISSET(nSocket, &fr))
    {
        for (int i = 0; i < 5; i++)
        {
            if (nClientArr[i] == 0)
            {
                cout << "Accepting connection..." << endl;
                cliIndex = i;
                flag = TRUE;
                break;
            }
        }
        if (flag == FALSE)
        {
            cout << "Connection list full..." << endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        int nLen = sizeof(struct sockaddr);
        int nClientSocket = accept(nSocket, NULL, &nLen); //This is the listener socket
        if (nClientSocket > 0)
        {
            // Put it into the client fd_set  
            nClientArr[cliIndex] = nClientSocket;
            send(nClientSocket, "Connected Successfully...", 26, 0);
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (FD_ISSET(nClientArr[i], &fr))
            {
                //Get the new message from the client using recv and process it
                processNewMessage(nClientArr[i]);
            }
        }
    }


}




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
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*
        Address_Family =  AF_INET  This corresponds to the internetwork address families like UDP, TCP, etc.
        Socket_Type = SOCK_STREAM This corresponds to the TCP packet type. We can use SOCK_DGRAM for UDP sockets.
        Protocol = IPPROTO_TCP This corresponds to the protocol that we propose to use. Here we use TCP. For UDP, we have IPPROTO_UDP.
    */
    if (nSocket < 0)
    {
        cout << "Failed to open the socket... " << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Socket opened successfully... " << endl;
        cout << "Socket Descriptor ID: " << nSocket << endl;
    }

    //Initialise the environment for sockaddr structure
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY; //assign ip address of this machine to s_addr
    //srv.sin_addr.s_addr = inet_addr("127.0.0.1"); //another way to do the above, inet_addr converts string to byte order

    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

    //Setting Socket Options

    int nOptVal = 0;
    int nOptLen = sizeof(nOptVal);
    nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);

    if (nRet == 0)
    {
        cout << "Socket options successfully set... " << endl;
    }
    else
    {
        cout << "Failed to set socket options... " << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }



    //Bind the socket to the local port

    nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));

    if (nRet < 0)
    {
        cout << "Failed to bind socket to local port... " << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Socket successfully bound to local port... " << endl;
    }

    //Listen to requests from the client

    nRet = listen(nSocket, 5); //Second argument - backlog - specifies how many requests at a time the server can pull in the active queue
    //Here, if more than 5 clients come, they need to wait in the wait queue (maintained internally)    

    if (nRet < 0)
    {
        cout << "Failed to start lietening on local port... " << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Socket listening on local port " << ntohs(srv.sin_port) << "... " << endl;

    }

    nMaxFd = nSocket;
    struct timeval tv;
    tv.tv_sec = 1;//1 second
    tv.tv_usec = 0;//No nanoseconds

    SEPARATOR;
    while (true)
    {
        FD_ZERO(&fr);
        FD_ZERO(&fw);
        FD_ZERO(&fe);

        FD_SET(nSocket, &fr);
        FD_SET(nSocket, &fe);

        //Put newly accepted clients
        for (int i = 0; i < 5; i++)
        {
            if (nClientArr[i] != 0)
            {
                FD_SET(nClientArr[i], &fr);
                FD_SET(nClientArr[i], &fe);
            }
            
        }

        //Keep waiting for new requests, and proceed as per the request
        nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);

        if (nRet > 0)
        {
            //When someone connects with a dedicated connection
            //OR when an old client sends a message
            //cout <<"nSocket "<<FD_ISSET(nSocket, &fr) << endl;
            //cout <<"Client1 " << FD_ISSET(nClientArr[0], &fr) << endl;

            cout << "Data on port... Processing now..." << endl;
            
            //Process client data here
            processNewRequest(nSocket);
       
        }
        else if (nRet == 0)
        {
            // No connection or communication request mande
            //OR: None of the socket descriptors are ready
            
            
            
            //cout << "nSocket " << FD_ISSET(nSocket, &fr) << endl;
            //cout << "Client1 " << FD_ISSET(nClientArr[0], &fr) << endl;
            //cout << "Nothing on port " << ntohs(srv.sin_port) << endl;
        }
        else
        {
            //select call failed
            cout << "Select call failed... " << endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        //SEPARATOR;

    }



    return 0;
}
