#include "blake2.hpp"
#include <blake2.h>
#include <string>
namespace blake2pp {
  namespace encoding {
    std::string Base32::encode(uint8_t *out) {
       return "Bogus";
    }
    std::string Hex::encode(uint8_t *out) {
       return "Bogus";
    }
  }
  namespace libb2 {
    int state<blake2pp::architecture::I32, blake2pp::cores::Single>::init() {
      return blake2s_init(&s,32);
    } 
    int state<blake2pp::architecture::I32, blake2pp::cores::Multi>::init() {
      return blake2sp_init(&s,32);
    }    
    int state<blake2pp::architecture::I64, blake2pp::cores::Single>::init() {
      return blake2b_init(&s,32);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Multi>::init() {
      return blake2bp_init(&s,32);
    }
    int state<blake2pp::architecture::I32, blake2pp::cores::Single>::init_key(const void *key) {
      return blake2s_init_key(&s,32,key,32);
    }
    int state<blake2pp::architecture::I32, blake2pp::cores::Multi>::init_key(const void *key) {
      return blake2sp_init_key(&s,32,key,32);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Single>::init_key(const void *key) {
      return blake2b_init_key(&s,32,key,32);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Multi>::init_key(const void *key) {
      return blake2bp_init_key(&s,32,key,32);
    }
    int state<blake2pp::architecture::I32, blake2pp::cores::Single>::update(const uint8_t *in, size_t inlen) {
      return blake2s_update(&s,in,inlen);
    }
    int state<blake2pp::architecture::I32, blake2pp::cores::Multi>::update(const uint8_t *in, size_t inlen) {
      return blake2sp_update(&s,in,inlen);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Single>::update(const uint8_t *in, size_t inlen) {
      return blake2b_update(&s,in,inlen);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Multi>::update(const uint8_t *in, size_t inlen) {
      return blake2bp_update(&s,in,inlen);
    }
    int state<blake2pp::architecture::I32, blake2pp::cores::Single>::dofinal(uint8_t *out) {
      return blake2s_final(&s,out,32);
    }
    int state<blake2pp::architecture::I32, blake2pp::cores::Multi>::dofinal(uint8_t *out) {
      return blake2sp_final(&s,out,32);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Single>::dofinal(uint8_t *out) {
      return blake2b_final(&s,out,32);
    }
    int state<blake2pp::architecture::I64, blake2pp::cores::Multi>::dofinal(uint8_t *out) {
      return blake2bp_final(&s,out,32);
    }
  }
}
