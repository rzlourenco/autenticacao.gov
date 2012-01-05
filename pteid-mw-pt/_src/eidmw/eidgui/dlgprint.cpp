/* ****************************************************************************

 * eID Middleware Project.
 * Copyright (C) 2008-2009 FedICT.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version
 * 3.0 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, see
 * http://www.gnu.org/licenses/.

**************************************************************************** */

#include <QPixmap>
#include <QImage>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>

#include "dlgprint.h"
#include "eidlib.h"
#include "mainwnd.h"

using namespace eIDMW;

dlgPrint::dlgPrint( QWidget* parent, CardInformation& CI_Data, GenPur::UI_LANGUAGE lng, QString const& cardTypeText) 
: QDialog(parent)
, m_CI_Data(CI_Data)
, m_CurrReaderName("")
{	
	ui.setupUi(this);
	const QIcon Ico = QIcon( ":/images/Images/Icons/Print.png" );
	this->setWindowIcon( Ico );

	QDesktopWidget* desktop = QApplication::desktop();
	int screenNr = desktop->screenNumber();
	QRect rect = desktop->availableGeometry(screenNr);
	int height = rect.height();

	int thiswidth = this->width();
	int thisheight = this->height();

	if (thisheight > height)
	{
		this->resize(thiswidth,height-20); //make sure the window fits
	}

	QTextBlockFormat textBlockFormat;
	textBlockFormat.setLeftMargin ( 100 );

	/*PQTextCursor cursor(ui.paperview->textCursor());
	QTextFrame*  topFrame = cursor.currentFrame();
	cursor.movePosition(QTextCursor::Start);*/

	/*if (on_pbPrint_clicked())
	{
		drawpdf(CI_Data);
	}*/
}

dlgPrint::~dlgPrint()
{
}

void dlgPrint::on_pbPrint_clicked( void )
{
	CardInformation cdata = m_CI_Data;
	QString pdffilepath;
	QString defaultfilepath;

	defaultfilepath = QDir::homePath();
	defaultfilepath.append("/CartaoCidadao.pdf");
	pdffilepath = QFileDialog::getSaveFileName(this, tr("Save File"), defaultfilepath, tr("Pdf Files (*.pdf)"));

	drawpdf(cdata, pdffilepath.toStdString().c_str());

	/*QPrinter	  printer;
 	QPrintDialog* dialog = new QPrintDialog(&printer, this);
 	dialog->setWindowTitle(tr("Print Document"));
	if (ui.paperview->textCursor().hasSelection())
	{
		dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
	}
	if (dialog->exec() != QDialog::Accepted)
	{
		return;
	}

	ui.paperview->print(&printer);
	*/
	done(0);
}

void dlgPrint::on_pbCancel_clicked( void )
{
	done(0);
}

void dlgPrint::on_chboxID_toggled( bool bChecked )
{

}

void dlgPrint::on_chboxAddress_toggled( bool bChecked )
{

}

void dlgPrint::on_chboxIDExtra_toggled( bool bChecked )
{

}

void dlgPrint::on_chboxPersoData_toggled( bool bChecked )
{

}

bool dlgPrint::persodata_triggered(CardInformation& CI_Data)
{
	try
	{
		unsigned long	ReaderStartIdx = 1;
		bool			bRefresh	   = false;
		unsigned long	ReaderEndIdx   = ReaderSet.readerCount(bRefresh);
		unsigned long	ReaderIdx	   = 0;

		if (ReaderStartIdx!=(unsigned long)-1)
		{
			ReaderEndIdx = ReaderStartIdx+1;
		}
		else
		{
			ReaderStartIdx=0;
		}

		bool bCardPresent = false;
		PTEID_CardType lastFoundCardType = PTEID_CARDTYPE_UNKNOWN;

		const char* readerName = ReaderSet.getReaderName(ReaderIdx);
		m_CurrReaderName = readerName;
		PTEID_ReaderContext &ReaderContext = ReaderSet.getReaderByName(m_CurrReaderName.toLatin1().data());

		if (ReaderContext.isCardPresent())
		{
			PTEID_EIDCard&	Card	= ReaderContext.getEIDCard();
			CI_Data.LoadDataPersoData(Card,m_CurrReaderName);
		}
	}	catch (PTEID_Exception &e) {
		QString msg(tr("General exception"));
		//ShowPTEIDError( e.GetError(), msg );
		return false;
	}
}

bool dlgPrint::addressPINRequest_triggered(CardInformation& CI_Data)
{
	//Workaround: Make PIN window called only one time

	/*if (!m_CI_Data.isDataLoaded())
	{
		return true;
	}*/
	try
	{
		unsigned long	ReaderStartIdx = 1;
		bool			bRefresh	   = false;
		unsigned long	ReaderEndIdx   = ReaderSet.readerCount(bRefresh);
		unsigned long	ReaderIdx	   = 0;

		if (ReaderStartIdx!=(unsigned long)-1)
		{
			ReaderEndIdx = ReaderStartIdx+1;
		}
		else
		{
			ReaderStartIdx=0;
		}

		bool bCardPresent = false;
		PTEID_CardType lastFoundCardType = PTEID_CARDTYPE_UNKNOWN;

		const char* readerName = ReaderSet.getReaderName(ReaderIdx);
		m_CurrReaderName = readerName;
		PTEID_ReaderContext &ReaderContext = ReaderSet.getReaderByName(m_CurrReaderName.toLatin1().data());


		QString caption(tr("Identity Card: PIN verification"));


		//------------------------------------
		// make always sure a card is present
		//------------------------------------
		if (ReaderContext.isCardPresent())
		{
			QString PinName = "PIN da Morada";

			PTEID_EIDCard&	Card	= ReaderContext.getEIDCard();
			PTEID_Pins&		Pins	= Card.getPins();

			for (unsigned long PinIdx=0; PinIdx<Pins.count(); PinIdx++)
			{
				PTEID_Pin&	Pin			= Pins.getPinByNumber(PinIdx);
				QString		CurrPinName	= Pin.getLabel();

				if (CurrPinName==PinName)
				{
					unsigned long triesLeft = -1;
					bool		  bResult   = Pin.verifyPin("",triesLeft);

					CI_Data.LoadDataAddress(Card,m_CurrReaderName);
					//QString		  msg(tr("PIN verification "));

					QString msg = bResult ? tr("PIN verification passed"):tr("PIN verification failed");
					if (!bResult)
					{
							/*
							QString nrTriesLeft;
							nrTriesLeft.setNum(triesLeft);
							msg += "\n";
							msg += "( ";
							msg += tr("Number of tries left: ") + nrTriesLeft + " )";
							m_ui.txtPIN_Status->setText(msg);
							m_ui.txtPIN_Status->setAccessibleName(msg);
							*/

							return false;
					}
					else
					{
						//m_ui.txtPIN_Status->setText("Restam 3 tentativas");
						//m_ui.txtPIN_Status->setAccessibleName("Restam 3 tentativas");
					}
					QMessageBox::information( this, caption,  msg, QMessageBox::Ok );
					break;
				}
			}
		}
		else
		{
			std::cout << "no card" << std::endl;
			QString msg(tr("No card present"));
			QMessageBox::information( this, caption,  msg, QMessageBox::Ok );
			return false;
		}
	}
	catch (PTEID_Exception &e)
	{
		QString msg(tr("General exception"));
		//ShowPTEIDError( e.GetError(), msg );
		return false;
	}
	catch (...)
	{
		QString msg(tr("Unknown exception"));
		//ShowPTEIDError( 0, msg );
		return false;
	}
	return true;
}

void dlgPrint::drawpdf(CardInformation& CI_Data, const char *filepath)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_surface_t *imagefront;
	cairo_surface_t *imageback;
	int w, h;

	tFieldMap& PersonFields = CI_Data.m_PersonInfo.getFields();
	tFieldMap& CardFields = CI_Data.m_CardInfo.getFields();

	//// Create pdf with cairo
	surface = cairo_pdf_surface_create(filepath, 504, 648);

	cr = cairo_create(surface);

	//// Set Image - Front
	QImage *qimg = new QImage (":/images/Images/application_print_PDF.png");
	imagefront = cairo_image_surface_create_for_data(qimg->bits(), CAIRO_FORMAT_RGB24, qimg->width(), qimg->height(), qimg->bytesPerLine());
	w = cairo_image_surface_get_width (imagefront);
	h = cairo_image_surface_get_height (imagefront);

	cairo_scale (cr, 510.0/w, 650.0/h);

	cairo_set_source_surface(cr, imagefront, 0, 0);
	cairo_paint(cr);


	//// Set Text Entries ////
	cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 12.0);
	cairo_set_source_rgb(cr, 0, 0, 0);

	//////////////////////////////ID FIELDS///////////////////////////
	if (ui.chboxID->isChecked())
	{
		////Surname
		cairo_move_to(cr, 20.0, 90.0);
		cairo_show_text(cr, PersonFields[NAME].toStdString().c_str());


		////Given Name
		cairo_move_to(cr, 20.0, 125.0);
		cairo_show_text(cr, PersonFields[GIVENNAME].toStdString().c_str());

		///Sex
		cairo_move_to(cr, 20.0, 165.0);
		cairo_show_text(cr, PersonFields[SEX].toStdString().c_str());

		///Height
		cairo_move_to(cr, 145.0, 170.0);
		cairo_show_text(cr, PersonFields[HEIGHT].toStdString().c_str());

		///Nacionality
		cairo_move_to(cr, 20.0, 200.0);
		cairo_show_text(cr, PersonFields[NATIONALITY].toStdString().c_str());

		////Date of Birth
		cairo_move_to(cr, 145.0, 200.0);
		cairo_show_text(cr, PersonFields[BIRTHDATE].toStdString().c_str());

		///Document nr
		cairo_move_to(cr, 20.0, 240.0);
		cairo_show_text(cr, PersonFields[DOCUMENTNUMBER].toStdString().c_str());

		///Expirity Date
		cairo_move_to(cr, 145.0, 240.0);
		cairo_show_text(cr, CardFields[CARD_VALIDUNTIL].toStdString().c_str());

		////Country
		cairo_move_to(cr, 290.0, 240.0);
		cairo_show_text(cr, PersonFields[COUNTRY].toStdString().c_str());

		////Father
		cairo_move_to(cr, 20.0, 270.0);
		cairo_show_text(cr, PersonFields[FATHER].toStdString().c_str());

		////Mother
		cairo_move_to(cr, 20.0, 295.0);
		cairo_show_text(cr, PersonFields[MOTHER].toStdString().c_str());
	}

	//////////////////////////////IDExtra FIELDS///////////////////////////
	if (ui.chboxIDExtra->isChecked())
	{
		////NIF
		cairo_move_to(cr, 20.0, 380.0);
		cairo_show_text(cr, PersonFields[TAXNO].toStdString().c_str());

		////NSS
		cairo_move_to(cr, 220.0, 380.0);
		cairo_show_text(cr, PersonFields[SOCIALSECURITYNO].toStdString().c_str());

		///NUS
		cairo_move_to(cr, 450.0, 380.0);
		cairo_show_text(cr, PersonFields[HEALTHNO].toStdString().c_str());

		///Card Version
		cairo_move_to(cr, 20.0, 420.0);
		cairo_show_text(cr, PersonFields[CARDVERSION].toStdString().c_str());

		///Issuing Date
		cairo_move_to(cr, 180.0, 420.0);
		cairo_show_text(cr, CardFields[CARD_VALIDFROM].toStdString().c_str());

		////Issuing Entity
		cairo_move_to(cr, 300.0, 420.0);
		cairo_show_text(cr, PersonFields[ISSUINGENTITY].toStdString().c_str());

		///Document type
		cairo_move_to(cr, 470.0, 420.0);
		cairo_show_text(cr, PersonFields[DOCUMENTTYPE].toStdString().c_str());

		///Card Status
		cairo_move_to(cr, 20.0, 460.0);
		cairo_show_text(cr, "???????");

		////Local of Request
		cairo_move_to(cr, 220.0, 460.0);
		cairo_show_text(cr, PersonFields[LOCALOFREQUEST].toStdString().c_str());
	}

	//////////////////////////////Address FIELDS///////////////////////////
	if (ui.chboxAddress->isChecked())
	{
		addressPINRequest_triggered(CI_Data);
		tFieldMap& AddressFields = CI_Data.m_AddressInfo.getFields();
		////ADDRESS Locality
		cairo_move_to(cr, 410.0, 670.0);
		cairo_show_text(cr, AddressFields[ADDRESS_LOCALITY].toStdString().c_str());

		////ADDRESS Municipality
		cairo_move_to(cr, 20.0, 550.0);
		cairo_show_text(cr, AddressFields[ADDRESS_MUNICIPALITY].toStdString().c_str());

		////ADDRESS District
		cairo_move_to(cr, 20.0, 510.0);
		cairo_show_text(cr, AddressFields[ADDRESS_DISTRICT].toStdString().c_str());

		////ADDRESS Civil Parish
		cairo_move_to(cr, 320.0, 550.0);
		cairo_show_text(cr, AddressFields[ADDRESS_CIVILPARISH].toStdString().c_str());

		////ADDRESS ABBrStreetType
		cairo_move_to(cr, 20.0, 590.0);
		cairo_show_text(cr, AddressFields[ADDRESS_ABBRSTREETTYPE].toStdString().c_str());

		////ADDRESS Street Type
		cairo_move_to(cr, 240.0, 590.0);
		cairo_show_text(cr, AddressFields[ADDRESS_STREETTYPE].toStdString().c_str());

		////ADDRESS Street Name
		cairo_move_to(cr, 350.0, 590.0);
		cairo_show_text(cr, AddressFields[ADDRESS_STREETNAME].toStdString().c_str());

		////ADDRESS Abbr Building Type
		cairo_move_to(cr, 20.0, 380.0);
		cairo_show_text(cr, AddressFields[ADDRESS_ABBRBUILDINGTYPE].toStdString().c_str());

		////ADDRESS Building Type
		cairo_move_to(cr, 20.0, 380.0);
		cairo_show_text(cr, AddressFields[ADDRESS_BUILDINGTYPE].toStdString().c_str());

		////ADDRESS Door No
		cairo_move_to(cr, 20.0, 670.0);
		cairo_show_text(cr, AddressFields[ADDRESS_DOORNO].toStdString().c_str());

		////ADDRESS Floor
		cairo_move_to(cr, 180.0, 670.0);
		cairo_show_text(cr, AddressFields[ADDRESS_FLOOR].toStdString().c_str());

		////ADDRESS Side
		cairo_move_to(cr, 340.0, 670.0);
		cairo_show_text(cr, AddressFields[ADDRESS_SIDE].toStdString().c_str());

		////ADDRESS Zip4
		cairo_move_to(cr, 20.0, 710.0);
		cairo_show_text(cr, AddressFields[ADDRESS_ZIP4].toStdString().c_str());

		////ADDRESS Zip3
		cairo_move_to(cr, 90.0, 710.0);
		cairo_show_text(cr, AddressFields[ADDRESS_ZIP3].toStdString().c_str());

		////ADDRESS Place
		cairo_move_to(cr, 20.0, 420.0);
		cairo_show_text(cr, AddressFields[ADDRESS_PLACE].toStdString().c_str());

		////ADDRESS Postal Locality
		cairo_move_to(cr, 180.0, 710.0);
		cairo_show_text(cr, AddressFields[ADDRESS_POSTALLOCALITY].toStdString().c_str());
	}

	if (ui.chboxPersoData->isChecked())
	{
		persodata_triggered(CI_Data);
		tFieldMap& PersoDataFields = CI_Data.m_PersoDataInfo.getFields();
		cairo_move_to(cr, 20.0, 760.0);
		cairo_show_text(cr, (QString::fromUtf8(PersoDataFields[PERSODATA_INFO].toStdString().c_str())).toStdString().c_str());
	}

	//Page 1
	cairo_show_page(cr);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
	done(0);
	return;
}


//-----------------------------------------------------------
// a button btnPDF can be inserted on this window.
// The PDF export is not very good though...
//-----------------------------------------------------------
/*
void dlgPrint::on_btnPDF_clicked( void )
{
	QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", QString(), "*.pdf");
	if (!fileName.isEmpty()) {
		if (QFileInfo(fileName).suffix().isEmpty())
		{
			fileName.append(".pdf");
		}
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		ui.paperview->document()->print(&printer);
	} 
	done(0);
}
*/
