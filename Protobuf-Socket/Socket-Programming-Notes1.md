# Important Points

# Endianness-
We need to be careful regarding the endianness of the data we are sending through the sockets. There are two types of byte orders that different processors (hosts) follow.

## Little Endian Byte Order:
Here, the least significant byte (LSB) is placed at the byte with the lowest address. Very common, used mostly in Intel/AMD or other x86 compatible processors.

## Big Endian Byte Order:
Here, the most significant byte (MSB) is placed at the byte with the lowest address. Much rare, used in Motorola processors.

```
**NOTE:**  Intel, AMD and other x86 and x86-64 (AMD64) processors use the little-endian format to store bytes. However, for socket programming, the network byte order is big endian.
```

This means that if our system is `little endian` (which it usually is), we need to convert our endianness from `little endian` to `big endian` to transfer packets to and from the network.

Thus, in order to send packets over the network, we need to use the apis `htons` (host-to-network-short) to convert from `little endian` (host byte order) to `big endian` (network byte order).

Similarly, while receiving packets from the network, we need to use the apis `ntohs` (network-to-host-short) to convert from `big endian` (network byte order) to `little endian` (host byte order).

  
