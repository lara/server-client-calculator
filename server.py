# server.py - server side
import socket
import re

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creates a socket object
host = "0.0.0.0" # socket.gethostname() # gets local machine name
port = 7264

serversocket.bind((host, port)) # binds to the port
serversocket.listen(5) # queue up to 5 requests

def calculate(question):
    regexparse = re.compile(r'^(\.\d+|\d+(\.\d+)?)\s*([+/*-])\s*(\.\d+|\d+(\.\d+)?)$')

    if regexparse.match(question):
        return "Answer from server: {}".format(eval(question))
    else:
        return "Invalid question! Please enter the math question again."

while True:
    clientsocket, addr = serversocket.accept() # establishes a connection
    print("Connected by client on %s" % str(addr))

    while True:
        question = clientsocket.recv(1024)
        if not question:
            print("Server program ends due to the client closing the connection")
            serversocket.close()
            clientsocket.close()
            exit()
        answer = calculate(question)
        print("Received question {}; send back answer {}".format(question, answer))
        clientsocket.send(answer)
