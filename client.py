# client.py - client side
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creates a socket object

host = socket.gethostbyname(socket.gethostname()) # '0.0.0.0' # socket.gethostname() # gets local machine name or s
port = 7261

s.connect((host, port)) # connection to hostname on the port
print("Connected with server on %s" % str(host))

while True:
    # receive no more than 1024 bytes
    user_input = raw_input().replace(" ", "")
    if user_input == '0/0':
        print("User input ends; end the client program.")
        s.close()
        break
    s.send(str(user_input))
    print(s.recv(1024))
