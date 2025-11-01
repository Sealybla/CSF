TODO: names of team members and their contributions to the project

TODO (for MS3): best cache report

lru
- write-allocate vs no-write allocate
- write-through vs write-back
- direct-mapped (n sets of 1 block)/m-way set-associative (n sets m blocks)/fully associative (1 set n blocks)
        - let n = 256, 512 m = 4, 8
        - set bytes to 16


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




