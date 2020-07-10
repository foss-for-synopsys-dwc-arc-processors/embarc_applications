
 MEMORY {
  // Note: overlap of code and data spaces is not recommended since it makes
  //   Address validity checking impossible with the debugger and simulator
    MyBlock: ORIGIN = 0x00002000, LENGTH = 32K
    MEMORY_BLOCK1:  ORIGIN = 0x0010000, LENGTH = 64K
    MEMORY_BLOCK2:  ORIGIN = 0x0020000, LENGTH = 128K
 }

SECTIONS {
    GROUP: {
          modify_seg: {}
          }>MyBlock

    GROUP: {
          * (TEXT): {}
          }>MEMORY_BLOCK1

    GROUP: {
          .sdata?: {}
          .sbss?: { * {.sbss} }
          _SDA_BASE_=SIZEOF(.sdata)+SIZEOF(.sbss)<= 255?ADDR(.sdata):ADDR(.sdata)+256;
          * (LIT): {}
          *(DATA): {}
          *(BSS): {}
          .stack SIZE(DEFINED _STACKSIZE?_STACKSIZE:65536): {}
          .heap? SIZE(DEFINED _HEAPSIZE?_HEAPSIZE:0): {}
          }> MEMORY_BLOCK2
     }
