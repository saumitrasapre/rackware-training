# The Client Program - 

Most of the steps in the client code are similar to the server code. The initial functions of creating a socket using  the `socket()` function and initialising the `sockaddr_in` structure are same as described [here](Socket-Programming-Notes2.md).

Refer [this](Protobuf-Socket-Client/Protobuf-Socket-Client/Protobuf-Socket-Client.cpp) program for the actual code.

```
NOTE - While specifying the sin_addr.s_addr property in the client program, we need to specify the server's IP address using inet_addr("127.0.0.1") and not using INADDR_ANY.
```

Which means that this-
```
srv.sin_addr.s_addr = INADDR_ANY;
```
is WRONG.

And this - 
```
srv.sin_addr.s_addr = inet_addr("127.0.0.1");
```

is CORRECT.

# Connecting to the server - 

In order to connect to the server (assuming we have filled in the server's IP address and port number while initialising the `sockaddr_in` structure), we use the `connect()` function.

```
int connect(
SOCKET         s,
const sockaddr *name,
int            namelen
);
```
Here - 

* `s` - A descriptor identifying an unconnected socket.
* `name` - A pointer to the sockaddr structure to which the connection should be established.
* `namelen` - The length, in bytes, of the sockaddr structure pointed to by the name parameter.
  
Thus, we write - 
```
nRet = connect(nClientSocket, (const sockaddr*)&srv, sizeof(srv));
```



