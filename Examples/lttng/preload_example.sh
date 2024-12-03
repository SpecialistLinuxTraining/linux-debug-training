echo "Creating an Lttng Session"
lttng create my-tracing-session --output=./


echo "Enabling a tracepoint event for the sample application and libc calls"
lttng enable-event --userspace sample_application:information_tracepoint,lttng_ust_libc:malloc,lttng_ust_libc:free


echo "Start Lttng tracing..."
lttng start

echo "Run application..."
LD_PRELOAD=liblttng-ust-libc-wrapper.so:liblttng-ust-dl.so ./sample_application

echo "Destroying session"
lttng destroy

echo "Displaying trace information using babeltrace"
babeltrace ./ust/

echo "Cleaning up"
rm -rf ./ust




