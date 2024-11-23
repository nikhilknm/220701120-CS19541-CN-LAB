from socket import *
HOST = "172.16.10.227"
PORT = 65432
with socket(AF_INET,SOCK_STREAM) as s:
    s.bind((HOST,PORT))
    s.listen()
    conn,addr=s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data = conn.recv(1024).decode()
            print(f"Received: {data}")
            toSend=input("Enter message:")
            if toSend=="exit":
                break
            conn.sendall(toSend.encode())