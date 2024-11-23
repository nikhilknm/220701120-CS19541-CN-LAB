from socket import *
HOST = "172.16.9.214"
PORT = 65432
with socket(AF_INET,SOCK_STREAM) as s:
    s.connect((HOST,PORT))
    s.sendall(b"hello, world")
    data = s.recv(1024)
print(f"Received {data!r}")
            