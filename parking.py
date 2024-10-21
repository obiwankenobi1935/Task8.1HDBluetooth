from bluepy import btle
import RPi.GPIO as GPIO
import time
import struct  # To unpack binary data

# Set up GPIO for Buzzer
BUZZER_PIN = 17  # Define the GPIO pin for the buzzer
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUZZER_PIN, GPIO.OUT)  # Set up the buzzer pin as output

# Connect to Bluetooth device
target_address = "E0:5A:1B:78:69:52"  # Replace with your Arduino's MAC address
device = btle.Peripheral(target_address)

try:
    while True:
        # Read the characteristic (assuming handle 12)
        data = device.readCharacteristic(12)  # Handle 12, adjust if necessary
        
        # Check the length of the received data
        print(f"Received data length: {len(data)} bytes")

        if len(data) == 4:
            # Unpack the data as an unsigned integer (4 bytes)
            distance = struct.unpack('I', data)[0]  # 'I' means unsigned int (4 bytes)
            print(f"Distance: {distance} cm")

            # Control Buzzer based on distance
            if distance < 20:
                # Buzzer ON for distance < 20 cm
                for _ in range(5):  # Repeat the beep 5 times
                    GPIO.output(BUZZER_PIN, GPIO.HIGH)  # Buzzer ON
                    time.sleep(0.1)  # Beep duration
                    GPIO.output(BUZZER_PIN, GPIO.LOW)   # Buzzer OFF
                    time.sleep(0.1)  # Pause between beeps

            else:
                GPIO.output(BUZZER_PIN, GPIO.LOW)  # Buzzer OFF for distance >= 20 cm

        else:
            print(f"Unexpected data length: {len(data)}. Skipping this reading.")

        time.sleep(0.5)  # Slightly increase the delay between measurements

finally:
    device.disconnect()
    GPIO.cleanup()

