#include "ImageWindow.h"
#include <QApplication>
#include <QWidget>

int main (int argc, char* argv[]){
	QApplication app(argc, argv);
	ImageWindow win(&app);
	win.show();
	app.exec();
}
