import socket


def main():
    host = '127.0.0.1'  # Change to the IP address of your proxy server
    port = 8888  # Change to the port your proxy is listening on

    message = "Hello So Cool\n"
    print(f'Sending: "{message}" to {host}:{port}')

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((host, port))
        client_socket.sendall(message.encode())

    print("Message sent to proxy.")


if __name__ == "__main__":
    main()
