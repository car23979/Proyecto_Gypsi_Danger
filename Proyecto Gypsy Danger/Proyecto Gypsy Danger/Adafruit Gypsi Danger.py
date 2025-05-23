import serial
import time
import threading
from Adafruit_IO import MQTTClient

AIO_USERNAME = ""
AIO_KEY = ""

SERIAL_PORT = "COM10"
BAUD_RATE = 9600

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
print(f"Conectado a {SERIAL_PORT} a {BAUD_RATE} baudios.")

def connected(client):
    print("Conectado a Adafruit IO!")
    client.subscribe("joystick-cabeza")
    client.subscribe("joystick-hombros-izquierdo")
    client.subscribe("joystick-hombro-derecho")
    client.subscribe("joystick-antebrazo-izquierdo")
    client.subscribe("joystick-antebrazo-derecho")

def disconnected(client):
    print("Desconectado de Adafruit IO MQTT")

def message(client, feed_id, payload):
    print(f"{feed_id}: {payload}")
    try:
        valor = int(payload)
        if feed_id == "joystick-cabeza":
            ser.write(f"C:{valor}\n".encode())
        elif feed_id == "joystick-hombros-izquierdo":
            ser.write(f"H2:{valor}\n".encode())
        elif feed_id == "joystick-hombro-derecho":
            ser.write(f"H1:{valor}\n".encode())
        elif feed_id == "joystick-antebrazo-izquierdo":
            ser.write(f"B1:{valor}\n".encode())
        elif feed_id == "joystick-antebrazo-derecho":
            ser.write(f"B2:{valor}\n".encode())
    except ValueError:
        pass

def listen_serial():
    while True:
        if ser.in_waiting:
            line = ser.readline().decode(errors="ignore").strip()
            if line:
                print(f"[UART] {line}")

client = MQTTClient(AIO_USERNAME, AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

print("Conectando a Adafruit IO...")
client.connect()
threading.Thread(target=client.loop_blocking, daemon=True).start()
print("Escuchando feeds y enviando comandos UART...")

try:
    listen_serial()
except KeyboardInterrupt:
    print("Interrupción por teclado, cerrando...")
finally:
    client.disconnect()
    ser.close()
