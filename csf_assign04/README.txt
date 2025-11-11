CONTRIBUTIONS

TODO: write a brief summary of how each team member contributed to
the project.
Lijia: Worked on code together and analyzed Experiments
Wendy: Worked on code together and analyzed Experiments
REPORT

TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.
Test run with threshold 2097152

real    0m0.385s
user    0m0.368s
sys     0m0.012s
Test run with threshold 1048576

real    0m0.218s
user    0m0.370s
sys     0m0.033s
Test run with threshold 524288

real    0m0.157s
user    0m0.428s
sys     0m0.030s
Test run with threshold 262144

real    0m0.128s
user    0m0.440s
sys     0m0.049s
Test run with threshold 131072

real    0m0.110s
user    0m0.447s
sys     0m0.062s
Test run with threshold 65536

real    0m0.109s
user    0m0.464s
sys     0m0.079s
Test run with threshold 32768

real    0m0.113s
user    0m0.483s
sys     0m0.115s
Test run with threshold 16384

real    0m0.123s
user    0m0.524s
sys     0m0.153s

The computation works by when the region is big, parallel quicksort is run and otherwise without parallelization. The forking works by running 
the left subtree of the computation on one process and the right subtree of the computation on antoher process (which has no conflicts due to the structure of quicksort).
Since the OS can schedule both processes on different CPU cores, we can run processes in parallel. At first, with a higher threshold, there are less parallel processes getting
run. As the threshold does down, more parallel processes are being run and we see a speedup on the real and system time (with an increase on user time since more processes on more CPUs mean more user time)
. The performance does down again at threshold 16384 due to how the time to fork so many processes costing over the benefit of having the parallelization. 
