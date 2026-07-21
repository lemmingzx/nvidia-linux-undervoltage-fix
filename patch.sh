#!/bin/bash

# 1. Check if the user is running this as root (sudo)
if [ "$EUID" -ne 0 ]; then
  echo "❌ Error: Please run this installer with sudo."
  echo "Usage: sudo ./install_patch.sh"
  exit 1
fi

echo "🚀 Starting NVIDIA Bugfix Installation..."

# 2. Check if the raw C file is in the same folder
if [ ! -f "nvidia_bugfix.c" ]; then
    echo "❌ Error: nvidia_bugfix.c not found in the current directory!"
    exit 1
fi

# 3. Compile the C code (FIXED: compiling output directly as nvidia_bugfix)
echo "⚙️  Compiling raw C code..."
gcc -o nvidia_bugfix nvidia_bugfix.c
if [ $? -ne 0 ]; then
    echo "❌ Error: Compilation failed. Do you have gcc installed? (sudo apt install gcc)"
    exit 1
fi

# 4. Move and set permissions (FIXED: moving and chmodding the exact same name)
echo "📦 Moving binary to /usr/bin/nvidia_bugfix..."
mv nvidia_bugfix /usr/bin/nvidia_bugfix
chmod +x /usr/bin/nvidia_bugfix

# 5. Create the udev hardware trigger
echo "🔗 Setting up hardware trigger (udev rule)..."
RULE_FILE="/etc/udev/rules.d/99-nvidia-ac-fix.rules"
echo 'SUBSYSTEM=="power_supply", ATTR{type}=="Mains", ATTR{online}=="1", RUN+="/usr/bin/nvidia_bugfix"' > $RULE_FILE

# 6. Reload system rules
echo "🔄 Reloading system rules..."
udevadm control --reload-rules
udevadm trigger

echo "✅ Patch applied successfully! The fix is now permanently installed and will run automatically when you plug in the charger."
