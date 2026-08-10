# Charon Framework

An architectural, low-overhead post-exploitation and adversarial simulation framework. The platform provides a multi-threaded, asynchronous Python tracking engine coupled with an independent native Windows C++ endpoint agent and a specialized PHP transit stager.

## 📊 Project Metadata & Tooling

`Language: C++17` | `Language: Python 3.10+` | `Language: PHP 7.4+` | `Platform: Windows NT Target` | `License: MIT`

---

## 🧬 Framework Blueprint

Charon operates as a single unified framework across three distinct operational layers:

1. **Charon Listening Post (`server.py`)**: An asynchronous multi-threaded master process tracking socket pipelines, validating packet streams, and executing commands via an interactive shell.
2. **Charon Native Agent (`agent.cpp`)**: A discrete, multi-threaded C++ client executing completely detached from console screens. It maps commands directly to anonymous system pipes.
3. **Charon Ferryman (`delivery.php`)**: An HTTP/HTTPS transit stager used to ferry compiled binaries down to target nodes and capture incoming data logs without maintaining open socket connections.

---

## ⚙️ Compilation & Deployment

### 1. Host the Core Listening Post
Execute the server file using Python 3 to open your main network sockets:
```bash
python3 server.py
```

### 2. Configure and Deploy the Stager
Place `delivery.php` onto your Apache or Nginx web root directory along with your compiled agent binary file. Ensure permissions allow local log generation:
```bash
cp delivery.php /var/www/html/
```

### 3. Compile the Native Client
Build your C++ source file using an optimized compiler command structure to strip out symbols and run silently in the background:
```bash
x86_64-w64-mingw32-g++ agent.cpp -o agent.exe -lws2_32 -s -O3 -mwindows
```

---

## 🛰️ Console Syntax Matrix
* `help` — Review the core administrative interface navigation mapping grid.
* `info` — Query underlying system hardware telemetry properties natively.
* `exec <system_command>` — Route task strings to background process instances.
* `exit` — Safe socket teardown sequence.
