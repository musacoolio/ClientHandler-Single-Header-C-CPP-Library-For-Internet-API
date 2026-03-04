# ClientHandler-Single-Header-C-C++-Library-For-Internet-API
A simple raw TCP client handler made to make networking over TCP stupidly simple.
Made in 2 days because networking APIs like Boost.asio are too big and hard to learn by someone beginning in the field, like me.

## Why use this header?
  Well, for starters you could look at the section for features but... I'll explain why.
  ClientHandler.h is... stupidly simple. as in define one PacketHandle struct and there, an entire client in the blink of an eye.
  And its fully accomplished for its goal of being raw.
  This header is also cross-compatible (yes, YOU Linux users can use this).

## Struct Explanation
  - PacketHandle is a struct that contains all operations, and acts as your main "handler" for connecting to raw TCP servers.
  - PacketHandle has RecievePacket(), SendPacket(), and StringWrite() as helper functions.
  - RecievePacket() receives a stream of data from the connected host and should only be called once connection is valid (SocketState has been set to true). It then writes the recieved stream to a vector called recieveLine, which is a member of the PacketHandle struct.
  - SendPacket() sends a stream of data to the connected host and should only be called once the connection is valid (SocketState has been set to true). It takes what has been written to sendLine (a vector member of the PacketHandle struct) and then sends it to the connected host.
  - StringWrite() takes a string by reference and edits it to include the contents of recieveLine.
  - Thats the entire struct function set! Simple eh?
  - However, how do you connect to a host?
  - Use Connect()!
  - Connect() uses the PacketHandle members IP and port to connect to a host thats listening on the provided IP and port members. it also contains automatic reconnection incase connection fails.
  - And now, thats it!

 ## Usage Example
 ### The example below connects to 127.0.0.1, sends "HI!" to 127.0.0.1, and then recieves a SINGULAR stream of data from 127.0.0.1, all while operating on port 4444.
 ```C++
#include "ClientHandler.h"

int main() {
   PacketHandle test;
   test.port = 4444;
   strncpy(test.IP, sizeof(test.IP), "127.0.0.1"); // You have to use strncpy to write to the IP member of PacketHandle.
   test.sendLine.push_back('H');
   test.sendLine.push_back('I');
   test.sendLine.push_back('!');
   test.Connect();
   test.SendPacket();
   test.RecievePacket();
   return 0;
   }
```

## Some questions you might have before using this header:

### So... what features does this header file have?

This header handles:

  -Socket Creation
  
  -Connection with failsafes and reconnection built in
  
  -Sending bytes to a TCP server
  
  -Recieving bytes from a TCP server
  
  -The ability to write recieved messages onto a std::string (or char [])

### But I work only on Linux/MacOS/Windows! I can't switch around for a TCP client header!

This header is cross-platform, as it uses WinSock2 (for Windows users) and POSIX (for MacOS/Linux users).

### I want a header that could change for my purposes stupidly easy!

This header is raw TCP, meaning you can append or change the header to your liking to work with any TCP protocol.

# This repository is licensed under the MIT license. Read LICENSE for more information on this repository's license.
