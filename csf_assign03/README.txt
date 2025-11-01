TODO: names of team members and their contributions to the project
Wendy - fifo function + debugging together + analysis of the best cache
Lijia - lru function + debugging together + analysis of the best cache
TODO (for MS3): best cache report

lru
- write-allocate vs no-write allocate
- write-through vs write-back
- direct-mapped (n sets of 1 block)/m-way set-associative (n sets m blocks)/fully associative (1 set n blocks)
        - let n = 256, 512 m = 4, 8
        - set bytes to 16

We used pcycles_per_block as a rough estimate for miss penalty to represent a comparison for miss penalty.

DATA: 

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/1/16/no-write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.882606
Total Cache size: 4096 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/1/16/no-write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.882606
Total Cache size: 4096 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/1/16/write-allocate/write-back/fifo
Miss Penalty: 400
Hit rate: 0.938687
Total Cache size: 4096 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/1/16/write-allocate/write-back/lru
Miss Penalty: 400
Hit rate: 0.938687
Total Cache size: 4096 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/1/16/write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.938687
Total Cache size: 4096 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/1/16/write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.938687
Total Cache size: 4096 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/4/16/no-write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.920569
Total Cache size: 16384 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/4/16/no-write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.923885
Total Cache size: 16384 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/4/16/write-allocate/write-back/fifo
Miss Penalty: 400
Hit rate: 0.973889
Total Cache size: 16384 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/4/16/write-allocate/write-back/lru
Miss Penalty: 400
Hit rate: 0.975499
Total Cache size: 16384 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/4/16/write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.973889
Total Cache size: 16384 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/4/16/write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.975499
Total Cache size: 16384 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/8/16/no-write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.923965
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/8/16/no-write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.925165
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/8/16/write-allocate/write-back/fifo
Miss Penalty: 400
Hit rate: 0.976074
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/8/16/write-allocate/write-back/lru
Miss Penalty: 400
Hit rate: 0.976906
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/8/16/write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.976074
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 256/8/16/write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.976906
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/1/16/no-write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.903596
Total Cache size: 8192 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/1/16/no-write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.903596
Total Cache size: 8192 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/1/16/write-allocate/write-back/fifo
Miss Penalty: 400
Hit rate: 0.958387
Total Cache size: 8192 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/1/16/write-allocate/write-back/lru
Miss Penalty: 400
Hit rate: 0.958387
Total Cache size: 8192 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/1/16/write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.958387
Total Cache size: 8192 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/1/16/write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.958387
Total Cache size: 8192 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/4/16/no-write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.923575
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/4/16/no-write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.924892
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/4/16/write-allocate/write-back/fifo
Miss Penalty: 400
Hit rate: 0.975875
Total Cache size: 32768 bytes


num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/4/16/write-allocate/write-back/lru
Miss Penalty: 400
Hit rate: 0.976722
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/4/16/write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.975875
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/4/16/write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.976722
Total Cache size: 32768 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/8/16/no-write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.925148
Total Cache size: 65536 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/8/16/no-write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.925644
Total Cache size: 65536 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/8/16/write-allocate/write-back/fifo
Miss Penalty: 400
Hit rate: 0.977075
Total Cache size: 65536 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/8/16/write-allocate/write-back/lru
Miss Penalty: 400
Hit rate: 0.977539
Total Cache size: 65536 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/8/16/write-allocate/write-through/fifo
Miss Penalty: 400
Hit rate: 0.977075
Total Cache size: 65536 bytes

num set/num block/num byte/write-allocate policy/write policy/eviction policy: 512/8/16/write-allocate/write-through/lru
Miss Penalty: 400
Hit rate: 0.977539
Total Cache size: 65536 bytes




The above display the data to the experiments that we ran. To find our best cache configuration, we prioritize the metrics in the following order: hit rate, cache size, miss penalty. With this order of prioritization, we find that
the configuration with the highest hit rate is for num set = 512, num block = 8, num byte = 15, write-allocate, write-through, and lru for gcc.trace. However, just taking hit rate 
in consideration may not work as well in the average case since the improvement of 0.01 percent would not really make that big of a difference. Instead, we advocate for the configuration 
with 256/4/16/write-allocate/write-back/lru due to it taking less than a quarter of the cache size with comparable hit rate performance of about 0.96, which should save much more resources and 
achieve comparable performance.
