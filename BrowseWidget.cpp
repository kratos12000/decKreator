#include "BrowseWidget.h"

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>

#include <KUrl>
#include <KUrlRequester>

BrowseWidget::BrowseWidget(QWidget *parent) : QWidget(parent)
{
	m_nameLabel = new QLabel(tr("Name:"));
        m_nameLineEdit = new QLineEdit;
        m_nameLabel->setBuddy(m_nameLineEdit);

	m_fileLabel = new QLabel(tr("File:"));
	m_requester = new KUrlRequester(this);
	m_fileLabel->setBuddy(m_requester);

	QFormLayout* layout = new QFormLayout(this);
	layout->addRow(m_nameLabel, m_nameLineEdit);
	layout->addRow(m_fileLabel, m_requester);
	setLayout(layout);
}

KUrl BrowseWidget::url() const
{
	return m_requester->url();
}

QString BrowseWidget::name() const
{
	return m_nameLineEdit->text();
}
