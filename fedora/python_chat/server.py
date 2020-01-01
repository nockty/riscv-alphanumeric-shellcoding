# Python program to implement server side of chat room.
import socket
import select
import sys
import threading

from messages import recv_msg, send_msg
from url_processing import is_url, process_url


def broadcast(conn, msg):
    print(msg)
    send_msg(conn, msg.encode("utf-8"))


def client_thread(conn, addr):
    send_msg(conn, "Chat initialized".encode("utf-8"))

    while True:
        try:
            msg = recv_msg(conn)
            if not msg:
                client_connected = False
                continue
            msg_str = msg.decode("utf-8")
            print(f"<{addr}> {msg_str}")
            if not msg_str.startswith("@URL:"):
                continue
            msg_url = msg_str.split("@URL:")[1]
            if not is_url(msg_url):
                broadcast(conn, f"{addr} shared an invalid URL")
                continue
            url_description = str(process_url(msg_url))
            broadcast(conn, f"{addr} shared an URL about:")
            broadcast(conn, url_description)

        except Exception:
            continue


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

if len(sys.argv) != 3:
    print("Correct usage: script, IP address, port number")
    exit()

ip_address = str(sys.argv[1])
port = int(sys.argv[2])

server.bind((ip_address, port))
server.listen(1)

client_connected = False

while True:
    if client_connected:
        continue
    conn, addr = server.accept()
    client_connected = True
    print(f"{addr[0]} connected")
    threading.Thread(
        target=client_thread,
        args=(conn, addr[0]),
    ).start()

conn.close()
server.close()
