#include <boost/test/unit_test.hpp>

#include "Params.h"
#include "AC3Frame.h"
#include "AC3Parser.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

BOOST_AUTO_TEST_SUITE(parsing);


class Consumer: public AC3Parser::Consumer
{
  public:
    struct Event
    {
      AC3Frame    m_frame;
      EventInfo   m_info;
    };
    
    struct Warning
    {
      std::string         m_warning;
      EventInfo           m_info;
    };

    virtual void onFrame(const AC3Frame *pframe, const EventInfo *pinfo)
    {
      Event event;
      event.m_frame = *pframe;
      event.m_info = *pinfo;
      m_frames.push_back(event);      
    }
    
    virtual void onWarning(const std::string &wrng, const EventInfo *pinfo)
    {
      Warning warning;
      warning.m_warning = wrng;
      warning.m_info = *pinfo;
      m_warnings.push_back(warning);
    }
    
    std::vector<Event>    m_frames;
    std::vector<Warning>  m_warnings;
};

BOOST_AUTO_TEST_CASE(ALCORN_TEST_FIRST_5)
{
  AC3Parser *pparser = AC3Parser::create();
  
  Consumer consumer;
  
  pparser -> addConsumer(&consumer);
  
  std::ifstream in(getSourceDir() + "/../../samples/AlcornTest_first_5.ac3", std::ios::binary);
  
  in.seekg(0, std::ios::end);
  std::size_t size = in.tellg();
  in.seekg(0, std::ios::beg);
  
  char *pdata = new char[size];
  in.read(pdata, size);
  size = in.gcount();
  pparser -> process((const uint8_t *)pdata, size);
 
  AC3Parser::release(pparser);
  
  BOOST_CHECK_EQUAL(consumer.m_frames.size(), 5);
  BOOST_CHECK_EQUAL(consumer.m_warnings.size(), 0);
  
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_info.m_position, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_syncinfo.m_crc1, 41318);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_syncinfo.m_fscod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_syncinfo.m_frmsizecod, 28);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_bsid, 8);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_bsmod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_acmod, 7);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_cmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_surmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_lfeon, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_dialnorm, 27);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_compre, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_compr, 215);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_langcode, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_bsi.m_langcod, 9);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_sampleRate, 48000);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_bitrate, 384000);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_channels, 6);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_frameSize, 1536);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_channelLayout,  AC3_LAYOUT_5POINT0 | AC3_LAYOUT_LOW_FREQUENCY);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_duration,  0.032);

  
  
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_info.m_position, 0x600);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_syncinfo.m_crc1, 30786);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_syncinfo.m_fscod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_syncinfo.m_frmsizecod, 28);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_bsid, 8);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_bsmod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_acmod, 7);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_cmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_surmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_lfeon, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_dialnorm, 27);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_compre, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_compr, 215);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_langcode, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_bsi.m_langcod, 9);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_common.m_sampleRate, 48000);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_common.m_bitrate, 384000);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_common.m_channels, 6);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_common.m_frameSize, 1536);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_common.m_channelLayout,  AC3_LAYOUT_5POINT0 | AC3_LAYOUT_LOW_FREQUENCY);
  BOOST_CHECK_EQUAL(consumer.m_frames[1].m_frame.m_common.m_duration,  0.032);  


  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_info.m_position, 0xc00);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_syncinfo.m_crc1, 61733);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_syncinfo.m_fscod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_syncinfo.m_frmsizecod, 28);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_bsid, 8);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_bsmod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_acmod, 7);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_cmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_surmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_lfeon, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_dialnorm, 27);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_compre, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_compr, 211);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_langcode, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_bsi.m_langcod, 9);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_common.m_sampleRate, 48000);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_common.m_bitrate, 384000);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_common.m_channels, 6);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_common.m_frameSize, 1536);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_common.m_channelLayout,  AC3_LAYOUT_5POINT0 | AC3_LAYOUT_LOW_FREQUENCY);
  BOOST_CHECK_EQUAL(consumer.m_frames[2].m_frame.m_common.m_duration,  0.032);


  
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_info.m_position, 0x1200);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_syncinfo.m_crc1, 30941);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_syncinfo.m_fscod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_syncinfo.m_frmsizecod, 28);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_bsid, 8);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_bsmod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_acmod, 7);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_cmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_surmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_lfeon, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_dialnorm, 27);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_compre, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_compr, 211);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_langcode, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_bsi.m_langcod, 9);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_common.m_sampleRate, 48000);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_common.m_bitrate, 384000);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_common.m_channels, 6);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_common.m_frameSize, 1536);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_common.m_channelLayout,  AC3_LAYOUT_5POINT0 | AC3_LAYOUT_LOW_FREQUENCY);
  BOOST_CHECK_EQUAL(consumer.m_frames[3].m_frame.m_common.m_duration,  0.032);

  
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_info.m_position, 0x1800);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_syncinfo.m_crc1, 6368);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_syncinfo.m_fscod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_syncinfo.m_frmsizecod, 28);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_bsid, 8);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_bsmod, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_acmod, 7);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_cmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_surmixlev, 0);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_lfeon, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_dialnorm, 27);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_compre, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_compr, 211);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_langcode, 1);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_bsi.m_langcod, 9);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_common.m_sampleRate, 48000);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_common.m_bitrate, 384000);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_common.m_channels, 6);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_common.m_frameSize, 1536);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_common.m_channelLayout,  AC3_LAYOUT_5POINT0 | AC3_LAYOUT_LOW_FREQUENCY);
  BOOST_CHECK_EQUAL(consumer.m_frames[4].m_frame.m_common.m_duration,  0.032);

}



BOOST_AUTO_TEST_CASE(ALCORN_TEST)
{
  AC3Parser *pparser = AC3Parser::create();
  
  Consumer consumer;
  
  pparser -> addConsumer(&consumer);
  
  std::ifstream in(getSourceDir() + "/../../samples/AlcornTest.ac3", std::ios::binary);
  
  in.seekg(0, std::ios::end);
  std::size_t size = in.tellg();
  in.seekg(0, std::ios::beg);
  
  char *pdata = new char[size];
  in.read(pdata, size);
  size = in.gcount();
  pparser -> process((const uint8_t *)pdata, size);
    
  AC3Parser::release(pparser);
  
  BOOST_CHECK_EQUAL(consumer.m_frames.size(), 391);
  BOOST_CHECK_EQUAL(consumer.m_warnings.size(), 0);
   
}





BOOST_AUTO_TEST_CASE(MATRIX2_COMMENTARY1)
{
  AC3Parser *pparser = AC3Parser::create();
  
  Consumer consumer;
  
  pparser -> addConsumer(&consumer);
  
  std::ifstream in(getSourceDir() + "/../../samples/matrix2_commentary1_stereo_192.eac3", std::ios::binary);
  
  in.seekg(0, std::ios::end);
  std::size_t size = in.tellg();
  in.seekg(0, std::ios::beg);
  
  char *pdata = new char[size];
  in.read(pdata, size);
  size = in.gcount();
  pparser -> process((const uint8_t *)pdata, size);
    
  AC3Parser::release(pparser);
  
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_sampleRate, 48000);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_bitrate, 191500);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_channels, 2);
  BOOST_CHECK_EQUAL(consumer.m_frames[0].m_frame.m_common.m_channelLayout, AC3_LAYOUT_STEREO);   
}



BOOST_AUTO_TEST_SUITE_END();
