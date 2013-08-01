#pragma once

#include <QWidget>
#include <QAbstractButton>

namespace Ui {
class topo_blend_widget;
}

class topoblend;

class topo_blend_widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit topo_blend_widget(topoblend * topo_blend, QWidget *parent = 0);
    ~topo_blend_widget();

	int synthesisSamplesCount();

public slots:
    void renderViewer();
    void renderAnimation();
    void loadAnimationModel();
    void doBlend();
	void loadCorrespondenceModel();
	void showGroupingDialog();
	void vizButtonClicked(QAbstractButton* b);
	void toggleCheckOption(QString optionName);
	void setCheckOption( QString optionName, bool toValue = true );
	// Model manipulation
	void normalizeModel();
	void bottomCenterModel();
	void moveModel();
	void rotateModel();
	void scaleModel();
	void exportAsOBJ();

	// Part manipulation
	void reverseCurve();
	void updatePartsList();

	void splatSizeChanged(double newSize);

	// Jobs
	void saveJob();
	void loadJob();
	void loadJobFile(QString job_filename);
private:
	Ui::topo_blend_widget *ui;
    topoblend * tb;
};
