#ifndef DPYWIDGET_H
#define DPYWIDGET_H

#include <map>

#include <QFont>
#include <QPixmap>
#include <QWidget>

#include "properties.h"

class EyeGen;

class DpyWidget : public QWidget
	{
	Q_OBJECT

	GETSET(int, current, Current);	// Current pixmap to show

	public:
		static const int BG_R		= 240;
		static const int BG_G		= 240;
		static const int BG_B		= 240;

		static const int CELL_R		= 40;
		static const int CELL_G		= 40;
		static const int CELL_B		= 40;

		static const int LS_CELL_R	= 240;
		static const int LS_CELL_G	= 10;
		static const int LS_CELL_B	= 0;

		typedef std::map<int, QPixmap> PixList;

	protected:
		PixList _imgs;				// List of pixmap images, one per generation
		QFont _largeFont;			// Larger font
		QFont _smallFont;			// Smaller font

	public:
		explicit DpyWidget(QWidget *parent = nullptr);

		void paintEvent(QPaintEvent *e);

	signals:

	public slots:
		void render(EyeGen *state);
	};

#endif // DPYWIDGET_H
