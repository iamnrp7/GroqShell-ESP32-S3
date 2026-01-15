

````markdown
# GroqShell on ESP32-S3 – AI Powered UART Terminal

## Overview

This project implements an interactive AI shell on the **ESP32-S3** using the **Groq Cloud API** and the **LLaMA-3.1** model.  
The system connects the ESP32-S3 to Wi-Fi, accepts user queries through UART, sends them securely over HTTPS to Groq’s OpenAI-compatible endpoint, and prints the AI responses back on the serial terminal in real time.

This project demonstrates embedded networking, TLS communication, JSON parsing, and FreeRTOS task handling using **ESP-IDF**.

## Table of Contents

- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [WiFi Setup](#wifi-setup)
  - [UART AI Shell](#uart-ai-shell)
  - [Groq API Communication](#groq-api-communication)
- [Results](#results)

## Getting Started

### Prerequisites

To run this project, you will need:

- ESP32-S3 development board  
- ESP-IDF (v5.x recommended)  
- Groq API Key  
- USB cable  
- Serial terminal (minicom / putty / screen / Arduino Serial Monitor)  
- Internet-connected Wi-Fi network  

### Installation

1. Clone the repository:

```bash
git clone https://github.com/iamnrp7/GroqShell-ESP32-S3.git
cd GroqShell-ESP32-S3
````

2. Set the target and configure:

```bash
idf.py set-target esp32s3
idf.py menuconfig
```

3. Build and flash:

```bash
idf.py build
idf.py flash monitor
```

## Usage

### WiFi Setup

Edit the following in `main.c`:

```c
#define WIFI_SSID       "Your_WiFi_Name"
#define WIFI_PASSWORD   "Your_WiFi_Password"
```

### UART AI Shell

Open the serial monitor at **115200 baud**.
After boot, you will see:

```
>
```

Type a query and press Enter:

```
> What is a Tunnel FET?
```

### Groq API Communication

The ESP32-S3 sends the prompt to:

```
https://api.groq.com/openai/v1/chat/completions
```

Model used:

```
llama-3.1-8b-instant
```

The response is parsed using `cJSON` and printed on UART along with execution time.

## Results

* Successful HTTPS connection with TLS verification
* Real-time LLM inference using Groq Cloud
* Interactive serial-based AI terminal
* Measured response latency in milliseconds
* Stable operation using FreeRTOS tasks and ESP-IDF networking stack

```
```
