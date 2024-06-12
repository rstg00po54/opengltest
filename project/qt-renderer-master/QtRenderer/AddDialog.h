#pragma once

#include <QDialog>
#include <QFileDialog>
#include "uic/ui_AddDialog.h"

class AddDialog : public QDialog {
	Q_OBJECT

public:
	AddDialog(QWidget* parent = nullptr);
	~AddDialog() = default;
	void load() {
		ui.modelFile->setText("-");
		ui.normalFile->setText("-");
		ui.textureFile->setText("-");
		model.clear();
		normal.clear();
		texture.clear();

		show();
	}
signals:
	void modelLoaded(QString& m, QString& t, QString& n);

private slots:
	void selectModel();
	void selectNormal();
	void selectTexture();
private:
	Ui::AddDialog ui;
	QFileDialog* fd;

	QString model, normal, texture;
};