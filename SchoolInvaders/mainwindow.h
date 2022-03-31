#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <QTimer>
#include <random>
#include <iostream>
#include <string>
#include <fstream>
#include <QSound>
#include <QDebug>
#include <random>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using namespace std;

struct enemy  {
    QImage en;
    int pointval;
    int x;
    int y;
    int width;
    int hieght;
};

struct shot {
    int x;
    int y;
    int width;
    int hieght;
    QImage sho_img;
};

struct enemy_shot {
    int x;
    int y;
    int width;
    int hieght;
    QImage sho_img;
};

struct barrier {
    int x;
    int y;
    QImage img;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadImages();

protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;

    QImage Khadka;
    QImage Sharma;
    QImage Carr;
    QImage Mitofsky;
    QImage Carroll;
    QImage Woolverton;
    QImage MainShot;
    QImage player_img;
    QImage EnemyShot;
    QImage background;
    QImage desk;

    const int start_width=800;
    const int start_height=900;

    const int gamewidth = 1200;
    const int gameheight = 900;

    QTimer *timer;

    vector<enemy> enemies;
    vector<shot> shots;
    vector<enemy_shot> enshots;
    vector<barrier> barriers;

    void moveEnemies();
    enemy makeEnemy(QImage img, int x, int y, int pointVal);
    void makeLevel();
    void moveShoot();
    bool checkCollision(enemy *en, shot *sh);
    bool checkEnCollision(enemy_shot *enshot);
    void makeBarriers();

    int speed = 1000/50;
    int moveFrame;
    int movement;
    bool ingame = false;
    bool start = false;
    bool gameover = false;
    int score = 0;
    int level = 0;
    int lives = 3;
    bool leftdir=false;
    const int leftboundry = 50;
    const int rightboundry = gamewidth-50;

    int player_x=200;
    int player_y=gameheight-200;
    int player_width;
    int player_height;

    int cooldown = 0;
    const int COOLDOWN_LENGTH = 1;
    //const int COOLDOWN_LENGTH = 15; Normal cooldown

    bool enemiesDown = false;

    bool pause = false;

    int levelUpScreen = 0;


private slots:
    void painting();
    void on_Beginbut_clicked();
};
#endif // MAINWINDOW_H
