#include <iostream>
using namespace std;


//direct-mapped cache simulator


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
    std::cerr << "Usage: " << argv[0] << "<num sets in cache (pos power of 2)> 
    <num blocks each set (pos power of 2)> <num bytes per block (pos power of 2 >=4)> 
    <write-allocate/no-write-allocate> <write-through/write-back> <lru/fifo eviction>" << std::endl;
    return 1;
  }
  int set_size = std::stoi(argv[1]);
  if (set_size <= 0 || (set_size & (set_size - 1)) != 0) {
    std::cerr << "Error: Number of sets must be a positive power of 2." << std::endl;
    return 1;
  }
  int block_size = std::stoi(argv[2]);
  if (block_size <= 0 || (block_size & (block_size - 1)) != 0) {
    std::cerr << "Error: Number of blocks per set must be a positive power of 2." << std::endl;
    return 1;
  }
  int byte_size = std::stoi(argv[3]);
  if (byte_size < 4 || (byte_size & (byte_size - 1)) != 0) {
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
  //read from file, stdin, etc. for load/store operations
  //simulate cache behavior based on parameters
  string line; 
  while (cin<<line) {
    //parse line for load/store and address
    //first character is s or l
    char operation = line[0];
    if (operation != 'l' && operation != 's') {
      cerr << "Error: Invalid operation '" << operation << "'. Expected 'l' or 's'." << endl;
      continue; //skip invalid lines
    }
    //rest is address in hex
    string address_str = line.substr(4, line.length() - 6);
    unsigned long address;
    //update cache state and statistics
  }

  cout<<"Total loads: "<<0<<endl;
  cout<<"Total stores: "<<0<<endl;
  cout<<"Load hits: "<<0<<endl;
  cout<<"Load misses: "<<0<<endl;
  cout<<"Store hits: "<<0<<endl;
  cout<<"Store misses: "<<0<<endl;
  cout<<"Total cycles: "<<0<<endl;
  return 0;
}
