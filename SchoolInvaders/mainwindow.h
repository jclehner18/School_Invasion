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
#include <QMouseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using namespace std;

struct enemy  {
    QImage *en;
    QImage *en_rev;
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
    QImage *sho_img;
};

struct enemy_shot {
    int x;
    int y;
    int width;
    int hieght;
    QImage *sho_img;
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
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;

    QImage Khadka;
    QImage Sharma;
    QImage Carr;
    QImage Mitofsky;
    QImage Carroll;
    QImage Woolverton;
    QImage Khadka_rev;
    QImage Sharma_rev;
    QImage Carr_rev;
    QImage Mitofsky_rev;
    QImage Carroll_rev;
    QImage Woolverton_rev;
    QImage MainShot;
    QImage player_img;
    QImage EnemyShot;
    QImage background;
    QImage desk;
    QImage chalkboard;

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
    enemy makeEnemy(QImage *img, QImage *img_rev, int x, int y, int pointVal);
    void makeLevel();
    void moveShoot();
    bool checkCollision(enemy *en, shot *sh);
    bool checkEnCollision(enemy_shot *enshot);
    void makeBarriers();
    void getHighScores();
    void setHighScores();
    void getScreenText();
    void Rainbow();

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

    int AnimateEnemies = 0;

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
    int gameOverScreen = 0;
    bool instructions = false;
    int mainScreen = 0;
    bool main = true;
    int selected = -1;

    string highScoreNames[10];
    int highScores[10];

    string name ="";
    vector<string> screenlines;
    int characters = 0;

    bool naming = false;
    bool donenaming = false;

    int blinking = 0;

    int Rain = QColor(0, 150, 0).hue();

private slots:
    void painting();
    void on_Mainbut_clicked();
};
#endif // MAINWINDOW_H
