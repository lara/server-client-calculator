# server.py - server side
import socket
import re

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creates a socket object
host =  socket.gethostbyname(socket.gethostname()) # '0.0.0.0' # socket.gethostname() # gets local machine name or socket.gethostbyname(socket.gethostname())
port = 7261

serversocket.bind((host, port)) # binds to the port
serversocket.listen(5) # queue up to 5 requests

def calculate(question):
    regexparse = re.compile(r'^(\.\d+|\d+(?:\.\d+)?)\s*([+/*-])\s*(\.\d+|\d+(?:\.\d+)?)$')

    if regexparse.match(question):
        elem = re.findall(regexparse, question)
        first = float(elem[0][0])
        op = elem[0][1]
        second = float(elem[0][2])

        if op == '/' and first % second != 0:
            return str(first / second)
        else:
            return str(eval(question))
    else:
        raise ValueError

while True:
    clientsocket, addr = serversocket.accept() # establishes a connection
    print("Connected by client on %s" % str(host))

    while True:
        question = clientsocket.recv(1024)
        if not question:
            print("Server program ends due to the client closing the connection.")
            serversocket.close()
            clientsocket.close()
            exit()
        try:
            answer = calculate(question)
            clientsocket.send("Answer from server: {}".format(answer))
            print("Received question \"{}\"; send back answer {}".format(question, answer))
        except ValueError:
            clientsocket.send("Invalid question! Please enter the math question again.")
