CONFIG += starlab
STARLAB_TEMPLATE += plugin
STARLAB_DEPENDS += ../../starlab/plugins-surfacemesh/surfacemesh
STARLAB_EXTERNAL += kdtree-matlab

QT += opengl

HEADERS += resample.h \
    resamplewidget.h \
    SpherePackSampling.h \
    Sampler.h
SOURCES += resample.cpp \
    resamplewidget.cpp \
    Sampler.cpp
RESOURCES += resample.qrc

FORMS += \
    resamplewidget.ui
