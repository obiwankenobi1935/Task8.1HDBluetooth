#include <ArduinoBLE.h>

// Define pin numbers for ultrasonic sensor
const int trigPin = 11;
const int echoPin = 12;

// Variables to store duration and distance
long duration;
int distance;

// Create a BLE service and characteristic
BLEService distanceService("180D");  // Custom service UUID
BLEUnsignedIntCharacteristic distanceCharacteristic("2A37", BLERead | BLENotify);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up pins for the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize the BLE hardware
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set up BLE
  BLE.setLocalName("ParkingSensor");
  BLE.setAdvertisedService(distanceService);
  distanceService.addCharacteristic(distanceCharacteristic);
  BLE.addService(distanceService);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Clear the trigger pin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);

      // Send a 10 microsecond pulse to trigger the sensor
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Read the echo pin and calculate the duration
      duration = pulseIn(echoPin, HIGH);

      // Calculate the distance (duration / 2) / 29.1 gives distance in cm
      distance = duration * 0.034 / 2;

      // Print the distance to the Serial Monitor
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");

      // Update the BLE characteristic with the new distance
      distanceCharacteristic.writeValue(distance);

      // Wait before taking another measurement
      delay(1000);
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
