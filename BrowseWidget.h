#ifndef __BROWSEWIDGET_H 
#define __BROWSEWIDGET_H

#include <QWidget>

class QLabel;
class QLineEdit;
class KUrl;
class KUrlRequester;

class BrowseWidget : public QWidget
{
        Q_OBJECT

        public:
                BrowseWidget(QWidget *parent = 0);
		KUrl url() const;
		QString name() const;
		bool validInputs() const;

	signals:
		void nameChanged();

        private:
                QLabel *m_nameLabel;
                QLabel *m_fileLabel;
                QLineEdit *m_nameLineEdit;
		KUrlRequester* m_requester;

	private slots:
		void slotNameChanged();
};

#endif //__BROWSEWIDGET_H
