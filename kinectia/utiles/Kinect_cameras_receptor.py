import zmq

# Two integers to be sent
first_value = 183
second_value = 358

# Concatenate the integers with a comma in between and convert to bytes
message = f"{first_value},{second_value}".encode('utf-8')

# Create a ZeroMQ context and socket
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5557")  # Replace with your server address

# Send the message as a request
socket.send(message)

# Receive the response
response = socket.recv()
print("Received response:", response.decode('utf-8'))