#!/bin/sh
#		copypkg-mac.sh  - copy ultrascan3 directories to pkg/ultrascan3

ME=`whoami`
RSYNC="rsync -av --exclude=.svn"
SRCDIR=$us3
SOMOBASE=~/us3/us3_somo
PKGDIR=`(cd $us3/../pkg;pwd)`
PDIRS=""
SYSTYPE=`uname -s`

if [ "${SYSTYPE}" != "Darwin" ]; then
  echo "$0 requires a SYSTYPE (uname -s) of \"Darwin\""
  echo "  SYSTYPE=${SYSTYPE}"
  exit 1
fi

for D in ultrascan3 somo Frameworks; do
  DDIR=${PKGDIR}/${D}
  if [ ! -d ${DDIR} ]; then
    mkdir ${DDIR}
  fi
  PDIRS="${PDIRS} ${DDIR}"
done
echo "Enter Admin password --"
sudo chown -R $ME:admin ${PDIRS}

DDIR=${PKGDIR}/ultrascan3
for D in Developer etc bin lib; do
  SDIR=${SRCDIR}/${D}
  echo "${RSYNC} ${SDIR} ${DDIR}"
  ${RSYNC} ${SDIR} ${DDIR}
done
chmod g+w ${DDIR}/etc

# Remove any somo*prev* files in etc/
rm -f ${PKGDIR}/etc/somo*prev*

if [ -d ${PKGDIR}/somo/demo ]; then
  # Clear out somo/demo so only present contents are copied
  /bin/rm -rf ${PKGDIR}/somo/demo/*
fi

# Copy somo demo
SBASE=${SOMOBASE}/somo
SDIR=${SBASE}/demo
DDIR=${PKGDIR}/ultrascan3/somo
echo "${RSYNC} ${SDIR} ${DDIR}"
${RSYNC} ${SDIR} ${DDIR}
# Copy somo doc
SDIR=${SBASE}/doc
echo "${RSYNC} ${SDIR} ${DDIR}"
${RSYNC} ${SDIR} ${DDIR}
/bin/rm -rf ${PKGDIR}/somo/doc/manual/attic

SDIR=${SRCDIR}/Frameworks
DDIR=${PKGDIR}
echo "${RSYNC} ${SDIR} ${DDIR}"
${RSYNC} ${SDIR} ${DDIR}

##FRLIST=`(cd ${SRCDIR}/Frameworks;ls|grep framework)`
##cd ${PKGDIR}/ultrascan3/lib
##for F in ${FRLIST}; do ln -s ../Frameworks/$F .;done

cd ${PKGDIR}
/bin/rm -rf ultrascan3/somo/develop ultrascan3/somo/*.pl
pwd
find ./ -name '.svn' | xargs rm -Rf
echo "find ./ -name '.svn' -print"
find ./ -name '.svn' -print

echo "(Possibly) Enter Admin password --"
sudo chown -R root:admin ${PDIRS}

