#!/usr/bin/env bash
set -e

ISO="build/myos.iso"
VNC_ADDR="127.0.0.1:5900"

# Start QEMU in background
qemu-system-x86_64 \
  -cdrom "$ISO" \
  &

QEMU_PID=$!

# Give QEMU a moment to start VNC
sleep 1

# Launch VNC viewer (pick one you have)
if command -v vncviewer >/dev/null; then
    vncviewer "$VNC_ADDR"
elif command -v tigervnc >/dev/null; then
    tigervnc "$VNC_ADDR"
elif command -v gvncviewer >/dev/null; then
    gvncviewer "$VNC_ADDR"
else
    echo "No VNC viewer found."
    echo "Install one of: tigervnc, realvnc, gvnc"
    wait "$QEMU_PID"
fi
