#!/bin/bash

GUIV=`cat VERSION`
BUILDV=`cat BUILDVER`

echo "Updating defines.h"
sed -i -e "s/^#define VERSION.*/#define VERSION \"$GUIV\"/" src/defines.h 
sed -i -e "s/^#define BUILD.*/#define BUILD \"$BUILDV\"/" src/defines.h 

echo "Updating packaging/win/tarta.nsi"
sed -i -e "s/^!define PRODUCT_VERSION.*/!define PRODUCT_VERSION \"$GUIV-b$BUILDV\"/" packaging/win/tarta.nsi
sed -i -e "s/^!define PRODUCT_BUILD.*/!define PRODUCT_BUILD \"$GUIV.$BUILDV\"/" packaging/win/tarta.nsi

echo "Updating packaging/rpm/tarta.spec"
sed -i -e "s/^Version:.*/Version: $GUIV/" packaging/rpm/tarta.spec

exit 0
