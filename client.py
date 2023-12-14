import socket

SERVER_IP = "localhost"
SERVER_PORT = 8888
MSG = "Hi!"

with socket.socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (SERVER_IP, SERVER_PORT)
    sock.connect(server_address)
    sock.sendall(MSG.encode())
    server_msg = sock.recv(1024)
    server_msg = server_msg.decode()
    print(server_msg)