 #! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui -o -name \*.rc -o -name \*.kcfg | grep -v '/unittests/'` >> rc.cpp
$EXTRACT_TR_STRINGS `find . -name \*.cc -o -name \*.cpp -o -name \*.h -name \*.qml | grep -v '/unittests/'` -o $podir/atcore_qt.pot

