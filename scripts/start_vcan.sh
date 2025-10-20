#!/usr/bin/env bash
set -e

# Check if vcan0 already exists and run 
if ! ip link show vcan0 > /dev/null 2>&1; then
  echo "[INFO] Setting up virtual CAN interface (vcan0)"
  sudo modprobe vcan
  sudo ip link add dev vcan0 type vcan
  sudo ip link set up vcan0
  echo "[OK] vcan0 created and ready"
else
  echo "[OK] vcan0 already present"
fi

echo "To monitor traffic, run: candump vcan0"
