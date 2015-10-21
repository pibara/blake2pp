#include <blake2.h>
#include <cryptopp/sha.h>
#include <string>
#include <stdint.h>
#include <cstdio>

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
  //Simple helper class for making a transitional BLAKE2BP/SHA1 comined hash behave somewhat the same 
  //as the Crypto++ hash classes.
  class TRANSITIONAL {
    blake2bp_state s;
    CryptoPP::SHA s2;
    public:
      enum {DIGESTSIZE = 52};
      TRANSITIONAL():s(){
        blake2bp_init(&s,32);
      }
      void Update(const byte *input, size_t length){
        blake2bp_update(&s,input,length);
        s2.Update(input,length);
      }
      void Final(byte *digest){
        blake2bp_final(&s,digest,32); //Write the BLAKE2BP hash first.
        s2.Final(digest+32); //Than the SHA1 after that.
      }
      void Restart() {
        blake2bp_init(&s,32);
        s2.Restart();
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
    std::string res;
  public:
    hash(bool anew=false,bool asparse=true):h(),offset(0),isnew(anew),issparse(asparse && anew),res("BOGUS-HASH"){}
    void sparse(uint64_t len) {
        byte sparsenull[1024];
        memset(sparsenull,0,len);
        h.Update(sparsenull,len);
        offset += len;
    }
    //The opportunistic hashing function will be fed chunks as user processes read/write to/from the pseudo file.
    void chunk(const char *buf, size_t size, uint64_t off){
      if (isnew) {
        //We are processing a file that is just being created.
        //If the offset is 0 and something happened before, we need to restart and ignore our previous work.
        if (off < offset) {
            h.Restart();
            offset=0;
        }
        if (off > offset) {
          //There is a gap. 
          if (issparse) {
            //The file is being written sequentially but sparse, lets process the sparse data as zeroes.
            uint64_t difference = off - offset;
            uint64_t times = difference / 1024;
            for (uint64_t i=0; i< times;i++) {
              sparse(1024);
            }
            sparse(difference % 1024);
          }
          //If not sparse, than ignore this chunk.
        }
        if (off == offset) {
          //We have an exact sequential match (possibly resulting from rewind or sparse filling). 
          //We can update here using our chunk data.
          h.Update(reinterpret_cast<const byte *>(buf),size);
          offset = off + size;
        }
      } else {
        //We are processing an immutable existing file; this should be easy.
        if (off <= offset and (off+size) > offset) {
          //Fragment overlaps our offset; find the part that we didn't process yet.
          const char *buffseg=buf + offset - off;
          size_t chunksize = size - (offset - off);
          //Process the yet unprocessed data chunk.
          h.Update(reinterpret_cast<const byte *>(buffseg),chunksize);
          //Update our offset.
          offset = off + size;
        }
        //Anything else we must ignore.
      }  
    }
    void done() {
       byte r[Hash::DIGESTSIZE];
       char hex[Hash::DIGESTSIZE*2];
       h.Final(r);
       for (int i=0;i<Hash::DIGESTSIZE;i++) {
         std::sprintf(hex+2*i, "%02X", r[i]);
       }
       res=std::string(hex,Hash::DIGESTSIZE*2);
    }
    uint64_t c_offset(){
      return offset;
    }
    std::string result() {
      return res;
    }
};
}

typedef mattock::opportunistic::hash<mattock::compatibility::BLAKE2BP> ohash;
typedef mattock::opportunistic::hash<mattock::compatibility::TRANSITIONAL> ohash_transitional;
typedef mattock::opportunistic::hash<CryptoPP::SHA> ohash_legacy;
 
}
