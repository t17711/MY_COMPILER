// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2016 by George Wolberg
//
// MainWindow.cpp - MainWindow class
//
// Written by: George Wolberg, 2016
// ===============================================================

#include "MainWindow.h"
#include "Dummy.h"
#include "Threshold.h"
#include "Contrast.h"

enum {DUMMY, THRESHOLD, CONTRAST, QUANTIZATION};
enum { RGB, R, G, B, GRAY };

QString GroupBoxStyle = "QGroupBox {\
									border: 4px solid blue;	\
									border-radius: 9px;	\
									background: yellow; \
									margin-top: 0.5em;}"; // sets main widget groupboxe's style

MainWindow *g_mainWindowP = NULL;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// MainWindow constructor.
//
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  m_code(-1)
{
	setWindowTitle("Capstone Project");

	// set global variable for main window pointer
	g_mainWindowP = this;

	// INSERT YOUR ACTIONS AND MENUS
	createActions();
	createMenus  ();
	createWidgets();
	setToolbarIcons();

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createActions:
//
// Create actions to associate with menu and toolbar selection.
//
void
MainWindow::createActions()
{
	//////////////////////////////
	// File Actions
	//////////////////////////////

	m_actionOpen = new QAction("&Open", this);
	m_actionOpen->setShortcut(tr("Ctrl+O"));
	connect(m_actionOpen, SIGNAL(triggered()), this, SLOT(open()));

	m_actionQuit = new QAction("&Quit", this);
	m_actionQuit->setShortcut(tr("Ctrl+Q"));
	connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	m_actionSave = new QAction(tr("&Save"), this);
	m_actionSave->setShortcut(tr("Ctrl+S"));
	connect(m_actionSave, SIGNAL(triggered()), this, SLOT(save()));
	m_actionSave->setDisabled(true);

	//////////////////////////////
	// Point Ops Actions
	//////////////////////////////

	m_actionThreshold = new QAction("&Threshold", this);
	m_actionThreshold->setShortcut(tr("Ctrl+T"));
	m_actionThreshold->setData(THRESHOLD);

	m_actionContrast = new QAction("&Contrast", this);
	m_actionContrast->setShortcut(tr("Ctrl+C"));
	m_actionContrast->setData(CONTRAST);

	// one signal-slot connection for all actions;
	// execute() will resolve which action was triggered
	connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(execute(QAction*)));

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createMenus:
//
// Create menus and install in menubar.
//
void
MainWindow::createMenus()
{
	// File menu
	m_menuFile = menuBar()->addMenu("&File");
	m_menuFile->addAction(m_actionOpen);
	m_menuFile->addAction(m_actionSave);
	m_menuFile->addAction(m_actionQuit);

	// Point Ops menu
	m_menuPtOps = menuBar()->addMenu("&Point Ops");
	m_menuPtOps->addAction(m_actionThreshold);
	m_menuPtOps->addAction(m_actionContrast );
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createWidgets:
//
// Create widgets for image display and filter control panels.
//
void
MainWindow::createWidgets()
{
	// assemble image display widget and control panel in horizontal layout
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(createGroupDisplay());
	hbox->setStretch(0, 1);
	// makes image part stretched or panel will be 50 50, 0 is 1st item that has set stretched to true(1)
	hbox->addWidget(createGroupPanel());

	// create container widget and set its layout
	QWidget *w = new QWidget;
	w->setLayout(hbox);

	// set central widget so that it can be displayed
	setCentralWidget(w); // need to set it for qmain window for it to show up as central widget
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createGroupPanel:
//
// Create group box for control panel.
//
QGroupBox*
MainWindow::createGroupPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox;
	groupBox->setMinimumWidth(350); // sets the minimum width for option panel

	// filter's enum indexes into container of image filters
	m_imageFilterType[DUMMY	   ] = new Dummy;
	m_imageFilterType[THRESHOLD] = new Threshold;
	m_imageFilterType[CONTRAST ] = new Contrast;

	// create a stacked widget to hold multiple control panels
	m_stackWidgetPanels = new QStackedWidget;

	// add filter control panels to stacked widget
	m_stackWidgetPanels->addWidget(m_imageFilterType[DUMMY    ]->controlPanel());
	m_stackWidgetPanels->addWidget(m_imageFilterType[THRESHOLD]->controlPanel());
	m_stackWidgetPanels->addWidget(m_imageFilterType[CONTRAST ]->controlPanel());

	// display blank dummmy panel initially
	m_stackWidgetPanels->setCurrentIndex(0);

	// assemble display and mode groups into horizontal layout
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(createDisplayButtons());
	hbox->addWidget(createModeButtons   ());
	// create histogram plot
	m_histogram = new QCustomPlot;

	// set histogram title
	m_histogram->plotLayout()->insertRow(0);
	m_histogram->plotLayout()->addElement(0, 0, new QCPPlotTitle(m_histogram, "Histogram"));

	// assign label axes
	m_histogram->xAxis->setLabel("Intensity");
	m_histogram->yAxis->setLabel("Frequency");
	m_histogram->xAxis->setAutoTickStep(0);
	m_histogram->xAxis->setTickStep(32.);

	// set axes ranges, so we see all the data
	m_histogram->xAxis->setRange(0, MXGRAY);
	m_histogram->yAxis->setRange(0, MXGRAY);
	m_histogram->setMinimumHeight(400);
	m_histogram->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	// create extension and insert histogram
	m_extension = new QWidget;
	QVBoxLayout *vBox2 = new QVBoxLayout(m_extension);
	vBox2->addWidget(m_histogram);

	// start dialog with hidden histogram
	m_extension->hide();

	// set signal-slot connection
	connect(m_checkboxHisto, SIGNAL(stateChanged(int)), this, SLOT(setHisto(int)));

	// assemble stacked widget in vertical layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addWidget(m_stackWidgetPanels);
	vbox->addWidget(m_extension);
	vbox->addStretch(1);
	vbox->addLayout(createExitButtons());
	groupBox->setLayout(vbox);

	return groupBox;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createGroupDisplay:
//
// Create group box for displaying images.
//
QGroupBox*
MainWindow::createGroupDisplay()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox;
	groupBox->setMinimumWidth(700);
	groupBox->setMinimumHeight(700);
	groupBox->setStyleSheet(GroupBoxStyle); // sets boundary for better visual, dont need on window

	// create stacked widget for input/output images
	m_stackWidgetImages = new QStackedWidget;

	// add QLabels to image stacked widget to display input/output images
	for(int i = 0; i<2; ++i)
		m_stackWidgetImages->addWidget(new QLabel);

	// add centering alignment on both labels
	QLabel *label; 
	// put image in center
	// extract qlabel from m_stacked images and align it to center
	label = (QLabel *) m_stackWidgetImages->widget(0); label->setAlignment(Qt::AlignCenter); // input
	label = (QLabel *) m_stackWidgetImages->widget(1); label->setAlignment(Qt::AlignCenter);  // output

	// set stacked widget to default setting: input image
	m_stackWidgetImages->setCurrentIndex(0);  // default to show input image

	// assemble stacked widget in vertical layout
	// add widget to layout , add layout to groupbox
	// cant put widget to groupbox, group is widget , cant add widget to widget

	QVBoxLayout *vbox = new QVBoxLayout; // create dummy layout and add widgets to it
	vbox->addWidget(m_stackWidgetImages); // add widget to layout
	groupBox->setLayout(vbox); // add layout to groupbox widget

	groupBox->setMinimumWidth	(500); // sets the minimum width for option panel
	groupBox->setMinimumHeight	(500);

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createDisplayButtons:
//
// Create preview window groupbox.
//
QGroupBox*
MainWindow::createDisplayButtons()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Display");

	// create radio buttons
	m_radioDisplay[0] = new QRadioButton("Input");
	m_radioDisplay[1] = new QRadioButton("Output");

	// create button group and add radio buttons to it
	QButtonGroup *bGroup = new QButtonGroup;
	for(int i = 0; i<2; ++i)
		bGroup->addButton(m_radioDisplay[i]);

	// set input radio button to be default
	m_radioDisplay[0]->setChecked(true);

	// create histogram checkbox
	m_checkboxHisto = new QCheckBox("Histogram");
	m_checkboxHisto->setCheckState(Qt::Unchecked);

	// assemble radio buttons into vertical widget
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_radioDisplay[0]);
	vbox->addWidget(m_radioDisplay[1]);
	vbox->addWidget(m_checkboxHisto);
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_radioDisplay[0], SIGNAL(clicked()), this, SLOT(displayIn()));
	connect(m_radioDisplay[1], SIGNAL(clicked()), this, SLOT(displayOut()));
	
	// disable at first
	m_radioDisplay[0]->setDisabled(true);
	m_radioDisplay[1]->setDisabled(true);
	m_checkboxHisto->setDisabled(true);

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createModeButtons:
//
// Create preview window groupbox.
//
QGroupBox*
MainWindow::createModeButtons()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Mode");

	// create radio buttons
	m_radioMode[0] = new QRadioButton("RGB");
	m_radioMode[1] = new QRadioButton("Gray");

	// create button group and add radio buttons to it
	QButtonGroup *bGroup = new QButtonGroup;
	for(int i = 0; i<2; ++i)
		bGroup->addButton(m_radioMode[i]);

	// set RGB radio button to be default
	m_radioMode[0]->setChecked(true);

	// assemble radio buttons into vertical widget
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_radioMode[0]);
	vbox->addWidget(m_radioMode[1]);
	vbox->addWidget(m_radioMode[1]);	// redundant radiobutton won't display; placeholder
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_radioMode[0], SIGNAL(clicked()), this, SLOT(modeRGB ()));
	connect(m_radioMode[1], SIGNAL(clicked()), this, SLOT(modeGray()));
	
	m_radioMode[0]->setDisabled(true);
	m_radioMode[1]->setDisabled(true);


	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createExitButtons:
//
// Create save/quit buttons.
//
QHBoxLayout*
MainWindow::createExitButtons()
{
	// create pushbuttons
	m_buttonReset = new QPushButton("Reset");
	QPushButton *buttonQuit  = new QPushButton("Quit");

	// init signal/slot connections
	connect(m_buttonReset, SIGNAL(clicked()), this, SLOT(reset()));
	connect(buttonQuit , SIGNAL(clicked()), this, SLOT(quit ()));

	// assemble pushbuttons in horizontal layout
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_buttonReset);
	buttonLayout->addWidget(buttonQuit );
	m_buttonReset->setDisabled(true);
	return buttonLayout;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::reset:
//
// Reset application.
//
void
MainWindow::reset()
{
	m_imageFilterType[ m_code ]->reset();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::quit:
//
// Quit application.
//
void
MainWindow::quit()
{
	// close the dialog window
	close();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::open:
//
// Open input image.
//
void
MainWindow::open() {
	QFileDialog dialog(this);

	// open the last known working directory
	if(!m_currentDir.isEmpty())
		dialog.setDirectory(m_currentDir);

	// display existing files and directories
	dialog.setFileMode(QFileDialog::ExistingFile);

	// invoke native file browser to select file
	m_file = dialog.getOpenFileName(this,
					"Open File", m_currentDir,
					"Images (*.jpg *.png *.ppm *.pgm *.bmp);;All files (*)");

	// verify that file selection was made
	if(m_file.isNull()) return;

	// save current directory
	QFileInfo f(m_file);
	m_currentDir = f.absolutePath();

	// DISABLE IMAGING FUNCTIONALITY FOR NOW....
	// read input image
	m_imageIn = IP_readImage(qPrintable(m_file));

	if(m_radioMode[1]->isChecked())
		IP_castImage(m_imageIn,  BW_IMAGE, m_imageSrc);
	else	IP_castImage(m_imageIn, RGB_IMAGE, m_imageSrc);

	// init vars
	m_width  = m_imageSrc->width ();
	m_height = m_imageSrc->height();
	enableActions();
	preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::display:
//
// Slot functions to display input and output images.
//
void MainWindow::displayIn () { display(0); }
void MainWindow::displayOut() { display(1); }

void MainWindow::display(int flag)
{
	// error checking
	if(m_imageSrc.isNull())		return;		// no input  image
	if(m_imageDst.isNull() && flag) return;		// no output image

	// raise the appropriate widget from the stack
	m_stackWidgetImages->setCurrentIndex(flag);

	// set radio button
	m_radioDisplay[flag]->setChecked(1);

	// determine image to be displayed
	ImagePtr I;
	if(flag == 0)
		I = m_imageSrc;
	else	I = m_imageDst;

	// init image dimensions
	int  w = m_imageDst->width();
	int  h = m_imageDst->height();

	// init view window dimensions
	int ww = m_stackWidgetImages->width();
	int hh = m_stackWidgetImages->height();

	// convert from ImagePtr to QImage to Pixmap
	QImage q;
	IP_IPtoQImage(I, q);
	
	// convert from QImage to Pixmap; rescale if image is larger than view window
	QPixmap p;
	if(MIN(w, h) > MIN(ww, hh))
		p = QPixmap::fromImage(q.scaled(QSize(ww, hh), Qt::KeepAspectRatio));
	else	p = QPixmap::fromImage(q);

	// assign pixmap to label widget for display
	QLabel *widget = (QLabel *) m_stackWidgetImages->currentWidget();
	widget->setPixmap(p);
	// compute histogram if histogram checkbox is set
	if (m_checkboxHisto->isChecked())
		displayHistogram(I);

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::displayHistogram:
//
// Display image histogram in control panel
//
void MainWindow::displayHistogram(ImagePtr I)
{
	int color;
	int histo[MXGRAY];
	int yminChannel = 0, ymaxChannel = 0;
	int yminHisto = 0, ymaxHisto = 0;
	int xmin, xmax;
	QVector<double> x, y;
	char buf[MXSTRLEN];

	// clear any previous histogram plots
	m_histogram->clearGraphs();

	// visit all selected channels in I: RGB, R, G, B, or gray
	for (int ch = 0; ch<I->maxChannel(); ch++) {
		// compute histogram
		IP_histogram(I, ch, histo, MXGRAY, m_histoXmin[ch], m_histoXmax[ch]);

		// init min and max for current channel
		yminChannel = ymaxChannel = histo[0];

		// init min and max histogram value among all channels
		if (!ch) yminHisto = ymaxHisto = histo[0];

		// clear vector of x- and y-coordinates
		x.clear();
		y.clear();

		// visit all histogram entries and save into x and y vectors
		for (int i = 0; i<MXGRAY; i++) {
			x.push_back(i);
			y.push_back(histo[i]);

			// save channel min and max
			yminChannel = MIN(yminChannel, histo[i]);
			ymaxChannel = MAX(ymaxChannel, histo[i]);
		}

		// add new graph for histogram channel
		m_histogram->addGraph();

		// if single channel was selected, it is in channel 0 and should
		// be drawn in corresponding color (based on m_histoColor).
		// Else, color is based on ch value: 0,1,2 corresponds to R,G,B
		// Add 1 to ch so that ch=1 corresponds to green, and ch=2 to blue
		if (!ch) color = m_histoColor;
		else	color = ch + 1;

		// convert xmin, xmax of channel into int
		xmin = m_histoXmin[ch];
		xmax = m_histoXmax[ch];

		// set histogram name and color
		switch (color) {
		case RGB:
		case R: sprintf(buf, "R: X[%d,%d] Y[%d,%d]",
			xmin, xmax, yminChannel, ymaxChannel);
			m_histogram->graph(ch)->setName(buf);
			m_histogram->graph(ch)->setPen(QPen(Qt::red));
			break;
		case G: sprintf(buf, "G: X[%d,%d] Y[%d,%d]",
			xmin, xmax, yminChannel, ymaxChannel);
			m_histogram->graph(ch)->setName(buf);
			m_histogram->graph(ch)->setPen(QPen(Qt::green));
			break;
		case B: sprintf(buf, "B: X[%d,%d] Y[%d,%d]",
			xmin, xmax, yminChannel, ymaxChannel);
			m_histogram->graph(ch)->setName(buf);
			m_histogram->graph(ch)->setPen(QPen(Qt::blue));
			break;
		case GRAY:
			sprintf(buf, "Gray: X[%d,%d] Y[%d,%d]",
				xmin, xmax, yminChannel, ymaxChannel);
			m_histogram->graph(ch)->setName(buf);
			m_histogram->graph(ch)->setPen(QPen(Qt::black));
			break;
		}

		// set data
		m_histogram->graph(ch)->setData(x, y);

		// update min and max histogram values among all channels
		yminHisto = MIN(yminChannel, yminHisto);
		ymaxHisto = MAX(ymaxChannel, ymaxHisto);
	}

	// turn on legend to print histogram params
	m_histogram->legend->setVisible(true);

	// set y-axis range and replot
	m_histogram->yAxis->setRange(yminHisto, ymaxHisto);
	m_histogram->rescaleAxes();
	m_histogram->replot();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::setHisto:
//
// Slot to show/hide histogram and set/reset histogram checkbox.
//
void
MainWindow::setHisto(int flag)
{
	m_extension->setVisible(flag);
	if (flag)
		m_checkboxHisto->setCheckState(Qt::Checked);
	else    m_checkboxHisto->setCheckState(Qt::Unchecked);
	preview();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::mode:
//
// Slot functions to display RGB and grayscale images.
//
void MainWindow::modeRGB () { mode(0); }
void MainWindow::modeGray() { mode(1); }

void MainWindow::mode(int flag)
{
	// error checking
	if(m_imageSrc.isNull()) return;		// no input image

	if(flag)
		IP_castImage(m_imageIn,  BW_IMAGE, m_imageSrc);
	else	IP_castImage(m_imageIn, RGB_IMAGE, m_imageSrc);

	if (m_imageSrc->imageType() == BW_IMAGE)
		m_histoColor = GRAY;	// gray
	else	m_histoColor = 0;	// RGB

	// re-apply filter for changed mode
	if(m_code > 0)
		m_imageFilterType[m_code]->applyFilter(m_imageSrc, m_imageDst);

	// display image
	preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::preview:
//
// Display preview image.
//
void
MainWindow::preview()
{
	// display requested image
	if(m_radioDisplay[0]->isChecked())
		display(0);
	else	display(1);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::execute:
//
// Determine which action was triggered and execute respective action.
// Skip this if action is not image-processing related: open(), quit()
//
void
MainWindow::execute(QAction* action)
{
	// skip over menu ops that don't require image processing
	QString name = action->text();
	if(name == QString("&Open") || name == QString("&Quit") || name == QString("&Save")) {
		m_code = -1;
		return;
	}

	// get code from action
	m_code = action->data().toInt();

	// set output radio button to true
	m_radioDisplay[1]->setChecked(true);

	// use code to index into stack widget and array of filters
	m_stackWidgetPanels->setCurrentIndex(m_code);
	m_imageFilterType[m_code]->applyFilter(m_imageSrc, m_imageDst);
	preview();
	m_buttonReset->setDisabled(false);

}
// MainWindow::setToolbarIcons
// set toolbar buttons

void MainWindow::setToolbarIcons(){
	m_toolBar = new QToolBar(this);
	m_toolBar->addAction(m_actionOpen);
	m_toolBar->addAction(m_actionSave);
	m_toolBar->addAction(m_actionThreshold);
	m_toolBar->addAction(m_actionContrast);

	m_toolBar->setIconSize(QSize(50, 50));
	addToolBarBreak();
	addToolBar(m_toolBar);
	addToolBarBreak();
}
// MainWindow::save
// saves current output image
void MainWindow::save(){

	QFileDialog dialog(this);

	// open the last known working directory
	if (m_currentDir.isEmpty())  return;
	if (m_imageDst.isNull()) return;

	QString filename = dialog.getSaveFileName(this,
		tr("Save Image"), m_currentDir + "/new image", tr("Image Files (*.png *.jpg *.bmp)"));

	QImage q;
	IP_IPtoQImage(m_imageDst, q);

	QPixmap p;
	p=QPixmap::fromImage(q);

	p.save(filename, "png");

}

/* disable buttons that are not used until button are enabled*/
void MainWindow::enableActions(){
	// enable save and groupbox actions
	m_actionSave->setDisabled(false);
	m_radioMode[0]->setDisabled(false);
	m_radioMode[1]->setDisabled(false);
	m_radioDisplay[0]->setDisabled(false);
	m_radioDisplay[1]->setDisabled(false);
	m_checkboxHisto->setDisabled(false);
	// enable image filters
	m_imageFilterType[THRESHOLD]->disable(false);
	m_imageFilterType[CONTRAST]->disable(false);


}