import socket
import time


def send_hello_world_messages(server_address, server_port):
    # Create a socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    i = 1
    try:
        # Connect to the server
        client_socket.connect((server_address, server_port))
        print(f"Connected to {server_address}:{server_port}")

        # Send 80 "hello world" messages per second
        for _ in range(2):
            message = "hello world"
            client_socket.send(message.encode())
            print(f"{i} Sent: {message}")
            i += 1
            time.sleep(0.5)



    except Exception as e:
        print(f"Error: {e}")

    finally:
        # Close the socket
        client_socket.close()
        print("Connection closed")


if __name__ == "__main__":
    # Replace 'localhost' and 12345 with the actual server address and port
    server_address = '127.0.0.1'
    server_port = 9090

    send_hello_world_messages(server_address, server_port)
