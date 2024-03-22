import serial
import struct

# Open the serial port
# Open the serial port
ser = serial.Serial('COM13', baudrate=9600)  # replace 'COM13' with your port
start_byte = b'\xAA'
end_byte = b'\x55'


while True:

    # Read until we get the start byte
    byte = ser.read(1)
    while byte != start_byte:
        pass

    # Now we're at the start of the float array, so read until we get the end byte
    data = b''
    while True:
        byte = ser.read(1)
        if byte == end_byte:
            break
        data += byte

    # Unpack the data into 4 floats
    floats = struct.unpack('3f', data)

    print(f'Received: {floats}')

# Don't forget to close the port when you're done
ser.close()
