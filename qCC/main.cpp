//##########################################################################
//#                                                                        #
//#                            CLOUDCOMPARE                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#include <ccIncludeGL.h>

//Qt
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QMessageBox>
#include <QLocale>
#include <QTime>
#include <QTranslator>

#ifdef Q_OS_MAC
#include <QFileOpenEvent>
#endif

//qCC_db
#include <ccTimer.h>
#include <ccNormalVectors.h>
#include <ccColorScalesManager.h>

//qCC_io
#include <FileIOFilter.h>

//local
#include "mainwindow.h"
#include "ccGuiParameters.h"
#include "ccCommandLineParser.h"

#ifdef USE_VLD
//VLD
#include <vld.h>
#endif

//! QApplication wrapper
class qccApplication : public QApplication
{
public:
	qccApplication( int &argc, char **argv )
		: QApplication( argc, argv )
	{
		setOrganizationName("CCCorp");
		setApplicationName("CloudCompare");
		setAttribute( Qt::AA_ShareOpenGLContexts );
#ifdef Q_OS_MAC
		// Mac OS X apps don't show icons in menus
		setAttribute( Qt::AA_DontShowIconsInMenus );
#endif
	}

#ifdef Q_OS_MAC
protected:
	bool event( QEvent *inEvent )
	{
		switch ( inEvent->type() )
		{
		case QEvent::FileOpen:
			{
				MainWindow* mainWindow = MainWindow::TheInstance();
				if ( mainWindow == NULL )
					return false;

				mainWindow->addToDB( QStringList(static_cast<QFileOpenEvent *>(inEvent)->file()) );
				return true;
			}

		default:
			return QApplication::event( inEvent );
		}
	}
#endif
};

int main(int argc, char **argv)
{
	//See http://doc.qt.io/qt-5/qopenglwidget.html#opengl-function-calls-headers-and-qopenglfunctions
	/** Calling QSurfaceFormat::setDefaultFormat() before constructing the QApplication instance is mandatory
		on some platforms (for example, OS X) when an OpenGL core profile context is requested. This is to
		ensure that resource sharing between contexts stays functional as all internal contexts are created
		using the correct version and profile.
	**/
	{
		QSurfaceFormat format = QSurfaceFormat::defaultFormat();
		format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
		format.setStereo(true);
		format.setStencilBufferSize(0);
#ifdef Q_OS_MAC
		format.setStereo(false);
		format.setVersion( 2, 1 );
		format.setProfile( QSurfaceFormat::CoreProfile );
#endif
#ifdef _DEBUG
		format.setOption(QSurfaceFormat::DebugContext);
#endif
		QSurfaceFormat::setDefaultFormat(format);
	}

	//QT initialiation
	qccApplication app(argc, argv);

	//Force 'english' local so as to get a consistent behavior everywhere
	QLocale::setDefault(QLocale::English);

#ifdef Q_OS_LINUX
    // we reset the numeric locale. As suggested in documetation
    // see http://qt-project.org/doc/qt-5/qcoreapplication.html#locale-settings
    // Basically - from doc: - "On Unix/Linux Qt is configured to use the system locale settings by default.
    // This can cause a conflict when using POSIX functions, for instance,
    // when converting between data types such as floats and strings"
    setlocale(LC_NUMERIC,"C");
#endif

#ifdef USE_VLD
	VLDEnable();
#endif

	//splash screen
	QSplashScreen* splash = 0;
	QTime splashStartTime;

	//Command line mode?
	bool commandLine = (argc > 1 && argv[1][0] == '-');
	
	//specific case: translation file selection
	int lastArgumentIndex = 1;
	QTranslator translator;
	if (commandLine && QString(argv[1]).toUpper() == "-LANG")
	{
		QString langFilename = QString(argv[2]);
		
		//Load translation file
		if (translator.load(langFilename, QCoreApplication::applicationDirPath()))
		{
			qApp->installTranslator(&translator);
		}
		else
		{
			QMessageBox::warning(0, QObject::tr("Translation"), QObject::tr("Failed to load language file '%1'").arg(langFilename));
		}
		commandLine = false;
		lastArgumentIndex = 3;
	}

	//command line mode
	if (!commandLine)
	{
		//DGM FIXME: do the same with Qt 5 + reject if Qt version is < 2.1
		//if (!QGLFormat::hasOpenGL())
		//{
		//	QMessageBox::critical(0, "Error", "This application needs OpenGL to run!");
		//	return EXIT_FAILURE;
		//}

		//splash screen
		splashStartTime.start();
		QPixmap pixmap(QString::fromUtf8(":/CC/images/imLogoV2Qt.png"));
		splash = new QSplashScreen(pixmap,Qt::WindowStaysOnTopHint);
		splash->show();
		QApplication::processEvents();
	}

	//global structures initialization
	ccTimer::Init();
	FileIOFilter::InitInternalFilters(); //load all known I/O filters (plugins will come later!)
	ccNormalVectors::GetUniqueInstance(); //force pre-computed normals array initialization
	ccColorScalesManager::GetUniqueInstance(); //force pre-computed color tables initialization

	int result = 0;

	if (commandLine)
	{
		//command line processing (no GUI)
		result = ccCommandLineParser::Parse(argc,argv);
	}
	else
	{
		//main window init.
		MainWindow* mainWindow = MainWindow::TheInstance();
		if (!mainWindow)
		{
			QMessageBox::critical(0, "Error", "Failed to initialize the main application window?!");
			return EXIT_FAILURE;
		}
		mainWindow->show();
		QApplication::processEvents();

		if (argc > lastArgumentIndex)
		{
			if (splash)
				splash->close();

			//any additional argument is assumed to be a filename --> we try to load it/them
			QStringList filenames;
			for (int i = lastArgumentIndex; i<argc; ++i)
				filenames << QString(argv[i]);

			mainWindow->addToDB(filenames);
		}
		
		if (splash)
		{
			//we want the splash screen to be visible a minimum amount of time (1000 ms.)
			while (splashStartTime.elapsed() < 1000)
			{
				splash->raise();
				QApplication::processEvents(); //to let the system breath!
			}

			splash->close();
			QApplication::processEvents();

			delete splash;
			splash = 0;
		}

		//let's rock!
		try
		{
			result = app.exec();
		}
		catch(...)
		{
			QMessageBox::warning(0, "CC crashed!","Hum, it seems that CC has crashed... Sorry about that :)");
		}
	}

	//release global structures
	MainWindow::DestroyInstance();
	FileIOFilter::UnregisterAll();

#ifdef CC_TRACK_ALIVE_SHARED_OBJECTS
	//for debug purposes
	unsigned alive = CCShareable::GetAliveCount();
	if (alive > 1)
	{
		printf("Error: some shared objects (%u) have not been released on program end!",alive);
		system("PAUSE");
	}
#endif

	return result;
}
