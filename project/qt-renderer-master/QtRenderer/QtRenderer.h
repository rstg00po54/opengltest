#pragma once

// #include <QtWidgets/QMainWindow>
// #include "uic/ui_QtRenderer.h"
#include "Renderer.h"
// #include <QKeyEvent>
// #include <QTimer>
// #include <QList>
#include "AddDialog.h"

class QtRenderer 
{
    

public:
    QtRenderer();
    ~QtRenderer();
private:
    // Ui::QtRendererClass ui;
    // QTimer* fpsTimer;
    // QLabel* fpsLabel;
    AddDialog* ad;

    // QList<QListWidgetItem*> modelList;
    // QList<QListWidgetItem*> lightList;
// private slots:
    void addModel();
    void removeItem();
    void onFPS() {
        // fpsLabel->setText("FPS: " + QString::number(ui.rendererLabel->getFPS()));
        // fpsTimer->start(1000);
    }
    void receiveModel(string& m, string& t, string& n);

    void itemChanged();
};
