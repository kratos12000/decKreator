#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <KDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QApplication;
class KUrl;
class KUrlRequester;

class BrowseWidget;

class ImageWindow : public KDialog
{
        Q_OBJECT

        public:
                ImageWindow(QApplication* app, QWidget *parent = 0);

        signals:
		void canceled();

	public slots:
		void okButtonPressed();
		void cancelButtonPressed();

        private:
		QApplication* m_app;
		BrowseWidget* m_browser;
};

#endif
