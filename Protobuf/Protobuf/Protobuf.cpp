
#include <iostream>
#include <fstream>
#include <string>
#include "Phone.pb.h"

using namespace std;

#define SPACER cout<<"==================================="<<endl

void takePersonInput(tutorial::User* user)
{
    string name,number,msg;
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

void showPersonOutput(tutorial::UserTextList& userlist)
{
    for (int i = 0; i < userlist.users_size();i++) {
        SPACER;
        tutorial::User u = userlist.users(i);
        cout << "Person Name: " << u.name()<<endl;
        tutorial::PhoneNumber num = u.phone();
        cout << "Phone number is: " << num.number() << endl;
        for (int j = 0; j < u.usernotes_size(); j++)
        {
            cout << "Note " << j + 1 << endl;
            cout << "----------------------------------" << endl;
            cout << u.usernotes(j).text() << endl;
            cout << "-------------------------------" << endl;

        }
        SPACER;
    }
}

int main()
{
   GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::UserTextList users;

    fstream fin,fout;
    fin.open("Userbook", ios::in | ios::binary);
    if (!fin.is_open())
    {
        cout << "No such file exists... Creating new file" << endl;
    }
    else if (!users.ParseFromIstream(&fin))
    {
        cout << "Failed to parse from file..." << endl;
        return -1;
    }
    fin.close();
    takePersonInput(users.add_users());
 

    fout.open("Userbook", ios::out | ios::trunc | ios::binary);
    if (!users.SerializeToOstream(&fout))
    {
        cout << "Failed to write to file" << endl;
        return -1;
    }
    fout.close();
    SPACER;
    cout << "Reading From File..." << endl;

    fstream finn("Userbook", ios::in | ios::binary);
    if (!users.ParseFromIstream(&finn))
    {
        cout << "Failed to parse from file..." << endl;
    }
    showPersonOutput(users);



    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}

