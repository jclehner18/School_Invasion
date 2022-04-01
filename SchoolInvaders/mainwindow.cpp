#include "mainwindow.h"
#include "ui_mainwindow.h"

//For the intro screen take a picture of a black background monitor
//That has HTML code with a comment detailing to select one of the options in the unordered list
//Also have an instructions tab that displays the instructions within comments of HTML code
//Some how animate the selected option

//Add music at some point

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
    grabMouse();
    player_width = player_img.width();
    player_height = player_img.height();
    makeBarriers();
    getHighScores();
    ui->Mainbut->hide();
    getScreenText();
    timer->start(speed);
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

    Carr_rev = Carr.mirrored(true, false);
    Carroll_rev = Carroll.mirrored(true, false);
    Khadka_rev = Khadka.mirrored(true, false);
    Mitofsky_rev = Mitofsky.mirrored(true, false);
    Sharma_rev = Sharma.mirrored(true, false);
    Woolverton_rev = Woolverton.mirrored(true, false);
}

void MainWindow::getScreenText() {
    ifstream infile;
    string txt;
    infile.open("../TitleScreen.txt");
    while(getline(infile, txt)) {
        screenlines.push_back(txt);
        characters += txt.length();
    }
    infile.close();
}

void MainWindow::getHighScores() {
    ifstream infile;
    string txt;
    infile.open("../highScores.txt");
    int i = 0;
    while(getline(infile, txt) && i<20) {
        if(i%2==0) {
           highScoreNames[i/2] = txt;
        } else {
            highScores[i/2] = stoi(txt);
        }
        i++;
    }
    infile.close();
}

void MainWindow::setHighScores() {
    ofstream ofile;
    ofile.open("../highScores.txt");
    for(int i =0, j=0; i<10; i++) {
        if(j==0 && score>highScores[i]) {
            ofile<<name<<endl<<to_string(score)<<endl;
            j++;
        } else {
            ofile<<highScoreNames[i-j]<<endl<<to_string(highScores[i-j])<<endl;
        }
    }
    ofile.close();
    getHighScores();
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
    QFont font("Courier", 30, QFont::DemiBold);
    p.setPen(Qt::white);
    p.setFont(font);
    QFontMetrics fm(font);
    if(ingame && gameOverScreen <= 50) {
        p.drawImage(0, 0, background);
        QString Player_score = "Score: " + QString::fromStdString(to_string(score));
        p.drawText(110, 56, Player_score);
        QString levelstr = "Level: " + QString::fromStdString(to_string(level));
        p.drawText(870, 56, levelstr);
        QString highstr = "High Score: " + QString::fromStdString(to_string(highScores[0]));
        p.drawText(400, 56, highstr);


        //Draw Objects
        for(int i = 0; i<enemies.size(); i++) {
            if(AnimateEnemies>=3) {
                p.drawImage(enemies[i].x, enemies[i].y, *enemies[i].en_rev);
            } else {
                p.drawImage(enemies[i].x, enemies[i].y, *enemies[i].en);
            }
        }
        for(int i = 0; i<shots.size(); i++) {
            p.drawImage(shots[i].x, shots[i].y, *shots[i].sho_img);
        }
        for(int i =0; i<enshots.size(); i++) {
            //p.drawImage(enshots[i].x, enshots[i].y, *enshots[i].sho_img);
        }
        for(int i =0; i<barriers.size(); i++) {
            //Destroy barriers in 5 chunks of 26
            p.drawImage(barriers[i].x, barriers[i].y, barriers[i].img);
        }
        if(start && !pause && !gameover) {
            p.drawImage(player_x, player_y, Carr);
            if(moveFrame==0) {
                moveEnemies();
                moveFrame = movement;        
                if(AnimateEnemies==5) {
                    AnimateEnemies=0;
                } else {
                    AnimateEnemies++;
                }
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
            p.drawText(gamewidth/2-fm.horizontalAdvance(startgamestr)/2, gameheight/2+100, startgamestr);
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
            p.drawText(gamewidth/2-fm.horizontalAdvance(gamestr)/2, gameheight/2+100, gamestr);
            gameOverScreen++;
        }
    } else if(gameover && ingame) {
        setFixedSize(1100, 800);
        p.drawImage(0, 0, chalkboard);
        QString gamestr = "High Scores";
        p.drawText(gamewidth/2-fm.horizontalAdvance(gamestr)/2, 80, gamestr);
        string txt;
        QString text;
        for(int i =0, j = 0; i<10; i++) {
            if(highScores[i]<score && j==0) {
                naming = true;
                txt = name + ": " + to_string(score);
                text = QString::fromStdString(txt);
                QString textname = QString::fromStdString(name);
                j++;
                if(!donenaming) {
                    blinking++;
                    if(blinking<=20) {
                        p.setBrush(Qt::white);
                        p.drawRect(gamewidth/2-fm.horizontalAdvance(text)/2+fm.horizontalAdvance(textname), i*65+120, 10, 50);

                    } else if(blinking==40) {
                        blinking=0;
                    }
                }
            } else {
                txt = highScoreNames[i-j] + ": " + to_string(highScores[i-j]);
                text = QString::fromStdString(txt);
            }
            p.drawText(gamewidth/2-fm.horizontalAdvance(text)/2, i*65+150, text);
        }
    } else if(!instructions && !ingame) {
        p.setBrush(Qt::black);
        p.drawRect(0, 0, start_width, start_height);
        p.setPen(QColor(0, 150, 0));
        if(mainScreen<=characters*2) {
                string txt;
                QString Qtxt;
                int count = 0;
                for(int i = 0; i<screenlines.size() && count<mainScreen/2; i++) {
                    for(int j=0; j<screenlines[i].length() && count<mainScreen/2; j++) {
                        txt +=screenlines[i][j];
                        count++;
                    }
                    Qtxt = QString::fromStdString(txt);
                    p.drawText(0, i*40+50, Qtxt);
                    txt = "";
                }
            qDebug() << mainScreen;
            mainScreen ++;
        } else {
            QColor col = Qt::green;
            col.setAlphaF(1);
            p.setBrush(col);
            for(int i = 0; i<screenlines.size(); i++) {
                QString Qtxt = QString::fromStdString(screenlines[i]);

                p.drawText(0, i*40+50, Qtxt);
            }
        }
    }
}

void MainWindow::makeLevel() {
    for(int i = 0; i<11; i++) {
        enemies.push_back(makeEnemy(&Khadka, &Khadka_rev, i*70+70, 100, 60));
        enemies.push_back(makeEnemy(&Sharma, &Sharma_rev, i*70+70, 160, 50));
        enemies.push_back(makeEnemy(&Mitofsky, &Mitofsky_rev, i*70+70, 220, 40));
        enemies.push_back(makeEnemy(&Carroll, &Carroll_rev, i*70+70, 280, 30));
        enemies.push_back(makeEnemy(&Carr, &Carr_rev, i*70+70, 340, 20));
        enemies.push_back(makeEnemy(&Woolverton, &Woolverton_rev, i*70+70, 420, 10));
    }
    level++;
    movement = 50/2-level*4;
    if(movement<5) {
        movement = 5;
    }
}

enemy MainWindow::makeEnemy(QImage *img, QImage *img_rev, int x, int y, int pointVal) {
    enemy en;
    en.en = img;
    en.en_rev = img_rev;
    en.x = x;
    en.y = y;
    en.width = en.en->width();
    en.hieght = en.en->height();
    en.pointval = pointVal;
    return en;
}

void MainWindow::painting() {
    repaint();
    timer->start(speed);
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
    if(start && !gameover) {
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

        if(key==Qt::Key_P && ingame) {
            pause = !pause;
        }
    } else if(ingame && gameover && naming && !donenaming && name.length()<8) {
        switch(key) {
            case Qt::Key_A: {
               name +="a";
               break;
            }
            case Qt::Key_B: {
               name +="b";
               break;
            }
            case Qt::Key_C: {
               name +="c";
               break;
            }
            case Qt::Key_D: {
               name +="d";
               break;
            }
            case Qt::Key_E: {
               name +="e";
               break;
            }
            case Qt::Key_F: {
               name +="f";
               break;
            }
            case Qt::Key_G: {
               name +="g";
               break;
            }
            case Qt::Key_H: {
               name +="h";
               break;
            }
            case Qt::Key_I: {
               name +="i";
               break;
            }
            case Qt::Key_J: {
               name +="j";
               break;
            }
            case Qt::Key_K: {
               name +="k";
               break;
            }
            case Qt::Key_L: {
               name +="l";
               break;
            }
            case Qt::Key_M: {
               name +="m";
               break;
            }
            case Qt::Key_N: {
               name +="n";
               break;
            }
            case Qt::Key_O: {
               name +="o";
               break;
            }
            case Qt::Key_P: {
               name +="p";
               break;
            }
            case Qt::Key_Q: {
               name +="q";
               break;
            }
            case Qt::Key_R: {
               name +="r";
               break;
            }
            case Qt::Key_S: {
               name +="s";
               break;
            }
            case Qt::Key_T: {
               name +="t";
               break;
            }
            case Qt::Key_U: {
               name +="u";
               break;
            }
            case Qt::Key_V: {
               name +="v";
               break;
            }
            case Qt::Key_W: {
               name +="w";
               break;
            }
            case Qt::Key_X: {
               name +="x";
               break;
            }
            case Qt::Key_Y: {
               name +="y";
               break;
            }
            case Qt::Key_Z: {
               name +="z";
               break;
            }
        }
    }

    if(naming && gameover && key==Qt::Key_Space && !donenaming && name.length()>0) {
        naming = false;
        donenaming = true;
        setHighScores();
        score = 0;
        ui->Mainbut->show();
    } else if(naming && gameover && key==Qt::Key_Backspace && !donenaming && name.length()>0) {
        name = name.substr(0, name.length()-1);
    }
    if(!naming && gameover) {
        ui->Mainbut->show();
    }

    if(key==Qt::Key_Space) {
        //shoot
        if(ingame && !start) {
            start=true;
        } else if(ingame && start && cooldown<=0 && enemies.size()>=0) {
            shot sho;
            sho.sho_img = &MainShot;
            sho.x = player_x + player_width/2;
            sho.y = player_y;
            sho.width = sho.sho_img->width();
            sho.hieght = sho.sho_img->height();
            shots.push_back(sho);
            cooldown=COOLDOWN_LENGTH;
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
   int mousey = e->y();
   int mousex= e->x();
   qDebug() << "test";
    if(e->buttons()==Qt::LeftButton && mousey>=9*40+30 && mousey<=9*40+110 && main) {
        setFixedSize(gamewidth, gameheight);
        ingame = true;
        level = 8;
        makeLevel();
        start = false;
        gameover = false;
        gameOverScreen = 0;
        player_x = 250;
        player_y = gameheight-100;
        score = 2450;
        timer->start(speed);
        donenaming = false;
        naming = false;
        leftdir = false;
        main = false;
        name ="";
    } else if(e->buttons()==Qt::LeftButton && mousey>=11*40+30 && mousey<=11*40+110 && main) {
        main = false;
        instructions = true;
    } else if (e->buttons()==Qt::LeftButton && mousey>=13*40+30 && mousey<=13*40+110 && main) {
        ingame = true;
        gameover = true;
        gameOverScreen = 100;
        main = false;
    } else if(e->buttons()==Qt::LeftButton && mousey>=15*40+30 && mousey<=15*40+110 && main) {
        qApp->exit(0);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Mainbut_clicked() {
    ui->Mainbut->hide();
    ingame = false;
    start = false;
    gameover = false;
    enemies.clear();
    shots.clear();
    enshots.clear();
    enemiesDown = false;
}

