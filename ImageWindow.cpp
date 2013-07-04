#include "ImageWindow.h"
#include "BrowseWidget.h"

#include <KFilterDev>

#include <QApplication>
#include <KMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFileInfo>
#include <QByteArray>
#include <QTextStream>
#include <QFileDialog>

#include <KStandardDirs>
#include <KUrl>

#define CHUNK 16384

ImageWindow::ImageWindow(QApplication* app, QWidget *parent) : KDialog(parent)
{
	m_app = app;
	m_browser = new BrowseWidget(this);

	setCaption("decKreator");
	setButtons(KDialog::Apply | KDialog::Cancel);
	enableButton(KDialog::Apply, false);
        
	this->setMainWidget(m_browser);
        connect(this, SIGNAL(applyClicked()), this, SLOT(applyButtonPressed()));
        connect(this, SIGNAL(cancelClicked()), this, SLOT(cancelButtonPressed()));
	connect(m_browser, SIGNAL(nameChanged()), this, SLOT(enableApply()));
}

void ImageWindow::enableApply(){
	enableButton(KDialog::Apply, m_browser->validInputs());
}

void ImageWindow::applyButtonPressed()
{
 	QString template_path;
	template_path =	KStandardDirs::locate( "data" , "deckreator/custom-deck.svg");
        QFile file(template_path);
           if (!file.open(QIODevice::ReadOnly)){
	       KMessageBox::error(this, QString("Could not open file custom-deck.svg.  Looked in ") + template_path, "ERROR");
               return;
	   }
        
	QDomDocument doc("Custom Deck");
           if (!doc.setContent(&file)) {
               file.close();
	       KMessageBox::error(this, "Could not parse custom-deck.svg file", "ERROR");
               return;
           }
        file.close();

	KUrl url = m_browser->url();
        QString path = url.path();
	if (path.isEmpty()){
		KMessageBox::error(this, "No image path selected.", "ERROR");
		return;
	}
	if (!QFile::exists(path)){
		KMessageBox::error(this, path + " is not a valid file path.", "ERROR");
		return;
	}
        QString name = m_browser->name(); //Put the name typed by the user into this varable
        QDomNodeList nodes = doc.elementsByTagName("image");
        QDomElement back;
        back = nodes.at(0).toElement();
        back.setAttribute("xlink:href", path);
        back.setAttribute("sodipodi:absref", path);

	QString deck_path = KStandardDirs::locateLocal( "data" , QString("carddecks/custom-") + name + "/", true);

	QFile index_file(deck_path + "index.desktop"); //Create a new index file for the custom deck.
	if (!index_file.open(QIODevice::WriteOnly | QFile::Truncate)){
		KMessageBox::error(this, QString("Could not open file ") + deck_path + "index.desktop for writing.", "ERROR");
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
		KMessageBox::error(this, QString("Could not open file ") + path, "ERROR");
		return;
	}

	preview_file.copy(deck_path + image_base);


	QString filename = deck_path + "custom-deck.svgz";	
	KFilterDev* zipfile = static_cast<KFilterDev*>(KFilterDev::deviceForFile(filename, "application/x-gzip"));
	if (!zipfile){
		KMessageBox::error(this, QString("Could not find compression filter"), "ERROR");
	}
	zipfile->setOrigFileName(filename.toLatin1());
	zipfile->open(QIODevice::WriteOnly);
	zipfile->write(doc.toString().toLatin1(), doc.toString().size()); 
	zipfile->close();
	KMessageBox::information(this, "Success!", "decKreator has successfully created a custom theme for your KPatience enjoyment.");
}

void ImageWindow::cancelButtonPressed()
{
	m_app->quit();
}
