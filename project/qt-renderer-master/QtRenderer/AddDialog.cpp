#include "AddDialog.h"
// #include <QDebug>
// #include "qpushbutton.h"
AddDialog::AddDialog() {
	// ui.setupUi(this);
	// fd = new QFileDialog(this);

	// connect(ui.toolButton, SIGNAL(clicked(bool)), this, SLOT(selectModel()));
	// connect(ui.toolButton_2, SIGNAL(clicked(bool)), this, SLOT(selectTexture()));
	// connect(ui.toolButton_3, SIGNAL(clicked(bool)), this, SLOT(selectNormal()));

	// connect(ui.okBtn, &QPushButton::clicked, this, [&] {
	// 	if (!model.isEmpty()) {
	// 		this->close();
	// 		emit modelLoaded(model, texture, normal);
	// 	}
	// 	else {
	// 		ui.modelFile->setText(u8"???????????");
	// 	}
	// 	});

	// connect(ui.cancelBtn, &QPushButton::clicked, this, [&] {
	// 	this->close();
	// 	});
}

void AddDialog::selectModel() {
	// model = fd->getOpenFileName(this, u8"??????", "./", "*.obj");
	// if(model.isEmpty()){
	// 	ui.modelFile->setText("-");
	// }else{
	// 	ui.modelFile->setText(QFileInfo(model).baseName());
	// 	//printf("select %s\n", QFileInfo(model).baseName().toStdString().c_str());
	// } 
}

void AddDialog::selectNormal() {
	// normal = fd->getOpenFileName(this, u8"????????", "./", "*.jpg *.png *.bmp *.tga");
	// if (normal.isEmpty()) ui.normalFile->setText("-");
	// else ui.normalFile->setText(QFileInfo(normal).baseName());
}

void AddDialog::selectTexture() {
	// texture = fd->getOpenFileName(this, u8"??????????", "./", "*.jpg *.png *.bmp *.tga");
	// if (texture.isEmpty()) ui.textureFile->setText("-");
	// else ui.textureFile->setText(QFileInfo(texture).baseName());
}
