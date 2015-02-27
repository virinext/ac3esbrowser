#ifndef WARNINGS_VIEWER_H_
#define WARNINGS_VIEWER_H_

#include <QTableWidget>
#include <AC3Parser.h>


class WarningsViewer:
  public QTableWidget,
  public AC3Parser::Consumer
{
  public:
    WarningsViewer(QWidget *pwgt = NULL);
    
    void clear();
    
    virtual void onFrame(const AC3Frame *, const EventInfo *);
    virtual void onWarning(const std::string &, const EventInfo *);  

  private:
    void readCustomData();
    void saveCustomData();

    void closeEvent(QCloseEvent* pevent);
};


#endif
