echo "List traceable events in the kernel"
lttng list --kernel
echo "Trace "sched_switch" event"
lttng create test_session --output=./test_trace
echo "Enable the event"
lttng enable-event -k sched_switch
echo "Start tracing"
lttng start
echo "Wait for 1 second"
sleep 1
echo "Destroy the session"
lttng destroy
echo "View 'sched_switch' event in trace file"
babeltrace ./test_trace | grep sleep
sleep 1
rm -rf ./test_trace