#include "MainWindow.h"

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QFileDialog>
#include <QMessageBox>
#include <QScopedArrayPointer>
#include <QDebug>
#include <QScrollArea>
#include <QLabel>
#include <QFile>
#include <QByteArray>
#include <QSettings>
#include <QDir>
#include <QMenu>
#include <QMenuBar>

#include "CentralWidget.h"
#include "WarningsViewer.h"
#include "version_info.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags):
  QMainWindow(parent, flags)
  ,m_pwarnViewer(new WarningsViewer)
{
  QToolBar *ptb = addToolBar("Menu");

  QAction *pactOpen = ptb -> addAction("Open...");
  pactOpen -> setShortcut(QKeySequence("Ctrl+O"));
  connect(pactOpen, SIGNAL(triggered()), SLOT(slotOpen()));

  QAction *pactShowWarn = ptb -> addAction("Warnings...");
  connect(pactShowWarn, SIGNAL(triggered()), SLOT(slotShowWarningsViewer()));


  QMenu *pmenu = menuBar() -> addMenu("&File");
  pmenu -> addAction(pactOpen);
  addAction (pactOpen);

  pmenu -> addSeparator();
  pmenu -> addAction(pactShowWarn);


  pmenu -> addSeparator();
  pmenu -> addAction("Exit", this, SLOT(close()));

  pmenu = menuBar() -> addMenu("&Help");
  pmenu -> addAction ("About AC3ESBrowser...", this, SLOT(slotAbout()));


  CentralWidget *pwgt = new CentralWidget(this);
  setCentralWidget(pwgt);
  
  readCustomData();
}


void MainWindow::process(const QString &fileName)
{
  QFile file(fileName);


  if(!file.open(QIODevice::ReadOnly))
  {
    QMessageBox::critical(this, "File opening problem", "Problem with open file `" + fileName + "`for reading");
    return;
  }
  
  QByteArray arr = file.readAll();
  
  AC3Parser *pparser = AC3Parser::create();

  CentralWidget *pcntwgt = dynamic_cast<CentralWidget *>(centralWidget());

  pparser -> addConsumer(pcntwgt -> m_pcomInfoViewer.data());
  pparser -> addConsumer(dynamic_cast<WarningsViewer *> (m_pwarnViewer));
  
  pparser -> process((const uint8_t *)arr.data(), arr.size());

  AC3Parser::release(pparser);
}


void MainWindow::slotOpen()
{
  QString dir = QDir::currentPath();
  QSettings settings("AC3ESBrowser", "AC3ESBrowser");

  if(settings.value("MainWindow/PrevDir").toString().length())
  {
    dir = settings.value("MainWindow/PrevDir").toString();

  }
  
  QString fileName = QFileDialog::getOpenFileName(this, "AC3 ES File", dir);
  if(!fileName.isEmpty())
  {
    CentralWidget *pcntwgt = dynamic_cast<CentralWidget *>(centralWidget());
    pcntwgt -> m_pcomInfoViewer -> clear();
    dynamic_cast<WarningsViewer *> (m_pwarnViewer) -> clear();

    process(fileName);
    QFileInfo info(fileName);
    settings.setValue("MainWindow/PrevDir", info.absoluteDir().absolutePath());
  }
}



void MainWindow::slotShowWarningsViewer()
{
  m_pwarnViewer -> show();
  m_pwarnViewer -> raise();
}


void MainWindow::closeEvent(QCloseEvent *pevent)
{
  saveCustomData();
  CentralWidget *pcntwgt = dynamic_cast<CentralWidget *>(centralWidget());
  pcntwgt -> m_pcomInfoViewer -> saveCustomData();
  
  m_pwarnViewer -> close();
  
  QWidget::closeEvent(pevent);
}


void MainWindow::saveCustomData()
{
  QSettings settings("AC3ESBrowser", "AC3ESBrowser");
  settings.setValue("MainWindow/geometry", saveGeometry());
}


void MainWindow::readCustomData()
{
  QSettings settings("AC3ESBrowser", "AC3ESBrowser");
  restoreGeometry(settings.value("MainWindow/geometry").toByteArray()); 
}

void MainWindow::slotAbout()
{
  QString message;
  message = "<center><b>AC3ESBrowser</b></center>";
  message += "<center>virinext@gmail.com</center>";
  message += QString("<center>Version: ") + VERSION_STR + "</center>";
  message += "<center>GUI Based on Qt</center>";
  QMessageBox::about(this, "About", message);
}
