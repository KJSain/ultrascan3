// ---------------------------------------------------------------------------------------------
// --------------- WARNING: this code is generated by an automatic code generator --------------
// ---------------------------------------------------------------------------------------------
// -------------- WARNING: any modifications made to this code will be overwritten -------------
// ---------------------------------------------------------------------------------------------

#ifndef US_HYDRODYN_CLUSTER_ONED_H
#define US_HYDRODYN_CLUSTER_ONED_H

#include "us_hydrodyn_cluster.h"
#include "qlabel.h"
#include "qstring.h"
#include "qlayout.h"
#include "qlineedit.h"
#include "qfontmetrics.h"
#include "qfile.h"
#include "q3filedialog.h"
//Added by qt3to4:
#include <QCloseEvent>
#include "us_hydrodyn.h"
#include "qpushbutton.h"
#include "qmessagebox.h"
#include "qregexp.h"
#include "us_json.h"

using namespace std;

class US_EXTERN US_Hydrodyn_Cluster_Oned : public QDialog
{
   Q_OBJECT

   public:
      US_Hydrodyn_Cluster_Oned(
                               void                     *              us_hydrodyn,
                               map < QString, QString > *              parameters,
                               QWidget *                               p = 0,
                               const char *                            name = 0
                               );

      ~US_Hydrodyn_Cluster_Oned();

   private:

      US_Config *                             USglobal;

      QLabel *                                lbl_title;
      QLabel *                                lbl_credits_1;
      QLabel *                                lbl_1drotationfile;
      QLineEdit *                             le_1drotationfile;
      QLabel *                                lbl_1drotationsuserand;
      QLineEdit *                             le_1drotationsuserand;
      QLabel *                                lbl_1dlambda;
      QLineEdit *                             le_1dlambda;
      QLabel *                                lbl_1ddetectordistance;
      QLineEdit *                             le_1ddetectordistance;
      QLabel *                                lbl_1ddetectorwidth;
      QLineEdit *                             le_1ddetectorwidth;
      QLabel *                                lbl_1ddetectorpixelswidth;
      QLineEdit *                             le_1ddetectorpixelswidth;
      QLabel *                                lbl_1drho0;
      QLineEdit *                             le_1drho0;
      QLabel *                                lbl_1ddeltar;
      QLineEdit *                             le_1ddeltar;
      QLabel *                                lbl_1dproberadius;
      QLineEdit *                             le_1dproberadius;
      QLabel *                                lbl_1dthreshold;
      QLineEdit *                             le_1dthreshold;
      QLabel *                                lbl_1dsamplerotations;
      QLineEdit *                             le_1dsamplerotations;
      QLabel *                                lbl_1daxisrotations;
      QLineEdit *                             le_1daxisrotations;
      QLabel *                                lbl_1dintermediatesaves;
      QLineEdit *                             le_1dintermediatesaves;
      QPushButton *                           pb_save;
      QPushButton *                           pb_load;

      QPushButton *                           pb_help;
      QPushButton *                           pb_close;
      void                     *              us_hydrodyn;
      map < QString, QString > *              parameters;
      void                                    update_fields();


      void                                    setupGUI();

   private slots:

      void                                    update_1drotationfile( const QString & );
      void                                    update_1drotationsuserand( const QString & );
      void                                    update_1dlambda( const QString & );
      void                                    update_1ddetectordistance( const QString & );
      void                                    update_1ddetectorwidth( const QString & );
      void                                    update_1ddetectorpixelswidth( const QString & );
      void                                    update_1drho0( const QString & );
      void                                    update_1ddeltar( const QString & );
      void                                    update_1dproberadius( const QString & );
      void                                    update_1dthreshold( const QString & );
      void                                    update_1dsamplerotations( const QString & );
      void                                    update_1daxisrotations( const QString & );
      void                                    update_1dintermediatesaves( const QString & );
      void                                    save();
      void                                    load();

      void                                    help();
      void                                    cancel();

   protected slots:

      void                                    closeEvent( QCloseEvent * );
};

#endif