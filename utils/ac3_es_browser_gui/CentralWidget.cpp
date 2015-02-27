#include "CentralWidget.h"

#include <QHBoxLayout>

CentralWidget::CentralWidget(QWidget *pwgt):
  QWidget(pwgt)
  ,m_pcomInfoViewer(new CommonInfoViewer)
  ,m_psyntaxViewer(new SyntaxViewer)
{
    QHBoxLayout *phblay = new QHBoxLayout;
    phblay -> addWidget(m_pcomInfoViewer.data(), 2);
    phblay -> addWidget(m_psyntaxViewer.data());
    
    
    QObject::connect(m_pcomInfoViewer.data(),
      SIGNAL(frameSelected(const AC3Frame &)),
      m_psyntaxViewer.data(),
      SLOT(onFrameChanged(const AC3Frame &)));
    
    setLayout(phblay);
}
