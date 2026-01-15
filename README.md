# GroqShell-ESP32-S3
ESP32-Groq-AI-Terminal is an interactive UART-based AI CLI built with ESP-IDF for ESP32-S3. It connects to Wi-Fi, sends user prompts to Groq’s LLM API, parses JSON responses, and displays clean, formatted AI output along with execution time.
Below is the **FINAL, CLEAN, READY-TO-UPLOAD `README.md`**.
It documents **everything meaningful you actually did**, is **professional**, and is suitable for **GitHub, interviews, and demos**.

👉 **Copy–paste this entire content into `README.md`**
(No edits required except Wi-Fi / API placeholders if you want)

---

```markdown
# GROQSHELL-ESP32-S3

An interactive **AI command-line terminal** built on **ESP32-S3** using **ESP-IDF**, allowing users to type questions via **UART (serial monitor)** and receive real-time responses from **Groq-hosted Large Language Models (LLMs)** over **Wi-Fi**.

This project demonstrates how a microcontroller can act as a practical **edge AI client**, handling networking, TLS, JSON parsing, and user interaction without Arduino or heavy SDKs.

---

## ✨ Features

- ✅ Pure **ESP-IDF** (no Arduino)
- ✅ ESP32-S3 Wi-Fi Station mode
- ✅ Interactive UART-based CLI
- ✅ Real-time AI responses from Groq API
- ✅ Clean, formatted AI output (no raw JSON)
- ✅ Execution time measurement per prompt
- ✅ Robust UART input handling (ENTER, backspace)
- ✅ Modular and readable code structure

---

## 🧠 System Overview

```

User (Serial Monitor)
↓
UART CLI (ESP32-S3)
↓
Wi-Fi + HTTPS (TLS)
↓
Groq Cloud API (LLM)
↓
JSON Parsing (cJSON)
↓
Formatted AI Response (UART)

```

---

## 🧰 Hardware Requirements

- ESP32-S3 development board
- USB cable
- Wi-Fi network
- Host PC (Linux recommended)

---

## 🧑‍💻 Software Requirements

- ESP-IDF v5.x
- Python 3.x
- Git
- Groq API key
- Linux / macOS / Windows

---

## 📁 Project Structure

```

GROQSHELL-ESP32-S3/
├── main/
│   ├── main.c        # UART CLI + Wi-Fi logic
│   ├── groq.c        # Groq API client
│   ├── groq.h
│   └── CMakeLists.txt
├── CMakeLists.txt
├── sdkconfig
├── README.md
├── .gitignore
└── LICENSE

````

---

## ⚙️ Configuration

### 1️⃣ Wi-Fi Credentials

Edit in `main.c`:

```c
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"
````

---

### 2️⃣ Groq API Key

Edit in `groq.c`:

```c
#define GROQ_API_KEY "YOUR_GROQ_API_KEY"
```

⚠️ **Do NOT commit your real API key to GitHub**
Rotate the key before making the repository public.

---

## 🧪 Key Development Steps (What Was Done)

### ✔ ESP-IDF Project Setup

* Created ESP32-S3 project using ESP-IDF
* Configured UART, Wi-Fi, and FreeRTOS tasks

---

### ✔ UART Interactive CLI

* Implemented **byte-by-byte UART input**
* Manual echo for characters
* Correct handling of:

  * ENTER (`\r`, `\n`)
  * Backspace (`0x08`, `0x7F`)
* Solved serial monitor input issues caused by buffering and line discipline

---

### ✔ Wi-Fi Connectivity

* Station mode (`WIFI_MODE_STA`)
* Event-driven Wi-Fi handling
* IP acquisition logging
* Retry logic on disconnection

---

### ✔ HTTPS + TLS Communication

* Used `esp_http_client`
* Enabled certificate validation using:

  ```c
  .crt_bundle_attach = esp_crt_bundle_attach
  ```
* Ensured secure HTTPS connection to Groq API

---

### ✔ Groq API Integration

* Sent chat completion requests to:

  ```
  https://api.groq.com/openai/v1/chat/completions
  ```
* Model used:

  ```
  llama-3.1-8b-instant
  ```

---

### ✔ JSON Parsing & Clean Output

* Buffered HTTP response
* Parsed JSON using **cJSON**
* Extracted only:

  ```
  choices[0].message.content
  ```
* Removed raw JSON output
* Displayed clean, readable AI responses

---

### ✔ Execution Time Measurement

* Used:

  ```c
  esp_timer_get_time()
  ```
* Measured end-to-end latency per prompt
* Displayed execution time in milliseconds

---

### ✔ NVS Encryption Investigation (Documented)

* Attempted encrypted NVS for API key storage
* Identified ESP-IDF v5.x HMAC/eFuse limitations on ESP32-S3
* Correctly disabled encrypted NVS for stability
* Final design uses **plain NVS only as required by Wi-Fi**

---

## 🚀 Build & Flash

```bash
idf.py set-target esp32s3
idf.py menuconfig
idf.py build
idf.py flash monitor
```

---

## 🖥️ Usage

Open the serial monitor after flashing.

Example interaction:

```
> What is Ohm's law?
```

Output:

```
AI RESPONSE
------------------------------
Ohm’s law states that the current through a conductor is directly proportional
to the voltage across it and inversely proportional to its resistance.
------------------------------
[Execution Time] 1243.62 ms
>
```

---

## 📸 Output Screenshot

Add your serial monitor screenshot here:

```
![UART Output](screenshots/output.png)
```

---

## ⚠️ Known Limitations

* Requires internet connection
* API usage limits apply
* No conversation memory
* Single-line input only

---

## 🛣️ Future Improvements

* [ ] Command history (↑ ↓)
* [ ] Streaming token output
* [ ] OLED / TFT display support
* [ ] Secure runtime key provisioning
* [ ] Multi-line prompts
* [ ] Offline inference (future)

---

## 📜 License

MIT License

---

## 🙌 Acknowledgements

* Espressif ESP-IDF Team
* Groq API
* ESP32 open-source community

---

## ⭐ Support

If you find this project useful:

* Star ⭐ the repository
* Fork 🍴 it
* Share feedback or improvements

Happy hacking with ESP32 & AI 🚀

```

---

### ✅ This is your **FINAL README**
- Accurate
- Honest
- Professional
- Matches your actual work
- Safe for GitHub

When you’re ready, upload your **output monitor screenshot**, add it to `screenshots/`, and commit.

If you want:
- **LICENSE file**
- **Badges**
- **Resume/portfolio version**
- **Demo GIF instructions**

Just tell me 👍
```
