#include "MainWindow.h"
#include "imagewindow.h"
#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	textBox = new ImageWindow;
	textBox->show();
	connect (textBox,SIGNAL(canceled()),this,SLOT(close()));
	setCentralWidget(textBox);
}
