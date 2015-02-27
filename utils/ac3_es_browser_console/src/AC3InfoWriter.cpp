#include "AC3InfoWriter.h"

#include <iostream>

void AC3InfoWriter::onFrame(const AC3Frame *pframe, const EventInfo *pinfo)
{
  Frame frame;
  frame.m_frame = *pframe;
  frame.m_info = *pinfo;
  m_frames.push_back(frame);      
}

void AC3InfoWriter::onWarning(const std::string &wrng, const EventInfo *pinfo)
{
  Warning warning;
  warning.m_warning = wrng;
  warning.m_info = *pinfo;
  m_warnings.push_back(warning);
}


void AC3InfoWriter::write(std::ostream &out)
{
  out << "Syntax elements (count = " << m_frames.size() << "):" << std::endl;
  for(std::size_t i=0; i<m_frames.size(); i++)
  {
    out << std::hex << "0x" << m_frames[i].m_info.m_position << std::dec << ": ";
    out << "\t" << "(frame_number = " << i
	<< ", sample_rate = " << m_frames[i].m_frame.m_common.m_sampleRate
	<< ", bitrate = " << m_frames[i].m_frame.m_common.m_bitrate
	<< ", frame_size = " << m_frames[i].m_frame.m_common.m_frameSize
	<< ", duration = " << m_frames[i].m_frame.m_common.m_duration
	<< ")"
	<< std::endl;
	
    bool isAc3Frame = m_frames[i].m_frame.m_bsi.m_bsmod <= 10;
	
    if(isAc3Frame)
    {
      writeAC3SyncInfo(out, m_frames[i].m_frame);
      writeAC3BSI(out, m_frames[i].m_frame);
    }
    else
    {
      writeEAC3SyncInfo(out, m_frames[i].m_frame);
      writeEAC3BSI(out, m_frames[i].m_frame);
    }
  }
  
  if(m_warnings.size())
  {
    out << std::endl << "Warnings (count = " << m_warnings.size() << "):" << std::endl;

    for(std::size_t i=0; i<m_warnings.size(); i++)
      out << "\t" << std::hex << "0x" << m_warnings[i].m_info.m_position << ": " << m_warnings[i].m_warning << std::endl;
  }
}


void AC3InfoWriter::writeAC3SyncInfo(std::ostream &out, const AC3Frame &frame)
{
  out << "\t" << "syncinfo()" << std::endl;
  out << "\t\t" << "syncword = 0x0B77" << std::endl;
  out << "\t\t" << "crc1 = 0x" << std::hex << (int) frame.m_syncinfo.m_crc1 << std::dec << std::endl;
  out << "\t\t" << "fscod = " << (int) frame.m_syncinfo.m_fscod << std::endl;
  out << "\t\t" << "frmsizecod = " << (int) frame.m_syncinfo.m_frmsizecod << std::endl; 
}


void AC3InfoWriter::writeEAC3SyncInfo(std::ostream &out, const AC3Frame &frame)
{
  out << "\t" << "syncinfo()" << std::endl;
  out << "\t\t" << "syncword = 0x0B77" << std::endl;
}


void AC3InfoWriter::writeAC3BSI(std::ostream &out, const AC3Frame &frame)
{
  out << "\t" << "bsi()" << std::endl;
  out << "\t\t" << "bsid = " << (int) frame.m_bsi.m_bsid << std::endl;
  out << "\t\t" << "bsmod = " << (int) frame.m_bsi.m_bsmod << std::endl;
  out << "\t\t" << "acmod = " << (int) frame.m_bsi.m_acmod << std::endl;

  
  
  if(frame.m_bsi.m_acmod == AC3_CHMODE_STEREO) 
  {
    out << "\t\t" << "dsurmod = " << (int) frame.m_bsi.m_dsurmod << std::endl;  
  } 
  else 
  {
    if((frame.m_bsi.m_acmod & AC3_CHMODE_MONO) && frame.m_bsi.m_acmod != AC3_CHMODE_MONO)
      out << "\t\t" << "cmixlev = " << (int) frame.m_bsi.m_cmixlev << std::endl;
    if(frame.m_bsi.m_acmod & AC3_CHMODE_2F1R)
      out << "\t\t" << "surmixlev" << (int) frame.m_bsi.m_surmixlev << std::endl;  
  }

  out << "\t\t" << "lfeon = " << (int) frame.m_bsi.m_lfeon << std::endl;
  out << "\t\t" << "dialnorm = " << (int) frame.m_bsi.m_dialnorm << std::endl;  
  out << "\t\t" << "compre = " << (int) frame.m_bsi.m_compre << std::endl;
  if(frame.m_bsi.m_compre)
    out << "\t\t\t" << "compr = " << (int) frame.m_bsi.m_compr << std::endl;

  out << "\t\t" << "langcode = " << (int) frame.m_bsi.m_langcode << std::endl;  
  if(frame.m_bsi.m_langcode)
    out << "\t\t\t" << "langcod = " << (int) frame.m_bsi.m_langcod << std::endl;
}


void AC3InfoWriter::writeEAC3BSI(std::ostream &out, const AC3Frame &frame)
{
  out << "\t" << "bsi()" << std::endl;

  out << "\t\t" << "strmtyp = " << (int) frame.m_bsi.m_strmtyp << std::endl;  
  out << "\t\t" << "substreamid = " << (int) frame.m_bsi.m_substreamid << std::endl;
  out << "\t\t" << "frmsiz = " << (int) frame.m_bsi.m_frmsiz << std::endl;  
  out << "\t\t" << "fscod = " << (int) frame.m_syncinfo.m_fscod << std::endl;  
  
  if(frame.m_syncinfo.m_fscod == 3)
    out << "\t\t\t" << "fscod2 = " << (int) frame.m_bsi.m_fscod2 << std::endl;  
  else
    out << "\t\t\t" << "numblkscod = " << (int) frame.m_bsi.m_numblkscod << std::endl;  
  
  out << "\t\t" << "acmod = " << (int) frame.m_bsi.m_acmod << std::endl;  
  out << "\t\t" << "lfeon = " << (int) frame.m_bsi.m_lfeon << std::endl;  
  out << "\t\t" << "bsid = " << (int) frame.m_bsi.m_bsid << std::endl;  
  out << "\t\t" << "dialnorm = " << (int) frame.m_bsi.m_dialnorm << std::endl;  

  out << "\t\t" << "compre = " << (int) frame.m_bsi.m_compre << std::endl;  
  if(frame.m_bsi.m_compre)
    out << "\t\t\t" << "compr = " << (int) frame.m_bsi.m_compr << std::endl;  

  if(frame.m_bsi.m_acmod == 0)
  {
    out << "\t\t\t" << "dialnorm2 = " << (int) frame.m_bsi.m_dialnorm2 << std::endl;  
    out << "\t\t\t" << "compr2e = " << (int) frame.m_bsi.m_compr2e << std::endl;  

    if(frame.m_bsi.m_compr2e)
      out << "\t\t\t" << "compr2 = " << (int) frame.m_bsi.m_compr2 << std::endl;  
  }

  if(frame.m_bsi.m_strmtyp == EAC3_FRAME_TYPE_DEPENDENT)
  {
    out << "\t\t\t" << "chanmape = " << (int) frame.m_bsi.m_chanmape << std::endl;  

    if(frame.m_bsi.m_chanmape)
      out << "\t\t\t\t" << "chanmap = " << (int) frame.m_bsi.m_chanmap << std::endl;  
  }
}
