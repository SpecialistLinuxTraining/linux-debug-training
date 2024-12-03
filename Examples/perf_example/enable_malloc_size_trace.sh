echo "Add the probe point"
perf probe -x /usr/lib/x86_64-linux-gnu/libc.so.6 --add='malloc allocated=%ax:u32' # On X86, the eax register holds the size value
echo "Enable the newly added probe"
echo 1 > /sys/kernel/tracing/events/probe_libc/malloc/enable
echo "Enable tracing"
echo 1 > /sys/kernel/tracing/tracing_on
echo "Dump the trace pipe to observe traced events"
cat /sys/kernel/tracing/trace_pipe

