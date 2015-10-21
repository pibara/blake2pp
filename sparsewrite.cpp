#include "mattock-ohash.hpp"
#include <iostream>

template <typename OHash>
std::string sparse(bool skip) {
  OHash oh(true); //Writing assuming sparse.
  char buf[4096];
  for (int index=0;index<4096;index++) {
    buf[index]=index%128;
  }
  for (int index=1024;index<3073;index++) {
    buf[index]=0;
  }
  oh.written_chunk(buf,1024,0); //First part
  if (not skip) {
    oh.written_chunk(buf+1024,1024,1024); //Second part
    oh.written_chunk(buf+2048,1024,2048); //Third part
  }
  oh.written_chunk(buf+3072,1024,3072); //Last part
  oh.done();
  return oh.result();
}; 

template <typename OHash>
bool testsparse() {
  return sparse<OHash>(false) == sparse<OHash>(true);
};

int main(int argc,char **argv) {
  bool ok=true;
  if (testsparse<mattock::ohash_legacy>() == false) {
    std::cerr << "Sparsetest failed for legacy." << std::endl;
    ok=false;
  }
  if (testsparse<mattock::ohash_transitional>() == false) {
    std::cerr << "Sparsetest failed for transitional." << std::endl;
    ok=false;
  }
  if (testsparse<mattock::ohash>() == false) {
    std::cerr << "Sparsetest failed for new." << std::endl;
    ok=false;
  }
  if (ok == false) {
    return -1;
  }
  std::cerr << "OK" << std::endl;
  return 0;
}
