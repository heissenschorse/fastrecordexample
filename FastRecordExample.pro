
#check Qt version
message(project doc2 build with qt version $$QT_VERSION)

QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)
lessThan(QT_VER_MAJ, 5) | lessThan(QT_VER_MIN, 1) {
    error(LibQt requires 5.1 or newer but Qt $$[QT_VERSION] was detected.)
}

TEMPLATE = subdirs
# ###
SUBDIRS = source 

# ###
# when using the subdirs template, this option specifies that the directories
# listed should be processed in the order in which they are given.
CONFIG += ordered


