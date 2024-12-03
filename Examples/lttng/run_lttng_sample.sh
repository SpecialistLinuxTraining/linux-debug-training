echo "Creating an Lttng Session"
lttng create my-tracing-session --output=./


echo "Enabling a tracepoint event"
lttng enable-event --userspace sample_application:information_tracepoint

echo "Start Lttng tracing..."
lttng start

echo "Run application..."
./sample_application

echo "Destroying session"
lttng destroy

echo "Displaying trace information using babeltrace"
babeltrace ./ust/

sleep 1
echo "Cleaning up ..."
rm -rf ./ust
