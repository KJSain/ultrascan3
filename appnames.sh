#!/bin/sh
#	appnames.sh  -  fix install names for all apps (Mac only)

CDIR=`pwd`
BDIR=`dirname $0`
if [ -z "${BDIR}" -o "${BDIR}" = "." ]; then
  BDIR=${CDIR}
fi
BDIR=`echo ${BDIR} | sed -e 's@/bin$@@'`
#		insure we work from the ~/ultrascan3/bin directory
cd ${BDIR}/bin
 
#		fix the application in each bundle
for BUND in `ls -d *.app|grep -v 'us3_'`;do

  FILE=`echo ${BUND} | sed -e 's/.app//'`

  APPP=${BUND}/Contents/MacOS/${FILE}

#		get list of library names to change
  LIBL=`otool -L ${APPP} \
    | egrep '_utils|us_gui|qwt|qca|mysql|framework' \
    | grep -v executable \
    | grep -v Library \
    | awk '{print $1}'`

#		change each of the library names
  for NAMI in ${LIBL}; do
    
    if [ `echo ${NAMI} | grep -ci mysql` -ne 0 ]; then
      NAMO=@executable_path/../../../../lib/libmysqlclient.18.dylib
    else
      if [ `echo ${NAMI} | grep -ci framework` -eq 0 ]; then
        #		use relative path to library
        NAMO=@executable_path/../../../../lib/${NAMI}
      else
        #		use relative path to Qt framework
        NAMJ=`echo ${NAMI} | sed -e 's#@rpath/##'`
        NAMO=@executable_path/../../../../Frameworks/${NAMJ}
      fi
    fi

#		report and do the install name change
    echo "install_name_tool -change ${NAMI} ${NAMO} ${APPP}"
    install_name_tool -change ${NAMI} ${NAMO} ${APPP}

  done

done

cd ${CDIR}

