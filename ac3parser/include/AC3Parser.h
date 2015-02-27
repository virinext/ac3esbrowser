#ifndef AC3_PARSER_H_
#define AC3_PARSER_H_

#include "AC3Frame.h"

#include <string>
#include <cstdlib>

struct EventInfo
{
  std::size_t         m_position;
};


class AC3Parser
{
  public:

    class Consumer
    {
      public:
	virtual void onFrame(const AC3Frame *, const EventInfo *) = 0;
	virtual void onWarning(const std::string &, const EventInfo *) = 0;
    };

    virtual ~AC3Parser();
    virtual void process(const uint8_t *pdata, std::size_t size) = 0;
    virtual void addConsumer(Consumer *pconsumer) = 0;
    virtual void releaseConsumer(Consumer *pconsumer) = 0;
    
    static AC3Parser *create();
    static void release(AC3Parser *);
};



#endif
