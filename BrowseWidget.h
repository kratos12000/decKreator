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

        private:
                QLabel *m_nameLabel;
                QLabel *m_fileLabel;
                QLineEdit *m_nameLineEdit;
		KUrlRequester* m_requester;
};

#endif //__BROWSEWIDGET_H
