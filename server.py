import socket


def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8888))
    server_socket.listen(5)  # Allow up to 5 queued connections

    print("Server listening on 127.0.0.1:8888")

    while True:
        try:
            client_socket, addr = server_socket.accept()
            print(f"Accepted connection from {addr}")

            while True:
                data = client_socket.recv(1024)
                if not data:
                    break  # Break the loop if no more data

                data = data[:-1]
                print(f"Received data: {data}")

        except ConnectionResetError:
            print("Connection reset by peer")

        except Exception as e:
            print(f"Error: {e}")

        finally:
            if 'client_socket' in locals():
                client_socket.close()


if __name__ == "__main__":
    main()
