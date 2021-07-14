#include <iostream>
#include <string>
#include <stdio.h>
#include "Phone.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

#define PORT 12345
#define SEPARATOR cout<<"============================================="<<endl

using namespace std;


class Server
{
    private:
    int serverSd;
    sockaddr_in servAddr;

    public:

    void initialiseSocket()
    {
        //Initialising the socket
        this->serverSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(this->serverSd < 0)
        {
            cout << "Failed to open the socket... Closing" << endl;
            exit(0);
        }
        else
        {
            cout << "Socket opened successfully... " << endl;
            cout << "Socket Descriptor ID: " << this->serverSd << endl;
        }
    }

    void initialiseSocketEnv()
    {
        //Initialise the environment for sockaddr structure
        bzero((char*)&this->servAddr, sizeof(this->servAddr));
        this->servAddr.sin_family = AF_INET;
        this->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->servAddr.sin_port = htons(PORT);
    }

    void setSocketOptions()
    {
        //Setting Socket Options
        int nOptVal = 0;// Allowing other servers to bind and listen to the port, but not accept new connections
        int nOptLen = sizeof(nOptVal);
        int sockoptstatus =  setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);

        if (sockoptstatus == 0)
        {
            cout << "Socket options successfully set... " << endl;
        }
        else
        {
            cout << "Failed to set socket options... " << endl;       
            exit(0);
        }

    }

    void bindSocket()
    {
        //Bind the socket to the local port
        int bindStatus = bind(this->serverSd, (struct sockaddr*) &this->servAddr, sizeof(this->servAddr));
        if(bindStatus < 0)
        {
            cerr << "Error binding socket to local address" << endl;
            exit(0);
        }
        else
        {
            cout << "Socket successfully bound to local port... " << endl;
            SEPARATOR;

            cout << "Waiting for a client to connect..." << endl;
        }

    }

    int getserverSd()
    {
        return this->serverSd;
    }

    void listenSocket()
    {
        //Listen to requests from the client

        int listenStatus = listen(serverSd, 5); //Second argument - backlog - specifies how many requests at a time the server can pull in the active queue
        //Here, if more than 5 clients come, they need to wait in the wait queue (maintained internally) 


        if (listenStatus < 0)
        {
            cout << "Failed to start lietening on local port... " << endl;
            
            exit(0);
        }
        else
        {
            cout << "Socket listening on local port " << ntohs(this->servAddr.sin_port) << "... " << endl;

        }
    }

    void displayDetails(tutorial::UserTextList &userlist)
    {
        
        for (int i = 0; i < userlist.users_size(); i++)
        {
            SEPARATOR;
            tutorial::User u = userlist.users(i);
            cout << "Person Name: " << u.name() << endl;
            tutorial::PhoneNumber num = u.phone();
            cout << "Phone number is: " << num.number() << endl;
            for (int j = 0; j < u.usernotes_size(); j++)
            {
                cout << "Note " << j + 1 << endl;
                cout << "----------------------------------" << endl;
                cout << u.usernotes(j).text() << endl;
                cout << "-------------------------------" << endl;

            }
            SEPARATOR;

        }
    }




};


int main(int argc, char *argv[])
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::UserTextList users;
    Server srv;
    char msg[1500]; //buffer to send and receive messages with

    srv.initialiseSocket();
    srv.initialiseSocketEnv();
    srv.setSocketOptions();
    srv.bindSocket();
    srv.listenSocket();

   
    //Receive requests from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    //Accept, create a new socket descriptor to 
    //handle the new connection with client

    int newSd = accept(srv.getserverSd(), (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    else
    {
        cout << "Connected with client!" << endl;
    }
    
    int bytesRead, bytesWritten = 0;
    
    //receive a message from the client (listen)
    cout << "Awaiting client response..." << endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer


    bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);

    string data(msg);
    users.ParseFromString(data);

    srv.displayDetails(users);

    
    //send the message to client
    // bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    
    //we need to close the socket descriptors after we're all done
    close(newSd);
    close(srv.getserverSd());
    cout << "********Session Ended********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Connection closed..." << endl;
    return 0;   
}