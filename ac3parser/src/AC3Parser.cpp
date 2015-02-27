#include "AC3Parser.h"
#include "AC3ParserImpl.h"

#include <cassert>

AC3Parser::~AC3Parser()
{
}

AC3Parser *AC3Parser::create()
{
  return new AC3ParserImpl;
}

void AC3Parser::release(AC3Parser *pparser)
{
  assert(pparser != NULL);

  delete pparser;
}
