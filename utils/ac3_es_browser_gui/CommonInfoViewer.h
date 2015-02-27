#ifndef COMMON_INFO_VIEWER_H_
#define COMMON_INFO_VIEWER_H_

#include <vector>

#include <QTableWidget>

#include <AC3Parser.h>
#include <AC3Frame.h>

class CommonInfoViewer: 
  public QTableWidget,
  public AC3Parser::Consumer
{
  Q_OBJECT

  signals:
    void frameSelected(const AC3Frame &frame);

  public:
    CommonInfoViewer(QWidget *pwgt = NULL);

    void saveCustomData();
    
    void clear();
    
    virtual void onFrame(const AC3Frame *, const EventInfo *);
    virtual void onWarning(const std::string &, const EventInfo *);
    
  private slots:
    void onSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    
  private:
    void readCustomData();
    
    std::vector<AC3Frame>     m_frames;
};


#endif
