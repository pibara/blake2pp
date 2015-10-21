#include "mattock-ohash.hpp"
#include <iostream>

template <typename OHash>
std::string sequential() {
  OHash oh;
  char buf[128];
  for (int index=0;index<128;index++) {
    buf[index]=index;
  }
  for (int i=0;i<4096;i+=128) {
    oh.written_chunk(buf,128,i);
  };
  oh.done();
  return oh.result();
}; 

int main(int argc,char **argv) {
  bool ok=true;
  std::string legacyhash = sequential<mattock::ohash_legacy>();
  std::string transitionalhash = sequential<mattock::ohash_transitional>();
  std::string fasthash = sequential<mattock::ohash>();
  std::string expectedlegacy="7E34C64D5AEDAB60090857BF54093AF6440001AC";
  std::string expectednew="30E84B7FFDBBA730A3B19D1475D6E389B42BCD26873E3107E9416A9DE408B529";
  std::string expectedtransitional = expectednew + expectedlegacy; 
  if (legacyhash != expectedlegacy) {
    std::cerr << "Invalid legacy hash. Expecting " << expectedlegacy << ", got " << legacyhash << std::endl;
    ok=false;  
  }
  if (fasthash != expectednew) {
    std::cerr << "Invalid fast hash. Expecting " << expectednew << ", got " << fasthash << std::endl;
    ok=false;
  }
  if (transitionalhash != expectedtransitional) {
    std::cerr << "Invalid transitional hash. Expecting " << expectedtransitional << ", got " << transitionalhash << std::endl;
    ok=false;
  }
  if (ok == false) {
    return -1;
  }
  std::cerr << "OK" << std::endl;
  return 0;
}
