import socket
import select
import sys

from messages import recv_msg, send_msg


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

if len(sys.argv) != 3:
    print("Correct usage: script, IP address, port number")
    exit()

ip_address = str(sys.argv[1])
port = int(sys.argv[2])
server.connect((ip_address, port))

while True:
    # maintains a list of possible input streams
    sockets_list = [sys.stdin, server]

    read_sockets, write_socket, error_socket = select.select(sockets_list,[],[])

    for sock in read_sockets:
        if sock == server:
            msg = recv_msg(sock)
            print(msg.decode("utf-8"))
        else:
            msg = sys.stdin.readline()
            send_msg(server, msg.encode("utf-8"))

server.close()
