#include "../include/us3_defines.h"
#include "../include/us_hydrodyn_saxs_residuals.h"
//Added by qt3to4:
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFrame>

US_Hydrodyn_Saxs_Residuals::US_Hydrodyn_Saxs_Residuals(
                                                       bool *saxs_residuals_widget,
                                                       unsigned int width,
                                                       QString title,
                                                       vector < double > r,
                                                       vector < double > difference,
                                                       vector < double > residuals,
                                                       vector < double > target,
                                                       bool plot_residuals,
                                                       bool plot_difference,
                                                       bool plot_as_percent,
                                                       unsigned pen_width,
                                                       QWidget *p, 
                                                       const char *name
                                                       ) : QFrame( p )
{
   this->saxs_residuals_widget = saxs_residuals_widget;
   this->title = title;
   this->r = r;
   this->difference = difference;
   this->residuals = residuals;
   this->target = target;
   this->plot_residuals = plot_residuals;
   this->plot_difference = plot_difference;
   this->plot_as_percent = plot_as_percent;
   this->pen_width = pen_width;

   plot_zoomer = (ScrollZoomer *)0;

   // make sure things aren't to big

   unsigned int min_len = r.size();
   if ( difference.size() <  min_len ) 
   {
      min_len = difference.size();
   }
   if ( residuals.size() <  min_len ) 
   {
      min_len = residuals.size();
   }
   if ( target.size() <  min_len ) 
   {
      min_len = target.size();
   }
   r.resize(min_len);
   difference.resize(min_len);
   residuals.resize(min_len);
   target.resize(min_len);
   difference_pct.resize(min_len);
   residuals_pct.resize(min_len);
   double area = 0e0;
   for ( unsigned int i = 0; i < target.size(); i++ )
   {
      area += target[i];
   }
   if ( target.size() )
   {
      area /= target.size();
   }
   for ( unsigned int i = 0; i < target.size(); i++ )
   {
      difference_pct[i] = 100.0 * difference[i] / area;
      residuals_pct[i] = 100.0 * residuals[i] / area;
   }

   USglobal = new US_Config();
   setPalette( PALET_FRAME );
   setupGUI();
   global_Xpos += 30;
   global_Ypos += 30;
   setGeometry(global_Xpos, global_Ypos, width, 0);
   update_plot();
}

US_Hydrodyn_Saxs_Residuals::~US_Hydrodyn_Saxs_Residuals()
{
   if ( saxs_residuals_widget )
   {
      *saxs_residuals_widget = false;
   }
}

void US_Hydrodyn_Saxs_Residuals::setupGUI()
{

   // int minHeight1 = 30;

   // lbl_title = new QLabel(title, this);
   // lbl_title->setFrameStyle(QFrame::WinPanel|QFrame::Raised);
   // lbl_title->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
   // lbl_title->setMinimumHeight(minHeight1);
   // lbl_title->setPalette( PALET_FRAME );
   // lbl_title->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize + 1, QFont::Bold));

//   plot = new QwtPlot(this);
   usp_plot = new US_Plot( plot, "", "", "", this );
   connect( (QWidget *)plot->titleLabel(), SIGNAL( customContextMenuRequested( const QPoint & ) ), SLOT( usp_config_plot( const QPoint & ) ) );
   ((QWidget *)plot->titleLabel())->setContextMenuPolicy( Qt::CustomContextMenu );
   connect( (QWidget *)plot->axisWidget( QwtPlot::yLeft ), SIGNAL( customContextMenuRequested( const QPoint & ) ), SLOT( usp_config_plot( const QPoint & ) ) );
   ((QWidget *)plot->axisWidget( QwtPlot::yLeft ))->setContextMenuPolicy( Qt::CustomContextMenu );
   connect( (QWidget *)plot->axisWidget( QwtPlot::xBottom ), SIGNAL( customContextMenuRequested( const QPoint & ) ), SLOT( usp_config_plot( const QPoint & ) ) );
   ((QWidget *)plot->axisWidget( QwtPlot::xBottom ))->setContextMenuPolicy( Qt::CustomContextMenu );
#if QT_VERSION < 0x040000
   plot->enableOutline(true);
   plot->setOutlinePen(Qt::white);
   plot->setOutlineStyle(Qwt::VLine);
   plot->enableGridXMin();
   plot->enableGridYMin();
   plot->setPalette( PALET_NORMAL );
   AUTFBACK( plot );
   plot->setGridMajPen(QPen(USglobal->global_colors.major_ticks, 0, DotLine));
   plot->setGridMinPen(QPen(USglobal->global_colors.minor_ticks, 0, DotLine));
#else
   grid = new QwtPlotGrid;
   grid->enableXMin( true );
   grid->enableYMin( true );
   plot->setPalette( PALET_NORMAL );
   AUTFBACK( plot );
   grid->setMajorPen( QPen( USglobal->global_colors.major_ticks, 0, Qt::DotLine ) );
   grid->setMinorPen( QPen( USglobal->global_colors.minor_ticks, 0, Qt::DotLine ) );
   grid->attach( plot );
#endif
   plot->setAxisTitle(QwtPlot::xBottom, us_tr("Distance (Angstrom)"));
   plot->setAxisTitle(QwtPlot::yLeft, us_tr("Frequency"));
#if QT_VERSION < 0x040000
   plot->setTitleFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize, QFont::Bold));
   plot->setAxisTitleFont(QwtPlot::yLeft, QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize, QFont::Bold));
#endif
   plot->setAxisFont(QwtPlot::yLeft, QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize - 1));
#if QT_VERSION < 0x040000
   plot->setAxisTitleFont(QwtPlot::xBottom, QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize, QFont::Bold));
#endif
   plot->setAxisFont(QwtPlot::xBottom, QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize - 1));
#if QT_VERSION < 0x040000
   plot->setAxisTitleFont(QwtPlot::yRight, QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize, QFont::Bold));
#endif
   plot->setAxisFont(QwtPlot::yRight, QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize - 1));
//    plot->setMargin(USglobal->config_list.margin);
   plot->setTitle(title);
   plot->setCanvasBackground(USglobal->global_colors.plot);

   cb_plot_residuals = new QCheckBox(this);
   cb_plot_residuals->setText(us_tr(" Plot residuals"));
   cb_plot_residuals->setEnabled(true);
   cb_plot_residuals->setChecked(plot_residuals);
   cb_plot_residuals->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   QPalette qcg_plot_residuals = QPalette(
                                                QBrush(Qt::green), // USglobal->global_colors.cg_normal.foreground(),
                                                USglobal->global_colors.cg_normal.button(), 
                                                USglobal->global_colors.cg_normal.light(), 
                                                USglobal->global_colors.cg_normal.dark(), 
                                                USglobal->global_colors.cg_normal.mid(), 
                                                USglobal->global_colors.cg_normal.text(),
                                                USglobal->global_colors.cg_normal.brightText(), 
                                                USglobal->global_colors.cg_normal.base(), 
                                                QBrush(Qt::black) // USglobal->global_colors.cg_normal.background()
                                                );
   cb_plot_residuals->setPalette( PALET_NORMAL );
   AUTFBACK( cb_plot_residuals );

   connect(cb_plot_residuals, SIGNAL(clicked()), SLOT(set_plot_residuals()));

   cb_plot_difference = new QCheckBox(this);
   cb_plot_difference->setText(us_tr(" Plot difference"));
   cb_plot_difference->setEnabled(true);
   cb_plot_difference->setChecked(plot_difference);
   cb_plot_difference->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   QPalette qcg_plot_difference = QPalette(
                                                QBrush(Qt::yellow), //USglobal->global_colors.cg_normal.foreground(),
                                                USglobal->global_colors.cg_normal.button(), 
                                                USglobal->global_colors.cg_normal.light(), 
                                                USglobal->global_colors.cg_normal.dark(), 
                                                USglobal->global_colors.cg_normal.mid(), 
                                                USglobal->global_colors.cg_normal.text(),
                                                USglobal->global_colors.cg_normal.brightText(), 
                                                USglobal->global_colors.cg_normal.base(), 
                                                QBrush(Qt::black) // USglobal->global_colors.cg_normal.background()
                                                );
   cb_plot_difference->setPalette( PALET_NORMAL );
   AUTFBACK( cb_plot_difference );
   connect(cb_plot_difference, SIGNAL(clicked()), SLOT(set_plot_difference()));

   cb_plot_as_percent = new QCheckBox(this);
   cb_plot_as_percent->setText(us_tr(" Plot as percent"));
   cb_plot_as_percent->setEnabled(true);
   cb_plot_as_percent->setChecked(plot_as_percent);
   cb_plot_as_percent->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   QPalette qcg_plot_as_percent = QPalette(
                                                 QBrush(Qt::white), //USglobal->global_colors.cg_normal.foreground(),
                                                 USglobal->global_colors.cg_normal.button(), 
                                                 USglobal->global_colors.cg_normal.light(), 
                                                 USglobal->global_colors.cg_normal.dark(), 
                                                 USglobal->global_colors.cg_normal.mid(), 
                                                 USglobal->global_colors.cg_normal.text(),
                                                 USglobal->global_colors.cg_normal.brightText(), 
                                                 USglobal->global_colors.cg_normal.base(), 
                                                 QBrush(Qt::black) // USglobal->global_colors.cg_normal.background()
                                                );
   cb_plot_as_percent->setPalette( PALET_NORMAL );
   AUTFBACK( cb_plot_as_percent );
   connect(cb_plot_as_percent, SIGNAL(clicked()), SLOT(set_plot_as_percent()));

   pb_help = new QPushButton(us_tr("Help"), this);
   Q_CHECK_PTR(pb_help);
   pb_help->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize + 1));
   // pb_help->setMinimumHeight(minHeight1);
   pb_help->setPalette( PALET_PUSHB );
   connect(pb_help, SIGNAL(clicked()), SLOT(help()));

   pb_cancel = new QPushButton(us_tr("Close"), this);
   Q_CHECK_PTR(pb_cancel);
   pb_cancel->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize + 1));
   // pb_cancel->setMinimumHeight(minHeight1);
   pb_cancel->setPalette( PALET_PUSHB );
   connect(pb_cancel, SIGNAL(clicked()), SLOT(cancel()));


   // build layout
   // left box / possible

   int rows = 0, columns = 2, spacing = 2, j=0, margin=4;
   QGridLayout * background = new QGridLayout( this ); background->setContentsMargins( 0, 0, 0, 0 ); background->setSpacing( 0 ); background->setSpacing( spacing ); background->setContentsMargins( margin, margin, margin, margin );

   // background->addWidget( lbl_title , j , 0 , 1 + ( j ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   // j++;

   background->addWidget( plot , j , 0 , 1 + ( j ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j++;

   QBoxLayout * hbl = new QHBoxLayout(); hbl->setContentsMargins( 0, 0, 0, 0 ); hbl->setSpacing( 0 );
   hbl->addWidget(cb_plot_residuals);
   hbl->addWidget(cb_plot_difference);
   hbl->addWidget(cb_plot_as_percent);

   background->addLayout( hbl , j , 0 , 1 + ( j ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   // background->addWidget(cb_plot_residuals, j, 0);
   // background->addWidget(cb_plot_difference, j, 1);
   j++;

   background->addWidget(pb_help, j, 0);
   background->addWidget(pb_cancel, j, 1);
   j++;

}

void US_Hydrodyn_Saxs_Residuals::cancel()
{
   close();
}

void US_Hydrodyn_Saxs_Residuals::help()
{
   US_Help *online_help;
   online_help = new US_Help(this);
   online_help->show_help("manual/somo/somo_saxs_residuals.html");
}

void US_Hydrodyn_Saxs_Residuals::closeEvent( QCloseEvent *e )
{
   *saxs_residuals_widget = false;
   global_Xpos -= 30;
   global_Ypos -= 30;
   e->accept();
}

void US_Hydrodyn_Saxs_Residuals::set_plot_residuals()
{
   plot_residuals = cb_plot_residuals->isChecked();
   if ( !plot_residuals && !plot_difference )
   {
      cb_plot_difference->setChecked(true);
      set_plot_difference();
   } else {
      update_plot();
   }
}

void US_Hydrodyn_Saxs_Residuals::set_plot_difference()
{
   plot_difference = cb_plot_difference->isChecked();
   if ( !plot_residuals && !plot_difference )
   {
      cb_plot_residuals->setChecked(true);
      set_plot_residuals();
   } else {
      update_plot();
   }
}

void US_Hydrodyn_Saxs_Residuals::set_plot_as_percent()
{
   plot_as_percent = cb_plot_as_percent->isChecked();
   update_plot();
}

void US_Hydrodyn_Saxs_Residuals::update_plot()
{
   plot->detachItems( QwtPlotItem::Rtti_PlotCurve ); plot->detachItems( QwtPlotItem::Rtti_PlotMarker );;
   plot->setAxisTitle(QwtPlot::yLeft, plot_as_percent ? us_tr("Percent") : us_tr("Frequency"));

   double minx;
   double maxx;
   double miny = 0e0;
   double maxy = 1e0;
   bool any_set = false;

   minx = r[ 0 ];
   maxx = r[ r.size() - 1 ];

   if ( plot_residuals ) 
   {
#if QT_VERSION < 0x040000
      long prr = plot->insertCurve("P(r) vs r");
      plot->setCurveStyle(prr, QwtCurve::Lines);
      plot->setCurveData(prr, 
                         (double *)&(r[0]), 
                         plot_as_percent ? (double *)&(residuals_pct[0]) : (double *)&(residuals[0]), 
                         (int)r.size());
      plot->setCurvePen(prr, QPen(Qt::green, pen_width, SolidLine));
#else
      QwtPlotCurve *curve = new QwtPlotCurve( "P(r) vs r" );
      curve->setStyle( QwtPlotCurve::Lines );
      curve->setSamples(
                     (double *)&(r[0]), 
                     plot_as_percent ? (double *)&(residuals_pct[0]) : (double *)&(residuals[0]), 
                     (int)r.size()
                     );
      curve->setPen( QPen(Qt::green, pen_width, Qt::SolidLine) );
      curve->attach( plot );
#endif
      double this_miny = plot_as_percent ? residuals_pct[ 0 ] : residuals[ 0 ];
      double this_maxy = this_miny;

      for ( unsigned int i = 1; i < r.size(); i++ )
      {
         double val = plot_as_percent ? residuals_pct[ i ] : residuals[ i ];
         if ( this_miny > val )
         {
            this_miny = val;
         }
         if ( this_maxy < val )
         {
            this_maxy = val;
         }
      }
      if ( any_set )
      {
         if ( miny > this_miny )
         {
            miny = this_miny;
         } 
         if ( maxy < this_maxy )
         {
            maxy = this_maxy;
         }
      } else {
         miny = this_miny;
         maxy = this_maxy;
         any_set = true;
      }
   }
   if ( plot_difference ) 
   {
#if QT_VERSION < 0x040000
      long prr = plot->insertCurve("P(r) vs r");
      plot->setCurveStyle(prr, QwtCurve::Lines);
      plot->setCurveData(prr, 
                         (double *)&(r[0]), 
                         plot_as_percent ? (double *)&(difference_pct[0]) : (double *)&(difference[0]),
                         (int)r.size());
      plot->setCurvePen(prr, QPen(Qt::yellow, pen_width, SolidLine));
#else
      QwtPlotCurve *curve = new QwtPlotCurve( "P(r) vs r" );
      curve->setStyle( QwtPlotCurve::Lines );
      curve->setSamples(
                     (double *)&(r[0]), 
                     plot_as_percent ? (double *)&(difference_pct[0]) : (double *)&(difference[0]),
                     (int)r.size()
                     );
      curve->setPen( QPen(Qt::yellow, pen_width, Qt::SolidLine) );
      curve->attach( plot );
#endif
      double this_miny = plot_as_percent ? difference_pct[ 0 ] : difference[ 0 ];
      double this_maxy = this_miny;

      for ( unsigned int i = 1; i < r.size(); i++ )
      {
         double val = plot_as_percent ? difference_pct[ i ] : difference[ i ];
         if ( this_miny > val )
         {
            this_miny = val;
         }
         if ( this_maxy < val )
         {
            this_maxy = val;
         }
      }
      if ( any_set )
      {
         if ( miny > this_miny )
         {
            miny = this_miny;
         } 
         if ( maxy < this_maxy )
         {
            maxy = this_maxy;
         }
      } else {
         miny = this_miny;
         maxy = this_maxy;
         any_set = true;
      }
   }

   // enable zooming
   if ( plot_zoomer )
   {
      delete plot_zoomer;
      plot_zoomer = (ScrollZoomer *)0;
   }
   
   plot->setAxisScale( QwtPlot::xBottom, minx, maxx );
   plot->setAxisScale( QwtPlot::yLeft,   miny, maxy );
   
   plot_zoomer = new ScrollZoomer(plot->canvas());
#if QT_VERSION < 0x040000
   plot_zoomer->setRubberBandPen(QPen(Qt::yellow, 0, Qt::DotLine));
   plot_zoomer->setCursorLabelPen(QPen(Qt::yellow));
#else
   plot_zoomer->setRubberBandPen( QPen(Qt::red, 1, Qt::DotLine ) );
   plot_zoomer->setTrackerPen( QPen(Qt::red ) );
#endif

   plot->replot();
}

void US_Hydrodyn_Saxs_Residuals::usp_config_plot( const QPoint & ) {
   US_PlotChoices *uspc = new US_PlotChoices( usp_plot );
   uspc->exec();
   delete uspc;
}
