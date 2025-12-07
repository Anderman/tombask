import os
import serial
from datetime import datetime
import paho.mqtt.client as mqtt
import time
import json
from dotenv import load_dotenv

load_dotenv()
MQTT_HOST = os.getenv("MQTT_HOST", "192.168.2.4")   # bv. "192.168.2.10"
MQTT_PORT = int(os.getenv("MQTT_PORT", 1883))
MQTT_USER = os.getenv("MQTT_USER", "username")
MQTT_PASS = os.getenv("MQTT_PASS", "password")
SERIAL_PORT = os.getenv("SERIAL_PORT", "COM12")
BAUDRATE = int(os.getenv("BAUDRATE", 9600))

def send_mqtt_discovery(client: mqtt.Client) -> None:
    device = {
        "identifiers": ["tombask_boiler"],
        "name": "Tombask Boiler",
        "manufacturer": "Custom",
        "model": "SerialToMQTT",
    }

    sensors = [
        ("t1", "Air inlet temperature"),
        ("t2", "Top tank water temperature"),
        ("t3", "Bottom tank water temperature"),
        ("t4", "Discharge gas temperature"),
        ("t5", "Air outlet temperature"),
        ("t6", "Suction gas temperature"),
        ("t7", "T7"),
        ("t8", "Target temperature"),
    ]

    base_topic = "homeassistant/sensor"

    for key, name in sensors:
        unique_id = f"tombask_{key}"
        state_topic = f"tombask/{key}"
        config_topic = f"{base_topic}/{unique_id}/config"

        payload = {
            "name": name,
            "state_topic": state_topic,
            "unit_of_measurement": "°C",
            "unique_id": unique_id,
            "device": device,
        }

        client.publish(config_topic, json.dumps(payload), qos=1, retain=True)
def crc16_modbus(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc

def read_serial(port, baudrate):
    """Lees frames in een vaste bytearray.

    Layout volgens jouw omschrijving (indexen 0-based in de array):
    0: 0x7E (start1)
    1: 0x7E (start2)
    2: dst
    3: src
    4: code1
    5: ??? (bijv. code2 / type)
    6: length  (max 40)
    7..(7+length-2): payload (length - 1 bytes)
    daarna 2 CRC bytes
    daarna nog 1 laatste byte (slot/terminator)
    """

    ser = serial.Serial(port, baudrate, timeout=1)

    # Eén MQTT-client voor de hele looptijd
    client = mqtt.Client()
    client.username_pw_set(MQTT_USER, MQTT_PASS)
    client.connect(MQTT_HOST, MQTT_PORT, 60)

    # Stuur één keer discovery zodat HA de sensoren automatisch ziet
    send_mqtt_discovery(client)

    MAX_LENGTH = 0x40
    MAX_FRAME_SIZE = 2 + 4 + MAX_LENGTH + 2 + 1  # ruimschoots

    frame = bytearray(MAX_FRAME_SIZE)

    try:
        while True:
            # Wacht op eerste 0x7E
            b = ser.read(1)
            if not b:
                continue
            if b[0] != 0x7E:
                continue

            # Tweede 0x7E
            b2 = ser.read(1)
            if not b2:
                continue
            if b2[0] != 0x7E:
                # Niet echt een start, opnieuw zoeken
                continue

            # Start gevonden: vul vast eerste twee bytes in de array
            frame[0] = 0x7E
            frame[1] = 0x7E

            # Lees tot en met byte index 6 (dus 5 bytes erbij)
            needed = 4  # indices 2..6
            read_bytes = ser.read(needed)
            if len(read_bytes) != needed:
                # onvolledig, opnieuw beginnen
                continue

            for i, val in enumerate(read_bytes):
                frame[2 + i] = val

            length = frame[5]
            if length > MAX_LENGTH:
                # onzin, weggooien en opnieuw
                continue

            # payload: length - 1 bytes
            payload_len = max(length - 1, 0)
            payload = ser.read(payload_len)
            if len(payload) != payload_len:
                continue

            start_payload_index = 6
            for i, val in enumerate(payload):
                frame[start_payload_index + i] = val

            # daarna 2 CRC bytes
            crc_bytes = ser.read(2)
            if len(crc_bytes) != 2:
                continue

            crc_index = start_payload_index + payload_len
            frame[crc_index] = crc_bytes[0]
            frame[crc_index + 1] = crc_bytes[1]

            # laatste slotbyte
            last = ser.read(1)
            if not last:
                continue

            last_index = crc_index + 2
            frame[last_index] = last[0]

            # Totale frame-lengte die we gevuld hebben
            total_len = last_index + 1

            # Log het frame dat we nu in de array hebben
            used = frame[:total_len]

            # Bereken CRC over frame zonder CRC en zonder slot-byte
            crc_calc = crc16_modbus(used[:-3])  # alles t/m laatste payload-byte
            crc_low = crc_calc & 0xFF
            crc_high = (crc_calc >> 8) & 0xFF
            crc_ok = (crc_low == used[crc_index] and crc_high == used[crc_index + 1])

            timestamp = datetime.now().strftime("%m-%d %H:%M:%S")
            src=used[3];
            dst=used[2];
            log_line = (
                f"{timestamp} "
                + "Frame: start=7E7E dst=0x{:02X} src=0x{:02X} code1=0x{:02X} "
                "len={:02X} crc=0x{:02X}{:02X} last=0x{:02X} payload={} CRC_OK={}".format(
                    dst,        # dst
                    src,        # src
                    used[4],        # code1
                    length,
                    used[crc_index],
                    used[crc_index + 1],
                    used[last_index],
                    used[start_payload_index:start_payload_index + payload_len].hex(" "),
                    crc_ok,
                )
            )
            print(log_line)
            if src==0xF0 and dst==0xF0 and crc_ok and length==0x3c:
                # Verstuur naar MQTT: 8 temperaturen + gemiddelde (t_tm)
                t1 = used[9]  - 100
                t2 = used[10] - 100
                t3 = used[12] - 100
                t4 = used[17] - 100
                t5 = used[19] - 100
                t6 = used[20] - 100
                t7 = used[26] - 100
                t8 = used[52] - 100


                client.publish("tombask/t1",   str(t1),       qos=0, retain=False)
                client.publish("tombask/t2",   str(t2),       qos=0, retain=False)
                client.publish("tombask/t3",   str(t3),       qos=0, retain=False)
                client.publish("tombask/t4",   str(t4),       qos=0, retain=False)
                client.publish("tombask/t5",   str(t5),       qos=0, retain=False)
                client.publish("tombask/t6",   str(t6),       qos=0, retain=False)
                client.publish("tombask/t7",   str(t7),       qos=0, retain=False)
                client.publish("tombask/t8",   str(t8),       qos=0, retain=False)

            # Naar bestand c:\temp\tombask{length}.log
            log_dir = r"c:\\temp"
            os.makedirs(log_dir, exist_ok=True)
            log_path = os.path.join(log_dir, f"tombask2{length:02X}.log")
            with open(log_path, "a", encoding="utf-8") as f:
                f.write(log_line + "\n")
    finally:
        try:
            client.disconnect()
        except Exception:
            pass
        ser.close()


if __name__ == "__main__":
    read_serial(SERIAL_PORT, BAUDRATE)