#include "BrowseWidget.h"

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>

#include <KLocale>
#include <KUrl>
#include <KUrlRequester>

BrowseWidget::BrowseWidget(QWidget *parent) : QWidget(parent)
{
	m_nameLabel = new QLabel(i18n("Name:"));
        m_nameLineEdit = new QLineEdit;
        m_nameLabel->setBuddy(m_nameLineEdit);

	m_fileLabel = new QLabel(i18n("File:"));
	m_requester = new KUrlRequester(this);
	m_fileLabel->setBuddy(m_requester);

	QFormLayout* layout = new QFormLayout(this);
	layout->addRow(m_nameLabel, m_nameLineEdit);
	layout->addRow(m_fileLabel, m_requester);
	
	connect(m_nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotNameChanged()));
	connect(m_requester, SIGNAL(textChanged(const QString&)), this, SLOT(slotNameChanged()));

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

bool BrowseWidget::validInputs() const
{
	if (!m_nameLineEdit->text().isEmpty() && !m_requester->text().isEmpty())
		return true;
	return false;
}

void BrowseWidget::slotNameChanged(){
	emit nameChanged();
}
