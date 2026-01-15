


# GroqShell on ESP32-S3 – AI Powered UART Terminal

## Overview

This project implements an interactive AI shell on the **ESP32-S3** using the **Groq Cloud API** with the **LLaMA-3.1** large language model.  
The system enables a user to communicate with a cloud-based LLM through a serial (UART) interface. User prompts are transmitted over Wi-Fi using HTTPS, processed by Groq’s OpenAI-compatible API, and the generated responses are displayed back on the serial terminal.

The project demonstrates practical embedded system integration of:
- Wi-Fi networking
- TLS secured REST communication
- JSON parsing
- FreeRTOS multitasking
- UART-based human–machine interface

--- 

## Table of Contents

- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [WiFi Configuration](#wifi-configuration)
  - [UART Shell Operation](#uart-shell-operation)
  - [Groq API Interface](#groq-api-interface)
- [Results](#results)

----

## Getting Started

### Prerequisites

The following hardware and software are required:

- ESP32-S3 development board  
- ESP-IDF (version 5.x recommended)  
- Groq Cloud API key  
- USB cable  
- Serial terminal (minicom / PuTTY / screen / Arduino Serial Monitor)  
- Internet-connected Wi-Fi network  

### Installation

1. Clone the repository:

```bash
git clone https://github.com/iamnrp7/GroqShell-ESP32-S3.git
cd GroqShell-ESP32-S3
````

2. Set target and configure the project:

```bash
idf.py set-target esp32s3
idf.py menuconfig
```

3. Build and flash the firmware:

```bash
idf.py build
idf.py flash monitor
```

---

## Usage

### WiFi Configuration

Update credentials in `main.c`:

```c
#define WIFI_SSID       "Your_WiFi_Name"
#define WIFI_PASSWORD   "Your_WiFi_Password"
```

### UART Shell Operation

Open the serial terminal at **115200 baud**.
After boot, the prompt appears:

```
>
```

Enter a query:

```
> Explain Tunnel FET
```

The response from the LLM will be printed along with execution time.

### Groq API Interface

* Endpoint: `https://api.groq.com/openai/v1/chat/completions`
* Model: `llama-3.1-8b-instant`
* Protocol: HTTPS with TLS certificate verification
* Format: JSON (parsed using cJSON)

---

## Results

* Successful secure HTTPS communication from ESP32-S3
* Interactive UART-based AI terminal
* End-to-end cloud LLM inference from embedded hardware
* Typical response latency: few hundred milliseconds
* Stable operation under FreeRTOS with Wi-Fi and TLS stack

```
```
