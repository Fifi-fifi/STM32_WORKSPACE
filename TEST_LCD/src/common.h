// (C)2016 by wek at efton dot sk

#ifndef COMMON_H__
#define COMMON_H__
  // some basic includes:

  #include <stdint.h>
  #include <stddef.h>

  #define CODE const // gcc_arm places const-tagged variables into FLASH

  // some preprocessor games
  #define SUPERGLUE(a, b, c, d) a##b##c##d
  #define GLUE(a, b) SUPERGLUE(a, b, , )
  #define GLUE4(a, b, c, d) SUPERGLUE(a, b, c, d)
  #define STRINGIFY2(a) #a
  #define STRINGIFY(a) STRINGIFY2(a)

  #define HWORD2BYTES(w)  (w) AND 0xFF, ((w) SHR 8) AND 0xFF
  #define WORD2BYTES(w)  (w) AND 0xFF, ((w) SHR 8) AND 0xFF, ((w) SHR 16) AND 0xFF, ((w) SHR 24) AND 0xFF

  #define ABS(X)  ((X) > 0 ? (X) : -(X))

  // compile-time assertion - if fails, throws error on incorrect (negative) array size
//  #define CT_ASSERT(cond) GLUE(typedef char assertion_ , __LINE__) [(cond) * 2 - 1];
  #define CT_ASSERT(cond) GLUE(typedef __attribute__((unused)) char assertion_ , __LINE__) [__builtin_constant_p(cond) ? ((cond) * 2 - 1) : 0];
  #define CT_GENERATE_SYMBOL(expression) void GLUE(dummy, __LINE__)(void) __attribute__((naked)); void GLUE(dummy, __LINE__)(void) {__asm__("//" #expression "\n\t" "// %0" "\n\t" ::"X" (expression));}

  // ... and we don't like C :-)
  #define ANL &&
  #define AND &
  #define OR |
  #define ORL ||
  #define SHL <<
  #define SHR >>
  #define XOR ^


  // games for binary include

  #if !defined(__STRINGIFY2)
  #define __STRINGIFY2(__p) #__p
  #define __STRINGIFY(__p) __STRINGIFY2(__p)
  #endif

  #define INCLUDE_BINARY_FILE(__type, __variable, __fileName, __section)           \
  __asm__ (                                                                        \
      ".pushsection " __section ",\"ax\",%progbits"                       "\n\t"    \
      ".global " __STRINGIFY(__variable)                                 "\n\t"    \
      ".global " __STRINGIFY(__variable) "_end"                          "\n\t"    \
      ".type " __STRINGIFY(__variable) ", %object"                       "\n"      \
      __STRINGIFY(__variable) ":"                                        "\n\t"    \
      ".incbin \"" __fileName "\""                                       "\n"      \
      __STRINGIFY(__variable) "_end" ":"                                 "\n\t"    \
      ".size " __STRINGIFY(__variable) ", .-" __STRINGIFY(__variable)    "\n\t"    \
      ".align 2"                                                         "\n\t"    \
      ".popsection"                                                                \
  );                                                                               \
  extern __type __variable __attribute__((section(__section)))


  // some of common bitfield stuff taken over from mxx/sm ucommon.h
  // general bit accessible variables

  typedef union __attribute__((packed)) {
    uint8_t all;
    struct __attribute__((packed)) { uint8_t nibH : 4, nibL : 4; };
    struct __attribute__((packed)) { uint8_t lsb: 1, : 6, msb: 1; };
    struct __attribute__((packed)) { uint8_t b0: 1, b1: 1, b2: 1, b3: 1, b4: 1, b5: 1, b6: 1, b7: 1; };
  } bit8;

  typedef union __attribute__((packed)) {
    uint16_t all;
    uint8_t byte[2];
    struct __attribute__((packed)) { uint8_t lsb: 1, : 7, : 7, msb: 1; };
    struct __attribute__((packed)) {
      uint8_t b0: 1, b1: 1, b2: 1, b3: 1, b4: 1, b5: 1, b6: 1, b7: 1;
      uint8_t b8: 1, b9: 1, b10: 1, b11: 1, b12: 1, b13: 1, b14: 1, b15: 1;
    };
  } bit16;

  typedef union __attribute__((packed)) {
    uint32_t  all;
    uint8_t byte[4];
    struct __attribute__((packed)) { uint8_t lsb: 1, : 7, : 8, : 8, : 7, msb: 1; };
    struct __attribute__((packed)) {
      uint8_t b0:  1, b1:  1, b2:  1, b3:  1, b4:  1, b5:  1, b6:  1, b7:  1;
      uint8_t b8:  1, b9:  1, b10: 1, b11: 1, b12: 1, b13: 1, b14: 1, b15: 1;
      uint8_t b16: 1, b17: 1, b18: 1, b19: 1, b20: 1, b21: 1, b22: 1, b23: 1;
      uint8_t b24: 1, b25: 1, b26: 1, b27: 1, b28: 1, b29: 1, b30: 1, b31: 1;
    };
  } bit32;

  typedef union __attribute__((packed)) {
    uint64_t all;
    uint32_t l[2];
    uint16_t h[4];
    uint8_t byte[8];
    struct __attribute__((packed)) {
      uint8_t n0 : 4, n1 : 4, n2 : 4, n3 : 4;
      uint8_t n4 : 4, n5 : 4, n6 : 4, n7 : 4;
      uint8_t n8 : 4, n9 : 4, n10: 4, n11: 4;
      uint8_t n12: 4, n13: 4, n14: 4, n15: 4;
    };
    struct __attribute__((packed)) { uint8_t lsb: 1, : 7, : 8, : 8, : 8, : 8, : 8, : 8, : 7, msb: 1; };
    struct __attribute__((packed)) {
      uint8_t b0:  1, b1:  1, b2:  1, b3:  1, b4:  1, b5:  1, b6:  1, b7:  1;
      uint8_t b8:  1, b9:  1, b10: 1, b11: 1, b12: 1, b13: 1, b14: 1, b15: 1;
      uint8_t b16: 1, b17: 1, b18: 1, b19: 1, b20: 1, b21: 1, b22: 1, b23: 1;
      uint8_t b24: 1, b25: 1, b26: 1, b27: 1, b28: 1, b29: 1, b30: 1, b31: 1;
      uint8_t b32: 1, b33: 1, b34: 1, b35: 1, b36: 1, b37: 1, b38: 1, b39: 1;
      uint8_t b40: 1, b41: 1, b42: 1, b43: 1, b44: 1, b45: 1, b46: 1, b47: 1;
      uint8_t b48: 1, b49: 1, b50: 1, b51: 1, b52: 1, b53: 1, b54: 1, b55: 1;
      uint8_t b56: 1, b57: 1, b58: 1, b59: 1, b60: 1, b61: 1, b62: 1, b63: 1;
    };
  } bit64;

  typedef union __attribute__((packed)) {
    struct __attribute__((packed)) {
      uint8_t fractional;
      uint8_t integer;
    };
    uint16_t raw;
  } fixed_8_8;

  typedef union __attribute__((packed)) {
    struct __attribute__((packed)) {
      uint8_t fractional3;
      uint8_t fractional2;
      uint8_t fractional1;
      uint8_t integer;
    };
    uint32_t  raw;
  } fixed_8_24;

  typedef void (*TVoidFunc)(void);

  #define RBIT_(X) __builtin_constant_p(X) ?  \
          (    (((X) SHR 7) AND 0x01)         \
            OR (((X) SHR 5) AND 0x02)         \
            OR (((X) SHR 3) AND 0x04)         \
            OR (((X) SHR 1) AND 0x08)         \
            OR (((X) SHL 1) AND 0x10)         \
            OR (((X) SHL 3) AND 0x20)         \
            OR (((X) SHL 5) AND 0x40)         \
            OR (((X) SHL 7) AND 0x80)         \
          ) : (__RBIT(X) SHR 24)


#endif  // COMMON_H__
