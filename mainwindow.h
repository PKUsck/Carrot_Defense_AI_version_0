#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include "gamescene.h"
#include "tower.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartWave();
    void onPauseResume();
    void onSelectArrow();
    void onSelectCannon();
    void onSelectIce();
    void onSelectPoison();
    void onSelectLightning();
    void onStatsChanged();
    void onGameEnded(bool won);

private:
    void setupUI();
    void setupToolbar();
    void updateStatusBar();
    void updateTowerButtons();

    GameScene* m_scene;

    QPushButton* m_btnStart;
    QPushButton* m_btnPause;
    QPushButton* m_btnArrow;
    QPushButton* m_btnCannon;
    QPushButton* m_btnIce;
    QPushButton* m_btnPoison;
    QPushButton* m_btnLightning;

    QLabel* m_lblGold;
    QLabel* m_lblLives;
    QLabel* m_lblWave;
    QLabel* m_lblInfo;

    TowerType m_selectedType;
};

#endif // MAINWINDOW_H
