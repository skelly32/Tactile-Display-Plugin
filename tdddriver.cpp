/*
 * This is a template implementation file for a user module derived from
 * DefaultGUIModel using the default GUI.
 */

#include <tdddriver.h>
#include <qwhatsthis.h>
#include <qcursor.h>
#include <qpoint.h>
#include <math.h>
#include <qgridview.h>
#include <qhbox.h>
#include <qhbuttongroup.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qvalidator.h>
#include <qvbox.h>
#include <qcanvas.h>
#include <qpainter.h>
#include <qpixmap.h>

extern "C" Plugin::Object *
createRTXIPlugin(void)
{
  return new TDDDriver();
}

static DefaultGUIModel::variable_t vars[] =
  {
    { "Xpos", "X position", DefaultGUIModel::STATE, },
    { "Ypos", "Y position", DefaultGUIModel::STATE, },
    };

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

TDDDriver::TDDDriver(void) :
  DefaultGUIModel("TDDDriver", ::vars, ::num_vars)
{

  QWhatsThis::add(this, "<p><b>TDDDriver:</b><br>Driver Application for the Tactile Display Device (TDD).</p>");
  update( INIT); // this is optional, you may place initialization code directly into the constructor
  createGUI(vars, num_vars); // this is required to create the GUI
  //QPainter *paint = new QPainter(this);
  refresh(); // this is required to update the GUI with parameter and state values
}

TDDDriver::~TDDDriver(void)
{
}

void
TDDDriver::getMousePos(void)
{
    cpos = QCursor::pos();
    mposx = cpos.x();
    mposy = cpos.y();
    return;
}

void
TDDDriver::execute(void)
{
    getMousePos();
    setState("Xpos",mposx);
    setState("Ypos",mposy);
    //printf("now we're settin states \n");
    //double xfrac = mposx/1920;
    //double yfrac = mposy/1200;
    //QCanvasPolygon *qpi = new QCanvasPolygon(qc);
    //QPointArray *qpa = new QPointArray(1);
    //qpa->putPoints(0,1,(int)(xfrac*640),(int)(yfrac*480));
    //qpi->setPoints(qpa);
    //paint->setPen( Qt::blue );
    //paint->drawPoint((int)(xfrac*640),(int)(yfrac*480));
    //paint->begin(drawsurface);
    //fprintf(stdout,"%d, %d",(int)(xfrac*640),(int)(yfrac*480));
    //qpi->drawShape(paint);
    //qc->update();
  return;
}

void
TDDDriver::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag)
    {
  case INIT:
    period = (RT::System::getInstance()->getPeriod() * 1e-6); // ms
    break;
  case MODIFY:
    break;
  case UNPAUSE:
    break;
  case PAUSE:
    //paint->end();
    break;
  case PERIOD:
    period = (RT::System::getInstance()->getPeriod() * 1e-6); // ms
    break;
  default:
    paintEvent(pev);
    break;

    }

}

void
TDDDriver::createGUI(DefaultGUIModel::variable_t *var, int size)
{

  setMinimumSize(200, 300); // Qt API for setting window size

  //overall GUI layout with a "horizontal box" copied from DefaultGUIModel

  QBoxLayout *layout = new QHBoxLayout(this);

  //additional GUI layouts with "vertical" layouts that will later
  // be added to the overall "layout" above
  QBoxLayout *leftlayout = new QVBoxLayout();
  QBoxLayout *rightlayout = new QVBoxLayout();

  //these 3 utility buttons are copied from DefaultGUIModel
  QHBox *utilityBox = new QHBox(this);
  pauseButton = new QPushButton("Pause", utilityBox);
  pauseButton->setToggleButton(true);
  QObject::connect(pauseButton, SIGNAL(toggled(bool)), this, SLOT(pause(bool)));
  QPushButton *modifyButton = new QPushButton("Modify", utilityBox);
  QObject::connect(modifyButton, SIGNAL(clicked(void)), this, SLOT(modify(void)));
  QPushButton *unloadButton = new QPushButton("Unload", utilityBox);
  QObject::connect(unloadButton, SIGNAL(clicked(void)), this, SLOT(exit(void)));

  QPixmap *drawsurface = new QPixmap(640,480);
  QWidget *drawplane = new QWidget;
  drawplane->resize(640, drawplane->width());
  drawsurface->fill(Qt::black);
  bitBlt(drawplane,0,0,drawsurface);
  printf("Widget Width = %d \n", drawplane->width());
  rightlayout->addWidget(drawplane);

  // copied from DefaultGUIModel DO NOT EDIT
  // this generates the text boxes and labels
  QScrollView *sv = new QScrollView(this);
  sv->setResizePolicy(QScrollView::AutoOneFit);
  leftlayout->addWidget(sv);

  QWidget *viewport = new QWidget(sv->viewport());
  sv->addChild(viewport);
  QGridLayout *scrollLayout = new QGridLayout(viewport, 1, 2);

  size_t nstate = 0, nparam = 0, nevent = 0, ncomment = 0;
  for (size_t i = 0; i < num_vars; i++)
    {
      if (vars[i].flags & (PARAMETER | STATE | EVENT | COMMENT))
        {
          param_t param;

          param.label = new QLabel(vars[i].name, viewport);
          scrollLayout->addWidget(param.label, parameter.size(), 0);
          param.edit = new DefaultGUILineEdit(viewport);
          scrollLayout->addWidget(param.edit, parameter.size(), 1);

          QToolTip::add(param.label, vars[i].description);
          QToolTip::add(param.edit, vars[i].description);

          if (vars[i].flags & PARAMETER)
            {
              if (vars[i].flags & DOUBLE)
                {
                  param.edit->setValidator(new QDoubleValidator(param.edit));
                  param.type = PARAMETER | DOUBLE;
                }
              else if (vars[i].flags & UINTEGER)
                {
                  QIntValidator *validator = new QIntValidator(param.edit);
                  param.edit->setValidator(validator);
                  validator->setBottom(0);
                  param.type = PARAMETER | UINTEGER;
                }
              else if (vars[i].flags & INTEGER)
                {
                  param.edit->setValidator(new QIntValidator(param.edit));
                  param.type = PARAMETER | INTEGER;
                }
              else
                param.type = PARAMETER;
              param.index = nparam++;
              param.str_value = new QString;
            }
          else if (vars[i].flags & STATE)
            {
              param.edit->setReadOnly(true);
              param.edit->setPaletteForegroundColor(Qt::darkGray);
              param.type = STATE;
              param.index = nstate++;
            }
          else if (vars[i].flags & EVENT)
            {
              param.edit->setReadOnly(true);
              param.type = EVENT;
              param.index = nevent++;
            }
          else if (vars[i].flags & COMMENT)
            {
              param.type = COMMENT;
              param.index = ncomment++;
            }

          parameter[vars[i].name] = param;
        }
    }

  // end DO NOT EDIT

  // add the 3 utility buttons to the bottom of the layout
  leftlayout->addWidget(utilityBox);

  // layouts can contain other layouts. if you added components to
  // "rightlayout" and added "rightlayout" to "layout," you would
  // have left and right panels in your custom GUI.
  layout->addLayout(leftlayout);
  layout->addLayout(rightlayout);

  show(); // this line is required to render the GUI
};

void
TDDDriver::paintEvent(QPaintEvent *) {
    printf("This paintevent is running \n");
   QPainter painter(this);

      //First Circle:
      //width&height:150px
      //Beginning(x/y):50px/30px
      //Each step for Angle parameter is 1/16th of a Degree
      //so multiply Angle Value with 16
      painter.drawArc(50,30,150,150,0,16*360);
      painter.drawArc(200,80,100,100,0,16*360);
      painter.drawArc(300,130,50,50,0,16*360);


}
