# Umbra Advanced Post-Exploitation Framework

Umbra is a low-overhead adversarial simulation framework built to study modular data transit layers and native process virtualization loops. The architecture couples a multi-threaded asynchronous Python tracking listener with an independent native Windows C++ endpoint agent and a PHP transit stager communicating entirely across an encrypted custom binary serialization layout.

---

## 📊 Framework Metrics & Core Tooling

`Language: C++17 Standard` | `Language: Python 3.10+` | `Language: PHP 7.4+` | `Platform: Windows NT Targets` | `License: MIT`

---

## 🧬 Architectural Topology

The framework decouples orchestration and deployment from backend network routing layers, operating across three specialized operational systems:

```text
         [ Operator Management Console ]
                       │
                       ▼
  ┌─────────────────────────────────────────┐
  │          Umbra Python Core C2           │ ◄─── [ Ingests: config.json ]
  └─────────────────────────────────────────┘
                       ▲
                       │  (Symmetric XOR Streams)
                       │  (Opcodes: 100, 101, 102, 999)
                       ▼
  ┌─────────────────────────────────────────┐
  │     Native Windows C++ Runtime Agent    │ ◄─── [ Automated CI Validation Check ]
  └─────────────────────────────────────────┘
```

1. **The Core Listener (`server.py`)**: An asynchronous multi-threaded engine running thread pools to process multiple socket streams, handle incoming target reporting packets, and host an interactive operator command-line interface.
2. **The Execution Agent (`agent.cpp`)**: A standalone C++ application that detaches itself entirely from console visibility loops. It manages long-term connections, verifies session authorization tracking tokens, and pipes commands to anonymous system channels.
3. **The Web Stager Engine (`delivery.php`)**: An HTTP/HTTPS transit component that handles payload stage-hosting and data collection pipelines without creating persistent raw TCP socket pathways.

---

## 🛠️ Repository Anatomy

* `.github/workflows/ci.yml` — Automated continuous integration pipeline verifying script syntax and compiler builds on every push.
* `config.json` — Global workspace configuration properties file storing connection ports and network masking validation tokens.
* `server.py` — Asynchronous multi-threaded listener post script and control shell environment interface.
* `agent.cpp` — Native Windows client implementation hosting background execution subroutines and in-memory decryption routines.
* `delivery.php` — Hardened web delivery script protecting deployment files from unvalidated scanner assets.
* `Makefile` — Multi-stage compilation file structuring automated build parameters into a single short command block.
* `Dockerfile` / `docker-compose.yml` — Container architecture layers enabling zero-dependency deployment capabilities across host clouds.
* `PROTOCOL.md` — In-depth networking specifications logging packet structural byte offsets and transactional opcodes.

---

## ⚙️ Compilation, Orchestration & Deployment

Follow these structured instructions to configure your parameters, initialize your tracking infrastructure, and compile binaries for endpoint testing.

### Step 1: Central Properties Mapping
Open the main workspace file `config.json` and adjust connection hosts, ports, and encryption parameters to match your target network environment constraints:

```json
{
  "network": {
    "listener_host": "0.0.0.0",
    "port": 4444
  },
  "security": {
    "session_token_id": 994821,
    "user_agent_signature": "UmbraAgent/1.0",
    "encryption_key_byte": 165
  }
}
```

### Step 2: Launch the Infrastructure Post
Deploy the server listening array natively via Python 3, or boot the entire unified multi-container system automatically using Docker Compose:

```bash
# Method A: Direct execution routing
python3 server.py

# Method B: Production Docker orchestration spin-up
docker-compose up --build -d
```

### Step 3: Run the Local Build System
Instead of copying long cross-compilation strings manually every time you adjust properties, leverage the embedded `Makefile` to output your background binary:

```bash
# Flush legacy build logs and clear temporary caches
make clean

# Compile the target C++ agent using an optimized toolchain
make
```

#### Underlying Compilation Directives Enforced:
* `-std=c++17`: Matches memory-safe structural standard data type patterns.
* `-lws2_32`: Links the runtime binary context against the Microsoft Winsock 2 network socket driver.
* `-s`: Strips debugging symbols, metadata indexes, and internal trace logs to maintain minimal footprint.
* `-O3`: Applies aggressive optimization layouts to loop paths.
* `-mwindows`: Hides the runtime loop completely, preventing console terminal windows from displaying on target screens.

---

## 🛰️ Operational Console Interface Guide

Once an authorized connection registers with the control server, manage endpoints using the custom terminal interactive options:

* **`help`** — Show the administrative terminal navigation grid.
* **`info`** — Collect machine telemetry properties and operating system metrics natively without spawning wrappers.
* **`exec <command>`** — Instruct the agent to unpack arguments, clear processing buffers, and pipe task strings to an anonymous background process.
* **`exit`** — Safely close open socket pipelines and drop active memory loops to end tracking cleanly.
