from socket import *
HOST = "172.16.10.227"
PORT = 65432
with socket(AF_INET,SOCK_STREAM) as s:
    s.connect((HOST,PORT))
    toSend=input("Enter message:")
    while toSend!="exit":
        s.sendall(toSend.encode())
        data = s.recv(1024).decode()
        print(f"Received: {data}")
        toSend=input("Enter message:")
            