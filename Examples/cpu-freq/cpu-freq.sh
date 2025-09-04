#!/bin/bash

# ------------------------------------------------------------------------
# Script Name: CPU Frequency Profiling with Stress or sysbench Workload
#
# Description:
#   This script runs a synthetic CPU workload (using `stress` or `sysbench`) while capturing
#   detailed CPU frequency and performance metrics. It logs:
#     - CPU governor and Energy Performance Preference (EPP)
#     - Available frequency scaling settings via cpufreq-info
#     - Pre- and post-run CPU frequency snapshots
#     - Live frequency, power, and temperature using turbostat
#     - Instruction and cycle counts using perf stat
#
#   The results are saved in a timestamped log file under ./cpu_frequency_logs.
#
# Requirements:
#   - stress
#   - cpufrequtils (for cpufreq-info)
#   - turbostat
#   - perf
#
# sudo apt install cpufrequtils linux-tools-common linux-tools-$(uname -r) s-tui sysbench stress
#
# Usage:
#   Run this script as a regular user with sudo privileges (for turbostat).
#
# Author: [Manas Marawaha]: https://www.linkedin.com/in/manasmarwah/
# ------------------------------------------------------------------------

#APP_CMD="sysbench cpu --threads=4 run"
APP_CMD="stress --cpu 4 --timeout 15"
RUN_TIME=15
OUTPUT_DIR="cpu_frequency_logs"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
LOG_FILE="$OUTPUT_DIR/log_$TIMESTAMP.txt"

mkdir -p "$OUTPUT_DIR"

echo "=== CPU SpeedShift & Governor Check ===" | tee "$LOG_FILE"
echo "App: $APP_CMD" | tee -a "$LOG_FILE"
echo "Duration: $RUN_TIME seconds" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"

# Enable perf event monitoring
echo -e "\n=== Enabling perf event monitoring ===" | tee -a "$LOG_FILE"
echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid

# Capture cpufreq-info output
echo -e "\n=== cpufreq-info Output (Per-CPU Frequency Capabilities) ===" | tee -a "$LOG_FILE"
cpufreq-info | tee -a "$LOG_FILE"

# Show current governor and EPP
echo -e "\n=== Current CPU Governor and Energy Preference ===" | tee -a "$LOG_FILE"
for CPU in /sys/devices/system/cpu/cpu[0-9]*; do
  GOV=$(cat $CPU/cpufreq/scaling_governor)
  EPP=$(cat $CPU/cpufreq/energy_performance_preference 2>/dev/null || echo "N/A")
  echo "${CPU##*/}: Governor = $GOV | EPP = $EPP" | tee -a "$LOG_FILE"
done

# Show available EPP settings
echo -e "\n=== Available EPP Values (if supported) ===" | tee -a "$LOG_FILE"
cat /sys/devices/system/cpu/cpu0/cpufreq/energy_performance_available_preferences 2>/dev/null || echo "N/A" | tee -a "$LOG_FILE"

# CPU frequency before run
echo -e "\n=== CPU Frequency Snapshot Before Run ===" | tee -a "$LOG_FILE"
grep "MHz" /proc/cpuinfo | sort | uniq | tee -a "$LOG_FILE"


# Run turbostat
echo -e "\n=== turbostat Output ===" | tee -a "$LOG_FILE"
sudo turbostat --quiet --interval 1 --num_iterations $RUN_TIME \
  --show CPU,Core,Avg_MHz,Bzy_MHz,Bzy%,PkgWatt,PkgTmp,GFXWatt \
  timeout $RUN_TIME $APP_CMD 2>&1 | tee -a "$LOG_FILE"

# Run perf stat
echo -e "\n=== perf stat Output ===" | tee -a "$LOG_FILE"
perf stat -e cycles,instructions timeout $RUN_TIME $APP_CMD 2>&1 | tee -a "$LOG_FILE"

# CPU frequency after run
echo -e "\n=== CPU Frequency Snapshot After Run ===" | tee -a "$LOG_FILE"
grep "MHz" /proc/cpuinfo | sort | uniq | tee -a "$LOG_FILE"

# EPP after run
echo -e "\n=== Final CPU Governor and EPP ===" | tee -a "$LOG_FILE"
for CPU in /sys/devices/system/cpu/cpu[0-9]*; do
  GOV=$(cat $CPU/cpufreq/scaling_governor)
  EPP=$(cat $CPU/cpufreq/energy_performance_preference 2>/dev/null || echo "N/A")
  echo "${CPU##*/}: Governor = $GOV | EPP = $EPP" | tee -a "$LOG_FILE"
done

echo -e "\nDone. Output saved in $LOG_FILE"
