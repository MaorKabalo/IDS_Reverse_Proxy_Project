import socket
import time


def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8888))
    server_socket.listen(5)  # Allow up to 5 queued connections

    print("Server listening on 127.0.0.1:8888")

    while True:
        try:
            client_socket, addr = server_socket.accept()
            #print(f"Accepted connection from {addr}")

            start_time = time.time()
            total_data_received = 0

            while True:
                data = client_socket.recv(1024)
                if not data:
                    break  # Break the loop if no more data
                total_data_received += len(data)  #1024
                elapsed_time = time.time() - start_time

                if elapsed_time > 1:  # Update bandwidth every second
                    bandwidth_mbps = (total_data_received * 8) / (1e6 * elapsed_time)
                    print(total_data_received)
                    print(f"Received bandwidth: {bandwidth_mbps:.2f} Mbps")
                    start_time = time.time()
                    total_data_received = 0

                #print(f"Received data: {data}")

        except ConnectionResetError:
            print("Connection reset by peer")

        except Exception as e:
            print(f"Error: {e}")

        finally:
            if 'client_socket' in locals():
                client_socket.close()


if __name__ == "__main__":
    main()
