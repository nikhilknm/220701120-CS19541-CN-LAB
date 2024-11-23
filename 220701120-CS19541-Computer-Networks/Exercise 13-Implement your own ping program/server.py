from socket import *
def start_server(host='127.0.0.1',port=12345):
    with socket(AF_INET,SOCK_DGRAM) as s:
        s.bind((host,port))
        print(f"UDP server running on {host}:{port}")
        while True:
            data,addr=s.recvfrom(1024)
            print(f"Received from {addr}:{data.decode()}")
            s.sendto(b'pong',addr)
start_server()