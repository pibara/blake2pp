#include <blake2.h>
#include <string>
namespace blake2pp {
  namespace architecture {
    struct I32 {};
    struct I64 {}; //Selection class for 64 bit architecture variants of blake2.
  }
  namespace cores {
    struct Single {};
    struct Multi {}; //selection class for multi-core variants of blake2.
  }
  namespace encoding {
    struct Base32 {
      static std::string encode(uint8_t *out); 
    };  //selection class for Base32 eincoding of output hashes.
    struct Hex {
      static std::string encode(uint8_t *out);
    };
  }
  namespace libb2 {
    template <typename Architecture,typename Multicore>
    struct state {};

    template <>
    struct state<blake2pp::architecture::I32, blake2pp::cores::Single> {
        int init();
        int init_key(const void *key);
        int update(const uint8_t *in, size_t inlen);
        int dofinal(uint8_t *out);
      private:
        blake2s_state s;
    };

    template <>
    struct state<blake2pp::architecture::I32, blake2pp::cores::Multi> {
        int init();
        int init_key(const void *key);
        int update(const uint8_t *in, size_t inlen);
        int dofinal(uint8_t *out);
      private:
        blake2sp_state s;
    };


    template <>
    struct state<blake2pp::architecture::I64, blake2pp::cores::Single> {
        int init();
        int init_key(const void *key);
        int update(const uint8_t *in, size_t inlen);
        int dofinal(uint8_t *out);
      private:
        blake2b_state s;
    };
    
    template <>
    struct state<blake2pp::architecture::I64, blake2pp::cores::Multi> {
        int init();
        int init_key(const void *key);
        int update(const uint8_t *in, size_t inlen);
        int dofinal(uint8_t *out);
      private:
        blake2bp_state s;
    };
    
  }
  template <typename Architecture,typename Multicore, typename Encoding >
  struct blake2 {
      blake2() { state.init(); }
      blake2(std::string key) {state.init_key(static_cast<const void *>(key.c_str()));}
      ~blake2(){}
      void update(std::string in) {
        if (res == "") {
          state.update(static_cast<const void *>(in.c_str()),in.size());
        }
      }
      operator std::string(){
        if (res == "") {
          uint8_t out[32];
          state.dofinal(out);
          res= Encoding::encode(out);
        }
        return res;
      }
    private:
      std::string res;
      blake2pp::libb2::state<Architecture,Encoding> state;
  };

  typedef blake2pp::blake2<blake2pp::architecture::I32, blake2pp::cores::Single, blake2pp::encoding::Base32 > blake2s;
  typedef blake2pp::blake2<blake2pp::architecture::I32, blake2pp::cores::Multi, blake2pp::encoding::Base32 > blake2sp;
  typedef blake2pp::blake2<blake2pp::architecture::I64, blake2pp::cores::Single, blake2pp::encoding::Base32 > blake2b;
  typedef blake2pp::blake2<blake2pp::architecture::I64, blake2pp::cores::Multi, blake2pp::encoding::Base32 > blake2bp;
  
}
