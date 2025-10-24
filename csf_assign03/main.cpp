#include <iostream>
#include <vector>
#include <map>
#include <cmath>
using namespace std;


//direct-mapped cache simulator
struct CacheBlock {
  int tag;
  bool valid;
  bool dirty;
  int last_used; //for LRU policy
  int load_time; //for FIFO policy
};

int lru(int set_index, map<int, vector<CacheBlock>> &cache, int num_block, int &total_cycles, CacheBlock &new_block, int pcycles_per_block) {
    vector<CacheBlock> &all_blocks = cache[set_index]; 
    //finding min total_cycles (most nonused)
    int min = total_cycles; 
    int min_idx = 0; 
    for (int i = 0; i < (int)num_block ; i++){
      CacheBlock block = all_blocks[i]; 
      if (block.last_used < min) {
        min = block.last_used;
        min_idx = i; 
      }
    }
    if (all_blocks[min_idx].dirty){
      //write back to memory before being replaced
      total_cycles += pcycles_per_block; 
    }
    total_cycles ++; //loading to cache takes 1 cycle
    all_blocks[min_idx] = new_block; 
    return min_idx; 
}

int main( int argc, char **argv ) {
  // TODO: implement
  //arg in following form: 
  // number of sets in the cache (a positive power-of-2)
  // number of blocks in each set (a positive power-of-2)
  // number of bytes in each block (a positive power-of-2, at least 4)
  // write-allocate or no-write-allocate
  // write-through or write-back
  // lru (least-recently-used) or fifo evictions
  if (argc != 7) {
    std::cerr << "Usage: " << argv[0] << "<num sets in cache (pos power of 2)> <num blocks each set (pos power of 2)> <num bytes per block (pos power of 2 >=4)> <write-allocate/no-write-allocate> <write-through/write-back> <lru/fifo eviction>" << std::endl;
    return 1;
  }
  size_t num_set = std::stoi(argv[1]);
  if (num_set <= 0 || (num_set & (num_set - 1)) != 0) {
    std::cerr << "Error: Number of sets must be a positive power of 2." << std::endl;
    return 1;
  }
  size_t num_block = std::stoi(argv[2]);
  if (num_block  <= 0 || (num_block  & (num_block  - 1)) != 0) {
    std::cerr << "Error: Number of blocks per set must be a positive power of 2." << std::endl;
    return 1;
  }
  size_t num_byte = std::stoi(argv[3]);
  if (num_byte < 4 || (num_byte & (num_byte - 1)) != 0) {
    std::cerr << "Error: Number of bytes per block must be a positive power of 2 and at least 4." << std::endl;
    return 1;
  }
  std::string write_allocate_policy = argv[4];
  if (write_allocate_policy != "write-allocate" && write_allocate_policy != "no-write-allocate") {
    std::cerr << "Error: Write policy must be either 'write-allocate' or 'no-write-allocate'." << std::endl;
    return 1;
  }
  std::string write_policy = argv[5];
  if (write_policy != "write-through" && write_policy != "write-back") {
    std::cerr << "Error: Write policy must be either 'write-through' or 'write-back'." << std::endl;
    return 1;
  }
  string eviction_policy = argv[6];
  if (eviction_policy != "lru" && eviction_policy != "fifo") {
    cerr << "Error: Eviction policy must be either 'lru' or 'fifo'." << endl;
    return 1;
  }



  //print total loads/stores, hits, misses, hit rate
  int total_loads = 0;
  int total_stores = 0;
  int load_hits = 0;
  int load_misses = 0;
  int store_hits = 0;
  int store_misses = 0;
  int total_cycles = 0;
  //simulate cache behavior based on parameters
  map<int, vector<CacheBlock>> cache; //map of set index to vector of cache blocks
  
  //read from file
  char operation;
  unsigned int address_str;
  int bytes;
  int block_size = num_byte;
  int pcycles_per_block = 100 * block_size/4; 
  while (cin>>operation>>std::hex>>address_str>>bytes) {
    //parse line for load/store and address
    //first character is s or l
    if (operation != 'l' && operation != 's') {
      cerr << "Error: Invalid operation '" << operation << "'. Expected 'l' or 's'." << endl;
      continue; //skip invalid lines
    }
    if (operation == 'l') {
      total_loads++;
    } else {
      total_stores++;
    }
    //extract set index and tag from address
    int b = log2(num_byte);
    int s = log2(num_set);
    int set_index = (address_str >> b) & ((1<<s) -1);
    int tag = address_str >> (b + s);
    cout<<tag; 
    //check if cache contains block (hit)
    bool hit = false;
    for (CacheBlock &block : cache[set_index]) {
      if (block.valid && block.tag == tag) {
        //hit = true
        cout<<"hit";
        hit = true;
        if (operation == 'l') {
          load_hits++;
          total_cycles += 1; //load hit takes 1 cycle
        } else {
          store_hits++;
          if (write_policy == "write-through") {//write to memory too
            total_cycles += pcycles_per_block; 
            total_cycles += 1; //store hit with write-through takes 101 cycles
          } else {
            total_cycles += 1; //store hit with write-back takes 1 cycle
            block.dirty = true; //mark block as dirty
          }
        }
        //update block metadata based on eviction policy
        if (eviction_policy == "lru") {
          block.last_used = total_cycles; //update last used time
        }
        break;
      }
    }
    if (!hit) {
      //miss
      if (operation == 'l') {
        load_misses ++; 
        total_cycles += pcycles_per_block; //load miss takes 100 cycles to fetch from memory
        //write block into cache
        CacheBlock new_block;
        new_block.tag = tag;
        new_block.valid = true;
        new_block.dirty = false;
        new_block.last_used = total_cycles;
        new_block.load_time = total_cycles;
        if (cache[set_index].size() < num_block) {
          cache[set_index].push_back(new_block); //space available in set
          total_cycles += 1; //loading to cache takes 1 cycle
        } else { //evict a block based on eviction policy
          if (eviction_policy == "lru") {
            lru(set_index, cache, num_block, total_cycles, new_block, pcycles_per_block);
          }
          else {//if (eviction_policy == "fifo")
            cout<<"heyheyhey";
          }
        }
      } else {//operation == 's'
        store_misses ++; 
        if (write_allocate_policy == "write-allocate") {
          //bring relevant memory block into cache before store proceeds
          total_cycles += pcycles_per_block; //bringing relevant memory takes 100 cycles
          CacheBlock new_block;
          new_block.tag = tag;
          new_block.valid = true;
          new_block.dirty = false;
          new_block.last_used = total_cycles;
          new_block.load_time = total_cycles;
          if (cache[set_index].size() < num_block) { //space available in set
            cache[set_index].push_back(new_block);
            total_cycles += 1; //loading to cache takes 1 cycle
            // perform the store that caused this miss: account cycles and mark dirty if write-back
            if (write_policy == "write-through") {// write-through: write goes to memory as well
              total_cycles += pcycles_per_block;
              total_cycles += 1; //store with write-through
            } else {// write-back: mark the newly loaded block dirty and charge the store cost             
              total_cycles += 1;
              cache[set_index].back().dirty = true;
            }
          } else {
            //evict a block based on eviction policy
            if (eviction_policy == "lru") {
              vector<CacheBlock> &all_blocks = cache[set_index];
              int min_idx = lru(set_index, cache, num_block, total_cycles, new_block, pcycles_per_block);
              if(write_policy == "write-through") {
                total_cycles += pcycles_per_block;
                total_cycles += 1;;  //store with write-through after miss
              } else {
                total_cycles += 1; //store with write-back after miss
                all_blocks[min_idx].dirty = true; //mark block as dirty
              }
            }
            else if (eviction_policy == "fifo") {//for next time}
              cout<<"heyheyhey";
            }
          }
        } else { //no write allocate- cache is not modified 
          total_cycles += pcycles_per_block; //writes to main memory 
        }
      }

    }
  }

  cout<<"Total loads: "<<total_loads<<endl;
  cout<<"Total stores: "<<total_stores<<endl;
  cout<<"Load hits: "<<load_hits<<endl;
  cout<<"Load misses: "<<load_misses<<endl;
  cout<<"Store hits: "<<store_hits<<endl;
  cout<<"Store misses: "<<store_misses<<endl;
  cout<<"Total cycles: "<<total_cycles<<endl;
  return 0;
}

