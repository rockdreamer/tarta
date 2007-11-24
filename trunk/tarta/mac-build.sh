#!/bin/bash

check()
{
echo "QTDIR is $QTDIR";

if [ -f $QTDIR/bin/qmake ]; then
  echo "Found qmake";
else
  echo "qmake not found";
  exit 1;
fi

if [ -f /usr/bin/make ]; then
  echo "Found make";
else
  echo "make not found. Install XCode";
  exit 1;
fi

if [ -f /usr/bin/gcc ]; then
  echo "Found gcc";
else
  echo "gcc not found. Install XCode";
  exit 1;
fi

if [ -f /usr/bin/otool ]; then
  echo "Found otool";
else
  echo "otool not found. Install XCode";
  exit 1;
fi

if [ -f /usr/bin/hdiutil ]; then
  echo "Found hdiutil";
else
  echo "hdiutil not found. Install BSD subsystem";
  exit 1;
fi

if [ -d $QTDIR/lib/QtCore.framework ]; then
  echo "Found QtCore framework";
else
  echo "QtCore Framework not found in QTDIR";
  exit 1;
fi

if [ -d $QTDIR/lib/QtGui.framework ]; then
  echo "Found QtGui framework";
else
  echo "QtGui Framework not found in QTDIR";
  exit 1;
fi

if [ -d $QTDIR/lib/QtNetwork.framework ]; then
  echo "Found QtNetwork framework";
else
  echo "QtNetwork Framework not found in QTDIR";
  exit 1;
fi
}

PKG=Tarta-`cat VERSION`-b`cat BUILD`

case "$1" in
'clean')
	make clean
	rm -rf tarta.app
	rm -rf $PKG
	rm -rf $PKG.dmg
	;;
'pkg')
	check
	echo "Running Qmake"
	qmake tarta.pro

	echo "Running Make"
	make

	echo "Creating Bundle Directories"
	mkdir tarta.app/Contents/Resources
	mkdir tarta.app/Contents/Frameworks

	echo "Copying Frameworks into Bundle"
	cp -r $QTDIR/lib/QtGui.framework tarta.app/Contents/Frameworks/
	cp -r $QTDIR/lib/QtCore.framework tarta.app/Contents/Frameworks/
	cp -r $QTDIR/lib/QtNetwork.framework tarta.app/Contents/Frameworks/

	echo "Setting Framework names"
	install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui tarta.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui
	install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore tarta.app/Contents/Frameworks/QtCore.framework/Versions/4.0/QtCore
	install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork tarta.app/Contents/Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork

	install_name_tool -change "$QTDIR"lib/QtGui.framework/Versions/4.0/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui tarta.app/Contents/MacOS/tarta
	install_name_tool -change "$QTDIR"lib/QtCore.framework/Versions/4.0/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore tarta.app/Contents/MacOS/tarta
	install_name_tool -change "$QTDIR"lib/QtNetwork.framework/Versions/4.0/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork tarta.app/Contents/MacOS/tarta

	install_name_tool -change "$QTDIR"lib/QtCore.framework/Versions/4.0/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore tarta.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui
	install_name_tool -change "$QTDIR"lib/QtCore.framework/Versions/4.0/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore tarta.app/Contents/Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork

	echo "Clean permissions"
	chmod -R u+rw tarta.app
	chmod -R uog+r tarta.app
	
	echo -n "Creating dmg directory..."
	echo $PKG
	mkdir $PKG/

	echo "Copying into dmg directory"
	mv tarta.app $PKG/
	cp README.mac $PKG/README.txt
	cp COPYING $PKG/COPYING.txt

	echo -n "Creating $PKG.dmg"
	hdiutil create -fs HFS+ -volname $PKG -srcfolder $PKG $PKG.dmg
	;;
*)
	echo "usage: $0 {clean | pkg}"
esac

exit 0
