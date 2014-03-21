/**
 * @file   colortable.h
 * @author Liangfu Chen <liangfu.chen@nlpr.ia.ac.cn>
 * @date   Fri Mar 21 12:05:45 2014
 * 
 * @brief  
 * 
 * 
 */
#ifndef __COLOR_TABLE_H__
#define __COLOR_TABLE_H__

#include "cxcore.h"
#include <QtGui>

static
QPixmap icvQPixmap(CvMat * bgr)
{
  assert(CV_MAT_TYPE(bgr->type==CV_8U)==CV_8U);
  assert(bgr->step==bgr->cols*3);
  CvMat * rgb=cvCreateMat(bgr->rows,bgr->cols,CV_8UC3);
  // cvCvtColor(bgr,rgb,CV_BGR2RGB);
  int from_to[]={0,2,2,0,1,1};
  cvMixChannels((const CvArr**)&bgr,1,(CvArr**)&rgb,1,from_to,3);
  return QPixmap::fromImage(QImage(rgb->data.ptr,rgb->cols,rgb->rows,rgb->step,QImage::Format_RGB888));
  cvReleaseMat(&rgb);
}

class ColorTable : public QWidget
{
  CvMat * m_bgImage;
  CvMat * m_points;
  
protected:
  void paintEvent(QPaintEvent *)
  {
	if (!m_bgImage){return;}
	QPainter painter(this);
	CvMat * dispImage = cvCloneMat(m_bgImage);
	//draw_points(dispImage);
	painter.drawPixmap(QRect(0,0,width(),height()),icvQPixmap(dispImage));
	cvReleaseMat(&dispImage);
  }
  
public:
  ColorTable(QWidget * parent):QWidget(parent),m_bgImage(0),m_points(0)
  {
	typedef uchar pixval[3];
	int i,cols;
	m_bgImage = cvCreateMat(200,1000,CV_8UC3);
	cols=m_bgImage->cols;
	float invcols=255./float(cols);
	for (i=0;i<cols;i++){
	  uchar fval=cvRound(i*invcols);
	  (m_bgImage->data.ptr+i*3)[0]=fval;
	  (m_bgImage->data.ptr+i*3)[1]=fval;
	  (m_bgImage->data.ptr+i*3)[2]=fval;
	}
	for (i=1;i<m_bgImage->rows;i++){
	  memcpy(m_bgImage->data.ptr+i*m_bgImage->step,m_bgImage->data.ptr,m_bgImage->step);
	}	
  }

  ~ColorTable()
  {
	if (m_bgImage){cvReleaseMat(&m_bgImage);m_bgImage=0;}
	if (m_points){cvReleaseMat(&m_points);m_points=0;}
  }
};

#endif // __COLOR_TABLE_H__
