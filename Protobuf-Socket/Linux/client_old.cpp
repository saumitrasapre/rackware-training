#include <iostream>
#include "Phone.pb.h"
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#define SEPARATOR cout<<"============================================="<<endl
#define PORT 9900   
using namespace std;

struct sockaddr_in srv;

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
    cout << "Enter the message you want to store: (LEAVE BLANK TO EXIT) " << endl;
    while (true)
    {
        cout << "> ";
        getline(cin, msg);
        if (msg.empty())
        {
            break;
        }
        tutorial::Notes* note = user->add_usernotes();
        note->set_text(msg);
    }
}



int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::UserTextList users;

    int nRet = 0;

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
        exit(0);
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
        exit(0);
    }
    else
    {
        //Connected to Server

        char buff[256]  ="";
        recv(nClientSocket, buff, sizeof(buff), 0);
        cout << "SERVER: "<<buff << endl;

        //Now send messages to server
       // while (true)
       // {
            takePersonInput(users.add_users());
            string buffer;
            users.AppendToString(&buffer);
            send(nClientSocket, buffer.c_str(), buffer.size(), 0);
        //}

      /*  while (TRUE)
        {
            cout << "Enter message: ";
            fgets(buff, 256, stdin);
            send(nClientSocket, buff, sizeof(buff), 0);
            memset(buff, ' ', 256);
            recv(nClientSocket, buff, sizeof(buff), 0);
            cout << "SERVER: " << buff << endl;

        }*/
    }




    return 0;
}


