QT += quick

INCLUDEPATH += headers

CONFIG += c++17

SOURCES += \
		sources/annotationlist.cpp \
        sources/annotation.cpp \
        sources/annotationmodel.cpp \
        sources/backend.cpp \
        main.cpp \
        sources/sample.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	main.qml \
	qml/AnnotationItem.qml \
	qml/MainMenuBar.qml \
	qml/MainToolBar.qml \
	qml/NotificationDialog.qml \
	qml/PassageContainer.qml \
	media/icons/close-icon.png \
	media/icons/copy-icon.png \
	media/icons/cut-icon.png \
	media/icons/open-icon.png \
	media/icons/paste-icon.png \
	media/icons/redo-icon.png \
	media/icons/save-icon.png \
	media/icons/undo-icon.png \
	media/logo-uit.ico

HEADERS += \
	headers/annotationlist.hpp \
	headers/annotation.hpp \
	headers/annotationmodel.hpp \
	headers/backend.hpp \
	headers/sample.hpp

RESOURCES += resources.qrc
