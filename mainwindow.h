#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EyeGen;

class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

		virtual void showEvent(QShowEvent* event);

	private:
		Ui::MainWindow *ui;
		EyeGen * _generator;
		QTimer * _cron;

		/*********************************************************************\
		|* Button handlers
		\*********************************************************************/
		void _start(void);						// Start things going
		void _increaseFocalLength(void);		// Change the focal length
		void _decreaseFocalLength(void);		// Change the focal length
		void _firstGeneration(void);			// Jump to the first generation
		void _decreaseGeneration(void);			// Decrease the generation
		void _increaseGeneration(void);			// Increase the generation
		void _playGenerations(void);			// Play generations as anim
		void _playFrame(void);					// Draw one frame
		void _quit(void);						// Terminate the app
	};
#endif // MAINWINDOW_H
