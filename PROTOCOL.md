# Umbra Advanced Framework Protocol Manual

This document defines the underlying communication architecture, binary framing format, and transit masking constraints enforced between listening posts and endpoint nodes.

---

## 1. Network Packet Serialization Layout
To minimize signature patterns and prevent packet drop traps, every data block sent across the socket must conform to a packed structural configuration before the network parsing stream reads it.

| Byte Offset | Data Type | Field Notation | Operational Function Description |
|---|---|---|---|
| 0x00 - 0x03 | uint32_t | Session ID | Unique framework operational handshake validation token |
| 0x04 - 0x07 | uint32_t | Opcode | Single task transaction index routed to process loops |
| 0x08 - 0x0B | uint32_t | Payload Length | Quantitative size in bytes of the trailing encrypted payload |
| 0x0C - 0x10 | uint32_t | Data Checksum | Modular arithmetic 32-bit verification checksum |

---

## 2. Dynamic Stream Encryption Masking (XOR Heuristics)
Data payload blocks traveling across transit lanes are fully masked to disrupt signature-based detection layers:
* The transaction header properties (`Session ID`, `Opcode`, `Payload Length`, `Checksum`) are transmitted raw to ensure optimal packet frame parsing performance.
* The trailing payload stream is automatically modified byte-by-byte using a single-byte symmetric bitwise masking key (`encryption_key_byte` value defined in `config.json`).
* Memory structures are unmasked sequentially inside process stacks during active run cycles and immediately wiped after execution blocks finish.

---

## 3. Core Operational Opcode Matrix

The control engine maps background operational routing routines to active client worker threads using specific unsigned 32-bit transaction tokens:

| Opcode ID | Protocol Macro | Vector Path Routing Direction | Functional Operational Description |
|---|---|---|---|
| **`100`** | `OP_HEARTBEAT` | Agent ──► Server Listener | Dispatched during handshake initialization and loop connection checks. |
| **`101`** | `OP_SHELL_EXEC` | Server ──► Target Node Agent | Passes encrypted command strings directly to native Win32 execution pipelines. |
| **`102`** | `OP_SYS_INFO` | Server ──► Target Node Agent | Forces the remote agent process loop to report client hardware characteristics. |
| **`999`** | `OP_DISCONNECT` | Server ──► Target Node Agent | Tears down active connections, closes socket parameters, and safely stops the process. |
