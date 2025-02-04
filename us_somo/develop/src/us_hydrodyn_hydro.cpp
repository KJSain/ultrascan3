#include "../include/us3_defines.h"
#include "../include/us_hydrodyn_hydro.h"
#include "../include/us_hydrodyn.h"
#include "../include/us_math.h"
//Added by qt3to4:
#include <QCloseEvent>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>

US_Hydrodyn_Hydro::US_Hydrodyn_Hydro(struct hydro_options *hydro,
                                     bool *hydro_widget, void *us_hydrodyn, QWidget *p, const char *name) : QFrame( p )
{
   this->hydro = hydro;
   this->hydro_widget = hydro_widget;
   this->us_hydrodyn = us_hydrodyn;
   *hydro_widget = true;
   USglobal=new US_Config();
   setPalette( PALET_FRAME );
   setWindowTitle(us_tr("SOMO Hydrodynamic Calculation Options"));
   this->setMinimumWidth(680);
   setupGUI();
   global_Xpos += 30;
   global_Ypos += 30;
   setGeometry(global_Xpos, global_Ypos, 0, 0);
}

US_Hydrodyn_Hydro::~US_Hydrodyn_Hydro()
{
   *hydro_widget = false;
}

void US_Hydrodyn_Hydro::setupGUI()
{
   int minHeight1 = 30;
   QString str;   
   lbl_info = new QLabel(us_tr("SOMO Hydrodynamic Calculation Options:"), this);
   Q_CHECK_PTR(lbl_info);
   lbl_info->setFrameStyle(QFrame::WinPanel|QFrame::Raised);
   lbl_info->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
   lbl_info->setMinimumHeight(minHeight1);
   lbl_info->setPalette( PALET_FRAME );
   AUTFBACK( lbl_info );
   lbl_info->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize + 1, QFont::Bold));


   lbl_unit = new QLabel(us_tr("Model scale (10^-x m) (10 = Angstrom, 9 = nanometer), where x is :"), this);
   Q_CHECK_PTR(lbl_unit);
   lbl_unit->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_unit->setMinimumHeight(minHeight1);
   lbl_unit->setPalette( PALET_LABEL );
   AUTFBACK( lbl_unit );
   lbl_unit->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   cnt_unit= new QwtCounter(this);
   US_Hydrodyn::sizeArrows( cnt_unit );
   Q_CHECK_PTR(cnt_unit);
   cnt_unit->setRange(5, 10); cnt_unit->setSingleStep( 1);
   cnt_unit->setValue(-(*hydro).unit);
   cnt_unit->setMinimumHeight(minHeight1);
   cnt_unit->setEnabled(true);
   cnt_unit->setNumButtons(2);
   cnt_unit->setFont(QFont(USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cnt_unit->setPalette( PALET_NORMAL );
   AUTFBACK( cnt_unit );
   connect(cnt_unit, SIGNAL(valueChanged(double)), SLOT(update_unit(double)));

#if  QT_VERSION < 0x040000
   bg_solvent_conditions = new QGroupBox(4, Qt::Horizontal, "Solvent type and conditions:", this);

   lbl_solvent_name = new QLabel(us_tr(" Solvent:"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_solvent_name);
   lbl_solvent_name->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_name->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_name );
   lbl_solvent_name->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_name = new QLineEdit( bg_solvent_conditions );    le_solvent_name->setObjectName( "Solvent_Name Line Edit" );
   le_solvent_name->setText((*hydro).solvent_name);
   le_solvent_name->setAlignment(Qt::AlignVCenter);
   le_solvent_name->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_name );
   le_solvent_name->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_name->setEnabled(true);
   connect(le_solvent_name, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_name(const QString &)));

   //   lbl_solvent_defaults = new QLabel(us_tr(" Set defaults:"), bg_solvent_conditions);
   //   Q_CHECK_PTR(lbl_solvent_defaults);
   //   lbl_solvent_defaults->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   //   lbl_solvent_defaults->setPalette( PALET_LABEL );
   //   lbl_solvent_defaults->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));
   bg_solvent_conditions->addSpace(0);

   cb_solvent_defaults = new QCheckBox(bg_solvent_conditions);
   cb_solvent_defaults->setText("Set defaults");
   cb_solvent_defaults->setEnabled(true);
   check_solvent_defaults();
   cb_solvent_defaults->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_solvent_defaults->setPalette( PALET_NORMAL );
   AUTFBACK( cb_solvent_defaults );
   connect(cb_solvent_defaults, SIGNAL(clicked()), this, SLOT(set_solvent_defaults()));

   lbl_solvent_acronym = new QLabel(us_tr(" Solvent acronym:"), bg_solvent_conditions);
   //   lbl_solvent_acronym = new QLabel(us_tr(" Solvent acronym (max 5 characters):"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_solvent_acronym);
   lbl_solvent_acronym->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_acronym->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_acronym );
   lbl_solvent_acronym->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_acronym = new QLineEdit( bg_solvent_conditions );    le_solvent_acronym->setObjectName( "Solvent_Acronym Line Edit" );
   le_solvent_acronym->setText((*hydro).solvent_acronym.left(5));
   le_solvent_acronym->setAlignment(Qt::AlignVCenter);
   le_solvent_acronym->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_acronym );
   le_solvent_acronym->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_acronym->setEnabled(true);
   connect(le_solvent_acronym, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_acronym(const QString &)));

   lbl_temperature = new QLabel(us_tr(" Temperature (�C):"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_temperature);
   lbl_temperature->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_temperature->setPalette( PALET_LABEL );
   AUTFBACK( lbl_temperature );
   lbl_temperature->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_temperature = new QLineEdit( bg_solvent_conditions );    le_temperature->setObjectName( "Temperature Line Edit" );
   le_temperature->setText(str.sprintf("%4.2f",(*hydro).temperature));
   le_temperature->setAlignment(Qt::AlignVCenter);
   le_temperature->setPalette( PALET_NORMAL );
   AUTFBACK( le_temperature );
   le_temperature->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_temperature->setEnabled(true);
   connect(le_temperature, SIGNAL(textChanged(const QString &)), SLOT(update_temperature(const QString &)));

   lbl_solvent_viscosity = new QLabel(us_tr(" Solvent viscosity (cP):"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_solvent_viscosity);
   lbl_solvent_viscosity->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_viscosity->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_viscosity );
   lbl_solvent_viscosity->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_viscosity = new QLineEdit( bg_solvent_conditions );    le_solvent_viscosity->setObjectName( "Solvent_Viscosity Line Edit" );
   le_solvent_viscosity->setText(str.sprintf("%f",(*hydro).solvent_viscosity));
   le_solvent_viscosity->setAlignment(Qt::AlignVCenter);
   le_solvent_viscosity->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_viscosity );
   le_solvent_viscosity->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_viscosity->setEnabled(true);
   connect(le_solvent_viscosity, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_viscosity(const QString &)));

   lbl_solvent_density = new QLabel(us_tr(" Solvent density (g/ml):"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_solvent_density);
   lbl_solvent_density->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_density->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_density );
   lbl_solvent_density->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_density = new QLineEdit( bg_solvent_conditions );    le_solvent_density->setObjectName( "Solvent_Density Line Edit" );
   le_solvent_density->setText(str.sprintf("%f",(*hydro).solvent_density));
   le_solvent_density->setAlignment(Qt::AlignVCenter);
   le_solvent_density->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_density );
   le_solvent_density->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_density->setEnabled(true);
   connect(le_solvent_density, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_density(const QString &)));
#else
   bg_solvent_conditions = new QGroupBox( "Solvent type and conditions:" );

   lbl_solvent_name = new QLabel(us_tr(" Solvent:"));
   Q_CHECK_PTR(lbl_solvent_name);
   lbl_solvent_name->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_name->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_name );
   lbl_solvent_name->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_name = new QLineEdit();
   le_solvent_name->setText((*hydro).solvent_name);
   le_solvent_name->setAlignment(Qt::AlignVCenter);
   le_solvent_name->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_name );
   le_solvent_name->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_name->setEnabled(true);
   connect(le_solvent_name, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_name(const QString &)));

   //   lbl_solvent_defaults = new QLabel(us_tr(" Set defaults:"), bg_solvent_conditions);
   //   Q_CHECK_PTR(lbl_solvent_defaults);
   //   lbl_solvent_defaults->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   //   lbl_solvent_defaults->setPalette( PALET_LABEL );
   //   lbl_solvent_defaults->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));
   //   bg_solvent_conditions->addSpace(0);

   cb_solvent_defaults = new QCheckBox();
   cb_solvent_defaults->setText("Set defaults");
   cb_solvent_defaults->setEnabled(true);
   check_solvent_defaults();
   cb_solvent_defaults->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_solvent_defaults->setPalette( PALET_NORMAL );
   AUTFBACK( cb_solvent_defaults );
   connect(cb_solvent_defaults, SIGNAL(clicked()), this, SLOT(set_solvent_defaults()));

   lbl_solvent_acronym = new QLabel(us_tr(" Solvent acronym:"));
   //   lbl_solvent_acronym = new QLabel(us_tr(" Solvent acronym (max 5 characters):"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_solvent_acronym);
   lbl_solvent_acronym->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_acronym->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_acronym );
   lbl_solvent_acronym->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_acronym = new QLineEdit();
   le_solvent_acronym->setText((*hydro).solvent_acronym.left(5));
   le_solvent_acronym->setAlignment(Qt::AlignVCenter);
   le_solvent_acronym->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_acronym );
   le_solvent_acronym->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_acronym->setEnabled(true);
   connect(le_solvent_acronym, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_acronym(const QString &)));

   lbl_temperature = new QLabel(us_tr(" Temperature (\u00b0C):"), bg_solvent_conditions);
   Q_CHECK_PTR(lbl_temperature);
   lbl_temperature->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_temperature->setPalette( PALET_LABEL );
   AUTFBACK( lbl_temperature );
   lbl_temperature->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_temperature = new QLineEdit();
   le_temperature->setText(str.sprintf("%4.2f",(*hydro).temperature));
   le_temperature->setAlignment(Qt::AlignVCenter);
   le_temperature->setPalette( PALET_NORMAL );
   AUTFBACK( le_temperature );
   le_temperature->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_temperature->setEnabled(true);
   connect(le_temperature, SIGNAL(textChanged(const QString &)), SLOT(update_temperature(const QString &)));

   lbl_solvent_viscosity = new QLabel(us_tr(" Solvent viscosity (cP):"));
   Q_CHECK_PTR(lbl_solvent_viscosity);
   lbl_solvent_viscosity->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_viscosity->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_viscosity );
   lbl_solvent_viscosity->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_viscosity = new QLineEdit();
   le_solvent_viscosity->setText(str.sprintf("%f",(*hydro).solvent_viscosity));
   le_solvent_viscosity->setAlignment(Qt::AlignVCenter);
   le_solvent_viscosity->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_viscosity );
   le_solvent_viscosity->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_viscosity->setEnabled(true);
   connect(le_solvent_viscosity, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_viscosity(const QString &)));

   lbl_solvent_density = new QLabel(us_tr(" Solvent density (g/ml):"));
   Q_CHECK_PTR(lbl_solvent_density);
   lbl_solvent_density->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_solvent_density->setPalette( PALET_LABEL );
   AUTFBACK( lbl_solvent_density );
   lbl_solvent_density->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_solvent_density = new QLineEdit();
   le_solvent_density->setText(str.sprintf("%f",(*hydro).solvent_density));
   le_solvent_density->setAlignment(Qt::AlignVCenter);
   le_solvent_density->setPalette( PALET_NORMAL );
   AUTFBACK( le_solvent_density );
   le_solvent_density->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_solvent_density->setEnabled(true);
   connect(le_solvent_density, SIGNAL(textChanged(const QString &)), SLOT(update_solvent_density(const QString &)));

   {
      QGridLayout * gl = new QGridLayout; gl->setContentsMargins( 0, 0, 0, 0 ); gl->setSpacing( 0 );
      int row = 0;
      gl->addWidget( lbl_solvent_name, row, 0 );
      gl->addWidget( le_solvent_name, row, 1 );
      gl->addWidget( cb_solvent_defaults, row, 3 );

      row++;
      gl->addWidget( lbl_solvent_acronym, row, 0 );
      gl->addWidget( le_solvent_acronym, row, 1 );
      gl->addWidget( lbl_temperature, row, 2 );
      gl->addWidget( le_temperature, row, 3 );

      row++;
      gl->addWidget( lbl_solvent_viscosity, row, 0 );
      gl->addWidget( le_solvent_viscosity, row, 1 );
      gl->addWidget( lbl_solvent_density, row, 2 );
      gl->addWidget( le_solvent_density, row, 3 );

      bg_solvent_conditions->setLayout( gl );
   }
      
#endif

#if QT_VERSION < 0x040000
   bg_reference_system = new QGroupBox(2, Qt::Horizontal, "Computations Relative to:", this);
   bg_reference_system->setExclusive(true);
   connect(bg_reference_system, SIGNAL(clicked(int)), this, SLOT(select_reference_system(int)));

   cb_diffusion_center = new QCheckBox(bg_reference_system);
   cb_diffusion_center->setText(us_tr(" Diffusion Center "));
   cb_diffusion_center->setEnabled(true);
   cb_diffusion_center->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_diffusion_center->setPalette( PALET_NORMAL );
   AUTFBACK( cb_diffusion_center );

   cb_cartesian_origin = new QCheckBox(bg_reference_system);
   cb_cartesian_origin->setText(us_tr(" Cartesian Origin "));
   cb_cartesian_origin->setEnabled(true);
   cb_cartesian_origin->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_cartesian_origin->setPalette( PALET_NORMAL );
   AUTFBACK( cb_cartesian_origin );

   bg_reference_system->setButton((*hydro).reference_system);
#else
   bg_reference_system = new QGroupBox( "Computations Relative to:" );

   rb_diffusion_center = new QRadioButton();
   rb_diffusion_center->setText(us_tr(" Diffusion Center "));
   rb_diffusion_center->setEnabled(true);
   rb_diffusion_center->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_diffusion_center->setPalette( PALET_NORMAL );
   AUTFBACK( rb_diffusion_center );
   connect(rb_diffusion_center, SIGNAL(clicked()), this, SLOT(select_reference_system()));

   rb_cartesian_origin = new QRadioButton();
   rb_cartesian_origin->setText(us_tr(" Cartesian Origin "));
   rb_cartesian_origin->setEnabled(true);
   rb_cartesian_origin->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_cartesian_origin->setPalette( PALET_NORMAL );
   AUTFBACK( rb_cartesian_origin );
   connect(rb_cartesian_origin, SIGNAL(clicked()), this, SLOT(select_reference_system()));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( rb_diffusion_center );
      bl->addWidget( rb_cartesian_origin );
      bg_reference_system->setLayout( bl );
   }

   if ( !(*hydro).reference_system ) {
      rb_diffusion_center->setChecked(true);
   } else {
      rb_cartesian_origin->setChecked(true);
   }
#endif

#if QT_VERSION < 0x040000
   bg_boundary_cond = new QGroupBox(2, Qt::Horizontal, "Boundary Conditions:", this);
   bg_boundary_cond->setExclusive(true);
   connect(bg_boundary_cond, SIGNAL(clicked(int)), this, SLOT(select_boundary_cond(int)));

   cb_stick = new QCheckBox(bg_boundary_cond);
   cb_stick->setText(us_tr(" Stick "));
   cb_stick->setEnabled(true);
   cb_stick->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_stick->setPalette( PALET_NORMAL );
   AUTFBACK( cb_stick );

   cb_slip = new QCheckBox(bg_boundary_cond);
   cb_slip->setText(us_tr(" Slip "));
   cb_slip->setEnabled(true);
   cb_slip->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_slip->setPalette( PALET_NORMAL );
   AUTFBACK( cb_slip );

   bg_boundary_cond->setButton((*hydro).boundary_cond);
#else
   bg_boundary_cond = new QGroupBox( "Boundary Conditions:" );

   rb_stick = new QRadioButton();
   rb_stick->setText(us_tr(" Stick "));
   rb_stick->setEnabled(true);
   rb_stick->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_stick->setPalette( PALET_NORMAL );
   AUTFBACK( rb_stick );
   connect(rb_stick, SIGNAL(clicked()), this, SLOT(select_boundary_cond()));

   rb_slip = new QRadioButton();
   rb_slip->setText(us_tr(" Slip "));
   rb_slip->setEnabled(true);
   rb_slip->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_slip->setPalette( PALET_NORMAL );
   AUTFBACK( rb_slip );
   connect(rb_slip, SIGNAL(clicked()), this, SLOT(select_boundary_cond()));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( rb_stick );
      bl->addWidget( rb_slip );
      bg_boundary_cond->setLayout( bl );
   }

   if ( !(*hydro).boundary_cond ) {
      rb_stick->setChecked(true);
   } else {
      rb_slip->setChecked(true);
   }

#endif
#if QT_VERSION < 0x040000
   bg_mass_correction = new QGroupBox(4, Qt::Horizontal, "Total Mass of Model:", this);
   bg_mass_correction->setExclusive(true);
   connect(bg_mass_correction, SIGNAL(clicked(int)), this, SLOT(select_mass_correction(int)));

   cb_auto_mass = new QCheckBox(bg_mass_correction);
   cb_auto_mass->setText(us_tr(" Automatic (Sum of Bead Masses)"));
   cb_auto_mass->setEnabled(true);
   cb_auto_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_auto_mass->setPalette( PALET_NORMAL );
   AUTFBACK( cb_auto_mass );

   cb_manual_mass = new QCheckBox(bg_mass_correction);
   cb_manual_mass->setText(us_tr(" Manual "));
   cb_manual_mass->setEnabled(true);
   cb_manual_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_manual_mass->setPalette( PALET_NORMAL );
   AUTFBACK( cb_manual_mass );

   bg_mass_correction->setButton((*hydro).mass_correction);

   lbl_mass = new QLabel(us_tr(" Enter mass:    "), bg_mass_correction);
   Q_CHECK_PTR(lbl_mass);
   lbl_mass->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_mass->setPalette( PALET_LABEL );
   AUTFBACK( lbl_mass );
   lbl_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_mass = new QLineEdit( bg_mass_correction );    le_mass->setObjectName( "Mass Line Edit" );
   le_mass->setText(str.sprintf("%5.3e", (*hydro).mass));
   le_mass->setAlignment(Qt::AlignVCenter);
   le_mass->setPalette( PALET_NORMAL );
   AUTFBACK( le_mass );
   le_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_mass->setEnabled((*hydro).mass_correction);
   connect(le_mass, SIGNAL(textChanged(const QString &)), SLOT(update_mass(const QString &)));
#else
   bg_mass_correction = new QGroupBox( "Total Mass of Model:" );

   rb_auto_mass = new QRadioButton();
   rb_auto_mass->setText(us_tr(" Automatic (Sum of Bead Masses)"));
   rb_auto_mass->setEnabled(true);
   rb_auto_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_auto_mass->setPalette( PALET_NORMAL );
   AUTFBACK( rb_auto_mass );
   connect(rb_auto_mass, SIGNAL(clicked()), this, SLOT(select_mass_correction()));

   rb_manual_mass =new QRadioButton();
   rb_manual_mass->setText(us_tr(" Manual "));
   rb_manual_mass->setEnabled(true);
   rb_manual_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_manual_mass->setPalette( PALET_NORMAL );
   AUTFBACK( rb_manual_mass );
   connect(rb_manual_mass, SIGNAL(clicked()), this, SLOT(select_mass_correction()));

   lbl_mass = new QLabel(us_tr(" Enter mass:    "), bg_mass_correction);
   Q_CHECK_PTR(lbl_mass);
   lbl_mass->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_mass->setPalette( PALET_LABEL );
   AUTFBACK( lbl_mass );
   lbl_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_mass = new QLineEdit( bg_mass_correction );    le_mass->setObjectName( "Mass Line Edit" );
   le_mass->setText(str.sprintf("%5.3e", (*hydro).mass));
   le_mass->setAlignment(Qt::AlignVCenter);
   le_mass->setPalette( PALET_NORMAL );
   AUTFBACK( le_mass );
   le_mass->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_mass->setEnabled((*hydro).mass_correction);
   connect(le_mass, SIGNAL(textChanged(const QString &)), SLOT(update_mass(const QString &)));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( rb_auto_mass );
      bl->addWidget( rb_manual_mass );
      bl->addWidget( lbl_mass );
      bl->addWidget( le_mass );
      bg_mass_correction->setLayout( bl );
   }

   if ( !(*hydro).mass_correction ) {
      rb_auto_mass->setChecked(true);
   } else {
      rb_manual_mass->setChecked(true);
   }

#endif
#if  QT_VERSION < 0x040000
   bg_volume_correction = new QGroupBox(4, Qt::Horizontal, "Total Volume of Model: (for Rotational Diff. and Intrinsic Visc. Volume Corr.)", this);
   bg_volume_correction->setExclusive(true);
   connect(bg_volume_correction, SIGNAL(clicked(int)), this, SLOT(select_volume_correction(int)));

   cb_auto_volume = new QCheckBox(bg_volume_correction);
   cb_auto_volume->setText(us_tr(" Automatic (Sum of Bead Volumes)"));
   cb_auto_volume->setEnabled(true);
   cb_auto_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_auto_volume->setPalette( PALET_NORMAL );
   AUTFBACK( cb_auto_volume );

   cb_manual_volume = new QCheckBox(bg_volume_correction);
   cb_manual_volume->setText(us_tr(" Manual "));
   cb_manual_volume->setEnabled(true);
   cb_manual_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_manual_volume->setPalette( PALET_NORMAL );
   AUTFBACK( cb_manual_volume );

   bg_volume_correction->setButton((*hydro).volume_correction);

   lbl_volume = new QLabel(us_tr(" Enter volume: "), bg_volume_correction);
   Q_CHECK_PTR(lbl_volume);
   lbl_volume->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_volume->setPalette( PALET_LABEL );
   AUTFBACK( lbl_volume );
   lbl_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_volume = new QLineEdit( bg_volume_correction );    le_volume->setObjectName( "Volume Line Edit" );
   le_volume->setText(str.sprintf("%5.3e", (*hydro).volume));
   le_volume->setAlignment(Qt::AlignVCenter);
   le_volume->setPalette( PALET_NORMAL );
   AUTFBACK( le_volume );
   le_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_volume->setEnabled((*hydro).volume_correction);
   connect(le_volume, SIGNAL(textChanged(const QString &)), SLOT(update_volume(const QString &)));
#else
   bg_volume_correction = new QGroupBox( "Total Volume of Model: (for Rotational Diff. and Intrinsic Visc. Volume Corr.)" );

   rb_auto_volume = new QRadioButton();
   rb_auto_volume->setText(us_tr(" Automatic (Sum of Bead Volumes)"));
   rb_auto_volume->setEnabled(true);
   rb_auto_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_auto_volume->setPalette( PALET_NORMAL );
   AUTFBACK( rb_auto_volume );
   connect(rb_auto_volume, SIGNAL(clicked()), this, SLOT(select_volume_correction()));

   rb_manual_volume = new QRadioButton();
   rb_manual_volume->setText(us_tr(" Manual "));
   rb_manual_volume->setEnabled(true);
   rb_manual_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_manual_volume->setPalette( PALET_NORMAL );
   AUTFBACK( rb_manual_volume );
   connect(rb_manual_volume, SIGNAL(clicked()), this, SLOT(select_volume_correction()));

   lbl_volume = new QLabel(us_tr(" Enter volume: "), bg_volume_correction);
   Q_CHECK_PTR(lbl_volume);
   lbl_volume->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_volume->setPalette( PALET_LABEL );
   AUTFBACK( lbl_volume );
   lbl_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_volume = new QLineEdit( bg_volume_correction );    le_volume->setObjectName( "Volume Line Edit" );
   le_volume->setText(str.sprintf("%5.3e", (*hydro).volume));
   le_volume->setAlignment(Qt::AlignVCenter);
   le_volume->setPalette( PALET_NORMAL );
   AUTFBACK( le_volume );
   le_volume->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_volume->setEnabled((*hydro).volume_correction);
   connect(le_volume, SIGNAL(textChanged(const QString &)), SLOT(update_volume(const QString &)));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( rb_auto_volume );
      bl->addWidget( rb_manual_volume );
      bl->addWidget( lbl_volume );
      bl->addWidget( le_volume );
      bg_volume_correction->setLayout( bl );
   }

   if ( !(*hydro).volume_correction ) {
      rb_auto_volume->setChecked(true);
   } else {
      rb_manual_volume->setChecked(true);
   }

#endif

#if QT_VERSION < 0x040000
   bg_bead_inclusion = new QGroupBox(2, Qt::Horizontal, "Inclusion of Buried Beads in Hydrodynamic Calculations:", this);
   bg_bead_inclusion->setExclusive(true);
   connect(bg_bead_inclusion, SIGNAL(clicked(int)), this, SLOT(select_bead_inclusion(int)));

   cb_exclusion = new QCheckBox(bg_bead_inclusion);
   cb_exclusion->setText(us_tr(" Exclude "));
   cb_exclusion->setEnabled(true);
   cb_exclusion->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_exclusion->setPalette( PALET_NORMAL );
   AUTFBACK( cb_exclusion );

   cb_inclusion = new QCheckBox(bg_bead_inclusion);
   cb_inclusion->setText(us_tr(" Include "));
   cb_inclusion->setEnabled(true);
   cb_inclusion->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_inclusion->setPalette( PALET_NORMAL );
   AUTFBACK( cb_inclusion );

   bg_bead_inclusion->setButton((*hydro).bead_inclusion);
#else
   bg_bead_inclusion = new QGroupBox( "Inclusion of Buried Beads in Hydrodynamic Calculations:" );

   rb_exclusion = new QRadioButton();
   rb_exclusion->setText(us_tr(" Exclude "));
   rb_exclusion->setEnabled(true);
   rb_exclusion->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_exclusion->setPalette( PALET_NORMAL );
   AUTFBACK( rb_exclusion );
   connect(rb_exclusion, SIGNAL(clicked()), this, SLOT(select_bead_inclusion()));

   rb_inclusion = new QRadioButton();
   rb_inclusion->setText(us_tr(" Include "));
   rb_inclusion->setEnabled(true);
   rb_inclusion->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_inclusion->setPalette( PALET_NORMAL );
   AUTFBACK( rb_inclusion );
   connect(rb_inclusion, SIGNAL(clicked()), this, SLOT(select_bead_inclusion()));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( rb_exclusion );
      bl->addWidget( rb_inclusion );
      bg_bead_inclusion->setLayout( bl );
   }

   if ( !(*hydro).bead_inclusion ) {
      rb_exclusion->setChecked(true);
   } else {
      rb_inclusion->setChecked(true);
   }
#endif

#if QT_VERSION < 0x040000
   bg_buried = new QGroupBox(2, Qt::Horizontal, "Include Buried Beads in Volume Correction for Calculation of:", this);

   cb_rotational = new QCheckBox(bg_buried);
   cb_rotational->setText(us_tr(" Rotational Diffusion "));
   if ((*hydro).volume_correction)
   {
      cb_rotational->setEnabled(false);
   }
   else
   {
      cb_rotational->setEnabled(!(*hydro).bead_inclusion);
   }
   cb_rotational->setChecked((*hydro).rotational);
   cb_rotational->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_rotational->setPalette( PALET_NORMAL );
   AUTFBACK( cb_rotational );
   connect(cb_rotational, SIGNAL(clicked()), this, SLOT(set_rotational()));

   cb_viscosity = new QCheckBox(bg_buried);
   cb_viscosity->setText(us_tr(" Intrinsic Viscosity "));
   if ((*hydro).volume_correction)
   {
      cb_viscosity->setEnabled(false);
   }
   else
   {
      cb_viscosity->setEnabled(!(*hydro).bead_inclusion);
   }
   cb_viscosity->setChecked((*hydro).viscosity);
   cb_viscosity->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_viscosity->setPalette( PALET_NORMAL );
   AUTFBACK( cb_viscosity );
   connect(cb_viscosity, SIGNAL(clicked()), this, SLOT(set_viscosity()));
#else
   bg_buried = new QGroupBox( "Include Buried Beads in Volume Correction for Calculation of:" );

   cb_rotational = new QCheckBox();
   cb_rotational->setText(us_tr(" Rotational Diffusion "));
   if ((*hydro).volume_correction)
   {
      cb_rotational->setEnabled(false);
   }
   else
   {
      cb_rotational->setEnabled(!(*hydro).bead_inclusion);
   }
   cb_rotational->setChecked((*hydro).rotational);
   cb_rotational->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_rotational->setPalette( PALET_NORMAL );
   AUTFBACK( cb_rotational );
   connect(cb_rotational, SIGNAL(clicked()), this, SLOT(set_rotational()));

   cb_viscosity = new QCheckBox();
   cb_viscosity->setText(us_tr(" Intrinsic Viscosity "));
   if ((*hydro).volume_correction)
   {
      cb_viscosity->setEnabled(false);
   }
   else
   {
      cb_viscosity->setEnabled(!(*hydro).bead_inclusion);
   }
   cb_viscosity->setChecked((*hydro).viscosity);
   cb_viscosity->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_viscosity->setPalette( PALET_NORMAL );
   AUTFBACK( cb_viscosity );
   connect(cb_viscosity, SIGNAL(clicked()), this, SLOT(set_viscosity()));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( cb_rotational );
      bl->addWidget( cb_viscosity);
      bg_buried->setLayout( bl );
   }
#endif

#if QT_VERSION < 0x040000
   bg_overlap = new QGroupBox(4, Qt::Horizontal, "Overlap cut-off:", this);
   bg_overlap->setExclusive(true);
   connect(bg_overlap, SIGNAL(clicked(int)), this, SLOT(select_overlap(int)));

   cb_auto_overlap = new QCheckBox(bg_overlap);
   cb_auto_overlap->setText(us_tr(" From Bead Model "));
   cb_auto_overlap->setEnabled(true);
   cb_auto_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_auto_overlap->setPalette( PALET_NORMAL );
   AUTFBACK( cb_auto_overlap );

   cb_manual_overlap = new QCheckBox(bg_overlap);
   cb_manual_overlap->setText(us_tr(" Manual "));
   cb_manual_overlap->setEnabled(true);
   cb_manual_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   cb_manual_overlap->setPalette( PALET_NORMAL );
   AUTFBACK( cb_manual_overlap );

   bg_overlap->setButton((*hydro).overlap_cutoff);

   lbl_overlap = new QLabel(us_tr(" Enter cut-off: "), bg_overlap);
   Q_CHECK_PTR(lbl_overlap);
   lbl_overlap->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_overlap->setPalette( PALET_LABEL );
   AUTFBACK( lbl_overlap );
   lbl_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_overlap = new QLineEdit( bg_overlap );    le_overlap->setObjectName( "Mass Line Edit" );
   le_overlap->setText(str.sprintf("%5.3e", (*hydro).overlap));
   le_overlap->setAlignment(Qt::AlignVCenter);
   le_overlap->setPalette( PALET_NORMAL );
   AUTFBACK( le_overlap );
   le_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_overlap->setEnabled((*hydro).overlap_cutoff);
   connect(le_overlap, SIGNAL(textChanged(const QString &)), SLOT(update_overlap(const QString &)));
#else
   bg_overlap = new QGroupBox( "Overlap cut-off:" );

   rb_auto_overlap = new QRadioButton();
   rb_auto_overlap->setText(us_tr(" From Bead Model "));
   rb_auto_overlap->setEnabled(true);
   rb_auto_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_auto_overlap->setPalette( PALET_NORMAL );
   AUTFBACK( rb_auto_overlap );
   connect(rb_auto_overlap, SIGNAL(clicked()), this, SLOT(select_overlap()));

   rb_manual_overlap = new QRadioButton();
   rb_manual_overlap->setText(us_tr(" Manual "));
   rb_manual_overlap->setEnabled(true);
   rb_manual_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   rb_manual_overlap->setPalette( PALET_NORMAL );
   AUTFBACK( rb_manual_overlap );
   connect(rb_manual_overlap, SIGNAL(clicked()), this, SLOT(select_overlap()));

   lbl_overlap = new QLabel(us_tr(" Enter cut-off: "), bg_overlap);
   Q_CHECK_PTR(lbl_overlap);
   lbl_overlap->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
   lbl_overlap->setPalette( PALET_LABEL );
   AUTFBACK( lbl_overlap );
   lbl_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize-1, QFont::Bold));

   le_overlap = new QLineEdit( bg_overlap );    le_overlap->setObjectName( "Mass Line Edit" );
   le_overlap->setText(str.sprintf("%5.3e", (*hydro).overlap));
   le_overlap->setAlignment(Qt::AlignVCenter);
   le_overlap->setPalette( PALET_NORMAL );
   AUTFBACK( le_overlap );
   le_overlap->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize));
   le_overlap->setEnabled((*hydro).overlap_cutoff);
   connect(le_overlap, SIGNAL(textChanged(const QString &)), SLOT(update_overlap(const QString &)));

   {
      QHBoxLayout * bl = new QHBoxLayout; bl->setContentsMargins( 0, 0, 0, 0 ); bl->setSpacing( 0 );
      bl->addWidget( rb_auto_overlap );
      bl->addWidget( rb_manual_overlap );
      bl->addWidget( lbl_overlap );
      bl->addWidget( le_overlap );
      bg_overlap->setLayout( bl );
   }

   if ( !(*hydro).overlap_cutoff ) {
      rb_auto_overlap->setChecked(true);
   } else {
      rb_manual_overlap->setChecked(true);
   }

#endif

   pb_cancel = new QPushButton(us_tr("Close"), this);
   Q_CHECK_PTR(pb_cancel);
   pb_cancel->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize + 1));
   pb_cancel->setMinimumHeight(minHeight1);
   pb_cancel->setPalette( PALET_PUSHB );
   connect(pb_cancel, SIGNAL(clicked()), SLOT(cancel()));

   pb_help = new QPushButton(us_tr("Help"), this);
   Q_CHECK_PTR(pb_help);
   pb_help->setFont(QFont( USglobal->config_list.fontFamily, USglobal->config_list.fontSize + 1));
   pb_help->setMinimumHeight(minHeight1);
   pb_help->setPalette( PALET_PUSHB );
   connect(pb_help, SIGNAL(clicked()), SLOT(help()));

   int rows=12, columns = 2, spacing = 2, j=0, margin=4;
   QGridLayout * background = new QGridLayout( this ); background->setContentsMargins( 0, 0, 0, 0 ); background->setSpacing( 0 ); background->setSpacing( spacing ); background->setContentsMargins( margin, margin, margin, margin );

   background->addWidget( lbl_info , j , 0 , 1 + ( j ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j++;
   background->addWidget(lbl_unit, j, 0);
   background->addWidget(cnt_unit, j, 1);
   j++;
   background->addWidget( bg_solvent_conditions , j , 0 , 1 + ( j+5 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=6;
   background->addWidget( bg_reference_system , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget( bg_boundary_cond , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget( bg_mass_correction , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget( bg_volume_correction , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget( bg_bead_inclusion , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget( bg_buried , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget( bg_overlap , j , 0 , 1 + ( j+2 ) - ( j ) , 1 + ( 1 ) - ( 0 ) );
   j+=3;
   background->addWidget(pb_help, j, 0);
   background->addWidget(pb_cancel, j, 1);
}

void US_Hydrodyn_Hydro::update_unit(double val)
{
   (*hydro).unit = (int) -val;
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_solvent_name(const QString &str)
{
   (*hydro).solvent_name = str;
   check_solvent_defaults();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_solvent_acronym(const QString &str)
{
   (*hydro).solvent_acronym = str.left(5);
   le_solvent_acronym->setText(hydro->solvent_acronym);
   check_solvent_defaults();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_temperature(const QString &str)
{
   (*hydro).temperature = str.toDouble();
   //   le_temperature->setText(QString("").sprintf("%4.2f",(*hydro).temperature));
   check_solvent_defaults();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_solvent_viscosity(const QString &str)
{
   (*hydro).solvent_viscosity = str.toDouble();
   // le_solvent_viscosity->setText(QString("").sprintf("%f",(*hydro).solvent_viscosity));
   check_solvent_defaults();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_solvent_density(const QString &str)
{
   (*hydro).solvent_density = str.toDouble();
   // le_solvent_density->setText(QString("").sprintf("%f",(*hydro).solvent_density));
   check_solvent_defaults();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_volume(const QString &str)
{
   (*hydro).volume = str.toDouble();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_mass(const QString &str)
{
   (*hydro).mass = str.toDouble();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::update_overlap(const QString &str)
{
   (*hydro).overlap = str.toDouble();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::select_reference_system() {
   if ( rb_diffusion_center->isChecked() ) {
      return select_reference_system( 0 );
   }
   if ( rb_cartesian_origin->isChecked() ) {
      return select_reference_system( 1 );
   }
}

void US_Hydrodyn_Hydro::select_reference_system(int val)
{
   (*hydro).reference_system = val;
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::select_boundary_cond() {
   if ( rb_stick->isChecked() ) {
      return select_boundary_cond( 0 );
   }
   if ( rb_slip->isChecked() ) {
      return select_boundary_cond( 1 );
   }
}

void US_Hydrodyn_Hydro::select_boundary_cond(int val)
{
   (*hydro).boundary_cond = val;
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::select_bead_inclusion() {
   if ( rb_exclusion->isChecked() ) {
      return select_bead_inclusion( 0 );
   }
   if ( rb_inclusion->isChecked() ) {
      return select_bead_inclusion( 1 );
   }
}

void US_Hydrodyn_Hydro::select_bead_inclusion(int val)
{
   (*hydro).bead_inclusion = val;
   if ((*hydro).volume_correction)
   {
      cb_viscosity->setEnabled(false);
      cb_rotational->setEnabled(false);
   }
   else
   {
      cb_viscosity->setEnabled(!(*hydro).bead_inclusion);
      cb_rotational->setEnabled(!(*hydro).bead_inclusion);
   }
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::select_volume_correction() {
   if ( rb_auto_volume->isChecked() ) {
      return select_volume_correction( 0 );
   }
   if ( rb_manual_volume->isChecked() ) {
      return select_volume_correction( 1 );
   }
}

void US_Hydrodyn_Hydro::select_volume_correction(int val)
{
   (*hydro).volume_correction = val;
   le_volume->setEnabled((*hydro).volume_correction);
   if ((*hydro).volume_correction)
   {
      cb_viscosity->setEnabled(false);
      cb_rotational->setEnabled(false);
   }
   else
   {
      cb_viscosity->setEnabled(!(*hydro).bead_inclusion);
      cb_rotational->setEnabled(!(*hydro).bead_inclusion);
   }
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::select_mass_correction() {
   if ( rb_auto_mass->isChecked() ) {
      return select_mass_correction( 0 );
   }
   if ( rb_manual_mass->isChecked() ) {
      return select_mass_correction( 1 );
   }
}

void US_Hydrodyn_Hydro::select_mass_correction(int val)
{
   (*hydro).mass_correction = val;
   le_mass->setEnabled((*hydro).mass_correction);
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::select_overlap() {
   if ( rb_auto_overlap->isChecked() ) {
      return select_overlap( 0 );
   }
   if ( rb_manual_overlap->isChecked() ) {
      return select_overlap( 1 );
   }
}

void US_Hydrodyn_Hydro::select_overlap(int val)
{
   (*hydro).overlap_cutoff = val;
   le_overlap->setEnabled((*hydro).overlap_cutoff);
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::set_rotational()
{
   (*hydro).rotational = cb_rotational->isChecked();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::set_viscosity()
{
   (*hydro).viscosity = cb_viscosity->isChecked();
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::set_solvent_defaults()
{
   if ( cb_solvent_defaults->isChecked() )
   {
      hydro->solvent_name = "Water";
      hydro->solvent_acronym = "w";
      hydro->temperature = K20 - K0;
      hydro->solvent_viscosity = VISC_20W * 100;
      hydro->solvent_density = DENS_20W;
      le_solvent_name->setText((*hydro).solvent_name);
      le_solvent_acronym->setText((*hydro).solvent_acronym.left(5));
      le_temperature->setText(QString("").sprintf("%4.2f",(*hydro).temperature));
      le_solvent_viscosity->setText(QString("").sprintf("%f",(*hydro).solvent_viscosity));
      le_solvent_density->setText(QString("").sprintf("%f",(*hydro).solvent_density));
   }
   ((US_Hydrodyn *)us_hydrodyn)->display_default_differences();
}

void US_Hydrodyn_Hydro::check_solvent_defaults()
{
   cb_solvent_defaults->setChecked(
                                   hydro->solvent_name == "Water" &&
                                   hydro->solvent_acronym == "w" &&
                                   hydro->temperature == K20 - K0 &&
                                   hydro->solvent_viscosity == VISC_20W * 100 &&
                                   hydro->solvent_density == DENS_20W
                                   );
}
      
   

void US_Hydrodyn_Hydro::cancel()
{
   close();
}

void US_Hydrodyn_Hydro::help()
{
   US_Help *online_help;
   online_help = new US_Help(this);
   online_help->show_help("manual/somo/somo_hydro.html");
}

void US_Hydrodyn_Hydro::closeEvent(QCloseEvent *e)
{
   *hydro_widget = false;
   global_Xpos -= 30;
   global_Ypos -= 30;
   e->accept();
}

