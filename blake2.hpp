#include <blake2.h>
#include <string>
namespace blake2pp {
  namespace architecture {
    struct I64 {}; //Selection class for 64 bit architecture variants of blake2.
  }
  namespace cores {
    struct Multi {}; //selection class for multi-core variants of blake2.
  }
  namespace encoding {
    struct Base32 {};  //selection class for Base32 eincoding of output hashes.
  }
  template <typename CharType, size_t Keysize , size_t Outputsize, typename Architecture,typename Multicore, typename Encoding,typename traits=std::char_traits<CharType> >
  struct blake2 {
    blake2();
    blake2(std::basic_string<CharType,traits> key);
    ~blake2();
    void update(std::basic_string<CharType,traits> in);
    operator std::basic_string<CharType,traits>();
  };

  typedef blake2pp::blake2<char, 32, 32, blake2::architecture::I64, blake2::cores::Multi, blake2::encoding::Base32 > blake2bp;
  
}
