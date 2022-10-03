import socket
import time
import sys

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
    if len(sys.argv) != 3:
        print("te faltan argumentos >:(")
        print("(a.k.a. chunk_size delay)")
        exit()

    chunk_size = int(sys.argv[1])
    delay = float(sys.argv[2])
#    line = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
#    line = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\nHello world!"
#    line = "POST / HTTP/1.1\r\nHost: localhost\r\nContent-type: text/html\r\nContent-length: 12\r\n\r\nHello world!"
#    line = "POST / HTTP/1.1\r\nHost: localhost\r\nContent-type: text/html\r\nContent-length: 122\r\n\r\nHello world!"
#    line = "POST /formula1/formula.php HTTP/1.1\r\nHost: localhost\r\nContent-type: text/html\r\nContent-length: 12\r\n\r\nHello world!"
#    line = "POST /formula1/formula.php HTTP/1.1\r\nHost: localhost\r\nContent-Type: multipart/form-data; boundary=---------------------------304573777018669010322921142749\r\nContent-Length: 361\r\n\r\n-----------------------------304573777018669010322921142749\r\nContent-Disposition: form-data; name=\"form_file_name\"; filename=\"hola.txt\"\r\nContent-Type: text/plain\r\n\r\nque taaaaal\n\r\n-----------------------------304573777018669010322921142749\r\nContent-Disposition: form-data; name=\"send\"\r\n\r\nSend form\r\n-----------------------------304573777018669010322921142749--\r\n"
    line = "HEAD / HTTP/1.1\r\nHost: localhost\r\n\r\nHello world!"

    response = send(line, chunk_size, delay)

    print("--RESPONSE--")
    print(response.decode())

