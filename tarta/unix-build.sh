#!/bin/bash

RPMDIR=/usr/src/packages
PKG=tarta-`cat VERSION`
TMPFOLDER=/tmp/PKG
TARBALL=$PKG.tar.bz2

check()
{
echo "QTDIR is $QTDIR";

if [ -f $QTDIR/bin/qmake ]; then
  echo "Found qmake";
else
  echo "qmake not found";
  exit 1;
fi

}

dist()
{

echo "Creating tarball"
rm -rf $TMPFOLDER
mkdir $TMPFOLDER
cp -r * $TMPFOLDER
find $TMPFOLDER -name .svn -type d |xargs rm -rf
find $TMPFOLDER -name *.tar.bz2 |xargs rm -rf
tar -C /tmp -jcvf $TARBALL $KCPKG

}

debcp()
{
echo "Preparing debian build"
}

debbuild()
{
echo "Running debian build"
}

rpmcp()
{
echo "Copying necessary files to $RPMDIR"
mkdir -p $RPMDIR/SOURCES
mkdir -p $RPMDIR/SPECS
mkdir -p $RPMDIR/SPECS
mkdir -p $RPMDIR/RPMS
mkdir -p $RPMDIR/SRPMS
mkdir -p $RPMDIR/SRPMS

cp $TARBALL $RPMDIR/SOURCES/
cp packaging/rpm/tarta.spec $RPMDIR/SPECS/
}

rpmpkg()
{
echo "Building binary rpm"
rpmbuild -bb $RPMDIR/SPECS/tarta.spec

}


clean()
{
	make clean
	rm -rf tarta
	rm -rf $PKG
	rm -rf $TARBALL
}

case "$1" in
'clean')
	clean
	;;
'dist')
	check
	dist
	;;
'deb')
	check
	dist
	deb
	;;
'rpm')
	check
	dist
	rpmcp
	rpmpkg
	;;
*)
	echo "usage: $0 {clean | dist | deb | rpm | sol}"
esac

exit 0
