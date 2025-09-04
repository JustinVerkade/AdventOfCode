#include <stdint.h>

int32_t ip = 0;
int32_t a = 0;
int32_t b = 0;
int32_t c = 0;
int32_t d = 0;
int32_t f = 0;

// 0 seti 123 0 2           c = 123;
//                          GOTO_1:
// 1 bani 2 456 2           c = c & 456;
// 2 eqri 2 72 2            c = c == 72;
// 3 addr 2 4 4             ip = ip + c
//                          if (c) {
//                              goto GOTO_5;
//                          }
// 4 seti 0 0 4             ip = 0;
//                          goto GOTO_1
//                          GOTO_5:
// 5 seti 0 1 2             c = 0;
//                          GOTO_6:
// 6 bori 2 65536 5         f = c | 65536;
// 7 seti 16123384 4 2      c = 16123384;
//                          GOTO_8:
// 8 bani 5 255 3           d = f & 255;
// 9 addr 2 3 2             c = c + d;
//10 bani 2 16777215 2      c = c & 16777215;
//11 muli 2 65899 2         c = c * 65899;
//12 bani 2 16777215 2      c = c & 16777215;
//13 gtir 256 5 3           d = 256 > f;
//14 addr 3 4 4             ip = ip + d;
//                          if (d) {
//                              goto GOTO_16;
//                          }
//15 addi 4 1 4             ip = ip + 1;
//                          goto GOTO_17;
//                          GOTO_16:
//16 seti 27 6 4            ip = 27;
//                          goto GOTO_28;
//                          GOTO_17:
//17 seti 0 3 3             d = 0;
//                          GOTO_18:
//18 addi 3 1 1             b = d + 1;
//19 muli 1 256 1           b = b * 256;
//20 gtrr 1 5 1             b = b > f;
//21 addr 1 4 4             ip = ip + b;
//                          if (b) {
//                              goto GOTO_23;
//                          }
//22 addi 4 1 4             ip = ip + 1;
//                          goto GOTO_24;
//                          GOTO_23:
//23 seti 25 6 4            ip = 25;
//                          goto GOTO_26;
//                          GOTO_24:
//24 addi 3 1 3             d = d + 1;
//25 seti 17 3 4            ip = 17;
//                          goto GOTO_18;
//                          GOTO_26;
//26 setr 3 8 5             f = d;
//27 seti 7 2 4             ip = 7;
//                          goto GOTO_8;
//                          GOTO_28:
//28 eqrr 2 0 3             d = c == a;
//29 addr 3 4 4             ip = ip + d;
//                          if (d) {
//                              goto GOTO_31;
//                          }
//30 seti 5 3 4             ip = 5;
//                          goto GOTO_6;
//                          GOTO_32: