#Server/Client serial communication using SOCAT emulation SW - CHRISTOS AXELOS


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

