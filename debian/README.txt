Always build with:
LDFLAGS_APPEND="-Lsrc -lnfo" dpkg-buildpackage -rfakeroot -b -us -uc -nc -tc
