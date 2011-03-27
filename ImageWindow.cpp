#include <QtGui>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFileInfo>
//#include <KTar>
#include <QByteArray>
#include <KStandardDirs>
#include <zlib.h>
#include "ImageWindow.h"
#define CHUNK 16384

ImageWindow::ImageWindow(QApplication* app, QWidget *parent) : QDialog(parent)
{
	
        nameLabel = new QLabel(tr("Name:"));
        nameLineEdit = new QLineEdit;
        nameLabel->setBuddy(nameLineEdit);

        browseLabel = new QLabel(tr("Image File:"));
        browseLineEdit = new QLineEdit;
        browseLabel->setBuddy(browseLineEdit);

        browseButton = new QPushButton(tr("&Browse"));
        browseButton->setDefault(true);
        browseButton->setEnabled(false);

        okButton = new QPushButton(tr("&OK"));

        cancelButton = new QPushButton(tr("Cancel"));

        connect(nameLineEdit, SIGNAL(textChanged(const QString &)),
                this, SLOT(enableBrowseButton(const QString &)));
        connect(browseButton, SIGNAL(clicked()),
                this, SLOT(getPath()));
        connect(okButton, SIGNAL(clicked()),
                this, SLOT(applyButtonPressed()));
        connect(cancelButton, SIGNAL(clicked()),
               this, SLOT(cancelButtonPressed()));

	connect(this, SIGNAL(canceled()), app, SLOT(quit()));

        //Layout
        QHBoxLayout *line1Layout = new QHBoxLayout;
        line1Layout->addWidget(nameLabel);
        line1Layout->addWidget(nameLineEdit);

        QHBoxLayout *line2Layout = new QHBoxLayout;
        line2Layout->addWidget(browseLabel);
        line2Layout->addWidget(browseLineEdit);
        line2Layout->addWidget(browseButton);

        QHBoxLayout *line3Layout = new QHBoxLayout;
        line3Layout->addWidget(okButton);
        line3Layout->addWidget(cancelButton);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(line1Layout);
        mainLayout->addLayout(line2Layout);
	mainLayout->addLayout(line3Layout);
        setLayout(mainLayout);
}

void ImageWindow::enableBrowseButton(const QString &text)
{
	browseButton->setEnabled(!text.isEmpty());
}

void ImageWindow::getPath()
{
	QString path;

	path = QFileDialog::getOpenFileName(
		this,
		"Choose an image file to open",
		QString::null,
		QString::null);
	browseLineEdit->setText(path);
}

void ImageWindow::applyButtonPressed()
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

        QString path = browseLineEdit->text();
	if (path.isEmpty()){
		QMessageBox::warning(this, "ERROR:", "No image path selected.");
		return;
	}
	if (!QFile::exists(path)){
		QMessageBox::warning(this, "ERROR:", path + " is not a valid file path.");
		return;
	}

        QString name = nameLineEdit->text(); //Put the name typed by the user into this varable
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
	emit canceled();	
}
