#pragma once

#include "registr.h"


namespace FLASH_ral {

enum Latency { _0 = 0, _1, _2, _3,_4, _5, _6, _7 };

struct ACRbits {
   Latency  LATENCY :3; // Bits 2:0 LATENCY[2:0]: Latency
   uint32_t res1    :5; // Bits 7:3 Reserved, must be kept cleared.
   bool     PRFTEN  :1; // Bit 8 PRFTEN: Prefetch enable
   bool     ICEN    :1; // Bit 9 ICEN: Instruction cache enable
   bool     DCEN    :1; // Bit 10 DCEN: Data cache enable
   bool     ICRST   :1; // Bit 11 ICRST: Instruction cache reset
   bool     DCRST   :1; // Bit 12 DCRST: Data cache reset
   uint32_t res2    :19; // Bits 31:13 Reserved, must be kept cleared.
};

struct ACRposition {
   enum { LATENCY = 0, PRFTEN = 8, ICEN, DCEN, ICRST, DCRST };
};

struct SRbits {
   bool     EOP    :1; // Bit 0 EOP: End of operation
   bool     OPERR  :1; // Bit 1 OPERR: Operation error
   uint32_t res1   :2; // Bits 3:2 Reserved, must be kept cleared.
   bool     WRPERR :1; // Bit 4 WRPERR: Write protection error
   bool     PGAERR :1; // Bit 5 PGAERR: Programming alignment error
   bool     PGPERR :1; // Bit 6 PGPERR: Programming parallelism error
   bool     PGSERR :1; // Bit 7 PGSERR: Programming sequence error
   uint32_t res2   :8; // Bits 15:8 Reserved, must be kept cleared.
   bool     BSY    :1; // Bit 16 BSY: Busy
   uint32_t res3   :15; // Bits 31:17 Reserved, must be kept cleared.
};

struct SRposition {
   enum { EOP = 0, OPERR, WRPERR = 4, PGAERR, PGPERR, PGSERR, BSY = 16 };
};

enum Sector { s0 = 0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11 };
enum ProgSize { x8  = 0b00, x16, x32, x64 };

struct CRbits {
   bool     PG    :1; // Bit 0 PG: Programming
   bool     SER   :1; // Bit 1 SER: Sector Erase
   bool     MER   :1; // Bit 2 MER: Mass Erase
   Sector   SNB   :4; // Bits 6:3 SNB[3:0]: Sector number
   uint32_t res1  :1; // Bit 7 Reserved, must be kept cleared.
   ProgSize PSIZE :2; // Bits 9:8 PSIZE[1:0]: Program size
   uint32_t res2  :6; // Bits 15:10 Reserved, must be kept cleared.
   bool     STRT  :1; // Bit 16 STRT: Start
   uint32_t res3  :7; // Bits 23:17 Reserved, must be kept cleared.
   bool     EOPIE :1; // Bit 24 EOPIE: End of operation interrupt enable
   bool     ERRIE :1; // Bit 25 ERRIE: Error interrupt enable
   uint32_t res4  :5; // Bits 30:26 Reserved, must be kept cleared.
   bool LOCK      :1; // Bit 31 LOCK: Lock

};

struct CRposition {
   enum { PG = 0, SER, MER, SNB, PSIZE = 8, STRT = 16, EOPIE = 24, ERRIE, LOCK = 31 };
};

struct ACR_t     : BitsRegistr<ACRbits>, Offset_t<0x00>, ACRposition {};
struct KEYR_t    : DataRegistr         , Offset_t<0x04>              {};
struct OPTKEYR_t : DataRegistr         , Offset_t<0x08>              {};
struct SR_t      : BitsRegistr<SRbits> , Offset_t<0x0C>, SRposition  {};
struct CR_t      : BitsRegistr<CRbits> , Offset_t<0x10>, CRposition  {};
struct OPTCR_t   : DataRegistr         , Offset_t<0x14>              {};
struct OPTCR1_t  : DataRegistr         , Offset_t<0x18>              {};




} // namespace FLASH_ral {


