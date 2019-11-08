import socket
import time

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('localhost', 9999))

print (client.send('Hello world!'), ' bytes sent.')
time.sleep(0.2)
print ('Received message:', client.recv(1024))
