#include "QtRenderer.h"
#include <Eigen3/Eigen/Eigen>
// #include <QDebug>
// #include <QMessageBox>
QtRenderer::QtRenderer()
{
	// ui.setupUi(this);

	// fpsLabel = new QLabel("fps");
	// ui.statusBar->addWidget(fpsLabel);

	// fpsTimer = new QTimer(this);
	// connect(fpsTimer, SIGNAL(timeout()), this, SLOT(onFPS()));
	// fpsTimer->start(1000);

	// ad = new AddDialog(this);
	// connect(ui.addButton, SIGNAL(clicked(bool)), this, SLOT(addModel()));
	// connect(ui.rmButton, SIGNAL(clicked(bool)), this, SLOT(removeItem()));

	// connect(ad, &AddDialog::modelLoaded, this, &QtRenderer::receiveModel);

	//ui.listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	// connect(ui.listWidget, &QListWidget::itemSelectionChanged, this, &QtRenderer::itemChanged);


}

QtRenderer::~QtRenderer()
{
	// delete fpsTimer;
	// delete fpsLabel;

}

void QtRenderer::addModel() {
	// ad->load();
}

void QtRenderer::receiveModel(string& m, string& t, string& n) {
	printf("%s\n", __func__);
	// int num = ui.rendererLabel->addModel(m, t, n);
	// if (num == 0) {
		// QMessageBox::warning(this, "Error", "Load Model Failed");
	// }
	// else {
		// for (int i = 0; i < num; i++) {
			// QListWidgetItem* item = new QListWidgetItem(QFileInfo(m).baseName());
			// item->setData(Qt::UserRole, 1);
			// modelList.append(item);
			// ui.listWidget->addItem(item);
		// }
	// }
}

void QtRenderer::itemChanged() {
	// QList<QListWidgetItem*> selectedItems = ui.listWidget->selectedItems();
	// if (!selectedItems.isEmpty()) {
	//     QListWidgetItem* item = selectedItems.first();
	//     if(item->data(Qt::UserRole) == 1) ui.rendererLabel->selectModel(modelList.indexOf(item));

	// }
}

void QtRenderer::removeItem() {
	// QList<QListWidgetItem*> selectedItems = ui.listWidget->selectedItems();
	// if (!selectedItems.isEmpty()) {
	//     QListWidgetItem* item = selectedItems.first();
	//     if (item->data(Qt::UserRole) == 1) {
	//         int index = modelList.indexOf(item);
	//         ui.rendererLabel->rmModel(index);
	//         modelList.removeAt(index);

	//         ui.listWidget->takeItem(ui.listWidget->currentRow());
	//     }

		
	// }
}


