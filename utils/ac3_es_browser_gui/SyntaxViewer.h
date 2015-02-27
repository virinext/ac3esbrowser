#ifndef SYNTAX_VIEWER_H_
#define SYNTAX_VIEWER_H_



#include <QTreeWidget>

#include <AC3Frame.h>

class SyntaxViewer: public QTreeWidget
{
  Q_OBJECT
  
  public:
    SyntaxViewer(QWidget *pwgt = NULL);
    
  public slots:
    void onFrameChanged(const AC3Frame &frame);

  private slots:
    void onItemCollapsed(QTreeWidgetItem * item);
    void onItemExpanded(QTreeWidgetItem * item);

private:
    void createAC3SyncInfo(QTreeWidgetItem *pitem, const AC3Frame &);
    void createEAC3SyncInfo(QTreeWidgetItem *pitem, const AC3Frame &);
    void createAC3BSI(QTreeWidgetItem *pitem, const AC3Frame &);
    void createEAC3BSI(QTreeWidgetItem *pitem, const AC3Frame &);

    struct SyntaxViewerState
    {
      SyntaxViewerState()
      {
	bsi = true;
	syncinfo = true;
	compre = true;
	langcode = true;
	fscod = true;
	acmod = true;
	compr2e = true;
	chanmape = true;
      }

      bool      bsi;
      bool      syncinfo;
      bool      compre;
      bool      langcode;
      bool      fscod;
      bool      acmod;
      bool      compr2e;
      bool      chanmape;
    };
    
    
    SyntaxViewerState     m_state;

};



#endif
