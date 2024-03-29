#ifndef _smScan_h_
#define _smScan_h_

#include "alanCommon.h"

typedef enum smScScanner {
	sm_MAIN_MAIN_Scanner = 0,
	sm_MAIN_FILENAME_Scanner = 1
} smScScanner;

typedef enum sm_MAIN_ScToken {
	sm_MAIN_UNKNOWN_Token = 0,
	sm_MAIN_ENDOFTEXT_Token = 1,
	sm_MAIN_IDENTIFIER_Token = 2,
	sm_MAIN_INTEGER_Token = 3,
	sm_MAIN_4_Token = 4,
	sm_MAIN_5_Token = 5,
	sm_MAIN_6_Token = 6,
	sm_MAIN_7_Token = 7,
	sm_MAIN_8_Token = 8,
	sm_MAIN_9_Token = 9,
	sm_MAIN_10_Token = 10,
	sm_MAIN_11_Token = 11,
	sm_MAIN_12_Token = 12,
	sm_MAIN_13_Token = 13,
	sm_MAIN_14_Token = 14,
	sm_MAIN_15_Token = 15,
	sm_MAIN_16_Token = 16,
	sm_MAIN_17_Token = 17,
	sm_MAIN_18_Token = 18,
	sm_MAIN_19_Token = 19,
	sm_MAIN_20_Token = 20,
	sm_MAIN_21_Token = 21,
	sm_MAIN_22_Token = 22,
	sm_MAIN_23_Token = 23,
	sm_MAIN_24_Token = 24,
	sm_MAIN_25_Token = 25,
	sm_MAIN_26_Token = 26,
	sm_MAIN_27_Token = 27,
	sm_MAIN_28_Token = 28,
	sm_MAIN_29_Token = 29,
	sm_MAIN_30_Token = 30,
	sm_MAIN_31_Token = 31,
	sm_MAIN_32_Token = 32,
	sm_MAIN_33_Token = 33,
	sm_MAIN_34_Token = 34,
	sm_MAIN_35_Token = 35,
	sm_MAIN_36_Token = 36,
	sm_MAIN_37_Token = 37,
	sm_MAIN_38_Token = 38,
	sm_MAIN_39_Token = 39,
	sm_MAIN_40_Token = 40,
	sm_MAIN_41_Token = 41,
	sm_MAIN_42_Token = 42,
	sm_MAIN_43_Token = 43,
	sm_MAIN_44_Token = 44,
	sm_MAIN_45_Token = 45,
	sm_MAIN_46_Token = 46,
	sm_MAIN_47_Token = 47,
	sm_MAIN_48_Token = 48,
	sm_MAIN_49_Token = 49,
	sm_MAIN_50_Token = 50,
	sm_MAIN_51_Token = 51,
	sm_MAIN_52_Token = 52,
	sm_MAIN_53_Token = 53,
	sm_MAIN_54_Token = 54,
	sm_MAIN_55_Token = 55,
	sm_MAIN_56_Token = 56,
	sm_MAIN_57_Token = 57,
	sm_MAIN_STRING_Token = 58,
	sm_MAIN_59_Token = 59,
	sm_MAIN_60_Token = 60,
	sm_MAIN_61_Token = 61,
	sm_MAIN_62_Token = 62,
	sm_MAIN_63_Token = 63,
	sm_MAIN_64_Token = 64,
	sm_MAIN_65_Token = 65,
	sm_MAIN_66_Token = 66,
	sm_MAIN_67_Token = 67,
	sm_MAIN_68_Token = 68,
	sm_MAIN_69_Token = 69,
	sm_MAIN_70_Token = 70,
	sm_MAIN_71_Token = 71,
	sm_MAIN_72_Token = 72,
	sm_MAIN_73_Token = 73,
	sm_MAIN_74_Token = 74,
	sm_MAIN_75_Token = 75,
	sm_MAIN_76_Token = 76,
	sm_MAIN_77_Token = 77,
	sm_MAIN_78_Token = 78,
	sm_MAIN_79_Token = 79,
	sm_MAIN_80_Token = 80,
	sm_MAIN_81_Token = 81,
	sm_MAIN_82_Token = 82,
	sm_MAIN_83_Token = 83,
	sm_MAIN_84_Token = 84,
	sm_MAIN_85_Token = 85,
	sm_MAIN_86_Token = 86,
	sm_MAIN_87_Token = 87,
	sm_MAIN_88_Token = 88,
	sm_MAIN_89_Token = 89,
	sm_MAIN_90_Token = 90,
	sm_MAIN_91_Token = 91,
	sm_MAIN_92_Token = 92,
	sm_MAIN_93_Token = 93,
	sm_MAIN_94_Token = 94,
	sm_MAIN_95_Token = 95,
	sm_MAIN_96_Token = 96,
	sm_MAIN_97_Token = 97,
	sm_MAIN_98_Token = 98,
	sm_MAIN_99_Token = 99,
	sm_MAIN_100_Token = 100,
	sm_MAIN_101_Token = 101,
	sm_MAIN_102_Token = 102,
	sm_MAIN_103_Token = 103,
	sm_MAIN_104_Token = 104,
	sm_MAIN_105_Token = 105,
	sm_MAIN_106_Token = 106,
	sm_MAIN_107_Token = 107,
	sm_MAIN_108_Token = 108,
	sm_MAIN_109_Token = 109,
	sm_MAIN_110_Token = 110,
	sm_MAIN_111_Token = 111,
	sm_MAIN_112_Token = 112,
	sm_MAIN_113_Token = 113,
	sm_MAIN_114_Token = 114,
	sm_MAIN_115_Token = 115,
	sm_MAIN_116_Token = 116,
	sm_MAIN_117_Token = 117,
	sm_MAIN_118_Token = 118,
	sm_MAIN_119_Token = 119,
	sm_MAIN_120_Token = 120,
	sm_MAIN_121_Token = 121,
	sm_MAIN_122_Token = 122,
	sm_MAIN_123_Token = 123,
	sm_MAIN_124_Token = 124,
	sm_MAIN_125_Token = 125,
	sm_MAIN_126_Token = 126,
	sm_MAIN_127_Token = 127,
	sm_MAIN_128_Token = 128,
	sm_MAIN_129_Token = 129,
	sm_MAIN_130_Token = 130,
	sm_MAIN_131_Token = 131,
	sm_MAIN_132_Token = 132,
	sm_MAIN_133_Token = 133,
	sm_MAIN_134_Token = 134,
	sm_MAIN_135_Token = 135,
	sm_MAIN_136_Token = 136,
	sm_MAIN_137_Token = 137,
	sm_MAIN_138_Token = 138,
	sm_MAIN_139_Token = 139,
	sm_MAIN_140_Token = 140,
	sm_MAIN_141_Token = 141,
	sm_MAIN_142_Token = 142,
	sm_MAIN_143_Token = 143,
	sm_MAIN_144_Token = 144,
	sm_MAIN_145_Token = 145,
	sm_MAIN_146_Token = 146,
	sm_MAIN_147_Token = 147,
	sm_MAIN_148_Token = 148,
	sm_MAIN_149_Token = 149,
	sm_MAIN_150_Token = 150,
	sm_MAIN_BLANKS_Token = -1,
	sm_MAIN_COMMENT_Token = -1,
	sm_MAIN_BLOCK_COMMENT_Token = -1,
	sm_MAIN_INCLUDE_Token = -1
} sm_MAIN_ScToken;


typedef struct smScContextItem *smScContext;

extern smScContext smScNew( smScScanner smScanner );
extern void smScDelete( smScContext smThis );
extern int smScSkip( smScContext smThis, int smLength );
extern int smScModify( smScContext smThis, unsigned char *smBuffer, int smLength );
extern int smScCopy( smScContext smThis, unsigned char *smBuffer, unsigned int smFrom, unsigned int smTo );
extern int smScMapCopy( smScContext smThis, unsigned char *smBuffer, unsigned int smFrom, unsigned int smTo );
extern void smScScrScanner( smScScanner scanner, int smOnOff );
extern void smScScrToken( smScScanner scanner, int smCode, int smOnOff );
extern void smScScrRule( smScScanner scanner, int smCode, int smOnOff );
extern int smScan( smScContext smThis, Token *smToken );

/* %%EXPORT */


#include <iconv.h>

#include "sysdep.h"
#include "types.h"
#include "charset.h"

#include "lst_x.h"
#include "options.h"


extern smScContext lexContext;

/* List of file names */
extern List *fileNames;

extern bool smScanEnter(Srcp srcp, char fnm[], bool search);
extern int scannedLines(void);
extern void setCharacterSet(CharSet charset);


/* END %%EXPORT */

typedef struct smScContextItem {

/* Private */

  unsigned char *smBegining;
  unsigned char *smContinue;
  unsigned char *smEnd;
  unsigned char *smPosPrevLine;
  unsigned char *smPosLine;
  unsigned char *smPosContinue;

/* Public */

  int smSize;

  int smBufferOverflow;
  int smLength;
  unsigned char *smText;

  int smLine;
  int smColumn;
  int smPosition;
  int smNextLine;
  int smNextColumn;
  int smNextPosition;

  smScScanner smScanner;



  smScContext previous;
  int fd;
  char *fileName;
  int fileNo;
  int previousCharSet;
  iconv_t conversionDescriptor;




} smScContextItem;

#endif
