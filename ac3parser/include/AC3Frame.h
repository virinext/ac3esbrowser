#ifndef AC3_H_
#define AC3_H_
#include <stdint.h>


enum AC3ChannelMode
{
    AC3_CHMODE_DUALMONO = 0,
    AC3_CHMODE_MONO,
    AC3_CHMODE_STEREO,
    AC3_CHMODE_3F,
    AC3_CHMODE_2F1R,
    AC3_CHMODE_3F1R,
    AC3_CHMODE_2F2R,
    AC3_CHMODE_3F2R
};



enum AC3ChannelLayout
{
  AC3_LAYOUT_MONO = 1,
  AC3_LAYOUT_STEREO = 2,
  AC3_LAYOUT_SURROUND = 4,
  AC3_LAYOUT_2_1 = 8,
  AC3_LAYOUT_4POINT0 = 16,
  AC3_LAYOUT_2_2 = 32,
  AC3_LAYOUT_5POINT0 = 64,
  AC3_LAYOUT_LOW_FREQUENCY = 128
  
};



enum EAC3FrameType
{
    EAC3_FRAME_TYPE_INDEPENDENT = 0,
    EAC3_FRAME_TYPE_DEPENDENT,
    EAC3_FRAME_TYPE_AC3_CONVERT,
    EAC3_FRAME_TYPE_RESERVED
};


struct syncinfo
{
  uint16_t          m_syncWord;
  uint16_t          m_crc1;
  uint8_t           m_fscod;
  uint8_t           m_frmsizecod;
};



struct bsi
{
  uint8_t           m_bsid;
  uint8_t           m_bsmod;
  uint8_t           m_acmod;
  int               m_cmixlev;
  int               m_surmixlev;
  int               m_dsurmod;
  uint8_t           m_lfeon;
  uint8_t           m_dialnorm;
  uint8_t           m_compre;
  uint8_t           m_compr;
  uint8_t           m_dialnorm2;
  uint8_t           m_compr2e;
  uint8_t           m_compr2;
  uint8_t           m_langcode;
  uint8_t           m_langcod;
  uint8_t           m_audprodie;
  uint8_t           m_mixlevel;
  uint8_t           m_roomtyp;
  uint8_t           m_langcod2e;
  uint8_t           m_langcod2;
  uint8_t           m_audprodi2e;
  uint8_t           m_mixlevel2;
  uint8_t           m_roomtyp2;
  uint8_t           m_strmtyp;
  uint8_t           m_substreamid;
  uint32_t          m_frmsiz;
  uint8_t           m_fscod2;
  uint8_t           m_numblkscod;
  uint8_t           m_chanmape;
  uint16_t          m_chanmap;
};


struct common
{
  uint16_t       m_sampleRate;
  uint32_t       m_bitrate;
  uint8_t        m_channels;
  uint16_t       m_frameSize;
  uint64_t       m_channelLayout;
  double         m_duration;
};


struct AC3Frame
{
  common        m_common;
  syncinfo      m_syncinfo;
  bsi           m_bsi;
};


#endif
