#include "AC3ParserImpl.h"
#include "BitstreamReader.h"

#include <sstream>
#include <cassert>
#include <iostream>



#define AC3_HEADER_SIZE 7


static const uint16_t ac3_frame_size_tab[38][3] = {
    { 64,   69,   96   },
    { 64,   70,   96   },
    { 80,   87,   120  },
    { 80,   88,   120  },
    { 96,   104,  144  },
    { 96,   105,  144  },
    { 112,  121,  168  },
    { 112,  122,  168  },
    { 128,  139,  192  },
    { 128,  140,  192  },
    { 160,  174,  240  },
    { 160,  175,  240  },
    { 192,  208,  288  },
    { 192,  209,  288  },
    { 224,  243,  336  },
    { 224,  244,  336  },
    { 256,  278,  384  },
    { 256,  279,  384  },
    { 320,  348,  480  },
    { 320,  349,  480  },
    { 384,  417,  576  },
    { 384,  418,  576  },
    { 448,  487,  672  },
    { 448,  488,  672  },
    { 512,  557,  768  },
    { 512,  558,  768  },
    { 640,  696,  960  },
    { 640,  697,  960  },
    { 768,  835,  1152 },
    { 768,  836,  1152 },
    { 896,  975,  1344 },
    { 896,  976,  1344 },
    { 1024, 1114, 1536 },
    { 1024, 1115, 1536 },
    { 1152, 1253, 1728 },
    { 1152, 1254, 1728 },
    { 1280, 1393, 1920 },
    { 1280, 1394, 1920 },
};

const uint8_t ac3_channels_tab[8] = {
  2, 1, 2, 3, 3, 4, 4, 5
};

const uint16_t ac3_bitrate_tab[19] = {
  32, 40, 48, 56, 64, 80, 96, 112, 128,
  160, 192, 224, 256, 320, 384, 448, 512, 576, 640
};

const uint16_t ac3_sample_rate_tab[3] = { 48000, 44100, 32000 };

const uint32_t ac3_channel_layout_tab[8] = {
    AC3_LAYOUT_STEREO,
    AC3_LAYOUT_MONO,
    AC3_LAYOUT_STEREO,
    AC3_LAYOUT_SURROUND,
    AC3_LAYOUT_2_1,
    AC3_LAYOUT_4POINT0,
    AC3_LAYOUT_2_2,
    AC3_LAYOUT_5POINT0
};



void AC3ParserImpl::process(const uint8_t *pdata, std::size_t size)
{
  std::size_t pos = 0;
  while(pos < size - 1)
  {
    if(pdata[pos] == 0x0B && pdata[pos + 1] == 0x77)
    {
      EventInfo info;
      info.m_position = pos;
      pos += processFrame(pdata + pos, size - pos, &info);
    }
    else
      pos++;
  }
}


std::size_t AC3ParserImpl::processFrame(const uint8_t *pdata, std::size_t size, const EventInfo *pevent)
{
  AC3Frame frame;
  BitstreamReader bs(pdata, size);
  bs.getBits(16);
  frame.m_bsi.m_cmixlev = 5;
  frame.m_bsi.m_surmixlev = 6;
  
  frame.m_bsi.m_bsmod = bs.showBits(29) & 0x1F;
  
  if(frame.m_bsi.m_bsmod > 16)
  {
    std::ostringstream ss;
    ss << "value `" << (int) frame.m_bsi.m_bsmod << "` is incorrect for bsmod"; 
    onWarning(ss.str(), pevent);
  }
  
  std::size_t res;
  if(frame.m_bsi.m_bsmod <= 10)
    res = processAC3Header(&bs, &frame, pevent);
  else
    res = processEAC3Header(&bs, &frame, pevent);

  frame.m_common.m_duration = (double) frame.m_common.m_frameSize / (frame.m_common.m_bitrate / 8);
  frame.m_common.m_channelLayout = ac3_channel_layout_tab[frame.m_bsi.m_acmod];

  if(frame.m_bsi.m_lfeon)
    frame.m_common.m_channelLayout |= AC3_LAYOUT_LOW_FREQUENCY;
  
  onFrame(&frame, pevent);

  return res;
}


std::size_t AC3ParserImpl::processAC3Header(BitstreamReader *pbs, AC3Frame *pframe, const EventInfo *pevent)
{
  assert(pframe != NULL);
  assert(pbs != NULL);
  assert(pevent != NULL);

  pframe -> m_syncinfo.m_crc1 = pbs -> getBits(16);
  pframe -> m_syncinfo.m_fscod = pbs -> getBits(2);
  
  if(pframe -> m_syncinfo.m_fscod == 3)
  {
    std::stringstream ss;
    ss << "value `" << (int) 3 << "` is incorrect for fscod";
    onWarning(ss.str(), pevent);
  }
  
  pframe -> m_syncinfo.m_frmsizecod  = pbs -> getBits(6);

  if(pframe -> m_syncinfo.m_frmsizecod > 37)
  {
    std::stringstream ss;
    ss << "value `" << (int) pframe -> m_syncinfo.m_frmsizecod << "` is incorrect for frmsizecod"; 
    onWarning(ss.str(), pevent);
    return 2;
  }
  
  pframe -> m_bsi.m_bsid = pbs -> getBits(5);
  pframe -> m_bsi.m_bsmod = pbs -> getBits(3);
  pframe -> m_bsi.m_acmod = pbs -> getBits(3);
  
  
  if(pframe -> m_bsi.m_acmod == AC3_CHMODE_STEREO) 
  {
    pframe -> m_bsi.m_dsurmod = pbs -> getBits(2);
  } 
  else 
  {
    if((pframe -> m_bsi.m_acmod & 1) && pframe -> m_bsi.m_acmod != AC3_CHMODE_MONO)
      pframe -> m_bsi.m_cmixlev  = pbs -> getBits(2);
    if(pframe -> m_bsi.m_acmod & 4)
      pframe -> m_bsi.m_surmixlev = pbs -> getBits(2);
  }
  
  pframe -> m_bsi.m_lfeon = pbs -> getBits(1);
  pframe -> m_bsi.m_dialnorm = pbs -> getBits(5);
  pframe -> m_bsi.m_compre = pbs -> getBits(1);
  
  if(pframe -> m_bsi.m_compre)
    pframe -> m_bsi.m_compr = pbs -> getBits(8);

  pframe -> m_bsi.m_langcode = pbs -> getBits(1);
  if(pframe -> m_bsi.m_langcode)
    pframe -> m_bsi.m_langcod = pbs -> getBits(8);
  
  uint8_t srShift = (pframe -> m_bsi.m_bsid > 8 ? pframe -> m_bsi.m_bsid : 8) - 8;
  
  if(pframe -> m_syncinfo.m_fscod != 3)
    pframe -> m_common.m_sampleRate = ac3_sample_rate_tab[pframe -> m_syncinfo.m_fscod] >> srShift;
  
  if(pframe -> m_syncinfo.m_frmsizecod <= 37)
  {
    pframe -> m_common.m_bitrate = (ac3_bitrate_tab[pframe -> m_syncinfo.m_frmsizecod >> 1] * 1000) >> srShift;
    pframe -> m_common.m_frameSize = ac3_frame_size_tab[pframe -> m_syncinfo.m_frmsizecod][pframe -> m_syncinfo.m_fscod] * 2;
  }
  
  pframe -> m_common.m_channels = ac3_channels_tab[pframe -> m_bsi.m_acmod] + pframe -> m_bsi.m_lfeon;

  pframe -> m_bsi.m_strmtyp = EAC3_FRAME_TYPE_AC3_CONVERT;
  pframe -> m_bsi.m_substreamid = 0;
  pframe -> m_bsi.m_numblkscod = 6;
  
  return pframe -> m_common.m_frameSize;
}


std::size_t AC3ParserImpl::processEAC3Header(BitstreamReader *pbs, AC3Frame *pframe, const EventInfo *pevent)
{
  assert(pframe != NULL);
  assert(pbs != NULL);
  assert(pevent != NULL);

  pframe -> m_syncinfo.m_crc1 = 0;
  pframe -> m_bsi.m_strmtyp = pbs -> getBits(2);
  
  if(pframe -> m_bsi.m_strmtyp == EAC3_FRAME_TYPE_RESERVED)
  {
    std::stringstream ss;
    ss << "value `" << (int) 3 << "` is incorrect for strmtyp";
    onWarning(ss.str(), pevent);
  }

  pframe -> m_bsi.m_substreamid = pbs -> getBits(3);
  
  pframe -> m_bsi.m_frmsiz = pbs -> getBits(11);

  if(pframe -> m_bsi.m_frmsiz < AC3_HEADER_SIZE)
  {
    std::stringstream ss;
    ss << "value `" << (int) pframe -> m_bsi.m_frmsiz << "` is too small for frmsiz (must be >= " << (int) AC3_HEADER_SIZE << ")";
    onWarning(ss.str(), pevent);
  }
  
  pframe -> m_syncinfo.m_fscod = pbs -> getBits(2);
  
  if(pframe -> m_syncinfo.m_fscod == 3)
  {
    pframe -> m_bsi.m_fscod2 = pbs -> getBits(2);
    if(pframe -> m_bsi.m_fscod2 == 3)
    {
      std::stringstream ss;
      ss << "value `" << (int) 3 << "` is incorrect for fscod2";
      onWarning(ss.str(), pevent);
    }
    else
      pframe -> m_common.m_sampleRate = ac3_sample_rate_tab[pframe -> m_bsi.m_fscod2] / 2;

    pframe -> m_bsi.m_numblkscod = 3;
  }
  else
  {
    pframe -> m_bsi.m_numblkscod = pbs -> getBits(2);
    pframe -> m_common.m_sampleRate = ac3_sample_rate_tab[pframe -> m_syncinfo.m_fscod];
  }

  pframe -> m_bsi.m_acmod = pbs -> getBits(3);
  pframe -> m_bsi.m_lfeon = pbs -> getBits(1);
  pframe -> m_bsi.m_bsid = pbs -> getBits(5);
  
  pframe -> m_bsi.m_dialnorm = pbs -> getBits(5);
  pframe -> m_bsi.m_compre = pbs -> getBits(1);

  if(pframe -> m_bsi.m_compre)
    pframe -> m_bsi.m_compr = pbs -> getBits(8);
  
  if(pframe -> m_bsi.m_acmod == 0)
  {
    pframe -> m_bsi.m_dialnorm2 = pbs -> getBits(5);
    pframe -> m_bsi.m_compr2e = pbs -> getBits(1);

    if(pframe -> m_bsi.m_compr2e)
      pframe -> m_bsi.m_compr2 = pbs -> getBits(8);
  }
  
  if(pframe -> m_bsi.m_strmtyp == EAC3_FRAME_TYPE_DEPENDENT)
  {
    pframe -> m_bsi.m_chanmape = pbs -> getBits(1);
    if(pframe -> m_bsi.m_chanmape)
      pframe -> m_bsi.m_chanmap = pbs -> getBits(16); 
  }

  
  pframe -> m_common.m_bitrate = (uint32_t)(8.0 * pframe -> m_bsi.m_frmsiz * pframe -> m_common.m_sampleRate / 
                                           (pframe -> m_bsi.m_numblkscod * 256.0));
  pframe -> m_common.m_channels = ac3_channels_tab[pframe -> m_bsi.m_acmod];
  pframe -> m_common.m_frameSize = pframe -> m_bsi.m_frmsiz;
  
  return pframe -> m_common.m_frameSize;
}



void AC3ParserImpl::addConsumer(Consumer *pconsumer)
{
  m_consumers.push_back(pconsumer);
}


void AC3ParserImpl::releaseConsumer(Consumer *pconsumer)
{
  m_consumers.remove(pconsumer);
}

void AC3ParserImpl::onFrame(const AC3Frame *pframe, const EventInfo *pinfo)
{
  std::list<Consumer *>::const_iterator itr = m_consumers.begin();

  for(; itr != m_consumers.end(); itr++)
    (*itr) -> onFrame(pframe, pinfo);
}


void AC3ParserImpl::onWarning(const std::string &msg, const EventInfo *pinfo)
{
  std::list<Consumer *>::const_iterator itr = m_consumers.begin();

  for(; itr != m_consumers.end(); itr++)
    (*itr) -> onWarning(msg, pinfo);
}
