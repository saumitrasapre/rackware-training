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
#include <cstring>

#define PORT 12345
#define SEPARATOR cout<<"============================================="<<endl

using namespace std;


class Server
{
    private:
    int serverSd;
    sockaddr_in servAddr;

    int newSd;
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    public:

    int getserverSd()
    {
        return this->serverSd;
    }
    int getnewSd()
    {
        return this->newSd;
    }

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

    void connectSocket()
    {

        //Accept, create a new socket descriptor to 
        //handle the new connection with client

        this->newSd = accept(this->serverSd, (sockaddr *)&this->newSockAddr, &this->newSockAddrSize);
        if(this->newSd < 0)
        {
            cerr << "Error accepting request from client!" << endl;
            exit(1);
        }
        else
        {
            cout << "Connected with client!" << endl;
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

    string execScript(tutorial::UserTextList &userlist)
    {
            FILE* in;
            char buff[256];
            string command;
            string output;
            tutorial::User u = userlist.users(0);
            tutorial::PhoneNumber num = u.phone();
            command = "./some_script.sh -n '" + u.name() + "' -p '" + num.number() + "' -o '" + u.usernotes(0).text() + "' -t '" + u.usernotes(1).text()+"'";
            cout<<"Executing script... "<<endl;
            cout<<"No. of characters- ";
            if(!(in = popen(command.c_str(), "r")))
                    {
                            cout<<"Script Failed"<<endl;
                            exit(0);
                    }   
            else
                    {
                            while(fgets(buff,sizeof(buff),in)!=NULL)
                                    {
                                            output += buff;
                                            //cout<<buff<<endl;
                                    }

                            pclose(in);
                    }
                 cout<<output<<endl;
            return output;
    }

};


int main(int argc, char *argv[])
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::UserTextList users;
    Server srv;
    char msg[1500]; //buffer to send and receive messages with

    int bytesRead, bytesWritten = 0;

    srv.initialiseSocket();
    srv.initialiseSocketEnv();
    srv.setSocketOptions();
    srv.bindSocket();
    srv.listenSocket();
    srv.connectSocket();



    //Connected with client, now receive a message from the client (listen)

    cout << "Awaiting client response..." << endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer


    bytesRead += recv(srv.getnewSd(), (char*)&msg, sizeof(msg), 0);

    string data(msg);
    users.ParseFromString(data);

    srv.displayDetails(users);

    strcpy(msg,srv.execScript(users).c_str());

    SEPARATOR;

    //Send the message to client

    cout<<"Sending message to client... "<<endl;
    bytesWritten += send(srv.getnewSd(), (char*)&msg, strlen(msg), 0);
    
    //we need to close the socket descriptors after we're all done
    close(srv.getnewSd());
    close(srv.getserverSd());
    cout << "********Session Ended********" << endl;
    //cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Connection closed..." << endl;
    return 0;   
}