#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "eyegen.h"

/*****************************************************************************\
|* Constructed at app launch
\*****************************************************************************/
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	{
	ui->setupUi(this);

	/*************************************************************************\
	|* Set up the generator
	\*************************************************************************/
	_generator = new EyeGen(this);
	connect(_generator, &EyeGen::drawScreen,
			ui->dpy, &DpyWidget::render);


	/*************************************************************************\
	|* Bind 'start'
	\*************************************************************************/
	connect(ui->btnF1, &QPushButton::clicked,
			this, &MainWindow::_start);

	/*************************************************************************\
	|* Bind 'focal length -'
	\*************************************************************************/
	connect(ui->btnF2, &QPushButton::clicked,
			this, &MainWindow::_decreaseFocalLength);

	/*************************************************************************\
	|* Bind 'focal length +'
	\*************************************************************************/
	connect(ui->btnF3, &QPushButton::clicked,
			this, &MainWindow::_increaseFocalLength);

	/*************************************************************************\
	|* Bind 'first generation'
	\*************************************************************************/
	connect(ui->btnF5, &QPushButton::clicked,
			this, &MainWindow::_firstGeneration);

	/*************************************************************************\
	|* Bind 'back a generation'
	\*************************************************************************/
	connect(ui->btnF6, &QPushButton::clicked,
			this, &MainWindow::_decreaseGeneration);

	/*************************************************************************\
	|* Bind 'plus a generation'
	\*************************************************************************/
	connect(ui->btnF7, &QPushButton::clicked,
			this, &MainWindow::_increaseGeneration);

	/*************************************************************************\
	|* Bind 'play generations'
	\*************************************************************************/
	connect(ui->btnF8, &QPushButton::clicked,
			this, &MainWindow::_playGenerations);

	/*************************************************************************\
	|* Bind 'quit'
	\*************************************************************************/
	connect(ui->btnF10, &QPushButton::clicked,
			this, &MainWindow::_quit);
	}

void MainWindow::showEvent(QShowEvent* event)
	{
	QWidget::showEvent(event);
	_generator->initialise();
	}

/*****************************************************************************\
|* Clear up
\*****************************************************************************/
MainWindow::~MainWindow()
	{
	delete ui;
	delete _generator;
	}

#pragma - Private Methods

/*****************************************************************************\
|* User clicked start
\*****************************************************************************/
void MainWindow::_start(void)
	{
	_generator->initialise();

	bool done = false;
	while (done == false)
		{
		int gen = _generator->evolveLens(done);
		_generator->setTotalGenerations(gen);
		}
	}

/*****************************************************************************\
|* User wants to increase the focal length
\*****************************************************************************/
void MainWindow::_increaseFocalLength(void)
	{
	_generator->initialise();
	_generator->setFocalLength(_generator->focalLength()+1);
	}

/*****************************************************************************\
|* User wants to decrease the focal length
\*****************************************************************************/
void MainWindow::_decreaseFocalLength(void)
	{
	_generator->initialise();
	_generator->setFocalLength(_generator->focalLength()-1);
	}

/*****************************************************************************\
|* Jump to the first generation
\*****************************************************************************/
void MainWindow::_firstGeneration(void)
	{
	ui->dpy->setCurrent(1);
	ui->dpy->repaint();
	}

/*****************************************************************************\
|* Decrease the generation
\*****************************************************************************/
void MainWindow::_decreaseGeneration(void)
	{
	int gen = ui->dpy->current();
	if (gen > 1)
		{
		ui->dpy->setCurrent(gen-1);
		ui->dpy->repaint();
		}
	}

/*****************************************************************************\
|* Increase the generation
\*****************************************************************************/
void MainWindow::_increaseGeneration(void)
	{
	int gen = ui->dpy->current();
	if (gen < _generator->totalGenerations())
		{
		ui->dpy->setCurrent(gen+1);
		ui->dpy->repaint();
		}
	}

/*****************************************************************************\
|* Run the animation
\*****************************************************************************/
void MainWindow::_playGenerations(void)
	{
	ui->dpy->setCurrent(1);
	_cron = new QTimer(this);
	_cron->setInterval(50);
	connect(_cron, &QTimer::timeout, this, &MainWindow::_playFrame);
	_cron->start();
	}

/*****************************************************************************\
|* Play a frame of anim
\*****************************************************************************/
void MainWindow::_playFrame(void)
	{
	int gen = ui->dpy->current();

	if (gen <= _generator->totalGenerations())
		{
		ui->dpy->setCurrent(gen+1);
		ui->dpy->repaint();
		}
	else
		_cron->stop();
	}


/*****************************************************************************\
|* Increase the generation
\*****************************************************************************/
void MainWindow::_quit(void)
	{
	QApplication::quit();
	}
