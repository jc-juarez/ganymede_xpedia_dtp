import socket

def tcp_client():
    # Define server address and port
    SERVER_ADDRESS = 'localhost'
    SERVER_PORT = 9090
    
    # Create a TCP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        # Connect to the server
        client_socket.connect((SERVER_ADDRESS, SERVER_PORT))
        print(f"Connected to server {SERVER_ADDRESS} on port {SERVER_PORT}")
        
        # Send data to the server
        message = "Hello, Server :DDDD!"
        client_socket.sendall(message.encode('utf-8'))
        print(f"Sent: {message}")
        
        # Receive data from the server
        response = client_socket.recv(1024)
        print("Received from server:", response.decode('utf-8'))
    
    except socket.error as e:
        print(f"Socket error: {e}")
    
    finally:
        # Close the connection
        client_socket.close()
        print("Connection closed")

if __name__ == "__main__":
    tcp_client()
