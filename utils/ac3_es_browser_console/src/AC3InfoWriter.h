#ifndef AC3_INFO_WRITER_H_
#define AC3_INFO_WRITER_H_

#include <AC3Parser.h>

#include <vector>
#include <ostream>

class AC3InfoWriter: public AC3Parser::Consumer
{
  public:
    virtual void onFrame(const AC3Frame *, const EventInfo *);
    virtual void onWarning(const std::string &, const EventInfo *);

    void write(std::ostream &out);

  protected:

    void writeAC3SyncInfo(std::ostream &out, const AC3Frame &frame);
    void writeEAC3SyncInfo(std::ostream &out, const AC3Frame &frame);    

    void writeAC3BSI(std::ostream &out, const AC3Frame &frame);
    void writeEAC3BSI(std::ostream &out, const AC3Frame &frame);

    struct Frame
    {
      AC3Frame    m_frame;
      EventInfo   m_info;
    };
    
    struct Warning
    {
      std::string         m_warning;
      EventInfo           m_info;
    };
  
    std::vector<Frame>    m_frames;
    std::vector<Warning>  m_warnings;    
};


#endif
