#!/bin/sh

rm -rf /var/tmp/mihl
mkdir -p /var/tmp/mihl-examples/usr/bin
cp ../example_1/Release/mihl_example_1  /var/tmp/mihl-examples/usr/bin
cp ../example_2/Release/mihl_example_2  /var/tmp/mihl-examples/usr/bin
cp ../example_3/Release/mihl_example_3  /var/tmp/mihl-examples/usr/bin
strip /var/tmp/mihl-examples/usr/bin/mihl_example_*

rm -rf /var/tmp/mihl-examples/etc
mkdir -p /var/tmp/mihl-examples/etc/mihl/examples/1
cp ../example_1/image.jpg /var/tmp/mihl-examples/etc/mihl/examples/1

mkdir -p /var/tmp/mihl-examples/etc/mihl/examples/2
cp ../example_2/image.jpg /var/tmp/mihl-examples/etc/mihl/examples/2

mkdir -p /var/tmp/mihl-examples/etc/mihl/examples/3
cp ../example_3/image.jpg /var/tmp/mihl-examples/etc/mihl/examples/3
cp ../example_3/prototype.js /var/tmp/mihl-examples/etc/mihl/examples/3

sudo rm -f /usr/src/redhat/RPMS/i386/mihl-examples-*
sudo touch /usr/src/redhat/RPMS/i386/mihl-examples-0-3.i386.rpm
sudo chmod a+rw /usr/src/redhat/RPMS/i386/mihl-examples-0-3.i386.rpm

rpmbuild -bb --quiet examples.spec
