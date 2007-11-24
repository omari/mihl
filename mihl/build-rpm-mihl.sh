#!/bin/sh

rm -rf /var/tmp/mihl
mkdir -p /var/tmp/mihl/usr/lib
cp Release/libmihl.so  /var/tmp/mihl/usr/lib
strip /var/tmp/mihl/usr/lib/libmihl.so
sudo rm -f /usr/src/redhat/RPMS/i386/mihl-*
sudo touch /usr/src/redhat/RPMS/i386/mihl-0-3.i386.rpm
sudo chmod a+rw /usr/src/redhat/RPMS/i386/mihl-0-3.i386.rpm

rpmbuild -bb --quiet mihl.spec
