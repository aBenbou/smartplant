import serial
import paho.mqtt.client as mqtt
import json
import time

# Serial communication initialization
serial_port = 'COM5'
baud_rate = 9600
btSerial = serial.Serial(serial_port, baud_rate)
btSerial.timeout = 1

# MQTT topics
client_id = 'b1576e17-8808-4463-a88a-aaabe833d5eb'
telemetry_topic = f"{client_id}/telemetry"
commands_topic = f"{client_id}/commands"

# MQTT initialization
mqtt_client = mqtt.Client(client_id)
mqtt_client.connect('test.mosquitto.org')
mqtt_client.loop_start()

# Command handling function
def handle_commands(client, userdata, message):
    command = json.loads(message.payload.decode())
    print("Command received:", command)
    # Parse and execute commands
    if command.get("relay") == "ON":
        btSerial.write(b"relayON\n")
    elif command.get("relay") == "OFF":
        btSerial.write(b"relayOFF\n")
    elif command.get("light") == "ON":
        btSerial.write(b"ledON\n")
    elif command.get("light") == "OFF":
        btSerial.write(b"ledOFF\n")

# Subscribe to the commands topic
mqtt_client.subscribe(commands_topic)
mqtt_client.on_message = handle_commands

# Telemetry handling loop
while True:
    # Read telemetry data from the serial port
    btTelemetry = btSerial.readline().decode('ascii').strip()
    if btTelemetry:
        print(f'Received raw BT telemetry: "{btTelemetry}"')
        try:
            # Process the telemetry data
            telemetry_data = btTelemetry.split(',')
            telemetry_dict = {k: float(v) for k, v in (x.split(':') for x in telemetry_data)}
            humidity = telemetry_dict.get('Humidity')
            temperature = telemetry_dict.get('Temp')
            light = telemetry_dict.get('Light')
            telemetryJSON = {
                'humidity': humidity,
                'temperature': temperature,
                'light': light,
                'timestamp': round(time.time() * 1000)
            }
            telemetryJSONStr = json.dumps(telemetryJSON)
            print("Formatted telemetry to send:", telemetryJSONStr)
            mqtt_client.publish(telemetry_topic, telemetryJSONStr, qos=1)
        except ValueError as e:
            print(f'ValueError: {e}, with telemetry data: {btTelemetry}')
    time.sleep(1)  # short delay to prevent spamming the serial port