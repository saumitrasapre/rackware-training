#include <iostream>
#include <string>
#include "Phone.pb.h"
#include <stdio.h>
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



class Client
{
    private:
    int clientSd;
    sockaddr_in sendSockAddr; 


    public:

    void initialiseSocket()
    {
        //Initialising the socket
        this->clientSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        if (this->clientSd < 0)
        {
            cout << "Failed to open the socket... " << endl;
            exit(0);
        }
        else
        {
            cout << "Socket opened successfully... " << endl;
            cout << "Socket Descriptor ID: " << this->clientSd << endl;
        }

    }

    void initialiseSocketEnv()
    {
        //Initialise the environment for sockaddr structure
        bzero((char*)&this->sendSockAddr, sizeof(this->sendSockAddr)); 
        this->sendSockAddr.sin_family = AF_INET; 
        this->sendSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        this->sendSockAddr.sin_port = htons(PORT);

    }

    int connectToServer()
    {
        //Connect to server
        int connectStatus = connect(this->clientSd,(sockaddr*) &this->sendSockAddr, sizeof(this->sendSockAddr));
        return connectStatus;
    }

    int getclientSd()
    {
        return this->clientSd;
    }

    void takePersonInput(tutorial::User* user)
    {
        //Function to accept user input which includes name, phone number and a list of messages
        //Stores information in tutorial::User object
        string name, number, msg;
        cout << "Enter the Person's name: " << endl;
        getline(cin, name);
        user->set_name(name);
        cout << "Enter the Person's phone number: " << endl;
        getline(cin, number);
        tutorial::PhoneNumber* num = new tutorial::PhoneNumber();

        num->set_number(number);
        user->set_allocated_phone(num);
        cout << "Enter the messages you want to store:" << endl;
        for(int i=0;i<2;i++)
        {
            cout << "Message "<<i+1<<"> ";
            getline(cin, msg);
            if (msg.empty())
            {
                break;
            }
            tutorial::Notes* note = user->add_usernotes();
            note->set_text(msg);
        }
    }




};


int main(int argc, char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::UserTextList users;
    Client cli;
    char msg[1500]; //create a message buffer 

    cli.initialiseSocket();
    cli.initialiseSocketEnv();

    if(cli.connectToServer() < 0)
    {
        cout << "Failed to connect to the server..." << endl;
        exit(0);
    }
    else
    {
        cout << "Connected to the server!" << endl;

        int bytesRead, bytesWritten = 0;

        string data;
        memset(&msg, 0, sizeof(msg));//clear the buffer

        cli.takePersonInput(users.add_users());
        users.AppendToString(&data);

        strcpy(msg, data.c_str());
        bytesWritten += send(cli.getclientSd(), (char*)&msg, strlen(msg), 0);
        SEPARATOR;
        //Reading from server...
        cout << "Awaiting server response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer

        bytesRead += recv(cli.getclientSd(), (char*)&msg, sizeof(msg), 0);

        cout<<"SERVER: Number of characters..." <<msg<<endl;

    close(cli.getclientSd());
    cout << "********Session Ended********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Connection closed" << endl;
    }
    
    
    return 0;    
}