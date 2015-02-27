#ifndef AC3_PARSER_IMPL_H_
#define AC3_PARSER_IMPL_H_

#include "AC3Parser.h"

#include <list>

class BitstreamReader;

class AC3ParserImpl: public AC3Parser
{
  public:
    virtual void process(const uint8_t *pdata, std::size_t size);

    void addConsumer(Consumer *pconsumer);
    void releaseConsumer(Consumer *pconsumer);
    
  private:
    void onFrame(const AC3Frame *, const EventInfo *);
    void onWarning(const std::string &, const EventInfo *);
    
    std::size_t processFrame(const uint8_t *pdata, std::size_t size, const EventInfo *pevent);
    std::size_t processAC3Header(BitstreamReader *pbs, AC3Frame *pheader, const EventInfo *pevent);
    std::size_t processEAC3Header(BitstreamReader *pbs, AC3Frame *pheader, const EventInfo *pevent);


    std::list<Consumer *>          m_consumers;
};



#endif
