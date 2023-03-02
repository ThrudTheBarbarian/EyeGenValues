#include <QtGui>

#include "dpywidget.h"
#include "eyegen.h"


/*****************************************************************************\
|* Constructor
\*****************************************************************************/
DpyWidget::DpyWidget(QWidget *parent)
		  :QWidget{parent}
		  ,_current(-1)
	{
	_largeFont = QFont("Arial", 18);
	_smallFont = QFont("Arial", 12);

	}


/*****************************************************************************\
|* Draw the screen
\*****************************************************************************/
void DpyWidget::paintEvent(QPaintEvent *e)
	{
	if (_current >= 0)
		{
		QPainter painter(this);
		painter.drawPixmap(QPoint(0,0), _imgs[_current]);
		}
	}


/*****************************************************************************\
|* Render the current bitmap
\*****************************************************************************/
void DpyWidget::render(EyeGen *state)
	{
	int W = width();
	int H = height();

	// Originally designed for 640x400 monochrome screen, scale up...
	float xs = W / 640.0f;
	float ys = H / 400.0f;

	int x1, x2, y1, y2, x3, y3;
	int flen = state->focalLength();

	QPixmap pix = QPixmap(W, H);
	pix.fill(QColor::fromRgb(BG_R, BG_G, BG_B, 255));

	QPainter paint(&pix);
	QPen pen(QColor::fromRgb(CELL_R, CELL_G, CELL_B));
	paint.setPen(pen);

	// Draw cell columns
	for (int i=1; i<= EyeGen::NUM_COLUMNS; i++)
		{
		x1	= xs*(120 + flen - state->parent(i));
		y1	= ys*(165 + i * EyeGen::CELLW);
		x2	= xs*(120 + flen);
		y2	= ys*(165 + (i-1) * EyeGen::CELLW);
		paint.drawRect(x1, y1, x2-x1, y2-y1);
		}

	// Draw lower skin outline
	x1 = x2 = xs * 120;
	y1 = ys * 357;
	y2 = y3 = ys * (165 + EyeGen::CELLW * EyeGen::NUM_COLUMNS);
	x3 = xs * (120 + flen - state->parent(EyeGen::NUM_COLUMNS));
	paint.drawLine(x1,y1,x2,y2);
	paint.drawLine(x2,y2,x3,y3);

	// Draw upper skin outline
	x1 = xs * (120 + flen - state->parent(1));
	y1 = ys * 165;
	x2 = x1;
	y2 = ys * (165 - 0.5f * EyeGen::CELLW);
	paint.drawLine(x1,y1,x2,y2);

	// Draw lens shape
	for (int i=1; i<= EyeGen::NUM_COLUMNS-1; i++)
		{
		x1	= xs * (120 + flen - state->parent(i));
		y1	= ys * (165 - (i-1) * EyeGen::CELLW - (0.5f * EyeGen::CELLW));
		x2	= xs * (120 + flen - state->parent(i+1));
		y2	= ys * (165 - (i) * EyeGen::CELLW - (0.5f * EyeGen::CELLW));
		paint.drawLine(x1, y1, x2, y2);
		}

	// Draw upper skin outline 2
	x1 = x2;
	y1 = y2;
	x2 = x3 = xs * 120;
	y2 = ys * (165 - EyeGen::NUM_COLUMNS * EyeGen::CELLW);
	y3 = 0;

	paint.drawLine(x1,y1,x2,y2);
	paint.drawLine(x2,y2,x3,y3);

	// Draw axes
	pen.setStyle(Qt::DashDotLine);
	paint.setPen(pen);
	x1 = xs * 5;
	y1 = y2 = ys * 165;
	x2 = xs * 635;
	paint.drawLine(x1,y1,x2,y2);

	x1 = x2 = xs * (120 + flen);
	y1 = ys * 5;
	y2 = ys * (175 + EyeGen::CELLW * EyeGen::NUM_COLUMNS);
	paint.drawLine(x1,y1,x2,y2);

	// Draw light rays
	pen.setStyle(Qt::DotLine);
	paint.setPen(pen);
	for (int i=1; i<= EyeGen::NUM_COLUMNS-1; i++)
		{
		x1	= xs * 5;
		y1	= y2  = ys * (165 - (i) * EyeGen::CELLW);

		double df = (state->parent(i) + state->parent(i+1))/2.0;
		x2	= xs * (120 + flen - df);
		x3	= xs * (120 + flen);
		y3	= ys * (165 - state->offset(i));

		paint.drawLine(x1, y1, x2, y2);
		paint.drawLine(x2,y2,x3,y3);
		}


	// Draw light-sensitive cell
	pen.setStyle(Qt::SolidLine);
	pen.setColor(QColor::fromRgb(LS_CELL_R, LS_CELL_G, LS_CELL_B));
	paint.setPen(pen);
	paint.drawEllipse(QRect(xs*(120+flen-2),ys*163,xs*4, ys*4));

	pen.setColor(QColor::fromRgb(0,0,0));
	paint.setPen(pen);

	// Draw title-sized text
	char text[1024];
	int gen  = state->generation() -1;
	double eff  = state->efficiency(gen);
	snprintf(text, 1024, "Generation:%-3d   Lens efficiency:%-5.3lf%%", gen, eff);
	paint.setFont(_largeFont);
	paint.drawText(xs*155, ys*350, text);
	paint.drawText(xs*86, ys*20, "Air");
	paint.drawText(xs*130, ys*20, "Skin");

	// Draw small text (cell numbers)
	paint.setFont(_smallFont);
	for (int i=1; i<=EyeGen::NUM_COLUMNS; i++)
		{
		int parent = state->parent(i);
		snprintf(text, 1024, "%d", parent);
		int tx = 124 + flen - parent;
		int ty = 165 + (i-1) * EyeGen::CELLW + 0.5*EyeGen::CELLW + 4;
		paint.drawText(xs*tx, ys*ty, text);
		}

	// Draw small text (show +/-)
	int best = state->bestChild();
	if ((best != 0) && (gen != 1))
		{
		int i = (1 + best)/2;
		int tx = 113 + flen - state->parent(i);
		int ty = 165 + (i-1) * EyeGen::CELLW + 0.5*EyeGen::CELLW + 4;
		paint.drawText(xs*tx, ys*ty, (best %2 == 0) ? "+" : "-");
		}

	// Draw small text (labels)
	snprintf(text, 1024, "%d", flen);
	paint.drawText(124*xs, ys*(165+EyeGen::NUM_COLUMNS*EyeGen::CELLW+10), text);
	paint.drawText((124+flen)*xs, 50*ys, "focal");
	paint.drawText((124+flen)*xs, 60*ys, "plane");
	paint.drawText((124+flen)*xs, 175*ys, "light");
	paint.drawText((124+flen)*xs, 185*ys, "sensitive");
	paint.drawText((124+flen)*xs, 195*ys, "cell");

	int ty = 165 - (EyeGen::NUM_COLUMNS-3)*EyeGen::CELLW - 7;
	paint.drawText(20*xs, ty*ys, "light rays");
	paint.drawText(25*xs, 163*ys, "centre line");
	paint.drawText(50*xs, 165*ys + (EyeGen::NUM_COLUMNS/2)* EyeGen::CELLW, "no. of");
	paint.drawText(55*xs, 175*ys + (EyeGen::NUM_COLUMNS/2)* EyeGen::CELLW, "cells");

	_current = state->generation();
	_imgs[_current] = pix;
	repaint();
	}
