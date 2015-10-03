import socket

# create an INET, STREAMing socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# bind the socket to a public host,
# and a well-known port
server_socket.bind(('localhost', 4444))
# become a server socket
server_socket.listen(5)

while True:
    # accept connections from outside
    client_socket, address = server_socket.accept()
    # now do something with the client_socket
    # in this case, we'll pretend this is a threaded server
    print('Connected by {}'.format(address))

    data = "Hi World\n"
    while True:
        client_socket.sendall(data)
        received_data = client_socket.recv(1024)
        if not received_data:
            break
        print(received_data)
client_socket.close()
