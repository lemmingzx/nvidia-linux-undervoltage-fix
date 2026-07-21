# nvidia-linux-undervoltage-fix

> **Automated, event-driven patch for proprietary NVIDIA GPU performance locking on Linux hybrid platforms.**

---

## 📌 Overview

`nvidia-linux-undervoltage-fix` solves a persistent bug in `nvidia-powerd` where dropping below the system's low-battery threshold locks the GPU into performance state **P5** (underwattaged/clock-throttled). 
 
On affected hybrid Linux laptops (e.g., Lenovo IdeaPad Gaming series with AMD/NVIDIA), reconnecting AC power fails to trigger a daemon re-evaluation. This leaves the GPU stuck in a low-power state even after the battery recharges to 100%.

This repository provides an event-driven `udev` hardware trigger and a compiled C binary to automatically reset `nvidia-powerd` state tracking immediately upon plugging in the charger—consuming **0% background CPU and 0 MB RAM**.

---

## ⚡ How It Works

Unlike basic workarounds that rely on persistent `cron` jobs or background loops, this fix is **entirely event-driven**:

1. **Hardware Event:** Plugging in the AC adapter emits a `udev` event (`SUBSYSTEM=="power_supply"`).
2. **Execution:** The `udev` rule calls a lightweight, compiled C binary located at `/usr/bin/nvidia_bugfix`.
3. **Evaluation:** The C binary parses `/etc/UPower/UPower.conf` dynamically to check your system's low-battery threshold and reads `/sys/class/power_supply/BAT1/capacity`.
4. **Action:** If the AC charger is reconnected after a low-battery event, `systemctl restart nvidia-powerd` is executed in milliseconds to restore full GPU performance state (**P0**).

---
🤝 Credits & Acknowledgements
Shaurya Odedara (@lemmingzx/@shauryaservice) – Bug discovery, empirical state testing, system architecture, udev integration, and project maintainer.

Google Gemini – AI pair-programming assistant for initial C logic implementation and installation script scaffolding and the readme section writer.
---

## 🛠️ Installation

Clone the repository and run the installer script with root privileges:

```bash
git clone [https://github.com/lemmingzx/nvidia-linux-undervoltage-fix.git](https://github.com/lemmingzx/nvidia-linux-undervoltage-fix.git)
cd nvidia-linux-undervoltage-fix
chmod +x patch.sh
sudo ./patch.sh

