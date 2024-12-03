echo "Create an LTTng Session"
lttng create kernel-session --output=./kernel_trace
echo "Trace open, close and write syscall"
lttng enable-event --kernel --syscall open,close,write
echo "Start Tracing .... "
lttng start
echo "Perform a file operaion"
echo "test file" > ./testfile.txt
echo "Destroy the session"
lttng destroy
echo "View the trace"
babeltrace ./kernel_trace
echo "Clean up"
rm -rf ./testfile.txt
rm -rf ./kernel_trace
