PERF=/mnt/500GB/linux-6.5/tools/perf/perf
echo "Disabling tracing"
echo 0 > /sys/kernel/tracing/tracing_on
echo "Disabling the get_value_entry probe"
echo 0 > /sys/kernel/tracing/events/probe_perf_example/get_value_entry/enable
echo "Delete the get_value_entry probe"
sudo ${PERF} probe -d get_value_entry
