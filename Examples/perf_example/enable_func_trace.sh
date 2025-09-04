PERF=/mnt/500GB/linux-6.5/tools/perf/perf
echo "Add the probe point"
${PERF} probe  -x ./perf_example get_value_entry=get_value # create a probe get_value_entry in /sys/kernel/tracing/events/probe_perf_example
echo "Enable the newly added probe get_value_entry"
echo 1 > /sys/kernel/tracing/events/probe_perf_example/get_value_entry/enable
echo "Enable tracing"
echo 1 > /sys/kernel/tracing/tracing_on
echo "Dump the trace pipe to observe traced events"
cat /sys/kernel/tracing/trace_pipe

# Additional examples of probes
# perf probe  -x ./perf_example get_value_entry=get_value # create a probe get_value_entry in /sys/kernel/tracing/events/probe_perf_example
# perf probe  -x ./perf_example get_value_exit=get_value%return
# perf probe  -x ./perf_example get_value_line_number=main.c:25
# perf probe  -x ./perf_example get_value__return='get_value%return +0($retval):string'


# sudo perf probe -l # used to list existing user probes