#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <KDialog>

class QApplication;
class KJob;
namespace KIO {
	class Job;
};

class BrowseWidget;

class ImageWindow : public KDialog
{
        Q_OBJECT

        public:
                ImageWindow(QApplication* app, QWidget *parent = 0);

        signals:
		void canceled();

	public slots:
		void applyButtonPressed();
		void cancelButtonPressed();
		void enableApply();
		void slotResult(KJob*);
		void slotOpen(KIO::Job*);
		void slotData(KIO::Job*, const QByteArray&);

        private:
		QApplication* m_app;
		BrowseWidget* m_browser;
};

#endif
