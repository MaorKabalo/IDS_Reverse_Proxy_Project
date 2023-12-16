import socket


def main():

    host = '127.0.0.1'  # Change to the IP address of your proxy server
    port = 8888  # Change to the port your server will listen on

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen()

        print(f"Server listening on {host}:{port}")

        while True:
            client_socket, client_address = server_socket.accept()
            print(f"Accepted connection from {client_address}")

            with client_socket:
                message = client_socket.recv(1024).decode()
                print(f"Received message: {message}")


if __name__ == "__main__":
    main()
