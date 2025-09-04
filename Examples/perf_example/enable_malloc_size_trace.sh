PERF=/mnt/500GB/linux-6.5/tools/perf/perf
echo "Add the probe point"
${PERF} probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --add='malloc allocated=%di:u64' # On X86_32 systems, the eax register holds the size value
echo "Enable the newly added probe"
echo 1 > /sys/kernel/tracing/events/probe_libc/malloc/enable
echo "Enable tracing"
echo 1 > /sys/kernel/tracing/tracing_on
echo "Dump the trace pipe to observe traced events"
cat /sys/kernel/tracing/trace_pipe

