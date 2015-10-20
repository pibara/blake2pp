#include <blake2.h>
#include <cryptopp/sha.h>
#include <string>
#include <stdint.h>

namespace mattock {
namespace compatibility {
  //Simple helper class for making libb2's BLAKE2BP behave somewhat the same as the Crypto++ hash classes.
  class BLAKE2BP {
      blake2bp_state s;
    public:
      enum {DIGESTSIZE = 32};
      BLAKE2BP():s(){
        blake2bp_init(&s,DIGESTSIZE);
      }
      void Update(const byte *input, size_t length){
        blake2bp_update(&s,input,length);
      }
      void Final(byte *digest){
        blake2bp_final(&s,digest,DIGESTSIZE);
      }
      void Restart() {
        blake2bp_init(&s,DIGESTSIZE);
      }
  };
}

namespace opportunistic {
template <typename Hash>
class hash {
    Hash h;
    uint64_t offset;
    bool isnew;
    bool issparse;
    byte r[Hash::DIGESTSIZE];
  public:
    hash(bool anew=false,bool asparse=false):h(),offset(0),isnew(anew),issparse(asparse && anew){
       memset(r,0,Hash::DIGESTSIZE);
    }
    void sparse(uint64_t len) {
        byte sparsenull[1024];
        memset(sparsenull,0,len);
        h.Update(sparsenull,len);
        offset += len;
    }
    void chunk(char *buf, size_t size, uint64_t off){
      if (isnew) {
        //We are processing a file that is just being created.
        //If the offset is 0 and something happened before, we need to restart.
        if (off < offset) {
            h.Restart();
            offset=0;
        }
        if (off == offset) {
          //We have an exact sequential match, we can always update here.
          h.Update(buf,size);
          offset = off + size;
        } else {
          //No seamlessness here, ignore or sparse.
          if (issparse) {
            //The file is being written sequentially but sparse, lets process the sparse data as zeroes.
            uint64_t difference = off - offset;
            uint64_t times = difference / 1024;
            for (uint64_t i=0; i< times;i++) {
              sparse(1024);
            }
            sparse(difference % 1024);
          }
        }
      } else {
        //We are processing an immutable existing file; this should be easy.
        if (off <= offset and (off+size) > offset) {
          //Fragment overlaps our offset; find the part that we didn't process yet.
          char *buffseg=buf + offset - off;
          size_t chunksize = size - (offset - off);
          //Process the yet unprocessed data chunk.
          h.Update(buffseg,chunksize);
          //Update our offset.
          offset = off + size;
        }
        //Anything else we must ignore.
      }  
    }
    void done() {
       h.Final(r);
    }
    uint64_t c_offset(){
      return offset;
    }
    byte *result() {
      return r;
    }
};
}

typedef mattock::opportunistic::hash<mattock::compatibility::BLAKE2BP> ohash;
typedef mattock::opportunistic::hash<CryptoPP::SHA> ohash_legacy;
 
}
