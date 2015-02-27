#include "SyntaxViewer.h"

#include <QStringList>
#include <QHeaderView>
#include <QString>

SyntaxViewer::SyntaxViewer(QWidget* pwgt):
  QTreeWidget(pwgt)
{
  header()->hide();
  
  QObject::connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem *)), SLOT(onItemCollapsed(QTreeWidgetItem *)));
  QObject::connect(this, SIGNAL(itemExpanded(QTreeWidgetItem *)), SLOT(onItemExpanded(QTreeWidgetItem *)));
}


void SyntaxViewer::onFrameChanged(const AC3Frame &frame)
{
  clear();
  
  QTreeWidgetItem *psyncInfo = new QTreeWidgetItem(QStringList("syncinfo"));
  QTreeWidgetItem *pbsi = new QTreeWidgetItem(QStringList("bsi"));
  
  
  addTopLevelItem(psyncInfo);
  addTopLevelItem(pbsi);
 
  bool isAc3Frame = frame.m_bsi.m_bsmod <= 10;
  if(isAc3Frame)
  {
    createAC3SyncInfo(psyncInfo, frame);
    createAC3BSI(pbsi, frame);
  }
  else
  {
    createEAC3SyncInfo(psyncInfo, frame);
    createEAC3BSI(pbsi, frame);
  }

  pbsi -> setExpanded(m_state.bsi);
  psyncInfo -> setExpanded(m_state.syncinfo);
 
}


void SyntaxViewer::createAC3SyncInfo(QTreeWidgetItem *pitem, const AC3Frame &frame)
{
  pitem -> addChild(new QTreeWidgetItem(QStringList("syncword = 0x0B7")));
  pitem -> addChild(new QTreeWidgetItem(QStringList("crc1 = 0x" + QString::number(frame.m_syncinfo.m_crc1, 16))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("fscod = " + QString::number(frame.m_syncinfo.m_fscod))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("frmsizecod = " + QString::number(frame.m_syncinfo.m_frmsizecod))));
}


void SyntaxViewer::createEAC3SyncInfo(QTreeWidgetItem *pitem, const AC3Frame &)
{
  pitem -> addChild(new QTreeWidgetItem(QStringList("syncword = 0x0B7")));
}


void SyntaxViewer::createAC3BSI(QTreeWidgetItem *pitem, const AC3Frame &frame)
{
  pitem -> addChild(new QTreeWidgetItem(QStringList("bsid = " + QString::number(frame.m_bsi.m_bsid))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("bsmod = " + QString::number(frame.m_bsi.m_bsmod))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("acmod = " + QString::number(frame.m_bsi.m_acmod))));


  if(frame.m_bsi.m_acmod == AC3_CHMODE_STEREO) 
  {
    pitem -> addChild(new QTreeWidgetItem(QStringList("dsurmod = " + QString::number(frame.m_bsi.m_dsurmod))));
  } 
  else 
  {
    if((frame.m_bsi.m_acmod & AC3_CHMODE_MONO) && frame.m_bsi.m_acmod != AC3_CHMODE_MONO)
      pitem -> addChild(new QTreeWidgetItem(QStringList("cmixlev = " + QString::number(frame.m_bsi.m_cmixlev))));
    if(frame.m_bsi.m_acmod & AC3_CHMODE_2F1R)
      pitem -> addChild(new QTreeWidgetItem(QStringList("surmixlev = " + QString::number(frame.m_bsi.m_surmixlev))));
  }

  pitem -> addChild(new QTreeWidgetItem(QStringList("lfeon = " + QString::number(frame.m_bsi.m_lfeon))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("dialnorm = " + QString::number(frame.m_bsi.m_dialnorm))));

  QTreeWidgetItem *pcompre = new QTreeWidgetItem(QStringList("compre = " + QString::number(frame.m_bsi.m_compre)));
  
  pitem -> addChild(pcompre);

  if(frame.m_bsi.m_compre)
    pcompre -> addChild(new QTreeWidgetItem(QStringList("compr = " + QString::number(frame.m_bsi.m_compr))));

  QTreeWidgetItem *plangcode = new QTreeWidgetItem(QStringList("langcode = " + QString::number(frame.m_bsi.m_langcode)));
  pitem -> addChild(plangcode);

  if(frame.m_bsi.m_langcode)
    plangcode -> addChild(new QTreeWidgetItem(QStringList("langcod = " + QString::number(frame.m_bsi.m_langcod))));

  
  plangcode -> setExpanded(m_state.langcode);
  pcompre -> setExpanded(m_state.compre);
}




void SyntaxViewer::createEAC3BSI(QTreeWidgetItem *pitem, const AC3Frame &frame)
{
  pitem -> addChild(new QTreeWidgetItem(QStringList("strmtyp = " + QString::number(frame.m_bsi.m_strmtyp))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("substreamid = " + QString::number(frame.m_bsi.m_substreamid))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("frmsiz = " + QString::number(frame.m_bsi.m_frmsiz))));

  QTreeWidgetItem *pfscod = new QTreeWidgetItem(QStringList("fscod = " + QString::number(frame.m_syncinfo.m_fscod)));
  pitem -> addChild(pfscod);
  
  if(frame.m_syncinfo.m_fscod == 3)
    pfscod -> addChild(new QTreeWidgetItem(QStringList("fscod2 = " + QString::number(frame.m_bsi.m_fscod2))));
  else
    pitem -> addChild(new QTreeWidgetItem(QStringList("numblkscod = " + QString::number(frame.m_bsi.m_numblkscod))));

  QTreeWidgetItem *pacmod = new QTreeWidgetItem(QStringList("acmod = " + QString::number(frame.m_bsi.m_acmod)));
  pitem -> addChild(pacmod);
  pitem -> addChild(new QTreeWidgetItem(QStringList("lfeon = " + QString::number(frame.m_bsi.m_lfeon))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("bsid = " + QString::number(frame.m_bsi.m_bsid))));
  pitem -> addChild(new QTreeWidgetItem(QStringList("dialnorm = " + QString::number(frame.m_bsi.m_dialnorm))));


  QTreeWidgetItem *pcompre = new QTreeWidgetItem(QStringList("compre = " + QString::number(frame.m_bsi.m_compre)));
  pitem -> addChild(pcompre);

  if(frame.m_bsi.m_compre)
    pcompre -> addChild(new QTreeWidgetItem(QStringList("compr = " + QString::number(frame.m_bsi.m_compr))));

  
  if(frame.m_bsi.m_acmod == 0)
  {
    pacmod -> addChild(new QTreeWidgetItem(QStringList("dialnorm2 = " + QString::number(frame.m_bsi.m_dialnorm2))));

    QTreeWidgetItem *pcompr2e = new QTreeWidgetItem(QStringList("compr2e = " + QString::number(frame.m_bsi.m_compr2e)));
    pacmod -> addChild(pcompr2e);

    if(frame.m_bsi.m_compr2e)
      pcompr2e -> addChild(new QTreeWidgetItem(QStringList("compr2 = " + QString::number(frame.m_bsi.m_compr2))));

     pcompr2e -> setExpanded(m_state.compr2e);
  }

  if(frame.m_bsi.m_strmtyp == EAC3_FRAME_TYPE_DEPENDENT)
  {
    QTreeWidgetItem *pchanmape = new QTreeWidgetItem(QStringList("chanmape = " + QString::number(frame.m_bsi.m_chanmape)));
    pitem -> addChild(pchanmape);


    if(frame.m_bsi.m_chanmape)
      pchanmape -> addChild(new QTreeWidgetItem(QStringList("chanmap = " + QString::number(frame.m_bsi.m_chanmap))));

    pchanmape -> setExpanded(m_state.chanmape);
  }
  
  pcompre -> setExpanded(m_state.compre);
  pfscod -> setExpanded(m_state.fscod);
}



void SyntaxViewer::onItemCollapsed(QTreeWidgetItem * item)
{
  QString text = item -> text(0);

  if(text.startsWith("bsi"))
    m_state.bsi = false;
  else if(text.startsWith("syncinfo"))
    m_state.syncinfo = false;
  else if(text.startsWith("compre"))
    m_state.compre = false;
  else if(text.startsWith("langcode"))
    m_state.langcode = false;
  else if(text.startsWith("fscod"))
    m_state.fscod = false;
  else if(text.startsWith("acmod"))
    m_state.acmod = false;
  else if(text.startsWith("compr2e"))
    m_state.compr2e = false;
  else if(text.startsWith("chanmape"))
    m_state.chanmape = false;
}



void SyntaxViewer::onItemExpanded(QTreeWidgetItem * item)
{
  QString text = item -> text(0);

  if(text.startsWith("bsi"))
    m_state.bsi = true;
  else if(text.startsWith("syncinfo"))
    m_state.syncinfo = true;
  else if(text.startsWith("compre"))
    m_state.compre = true;
  else if(text.startsWith("langcode"))
    m_state.langcode = true;
  else if(text.startsWith("fscod"))
    m_state.fscod = true;
  else if(text.startsWith("acmod"))
    m_state.acmod = true;
  else if(text.startsWith("compr2e"))
    m_state.compr2e = true;
  else if(text.startsWith("chanmape"))
    m_state.chanmape = true;

}
