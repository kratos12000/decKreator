#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <KDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QApplication;

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

        private slots:
                void getPath();
                void enableBrowseButton(const QString &text);

        private:
		QApplication* m_app;
                QLabel *m_nameLabel;
                QLineEdit *m_nameLineEdit;
                QLabel *m_browseLabel;
                QLineEdit *m_browseLineEdit;
                QPushButton *m_browseButton;
};

#endif
