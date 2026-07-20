#!/usr/bin/env python3
import argparse
import socket
import sys
import time

try:
    import paho.mqtt.client as mqtt
except ImportError:
    print("Missing dependency: paho-mqtt", file=sys.stderr)
    print("Install it with: python -m pip install paho-mqtt", file=sys.stderr)
    raise


def parse_args():
    parser = argparse.ArgumentParser(
        description="Forward raw RTCM packets from QGC UDP NTRIP forwarding to MQTT."
    )
    parser.add_argument("--udp-host", default="0.0.0.0", help="UDP bind address")
    parser.add_argument("--udp-port", type=int, default=21010, help="UDP bind port")
    parser.add_argument("--mqtt-host", default="47.112.204.68", help="MQTT broker host")
    parser.add_argument("--mqtt-port", type=int, default=1883, help="MQTT broker port")
    parser.add_argument("--mqtt-user", default="antg!@211520", help="MQTT username")
    parser.add_argument("--mqtt-password", default="admin", help="MQTT password")
    parser.add_argument("--topic", default="/gps", help="MQTT topic for raw RTCM")
    parser.add_argument("--qos", type=int, default=0, choices=(0, 1, 2), help="MQTT QoS")
    parser.add_argument(
        "--warn-non-rtcm",
        action="store_true",
        help="Warn when a UDP payload does not start with RTCM3 preamble 0xD3",
    )
    return parser.parse_args()


def main():
    args = parse_args()

    client = mqtt.Client(client_id=f"qgc-rtcm-bridge-{int(time.time())}")
    if args.mqtt_user:
        client.username_pw_set(args.mqtt_user, args.mqtt_password)

    print(f"Connecting MQTT {args.mqtt_host}:{args.mqtt_port} ...")
    client.connect(args.mqtt_host, args.mqtt_port, keepalive=30)
    client.loop_start()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((args.udp_host, args.udp_port))

    print(f"Listening UDP {args.udp_host}:{args.udp_port}")
    print(f"Forwarding raw bytes to MQTT topic {args.topic}")
    print("Do not close this window while testing. Press Ctrl+C to stop.")

    packets = 0
    bytes_total = 0

    try:
        while True:
            data, addr = sock.recvfrom(4096)
            if not data:
                continue

            packets += 1
            bytes_total += len(data)

            if args.warn_non_rtcm and data[0] != 0xD3:
                print(f"warning: packet from {addr} does not start with 0xD3: {data[:8].hex(' ')}")

            info = client.publish(args.topic, payload=data, qos=args.qos, retain=False)
            if args.qos:
                info.wait_for_publish()

            print(
                f"#{packets} {len(data)} bytes from {addr[0]}:{addr[1]} -> "
                f"{args.topic}, first={data[:8].hex(' ')}, total={bytes_total}"
            )
    except KeyboardInterrupt:
        print("\nStopping.")
    finally:
        sock.close()
        client.loop_stop()
        client.disconnect()


if __name__ == "__main__":
    main()
