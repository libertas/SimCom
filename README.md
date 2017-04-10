# SimCom
A simple communication protocol for micro-controllers and embedded devices

# What's Simcom
SimCom is a simple communication protocol for micro-controllers and embedded devices.
It's designed as some kind of light wrap for low level communication protocols such as RS-232 or anything like that.
It's designed to be portable to use different physical drivers and to be compiled on many micro-controllers and embedded devices.

# How to Use it
If you want to work on Linux and serial port drivers, there is a sample implementation.
Otherwise, you have to add your own code to the Physical Layer.

# The layers of SimCom
There are three layers.

The first one is Physical Layer.
It's something to have to modify by yourself.
These two functions, ph_receive_intr() and ph_send_intr() should be modified to adapt to different physical drivers.
They send and receive data in bytes.
You should call them manually when ever you need to send or receive data.

The second one is Datalink Layer.
It's something about frame synchronization.
It can use the Physical Layer to send and receive data in frames.

The third one is Service Layer.
It uses the Datalink Layer and only two fields of data are added, sender's port and receiver's port.
Each port can only be 0 to 255, which consists only one byte.
Every receiver can register callback functions to any of the ports.
When data received, the Service Layer will call the callback funcion registered to the port.
You have to call sl_receive_intr() timely and manually for that.

# Initialization
Every layer has to be initialized before using it.
