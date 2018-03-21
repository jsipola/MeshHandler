import os
import subprocess
import socket
import sys
import time

if __name__ == '__main__':
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_addr = ('192.168.10.26', 10000)
    sock.connect(server_addr)
    
    try:
        msg1 = "make a;move a 400 200 0;make b;move b -300 0 300;make c;move c 0 0 300;\0"
        sock.sendall(msg1)
        a = 0
        b = 0
        while a < 30:
            msge = "move a 0 " + str(b) + " 0;"
            sock.sendall(msge)
            #sock.sendall("a 0 300 0;")
            #a = a + 1
            time.sleep(0.1)
            #sock.sendall("a 0 -300 0;")
            a = a + 1
            #time.sleep(0.5)
            b = a * 10
    finally:
        sock.close()
