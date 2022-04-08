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
    int height;
};

struct shot {
    int x;
    int y;
    int width;
    int height;
    QImage *sho_img;
};

struct enemy_shot {
    int x;
    int y;
    int width;
    int height;
    QImage *sho_img;
};

struct barrier {
    int x;
    int y;
    QImage *img;
    int width;
    int height;
    vector<vector<bool>> hits;
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
    void keyReleaseEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;

    QImage Khadka;
    QImage Sharma;
    QImage Carr;
    QImage Mitofsky;
    QImage Carroll;
    QImage Woolverton;
    QImage Overton;
    QImage Khadka_rev;
    QImage Sharma_rev;
    QImage Carr_rev;
    QImage Mitofsky_rev;
    QImage Carroll_rev;
    QImage Woolverton_rev;
    QImage Overton_rev;
    QImage MainShot;
    QImage player_img;
    QImage EnemyShot;
    QImage background;
    //Just draw Over the QImage based on the bool double vector
    QImage desk;
    QImage chalkboard;

    const int start_width=800;
    const int start_height=900;

    const int gamewidth = 1200;
    const int gameheight = 900;

    QTimer *timer;

    QMap<int, bool> keys;

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
    void enemyShoot(enemy en);
    bool checkBarCollision(enemy_shot *enshot, barrier *barr);

    int speed = 1000/50; //50 is the amount of frames per second
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
    //const int COOLDOWN_LENGTH =1; //Debug timer
    const int COOLDOWN_LENGTH = 0; //Normal cooldown

    enemy special_en;
    int ENEMY_WAIT = 350;  //Amount of frames to wait before another special enemy can be spawned
    int enemywaittime = 0; //Frame counter for another special enemy to be spawned. If it is -1 special enemy is in play
    int ENSHOT_WAIT = 200;//Amount of frames to wait before an enemy can shoot
    int enshot_timer = 0; //Frame counter to wait for an enemy shot


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

    void MainMenu();

private slots:
    void painting();

};
#endif // MAINWINDOW_H
