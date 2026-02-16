# ClientHandler-Single-Header-C-C-Library-For-Internet-API
A simple raw TCP client handler made to make networking over TCP stupidly simple.
Made in 2 days because networking APIs like Boost.asio are too big and hard to learn by someone beginning in the field, like me.

## Why use this header?
  Well, for starters you could look at the section below for features but... I'll explain why.
  ClientHandler.h is... stupidly simple. as in define one PacketHandle struct and there, an entire client in the blink of an eye.
  And its fully accomplished for its goal of being raw.
  This header is also cross-compatible (yes, YOU Linux users can use this).

## Some questions you might have before using this header:

### So... what features does this header file have?

This header handles:
  -Socket Creation
  -Connection with failsafes and reconnection built in
  -Sending bytes to a TCP server
  -Recieving bytes from a TCP server
  -The ability to write recieved messages onto a std::string (or char* [])

### But I work only on Linux/MacOS/Windows! I can't switch around for a TCP client header!

This header is cross-platform, as it uses WinSock2 (for Windows users) or POSIX (for MacOS/Linux users).

### I want a header that could change for my purposes stupidly easy!

This header is raw TCP, meaning you can append or change the header to your liking to work with any TCP protocol.

# This repository is licensed under the MIT license. Read LICENSE for more information on this repository's license.
