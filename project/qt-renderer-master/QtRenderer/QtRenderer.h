#pragma once

#include <QtWidgets/QMainWindow>
#include "uic/ui_QtRenderer.h"
#include "Renderer.h"
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include "AddDialog.h"

class QtRenderer : public QMainWindow
{
    Q_OBJECT

public:
    QtRenderer(QWidget *parent = nullptr);
    ~QtRenderer();
private:
    Ui::QtRendererClass ui;
    QTimer* fpsTimer;
    QLabel* fpsLabel;
    AddDialog* ad;

    QList<QListWidgetItem*> modelList;
    QList<QListWidgetItem*> lightList;
private slots:
    void addModel();
    void removeItem();
    void onFPS() {
        fpsLabel->setText("FPS: " + QString::number(ui.rendererLabel->getFPS()));
        fpsTimer->start(1000);
    }
    void receiveModel(QString& m, QString& t, QString& n);

    void itemChanged();
};
