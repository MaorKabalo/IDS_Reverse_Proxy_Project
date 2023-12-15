import socket

LISTEN_PORT = 8888
REVERSE_PROXY_IP = "localhost"
REVERSE_PROXY_PORT = 9090


def main():
    with socket.socket() as listening_sock:
        listening_sock.bind(('', LISTEN_PORT))
        listening_sock.listen(1)
        openPorts()
        client_soc, client_address = listening_sock.accept()
        with socket.socket(client_soc, client_address) as client_soc:
            server_msg = socket.recv(1024)
            server_msg = server_msg.decode()
            print(server_msg)
            client_soc.sendall(server_msg)


def openPorts():
    with socket.socket():
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (REVERSE_PROXY_IP, REVERSE_PROXY_PORT)
        sock.connect(server_address)
        sock.sendall(LISTEN_PORT + "")


if __name__ == "__main__":
    main()
