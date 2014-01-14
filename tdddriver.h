/*
 * This is a template header file for a user modules derived from
 * DefaultGUIModel using the default GUI.
 */

#include <default_gui_model.h>
#include <qpoint.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qpixmap.h>

class TDDDriver : public DefaultGUIModel
{

public:

  TDDDriver(void);
  virtual
  ~TDDDriver(void);

  void
  execute(void);

  void
  getMousePos(void);

  void
  createGUI(DefaultGUIModel::variable_t *, int);

  void
  paintEvent(QPaintEvent *);

protected:

  virtual void
  update(DefaultGUIModel::update_flags_t);

private:

  double some_parameter;
  double some_state;
  double period;
  int steps;
  double xpos;
  double ypos;
  QPoint cpos;
  double mposx;
  double mposy;
  QPixmap *drawsurface;
  QWidget *drawplane;
  QPainter *paint;
  QPaintEvent *pev;

private slots:

};
