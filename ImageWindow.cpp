#include "ImageWindow.h"

#include <zlib.h>

#include <QApplication>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFileInfo>
#include <QByteArray>
#include <QTextStream>
#include <QFileDialog>

#include <KStandardDirs>


#define CHUNK 16384

ImageWindow::ImageWindow(QApplication* app, QWidget *parent) : KDialog(parent)
{
	m_app = app;
	setCaption("decKreator");
	setButtons(KDialog::Ok | KDialog::Cancel | KDialog::User1);
	setButtonText(KDialog::User1, "Browse");
	enableButton(KDialog::User1, false);
	setDefaultButton(KDialog::User1);

        m_nameLabel = new QLabel(tr("Name:"));
        m_nameLineEdit = new QLineEdit;
        m_nameLabel->setBuddy(m_nameLineEdit);

        m_browseLabel = new QLabel( tr("Image File:") );
        m_browseLineEdit = new QLineEdit;
        m_browseLabel->setBuddy(m_browseLineEdit);

        connect(m_nameLineEdit, SIGNAL(textChanged(const QString &)),
                this, SLOT(enableBrowseButton(const QString &)));


        connect(this, SIGNAL(okClicked()), this, SLOT(okButtonPressed()));

        connect(this, SIGNAL(cancelClicked()), this, SLOT(cancelButtonPressed()));
        
	connect(this, SIGNAL(user1Clicked()), this, SLOT(getPath()));

        //Layout
	QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow(m_nameLabel, m_nameLineEdit);
        formLayout->addRow(m_browseLabel, m_browseLineEdit);
        setLayout(formLayout);
}

void ImageWindow::enableBrowseButton(const QString &text)
{
	enableButton(KDialog::User1, !text.isEmpty());
}

void ImageWindow::getPath()
{
	QString path;

	path = QFileDialog::getOpenFileName(
		this,
		"Choose an image file to open",
		QString::null,
		QString::null);
	m_browseLineEdit->setText(path);
}

void ImageWindow::okButtonPressed()
{
 	QString template_path;
	template_path =	KStandardDirs::locate( "data" , "deckreator/custom-deck.svg");
        QFile file(template_path);
           if (!file.open(QIODevice::ReadOnly)){
	       QMessageBox::warning(this, "ERROR", QString("Could not open file custom-deck.svg.  Looked in ") + template_path);
               return;
	   }
        
	QDomDocument doc("Custom Deck");
           if (!doc.setContent(&file)) {
               file.close();
	       QMessageBox::warning(this, "ERROR:", "Could not parse custom-deck.svg file");
               return;
           }
        file.close();

        QString path = m_browseLineEdit->text();
	if (path.isEmpty()){
		QMessageBox::warning(this, "ERROR:", "No image path selected.");
		return;
	}
	if (!QFile::exists(path)){
		QMessageBox::warning(this, "ERROR:", path + " is not a valid file path.");
		return;
	}

        QString name = m_nameLineEdit->text(); //Put the name typed by the user into this varable
        QDomNodeList nodes = doc.elementsByTagName("image");
        QDomElement back;
        back = nodes.at(0).toElement();
        back.setAttribute("xlink:href", path);
        back.setAttribute("sodipodi:absref", path);

	QString deck_path = KStandardDirs::locateLocal( "data" , QString("carddecks/custom-") + name + "/", true);

	QFile index_file(deck_path + "index.desktop"); //Create a new index file for the custom deck.
	if (!index_file.open(QIODevice::WriteOnly | QFile::Truncate)){
		QMessageBox::warning(this, "ERROR:", QString("Could not open file ") + deck_path + "index.desktop for writing.");
		return;
	}
	

	QString image_base = QFileInfo(path).fileName();

	QTextStream out(&index_file);
	out << "[KDE Backdeck]" << endl;
	out << "Name=" << name << endl;
	out << "Preview=" << image_base << endl;
	out << "SVG=custom-deck.svgz" << endl;
	out << "Back=" << name << endl;
	out << "Author=decKreator Theming Tool" << endl;
	out << "Author=kratos_12000@yahoo.com" << endl;
	out << "Comment=Theme based on Oxygen Air Theme by Eugene Trounev" << endl;
	out << "Comment=Created by decKreator" << endl;

	index_file.close();

	QFile preview_file(path);
	if (!preview_file.open(QIODevice::ReadOnly)){
		QMessageBox::warning(this, "ERROR:", QString("Could not open file ") + path);
		return;
	}

	preview_file.copy(deck_path + image_base);

	

	QFile svg_file("custom_deck.svg");
	gzFile zipfile = gzopen((deck_path + "custom-deck.svgz").toLatin1(), "w");
	gzputs(zipfile, doc.toString().toLatin1()); 
	gzclose(zipfile);
	QMessageBox::information(this, "Success!", "decKreator has successfully created a custom theme for your KPatience enjoyment.");
}

void ImageWindow::cancelButtonPressed()
{
	m_app->quit();
}
