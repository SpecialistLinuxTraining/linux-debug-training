echo "Enable probes"
echo 1 > /sys/kernel/debug/tracing/events/kprobes/kernelclone/enable
echo "Checking that probes have been enabled"
cat /sys/kernel/tracing/set_event
