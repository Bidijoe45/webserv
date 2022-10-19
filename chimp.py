import socket
import time

HOST = "127.0.0.1"
PORT = 3000

def split(s, n):
    i=0
    f=[]
    while(i<len(s)):
        f.append(s[i:i+n])
        i+=n
    return(f)

def send(msg, chunk_size, delay):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        split_msg = split(msg, chunk_size)
        s.connect((HOST, PORT))

        for msg in split_msg:
            s.sendall(msg.encode("utf-8"))
            time.sleep(delay)

        data = s.recv(6000)

    return data


if __name__ == "__main__":
    line = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"

    response = send(line, 5, 2)

    print("--RESPONSE--")
    print(response.decode())


