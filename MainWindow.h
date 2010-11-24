#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class ImageWindow;

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(QWidget* parent = 0);
	private:
		ImageWindow* textBox;
};
#endif
