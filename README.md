#Server/Client serial communication using SOCAT  - CHRISTOS AXELOS

<img width="385" height="224" alt="serial-port-comm-arch" src="https://github.com/user-attachments/assets/d48bed7f-33e9-47a0-9e53-6cb4c5a4e8b9" />

## What does it achieve
* This project contains 2 apps, one AT-Command server based app and one AT-Command client based app, that communicate via a serial port connection.
* The server waits for commands and sends back the appropriate responses
* The serial port is emulated using a virtual serial port (pts). In our case, "SOCAT" tool is selected to achieve the emulation.
* The read/write is done in tty-like formatting and the exchanged payload is ASCII string. 
* Requirements: Any Linux distro, CMake
* To compile the project, open a terminal and type `cmake .` in  "evaluation_axelos" directory.


## Compile

* To compile the project, open a terminal and type `cmake .` in  "evaluation_axelos" directory.

* Then, type  `make`.


## Execute

* In the **same** terminal, type  `socat -d -d pty,rawer,link=/dev/ttyS0 PTY,rawer,link=/dev/ttyS1`, in order to create virtual serial
port connection. An example output can be as follows:

```
$ socat -d -d pty,rawer,link=/dev/ttyS0 PTY,rawer,link=/dev/ttyS1
021/04/17 19:25:58 socat[4313] N PTY is /dev/pts/1
2021/04/17 19:25:58 socat[4313] N PTY is /dev/pts/10
2021/04/17 19:25:58 socat[4313] N starting data transfer loop with FDs [5,5] and [7,7]
```
* Now open a **second** terminal in the same directory and run server as follows. **/dev/pts/1** is taken from the previous command's output.
```
./server -nregs 5 -endpoint /dev/pts/1

```

* Then, open a **third** terminal in the same directory and run client as follows. **/dev/pts/10** is taken from the previous command's output.
```
./client -endpoint /dev/pts/10

```
* Finally, the client is ready to interact with the server.

## System's Architecture

* The server and client side contain **3 threads each**. One master thread for main calculations, one slave thread for sending to serial port and one slave for receiving from it.

