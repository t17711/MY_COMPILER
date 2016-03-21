#include "HistogramStretch.h"
#include "MainWindow.h"

extern MainWindow *g_mainWindowP;

HistogramStretch::
HistogramStretch(QWidget *parent) : ImageFilter(parent)
{}

void
HistogramStretch::reset(){
	// disable signals
	m_sliderMax->			blockSignals(true);
	m_spinBoxMax->			blockSignals(true);

	m_sliderMin->			blockSignals(true);
	m_spinBoxMin->			blockSignals(true);

	m_CheckBoxAutoMax->		blockSignals(true);
	m_CheckBoxAutoMin->		blockSignals(true);
	// reset values
	m_sliderMin->			setValue(0);		 // min is 0
	m_spinBoxMin->			setValue(0);
	m_sliderMax->			setValue(MXGRAY);	 // max is 255
	m_spinBoxMax->			setValue(MXGRAY);	 // max is 255

	// enable signals
	m_sliderMax->			blockSignals(false);
	m_spinBoxMax->			blockSignals(false);

	m_sliderMin->			blockSignals(false);
	m_spinBoxMin->			blockSignals(false);

	m_CheckBoxAutoMax->		blockSignals(false);
	m_CheckBoxAutoMin->		blockSignals(false);

}

QGroupBox*
HistogramStretch::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Histogram Stretching");
	// create minimum slider

	// for minimum
	QLabel *label_min = new QLabel(tr("Minimum"));

	m_sliderMin =	new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderMin->	setTickPosition(QSlider::TicksBelow);
	m_sliderMin->	setTickInterval(25);
	m_sliderMin->	setRange(0, MXGRAY - 5);	/// max of max is mxgray so i have 5 difference on both
	m_sliderMin->	setValue(0); // min is 0

	m_spinBoxMin =	new QSpinBox(m_ctrlGrp);
	m_spinBoxMin->	setRange(0, MXGRAY - 5);
	m_spinBoxMin->	setValue(0);

	m_CheckBoxAutoMin= new QCheckBox(m_ctrlGrp);
	QLabel *label_min_auto = new QLabel(tr("auto"));


	// for maximum
	QLabel *label_max = new QLabel(tr("Minimum"));

	m_sliderMax		=new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderMax->	setTickPosition(QSlider::TicksBelow);
	m_sliderMax->	setTickInterval(25);
	m_sliderMax->	setRange(5,MXGRAY); /// max of max is mxgray so i have 5 difference on both
	m_sliderMax->	setValue(MXGRAY); // min is 255

	m_spinBoxMax	=new QSpinBox(m_ctrlGrp);
	m_spinBoxMax->	setRange(5, MXGRAY);
	m_spinBoxMax->	setValue(MXGRAY);

	m_CheckBoxAutoMax		= new QCheckBox(m_ctrlGrp);
	QLabel *label_max_auto	= new QLabel(tr("auto"));

	// layout
	QGridLayout *layout		= new QGridLayout;
	layout->addWidget	(label_min,				0, 0);
	layout->addWidget	(m_sliderMin,			1, 0);
	layout->addWidget	(m_spinBoxMin,			1, 1);
	layout->addWidget	(m_CheckBoxAutoMin,		1, 2, Qt::AlignRight);
	layout->addWidget	(label_min_auto,		1, 3, Qt::AlignLeft);

	layout->addWidget	(label_max,				2, 0);
	layout->addWidget	(m_sliderMax,			3, 0);
	layout->addWidget	(m_spinBoxMax,			3, 1);
	layout->addWidget	(m_CheckBoxAutoMax,		3, 2, Qt::AlignRight);
	layout->addWidget	(label_max_auto,		3, 3, Qt::AlignLeft);

	connect(m_sliderMin,		SIGNAL(valueChanged(int)),	this, SLOT(changeMin(int)));
	connect(m_spinBoxMin,		SIGNAL(valueChanged(int)),	this, SLOT(changeMin(int)));
	connect(m_CheckBoxAutoMin,	SIGNAL(stateChanged(int)),	this, SLOT(changeMinAuto(int)));
	
	connect(m_sliderMax,		SIGNAL(valueChanged(int)),	this, SLOT(changeMax(int)));
	connect(m_spinBoxMax,		SIGNAL(valueChanged(int)),	this, SLOT(changeMax(int)));
	connect(m_CheckBoxAutoMax,	SIGNAL(stateChanged(int)),	this, SLOT(changeMaxAuto(int)));


	m_ctrlGrp->		setLayout(layout);
	// connect signal here
	disable	(true);

	return(m_ctrlGrp);
}

bool
HistogramStretch::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	int min = m_sliderMin->value();
	int max = m_sliderMax->value();
	if (m_CheckBoxAutoMax->isChecked()) {
		changeMaxAuto(0);
		return 1;
	}
	else if (m_CheckBoxAutoMin->isChecked()){
		changeMinAuto(0);
		return 1;
	}
	// set min betn 0 to 255
	// max to 5 tp 255
	// diff betn min and max is 5

	min = CLIP(min, 0, 250);
	max = CLIP(max, 5, 255);

	if (min >= max - 5){
		max = min + 5;

		m_sliderMin->		blockSignals(true);
		m_spinBoxMin->		blockSignals(true);
		m_sliderMax->		blockSignals(true);
		m_spinBoxMax->		blockSignals(true);

		m_sliderMin->		setValue(min);
		m_spinBoxMin->		setValue(min);
		m_sliderMax->		setValue(max);
		m_spinBoxMax->		setValue(max);

		m_sliderMin->		blockSignals(false);
		m_spinBoxMin->		blockSignals(false);
		m_sliderMax->		blockSignals(false);
		m_spinBoxMax->		blockSignals(false);

	}

	int histogram[MXGRAY];
	getHistogram(I1, histogram);
	histogramStretch(I1, min, max, I2);
	return 1;
}

void
HistogramStretch::changeMin(int min){

		m_sliderMin->		blockSignals(true);
		m_spinBoxMin->		blockSignals(true);

		m_sliderMin->		setValue(min);
		m_spinBoxMin->		setValue(min);

		m_sliderMin->		blockSignals(false);
		m_spinBoxMin->		blockSignals(false);
	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}
void 
HistogramStretch::changeMinAuto(int){
	// get histogram
	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();

	int histogram[MXGRAY];
	getHistogram(I1, histogram);

	int max = m_sliderMax->value();
	int min;
	if (m_CheckBoxAutoMin->isChecked()){
		for (int i = 0; i < MXGRAY; ++i){
			if (!histogram[i]) continue;
			min = i;
			break;
		}
		m_sliderMin->		blockSignals(true);
		m_spinBoxMin->		blockSignals(true);

		m_sliderMin->		setValue(min);
		m_spinBoxMin->		setValue(min);

		m_sliderMin->		blockSignals(false);
		m_spinBoxMin->		blockSignals(false);

		m_sliderMin->		setDisabled(true);
		m_spinBoxMin->		setDisabled(true);

		histogramStretch(I1, min, max,I2);
	}
	else{
		m_sliderMin->		setDisabled(false);
		m_spinBoxMin->		setDisabled(false);
		applyFilter(I1, I2);
	}
	g_mainWindowP->displayOut();
}

void
HistogramStretch::changeMax(int max){
		m_sliderMax->blockSignals(true);
		m_sliderMax->setValue(max);
		m_spinBoxMax->blockSignals(true);
		m_spinBoxMax->setValue(max);

		m_sliderMax->blockSignals(false);
		m_spinBoxMax->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
HistogramStretch::changeMaxAuto(int){
	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();

	int histogram[MXGRAY];
	getHistogram(I1, histogram);

	int max;
	int min = m_sliderMin->value();

	if (m_CheckBoxAutoMax->isChecked()){
		for (int i = MXGRAY - 1; i >= 0; --i){
			if (!histogram[i]) continue;
			max = i;
			break;
		}
		m_sliderMax->		blockSignals(true);
		m_spinBoxMax->		blockSignals(true);

		m_sliderMax->		setValue(max);
		m_spinBoxMax->		setValue(max);

		m_sliderMax->		blockSignals(false);
		m_spinBoxMax->		blockSignals(false);

		m_sliderMax->		setDisabled(true);
		m_spinBoxMax->		setDisabled(true);
		histogramStretch(I1, min, max, I2);

	}
	else{
		m_sliderMax->		setDisabled(false);
		m_spinBoxMax->		setDisabled(false);

		applyFilter(I1, I2);
	}
	g_mainWindowP->displayOut();
}

void
HistogramStretch::histogramStretch(ImagePtr I1, int min, int max, ImagePtr I2){

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 
	
	int lut[MXGRAY];
	int diff = max - min;
	// get look up table
	for (int i = 0; i < MXGRAY; ++i)
		lut[i] = MXGRAY*(CLIP((i - min), 0 ,255)) / diff;
	// create image

	int type;
	ChannelPtr<uchar> p1, p2, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
	}
}

void
HistogramStretch::disable(bool flag){

	m_sliderMin->			blockSignals(true);
	m_sliderMax->			blockSignals(true);
	m_CheckBoxAutoMin->		blockSignals(true);

	m_spinBoxMin->			setDisabled(flag);
	m_spinBoxMax->			setDisabled(flag);

	m_CheckBoxAutoMin->		setDisabled(flag);
	m_CheckBoxAutoMax->		setDisabled(flag);

	m_sliderMin->			blockSignals(false);
	m_sliderMax->			blockSignals(false);
	m_CheckBoxAutoMin->		blockSignals(false);
}

void
HistogramStretch::getHistogram(ImagePtr I1, int histogram[]){

	int w		=	I1->width();  // input image
	int h		=	I1->height();
	int total	=	w * h; // 
	int type;
	
	for (int i = 0; i < MXGRAY; ++i) histogram[i] = 0;


	ChannelPtr<uchar> p1, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++){
		for (endd = p1 + total; p1 < endd;) histogram[*p1++]++;
	}
}	