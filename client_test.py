import socket as sock

server_socket = sock.socket(sock.AF_UNSPEC, sock.SOCK_STREAM)
server_socket.connect(("localhost", 3000))

data = server_socket.recv(1024)

print(data)