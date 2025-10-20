#!/usr/bin/env bash
set -e
RATE_HZ="${1:-5}"
DT=$((1000 / RATE_HZ))

echo "Simulating CAN traffic on vcan0 at ${RATE_HZ}Hz"

i=0
while true; do
  temp=$((3000 + (i % 1000)))   # 30.00°C–40.00°C
  hi=$(printf "%02X" $(( (temp >> 8) & 0xFF )))
  lo=$(printf "%02X" $(( temp & 0xFF )))
  cansend vcan0 100#${hi}${lo}
  cansend vcan0 101#01            # level OK
  cansend vcan0 102#01            # pump running
  cansend vcan0 200#41            # setpoint 65°C
  echo "Sent frame batch $i (temp=$((temp/100)).$((temp%100))°C)"
  i=$((i+1))
  sleep $(awk "BEGIN{print $DT/1000}")
done
