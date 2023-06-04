#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "annotationmodel.hpp"
#include "backend.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Backend* backend = new Backend(&app);
    AnnotationModel* annotationModel = new AnnotationModel(&app);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                     [url](QObject *obj, const QUrl &objUrl) {
                        if (!obj && url == objUrl)
                            QCoreApplication::exit(-1);
                    },
                    Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("backend", backend);
    engine.rootContext()->setContextProperty("annotationModel", annotationModel);

    QObject::connect(backend, &Backend::loadedAnnotations, annotationModel, &AnnotationModel::setAnnotations);
    QObject::connect(annotationModel, &AnnotationModel::annotationUpdated, backend, &Backend::updateAnnotation);
    QObject::connect(annotationModel, &AnnotationModel::startIndexChanged, backend, &Backend::onStartIndexChanged);
    QObject::connect(annotationModel, &AnnotationModel::endIndexChanged, backend, &Backend::onEndIndexChanged);

    engine.load(url);

    return app.exec();
}
