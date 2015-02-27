#include "CommonInfoViewer.h"

#include <QStringList>
#include <QString>
#include <QSettings>
#include <QDebug>

#include <AC3Frame.h>


CommonInfoViewer::CommonInfoViewer(QWidget *pwgt):
  QTableWidget(0, 7, pwgt)
{
  QStringList labels;
  labels << "Offset" << "Frame Size" << "Bitrate" << "Sample Rate" << "Duration" << "Channels" << "Channel Layout";
  setHorizontalHeaderLabels(labels);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  
  QObject::connect(selectionModel(), 
         SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
         this,
         SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
  
  readCustomData();
}





void CommonInfoViewer::onFrame(const AC3Frame *pframe, const EventInfo *pinfo)
{
  m_frames.push_back(*pframe);
  int row = rowCount();
  insertRow(row);

  setItem(row, 0, new QTableWidgetItem(QString::number(pinfo -> m_position)));
  setItem(row, 1, new QTableWidgetItem(QString::number(pframe -> m_common.m_frameSize)));
  setItem(row, 2, new QTableWidgetItem(QString::number(pframe -> m_common.m_bitrate)));
  setItem(row, 3, new QTableWidgetItem(QString::number(pframe -> m_common.m_sampleRate)));
  setItem(row, 4, new QTableWidgetItem(QString::number(pframe -> m_common.m_duration)));
  setItem(row, 5, new QTableWidgetItem(QString::number(pframe -> m_common.m_channels)));

  QString chLayout;
  
  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_MONO)
    chLayout = "AC3_LAYOUT_MONO";

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_STEREO)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_STEREO";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_SURROUND)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_SURROUND";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_SURROUND)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_SURROUND";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_2_1)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_2_1";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_4POINT0)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_4POINT0";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_2_2)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_2_2";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_5POINT0)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_5POINT0";
  }

  if(pframe ->m_common.m_channelLayout & AC3_LAYOUT_LOW_FREQUENCY)
  {
    if(chLayout.length() > 0)
      chLayout += " | ";
    chLayout = "AC3_LAYOUT_LOW_FREQUENCY";
  }
  
  setItem(row, 6, new QTableWidgetItem(chLayout));
}




void CommonInfoViewer::onWarning(const std::string &, const EventInfo *)
{
}


void CommonInfoViewer::onSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
  QModelIndexList idxs = selected.indexes();
  
  if(!idxs.empty())
    emit frameSelected(m_frames[idxs.begin() -> row()]);
}


void CommonInfoViewer::clear()
{
  setRowCount(0);

  m_frames.clear(); 
}


void CommonInfoViewer::saveCustomData()
{
  QSettings settings("AC3ESBrowser", "AC3ESBrowser");
  settings.setValue("CommonInfoViewer/0Width", columnWidth(0));
  settings.setValue("CommonInfoViewer/1Width", columnWidth(1));
  settings.setValue("CommonInfoViewer/2Width", columnWidth(2));
  settings.setValue("CommonInfoViewer/3Width", columnWidth(3));
  settings.setValue("CommonInfoViewer/4Width", columnWidth(4));
  settings.setValue("CommonInfoViewer/5Width", columnWidth(5));
  settings.setValue("CommonInfoViewer/6Width", columnWidth(6));
}


void CommonInfoViewer::readCustomData()
{
  QSettings settings("AC3ESBrowser", "AC3ESBrowser");
  
  if(settings.contains("CommonInfoViewer/0Width"))
    setColumnWidth(0, settings.value("CommonInfoViewer/0Width").toInt());

  if(settings.contains("CommonInfoViewer/1Width"))
    setColumnWidth(1, settings.value("CommonInfoViewer/1Width").toInt());

  if(settings.contains("CommonInfoViewer/2Width"))
    setColumnWidth(2, settings.value("CommonInfoViewer/2Width").toInt());

  if(settings.contains("CommonInfoViewer/3Width"))
    setColumnWidth(3, settings.value("CommonInfoViewer/3Width").toInt());

  if(settings.contains("CommonInfoViewer/4Width"))
    setColumnWidth(4, settings.value("CommonInfoViewer/4Width").toInt());

  if(settings.contains("CommonInfoViewer/5Width"))
    setColumnWidth(5, settings.value("CommonInfoViewer/5Width").toInt());

  if(settings.contains("CommonInfoViewer/6Width"))
    setColumnWidth(6, settings.value("CommonInfoViewer/6Width").toInt());
}
