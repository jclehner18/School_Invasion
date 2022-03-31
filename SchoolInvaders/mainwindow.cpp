#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadImages();
    setFixedSize(start_width, start_height);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(painting()));
    repaint();
    grabKeyboard();
    player_width = player_img.width();
    player_height = player_img.height();
    makeBarriers();
}

void MainWindow::loadImages() {
    background.load("../background.jpg");
    Carr.load("../Carr.png");
    Carroll.load("../Carroll.png");
    Khadka.load("../Khadka.png");
    Mitofsky.load("../Mitofsky.png");
    Sharma.load("../Sharma.png");
    Woolverton.load("../Woolverton.png");
    player_img.load("../Player_Img.png");
    MainShot.load("../MainShot.png");
    EnemyShot.load("../EnemyShot.png");
    desk.load("../desk.png");
    chalkboard.load("../chalkboard.jpg");

    Carr = Carr.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    Carroll = Carroll.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    Khadka = Khadka.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    Mitofsky = Mitofsky.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    Sharma = Sharma.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    Woolverton = Woolverton.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

void MainWindow::makeBarriers() {
    for(int i =0; i<4; i++) {
        barrier barr;
        barr.x = 124+254*i;
        barr.y = gameheight-220;
        barr.img = desk;
        barriers.push_back(barr);
    }
}

void MainWindow::paintEvent(QPaintEvent *e) {
    QPainter p(this);
    //Make sure to make the images smaller

    if(ingame && gameOverScreen <= 50) {
        p.drawImage(0, 0, background);
        QFont font("Courier", 30, QFont::DemiBold);
        p.setPen(Qt::white);
        p.setFont(font);
        QString Player_score = "Score: " + QString::fromStdString(to_string(score));
        p.drawText(110, 56, Player_score);
        QString levelstr = "Level: " + QString::fromStdString(to_string(level));
        p.drawText(870, 56, levelstr);
        //QString highstr = "High Score: " + QString::fromStdString(to_string(highscore));
        QString highstr = "High Score: 1000";
        p.drawText(400, 56, highstr);


        //Draw Objects
        for(int i = 0; i<enemies.size(); i++) {
            p.drawImage(enemies[i].x, enemies[i].y, enemies[i].en);
        }
        for(int i = 0; i<shots.size(); i++) {
            p.drawImage(shots[i].x, shots[i].y, shots[i].sho_img);
        }
        for(int i =0; i<enshots.size(); i++) {
            //p.drawImage(enshots[i].x, enshots[i].y, enshots[i].sho_img);
        }
        for(int i =0; i<barriers.size(); i++) {
            p.drawImage(barriers[i].x, barriers[i].y, barriers[i].img);
        }

        if(start && !pause && !gameover) {
            p.drawImage(player_x, player_y, Carr);
            if(moveFrame==0) {
                moveEnemies();
                moveFrame = movement;
            } else {
                moveFrame--;
            }
            moveShoot();
            cooldown--;
            qDebug() <<enemies.size();
            if(enemies.size()==0) {
                if(levelUpScreen<50) {
                    shots.clear();
                    enemies.clear();
                    QFontMetrics fm(font);
                    QString levelupstr = "Level " + QString::fromStdString(to_string(level)) + " Completed!!!";
                    p.setPen(Qt::black);
                    p.drawText(gamewidth/2-fm.horizontalAdvance(levelupstr)/2, gameheight/2, levelupstr);
                    levelUpScreen++;
                } else if(levelUpScreen==50) {
                    makeLevel();
                    start = false;
                    levelUpScreen = 0;
                }
            }
        } else if(!start && !gameover) {
            QFontMetrics fm(font);
            p.setPen(Qt::black);
            QString startgamestr = "Press Space to Start";
            p.drawText(gamewidth/2-fm.horizontalAdvance(startgamestr)/2, gameheight/2, startgamestr);
        } else if(start && pause && !gameover) {
            QFont font("Courier", 80, QFont::DemiBold);
            p.setPen(Qt::red);
            p.setFont(font);
            QFontMetrics fm(font);
            QString pausestr = "Paused";
            p.drawText(gamewidth/2-fm.horizontalAdvance(pausestr)/2, gameheight/2, pausestr);
        } else if(start && gameover)  {
            QFont font("Courier", 80, QFont::DemiBold);
            p.setPen(Qt::red);
            p.setFont(font);
            QFontMetrics fm(font);
            QString gamestr = "Game Over";
            p.drawText(gamewidth/2-fm.horizontalAdvance(gamestr)/2, gameheight/2, gamestr);
            gameOverScreen++;
        }
    } else if(gameover && ingame) {
        setFixedSize(1100, 800);
        p.drawImage(0, 0, chalkboard);
        ifstream infile;
        string txt;
        infile.open("../highScores.txt");
        while(getline(infile, txt)) {

        }
    }
}

void MainWindow::makeLevel() {
    for(int i = 0; i<11; i++) {
        enemies.push_back(makeEnemy(Khadka, i*70+70, 100, 60));
        enemies.push_back(makeEnemy(Sharma, i*70+70, 160, 50));
        enemies.push_back(makeEnemy(Mitofsky, i*70+70, 220, 40));
        enemies.push_back(makeEnemy(Carroll, i*70+70, 280, 30));
        enemies.push_back(makeEnemy(Carr, i*70+70, 340, 20));
        enemies.push_back(makeEnemy(Woolverton, i*70+70, 630, 10));
    }
    level++;
    movement = 50/2-level*4;
    if(movement<5) {
        movement = 5;
    }
}

enemy MainWindow::makeEnemy(QImage img, int x, int y, int pointVal) {
    enemy en;
    en.en = img;
    en.x = x;
    en.y = y;
    en.width = en.en.width();
    en.hieght = en.en.height();
    en.pointval = pointVal;
    return en;
}

void MainWindow::painting() {
    repaint();
    if(ingame) {
        timer->start(speed);
    }
}

void MainWindow::moveEnemies() {
    if(enemiesDown) {
        for(int i = 0; i<enemies.size(); i++) {
            enemies[i].y += 30;
            if(enemies[i].y+enemies[i].hieght>=gameheight-200) {
                gameover = true;
            } else if(movement>5) {
                movement -=1;
            }
        }
        leftdir = !leftdir;
        enemiesDown = false;

    } else {
        for(int i = 0; i<enemies.size(); i++) {
            if(leftdir) {
                enemies[i].x -=5;
            } else {
                enemies[i].x+=5;
            }
            if((enemies[i].x<=leftboundry &&leftdir) || (enemies[i].x+enemies[i].hieght>=rightboundry&&!leftdir)) {
                enemiesDown = true;
            }
        }
    }
}

void MainWindow::moveShoot() {
   for(int i = 0; i<shots.size(); i++) {
       shots[i].y -=10;
       if(shots[i].y+shots[i].hieght<0) {
           shots.erase(shots.begin()+i);
           i--;
       } else {
           for(int j=0; j<enemies.size(); j++) {
               if(checkCollision(&enemies[j], &shots[i])) {
                    score += enemies[j].pointval;
                    enemies.erase(enemies.begin()+j);
                    shots.erase(shots.begin()+i);
                    i--;
                    break;
               }
           }
       }
   }
}

bool MainWindow::checkCollision(enemy *en, shot *sh) {
    if(en->y+en->hieght>=sh->y && sh->y+sh->hieght>=en->y) {
        return en->x+en->width>=sh->x && sh->x+sh->width>=en->x;
    }
    return false;
}

bool MainWindow::checkEnCollision(enemy_shot *enshot) {
    if(player_y+player_height>=enshot->y && enshot->y+enshot->hieght>=player_y) {
        return player_x+player_width>=enshot->x && enshot->x+enshot->width>=player_x;
    }
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    switch(key) {
        case Qt::Key_Left: {
            if(player_x>10) {
                player_x -=5;
            }
           break;
        }
        case Qt::Key_Right: {
            if(player_x<gamewidth-10) {
                player_x +=5;
            }
            break;
        }
    }

    if(key==Qt::Key_Space) {
        //shoot
        if(ingame && !start) {
            start=true;
        } else if(ingame && start && cooldown<=0 && enemies.size()>=0) {
            shot sho;
            sho.sho_img = MainShot;
            sho.x = player_x + player_width/2;
            sho.y = player_y;
            sho.width = sho.sho_img.width();
            sho.hieght = sho.sho_img.height();
            shots.push_back(sho);
            cooldown=COOLDOWN_LENGTH;
        }
    }
    if(key==Qt::Key_P && ingame && start) {
        pause = !pause;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Beginbut_clicked() {
    ui->Beginbut->hide();
    setFixedSize(gamewidth, gameheight);
    ingame = true;
    level = 8;
    makeLevel();
    start = false;
    player_x = 250;
    player_y = gameheight-100;
    timer->start(speed);
}

