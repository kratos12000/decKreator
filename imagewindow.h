#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class ImageWindow : public QDialog
{
        Q_OBJECT

        public:
                ImageWindow(QWidget *parent = 0);

        signals:
		void canceled();

	public slots:
		void applyButtonPressed();
		void cancelButtonPressed();

        private slots:
                void getPath();
                void enableBrowseButton(const QString &text);

        private:
                QLabel *nameLabel;
                QLineEdit *nameLineEdit;
                QLabel *browseLabel;
                QLineEdit *browseLineEdit;
                QPushButton *browseButton;
                QPushButton *okButton;
		QPushButton *cancelButton;
};

#endif
