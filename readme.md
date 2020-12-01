# Producer and Consumer implementation in C++

This repository stores a C++ implementation of the producer consumer problem using semaphores and IPC with shared memory. There is no output, but the files run as expected. The code is commented to be easily readable. The files can be compiled as follows:

```
gcc producer.cpp -pthread -lrt -o producer
gcc consumer.cpp -pthread -lrt -o consumer
```

The compiled binaries can be run with the line:

`
./producer & ./consumer &
`
