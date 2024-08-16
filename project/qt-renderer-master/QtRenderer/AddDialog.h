#pragma once

// #include <QDialog>
// #include <QFileDialog>
// #include "uic/ui_AddDialog.h"
#include <iostream>

class AddDialog  {
	// Q_OBJECT

public:
	AddDialog();
	~AddDialog() = default;
	void load() {
		// ui.modelFile->setText("-");
		// ui.normalFile->setText("-");
		// ui.textureFile->setText("-");
		// model.clear();
		// normal.clear();
		// texture.clear();

		// show();
	}
// signals:
	void modelLoaded(std::string& m, std::string& t, std::string& n);

// private slots:
	void selectModel();
	void selectNormal();
	void selectTexture();
private:
	// Ui::AddDialog ui;
	// QFileDialog* fd;

	std::string model, normal, texture;
};