# webcam feed transmission via UDP

Server is asynchronous. When receive packet from a client, it takes the current frame from webcam using OpenCV and transmit through sockets using Asio.

Client sends packet to server to request for the current frame. Displays it using OpenCV.
