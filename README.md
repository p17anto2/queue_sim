Queue Simulation 
----------------

Simulates a M/M/2/10 Queue, where the second server wakes up after stage K=6.

Works on Linux, and uses gnuplot (and make).

It creates 4 plots:

- 2D Time - Average State
- 2D Arrival Rate - Average State and Throughput
- 3D Server_A Departure Rate - Server_B Departure Rate - Average State
- 3D Server_A Departure Rate - Server_B Departure Rate - Throughput

## How to use

To create a build folder, compile, run and open the generated png images with xdg-open, use 
```
make
```


If you want to rerun it,
```
make clean && make
```
