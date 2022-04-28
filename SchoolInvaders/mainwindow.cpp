#include "mainwindow.h"
#include "ui_mainwindow.h"

//That has HTML code with a comment detailing to select one of the options in the unordered list
//Some how animate the selected option

//Add music at some point
QSound music("../Undertale.wav");
QSound playerShoot("../Player_Shot.wav");
QSound enemyshoot("../enemyShoot.wav");
QSound enemyHit("../enemyDie.wav");
QSound Hit1("../Hit1.wav");
QSound Hit2("../Hit2.wav");
QSound Hit3("../Hit3.wav");
QSound Hit4("../Hit4.wav");
QSound Fail("../Fail.wav");


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadImages();
    setFixedSize(start_width, start_height);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(painting()));
    grabKeyboard();
    grabMouse();
    setMouseTracking(true);
    player_width = player_img.width();
    player_height = player_img.height();
    makeBarriers();
    getHighScores();
    getScreenText();
    getInstructions();

    srand(NULL);
    loopSetter();
    timer->start(speed);
}

void MainWindow::loopSetter() {
    music.setLoops(0);
    playerShoot.setLoops(0);
    Hit1.setLoops(0);
    Hit2.setLoops(0);
    Hit3.setLoops(0);
    Hit4.setLoops(0);
    Fail.setLoops(0);
    enemyshoot.setLoops(0);
    enemyHit.setLoops(0);
}

void MainWindow::loadImages() {
    background.load("../background.jpg");
    Carr.load("../Carr.png");
    Carroll.load("../Carroll.png");
    Khadka.load("../Khadka.png");
    Mitofsky.load("../Mitofsky.png");
    Sharma.load("../Sharma.png");
    Overton.load("../Overton.png");
    Woolverton.load("../Woolverton.png");
    player_img.load("../Kobe.png");
    MainShot.load("../MainShot.png");
    desk.load("../desk.png");
    chalkboard.load("../chalkboard.jpg");
    EnemyShot.load("../Turn_In.png");

    desk = desk.scaled(130, 45, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    MainShot = MainShot.scaled(22, 30, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    EnemyShot = EnemyShot.scaled(30, 30, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Carr = Carr.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Carroll = Carroll.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Khadka = Khadka.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Mitofsky = Mitofsky.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Sharma = Sharma.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Woolverton = Woolverton.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    Overton = Overton.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    player_img = player_img.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    Carr_rev = Carr.mirrored(true, false);
    Carroll_rev = Carroll.mirrored(true, false);
    Khadka_rev = Khadka.mirrored(true, false);
    Mitofsky_rev = Mitofsky.mirrored(true, false);
    Sharma_rev = Sharma.mirrored(true, false);
    Woolverton_rev = Woolverton.mirrored(true, false);
    Overton_rev = Overton.mirrored(true, false);
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

void MainWindow::getInstructions() {
    ifstream infile;
    string txt;
    infile.open("../Instructions.txt");
    while(getline(infile, txt)) {
        instructionLines.push_back(txt);
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
        barr.y = gameheight-180;
        barr.img = &desk;
        barr.width = desk.width();
        barr.height = desk.height();
        for(int j =0; j<5; j++) {
            vector<bool> vec;
            vec.push_back(false);
            vec.push_back(false);
            vec.push_back(false);
            vec.push_back(false);
            vec.push_back(false);
            barr.hits.push_back(vec);
        }
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
        QString lifes = "Lives: " + QString::fromStdString(to_string(lives));
        p.setPen(Qt::black);
        p.drawText(80, height()-7, lifes);


        //Draw Barriers
        for(int i =0; i<barriers.size(); i++) {
            //Destroy barriers in 5 chunks of 26
            p.drawImage(barriers[i].x, barriers[i].y, *barriers[i].img);
            for(int j=0; j<barriers[i].hits.size(); j++) {
                for(int k=0; k<barriers[i].hits[j].size(); k++) {
                    if(barriers[i].hits[j][k]) {
                        p.setBrush(QColor(235, 219, 195));
                        p.setPen(QColor(235, 219, 195));
                        p.drawRect(barriers[i].x+(barriers[i].width/barriers[i].hits.size())*j, barriers[i].y+(barriers[i].height/barriers[i].hits[j].size())*k, barriers[i].width/barriers[i].hits.size(), barriers[i].height/barriers[i].hits[j].size());
                    }
                }
            }
        }

        //Draw Enemies
        for(int i = 0; i<enemies.size(); i++) {
            if(AnimateEnemies>=3) {
                p.drawImage(enemies[i].x, enemies[i].y, *enemies[i].en_rev);
            } else {
                p.drawImage(enemies[i].x, enemies[i].y, *enemies[i].en);
            }
        }
        if(AnimateEnemies>=3 && enemywaittime==-1) {
            p.drawImage(special_en.x, special_en.y, *special_en.en_rev);
        } else if(enemywaittime==-1){
            p.drawImage(special_en.x, special_en.y, *special_en.en);
        }


        //Draw Character Shots
        for(int i = 0; i<shots.size(); i++) {
            p.drawImage(shots[i].x, shots[i].y, *shots[i].sho_img);
        }

        //Draw Enemy Shots
        for(int i =0; i<enshots.size(); i++) {
            p.drawImage(enshots[i].x, enshots[i].y, *enshots[i].sho_img);
        }


        if(invincible>=0) {
            if((invincible/5)%2==1) {
                p.drawImage(player_x, player_y, player_img);
            }
        } else {
            p.drawImage(player_x, player_y, player_img);
        }



        if(start && !pause && !gameover) {
            if(lives==0) {
                gameover = true;
                music.stop();
            } else if(invincible>=0) {
                invincible++;
                if(invincible ==50) {
                    invincible = -1;
                }
            }

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

            //Make Special Enemy
            if(enemywaittime>=ENEMY_WAIT && enemies.size()>0) {
                if(rand()%100<7+enemywaittime/10) {
                    special_en.x = -80;
                    special_en.y = 80;
                    special_en.en = &Overton;
                    special_en.en_rev = &Overton_rev;
                    special_en.width = special_en.en->width();
                    special_en.height = special_en.en->height();
                    special_en.pointval = 100;
                    enemywaittime = -1;
                } else {
                    enemywaittime ++;
                }
            } else if(enemywaittime!=-1){
               enemywaittime++;
            }

            //Decides if enemy shoots
            if(ENSHOT_WAIT<=enshot_timer && enemies.size()>0) {
                if(rand()%100<15+enshot_timer/10) {
                    enemyShoot(enemies[rand()%enemies.size()]);
                    if(enemyshoot.isFinished()) {
                        enemyshoot.play();
                    } else {
                        enemyshoot.play();
                    }
                    enshot_timer = 0;
                } else {
                    enshot_timer++;
                }
            } else {
                enshot_timer++;
            }

            if(enemies.size()==0) {
                if(levelUpScreen<50) {
                    shots.clear();
                    enemies.clear();
                    enshots.clear();
                    enemywaittime = 0;
                    QFontMetrics fm(font);
                    QString levelupstr = "Level " + QString::fromStdString(to_string(level)) + " Completed!!!";
                    p.setPen(Qt::black);
                    p.drawText(gamewidth/2-fm.horizontalAdvance(levelupstr)/2, gameheight/2, levelupstr);
                    levelUpScreen++;
                } else if(levelUpScreen==50) {
                    music.stop();
                    makeLevel();
                    start = false;
                    levelUpScreen = 0;
                    if(ENSHOT_WAIT>100) {
                        ENSHOT_WAIT -=100;
                    } else if(ENSHOT_WAIT>10) {
                        ENSHOT_WAIT -=10;
                    }
                } else {
                    //Key Presses
                    if(keys[Qt::Key_Left]) {
                        if(player_x>10) {
                            player_x -=5;
                        }
                    }
                    if(keys[Qt::Key_Right]) {
                        if(player_x<gamewidth-10) {
                            player_x +=5;
                        }
                    }
                    if(keys[Qt::Key_Space] && start && cooldown<=0 && enemies.size()>=0) {
                                shot sho;
                                sho.sho_img = &MainShot;
                                sho.x = player_x + player_width/2;
                                sho.y = player_y;
                                sho.width = sho.sho_img->width();
                                sho.height = sho.sho_img->height();
                                shots.push_back(sho);
                                cooldown=COOLDOWN_LENGTH;
                                playerShoot.play();
                    }


                }
            } else {
                if(music.isFinished()) {
                    music.play();
                }
                //Key Presses
                if(keys[Qt::Key_Left]) {
                    if(player_x>10) {
                        player_x -=5;
                    }
                }
                if(keys[Qt::Key_Right]) {
                    if(player_x<gamewidth-10) {
                        player_x +=5;
                    }
                }
                if(keys[Qt::Key_Space] && start && cooldown<=0 && enemies.size()>=0) {
                            shot sho;
                            sho.sho_img = &MainShot;
                            sho.x = player_x + player_width/2;
                            sho.y = player_y;
                            sho.width = sho.sho_img->width();
                            sho.height = sho.sho_img->height();
                            shots.push_back(sho);
                            cooldown=COOLDOWN_LENGTH;
                            playerShoot.play();
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
            if(gameOverScreen==1) {
                music.stop();
                if(Fail.isFinished()) {
                    Fail.play();
                } else {
                    Fail.play();
                }
            }
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
        if(!naming) {
            p.drawRect(28, 34, 285, 80);
            p.drawText(70, 88, "Main Menu");
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
            mainScreen = characters*2+1;
        } else {
            QColor col = Qt::green;
            col.setAlphaF(1);
            p.setBrush(col);
            for(int i = 0; i<screenlines.size(); i++) {
                QString Qtxt = QString::fromStdString(screenlines[i]);
                if(i==selected) {
                    Rainbow();
                    p.setPen(QColor::fromHsv(Rain, 255, 255, 255));
                } else {
                    p.setPen(QColor(0, 150, 0));
                }
                p.drawText(0, i*40+50, Qtxt);
            }
        }
    } else if(instructions) {
        p.setBrush(Qt::black);
        p.drawImage(0, 0, chalkboard);
        p.setPen(Qt::white);
        p.drawRect(28, 34, 285, 80);
        p.drawText(70, 88, "Main Menu");
        for(int i = 0; i<instructionLines.size(); i++) {
            QString Qtxt = QString::fromStdString(instructionLines[i]);
            p.drawText(30, i*48+170, Qtxt);
        }
    }
}

void MainWindow::enemyShoot(enemy en) {
    enemy_shot shot;
    shot.x = en.x+en.width/2;
    shot.y = en.y+en.height;
    shot.sho_img = &EnemyShot;
    shot.width = EnemyShot.width();
    shot.height = EnemyShot.height();
    enshots.push_back(shot);
} 

void MainWindow::Rainbow() {
    Rain += 2;
    if(Rain>=360) {
        Rain = Rain%360;
    }
}

void MainWindow::makeLevel() {
    enshots.clear();

    for(int i = 0; i<11; i++) {
        enemies.push_back(makeEnemy(&Khadka, &Khadka_rev, i*70+70, 100, 60));
        enemies.push_back(makeEnemy(&Sharma, &Sharma_rev, i*70+70, 160, 50));
        enemies.push_back(makeEnemy(&Mitofsky, &Mitofsky_rev, i*70+70, 220, 40));
        enemies.push_back(makeEnemy(&Carroll, &Carroll_rev, i*70+70, 280, 30));
        enemies.push_back(makeEnemy(&Carr, &Carr_rev, i*70+70, 340, 20));
        enemies.push_back(makeEnemy(&Woolverton, &Woolverton_rev, i*70+70, 400, 10));
    }
    level++;
    movement = 50/2-level*6;
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
    en.height = en.en->height();
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
            if(enemies[i].y+enemies[i].height>=gameheight-200) {
                gameover = true;
                music.stop();

            } else if(movement>5) {
                movement -=1;
            }
        }
        leftdir = !leftdir;
        enemiesDown = false;

    } else {
        for(int i = 0; i<enemies.size(); i++) {
            if(leftdir) {
                enemies[i].x -=9;
            } else {
                enemies[i].x+=9;
            }
            if((enemies[i].x<=leftboundry &&leftdir) || (enemies[i].x+enemies[i].height>=rightboundry&&!leftdir)) {
                enemiesDown = true;
            }
        }
    }
    if(enemywaittime==-1) {
        special_en.x += 10;
        if(special_en.x>=gamewidth) {
            enemywaittime=0;
        }
    }
}

void MainWindow::moveShoot() {
    //Move Player Shots and check Collisiosns
   for(int i = 0; i<shots.size(); i++) {
       shots[i].y -=10;
       if(shots[i].y+shots[i].height<0) {
           shots.erase(shots.begin()+i);
           i--;
       } else if(enemywaittime==-1 && checkCollision(&special_en, &shots[i])) {
           score += special_en.pointval;
           enemywaittime =0;
           shots.erase(shots.begin()+i);
           i--;
       } else {
           bool check = false;
           for(int j=0; j<enemies.size(); j++) {
               if(checkCollision(&enemies[j], &shots[i])) {
                    score += enemies[j].pointval;
                    enemies.erase(enemies.begin()+j);
                    shots.erase(shots.begin()+i);
                    i--;
                    check = true;
                    if(enemyHit.isFinished()) {
                        enemyHit.play();
                    } else {
                        enemyHit.play();
                    }
                    break;
               }
           }
           for(int j = 0; j<barriers.size() && !check; j++) {
               if(barriers[j].x<=shots[i].x+shots[i].width && barriers[j].x+barriers[j].width>shots[i].x) {
                   //Partially destroy barrier
                   //If the bullet is in contact with the barrier
                   if(checkBarCollision(&shots[i], &barriers[j])) {
                       shots.erase(shots.begin()+i);
                       i--;
                       break;
                   }
               }
           }
       }
   }
    //Move Enemy Shoots and check collions
    //Need to add Collision detection for barriers.
   for(int i = 0; i<enshots.size(); i++) {
       enshots[i].y +=5;
        if(enshots[i].y>gameheight) {
            enshots.erase(enshots.begin()+i);
            i--;
        } else if(checkEnCollision(&enshots[i]) && invincible == -1) {
            lives--;
            int rando = rand()%4;
            invincible ++;
            if(rando==1) {
                if(Hit1.isFinished()) {
                    Hit1.play();
                } else {
                    Hit1.play();
                }
            } else if(rando==2) {
                if(Hit2.isFinished()) {
                    Hit2.play();
                } else {
                    Hit2.play();
                }
            } else if(rando==3) {
                if(Hit3.isFinished()) {
                    Hit3.play();
                } else {
                    Hit3.play();
                }
            } else {
                if(Hit4.isFinished()) {
                    Hit4.play();
                } else {
                    Hit4.play();
                }
            }
            enshots.erase(enshots.begin()+i);
            i--;
        } else {
            for(int j = 0; j<barriers.size(); j++) {
                if(barriers[j].x<=enshots[i].x+enshots[i].width && barriers[j].x+barriers[j].width>enshots[i].x) {
                    //Partially destroy barrier
                    //If the bullet is in contact with the barrier
                    if(checkBarCollision(&enshots[i], &barriers[j])) {
                        enshots.erase(enshots.begin()+i);
                        i--;
                        break;
                    }
                }
            }
        }
   }
}

bool MainWindow::checkCollision(enemy *en, shot *sh) {
    if(en->y+en->height>=sh->y+3 && sh->y+sh->height>=en->y+3) {
        return en->x+en->width>=sh->x+3 && sh->x+sh->width>=en->x+3;
    }
    return false;
}

bool MainWindow::checkEnCollision(enemy_shot *enshot) {
    if(player_y+player_height>=enshot->y+10 && enshot->y+enshot->height>=player_y+10) {
        return player_x+player_width>=enshot->x+15 && enshot->x+enshot->width>=player_x+15;
    }
    return false;
}

bool MainWindow::checkBarCollision(enemy_shot *enshot, barrier *barr) {
    for(int i =0; i<barr->hits.size(); i++) {
        if(enshot->x+enshot->width/2>=barr->x+(barr->width/barr->hits.size())*i && enshot->x+enshot->width/2<=barr->x+(barr->width/barr->hits.size())*(i+1)) {
            for(int j = 0; j<barr->hits[i].size(); j++) {
                if(enshot->y+enshot->height>barr->y+(barr->height/barr->hits[i].size())*j && !barr->hits[i][j]) {
                    barr->hits[i][j] = true;
                    return true;
                }
            }
        } else if(i==0 && enshot->x+enshot->width>barr->x && enshot->x<barr->x) {
            for(int j = 0; j<barr->hits[i].size(); j++) {
                if(enshot->y+enshot->height>barr->y+(barr->height/barr->hits[i].size())*j && !barr->hits[i][j]) {
                    barr->hits[i][j] = true;
                    return true;
                }
            }
        } else if(i==barr->hits.size()-1 && enshot->x+enshot->width>barr->x+barr->width && enshot->x>barr->x+(barr->width/barr->hits.size())*(i-1)) {
            for(int j = 0; j<barr->hits[i].size(); j++) {
                if(enshot->y+enshot->height>barr->y+(barr->height/barr->hits[i].size())*j && !barr->hits[i][j]) {
                    barr->hits[i][j] = true;
                    return true;
                }
            }
        }
    }
    return false;
}

bool MainWindow::checkBarCollision(shot *shot, barrier *barr) {
    for(int i =0; i<barr->hits.size(); i++) {
        if(shot->x+shot->width/2>=barr->x+(barr->width/barr->hits.size())*i && shot->x+shot->width/2<=barr->x+(barr->width/barr->hits.size())*(i+1)) {
            for(int j = 0; j<barr->hits[i].size(); j++) {
                if(shot->y<barr->y+(barr->height/barr->hits[i].size())*j && !barr->hits[i][j]) {
                    barr->hits[i][j] = true;
                    return true;
                }
            }
        } else if(i==0 && shot->x+shot->width>barr->x && shot->x<barr->x) {
            for(int j = 0; j<barr->hits[i].size(); j++) {
                if(shot->y<barr->y+(barr->height/barr->hits[i].size())*j && !barr->hits[i][j]) {
                    barr->hits[i][j] = true;
                    return true;
                }
            }
        } else if(i==barr->hits.size()-1 && shot->x+shot->width>barr->x+barr->width && shot->x>barr->x+(barr->width/barr->hits.size())*(i-1)) {
            for(int j = 0; j<barr->hits[i].size(); j++) {
                if(shot->y<barr->y+(barr->height/barr->hits[i].size())*j && !barr->hits[i][j]) {
                    barr->hits[i][j] = true;
                    return true;
                }
            }
        }
    }
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    keys[event->key()] = true;
    int key = event->key();
    if(start && !gameover) {
        if(key==Qt::Key_P && ingame) {
            pause = !pause;
            if(pause) {
                music.stop();
            } else {
                music.play();
            }
        }
    } else if(ingame && gameover && naming && !donenaming && name.length()<8 && event->modifiers()!=Qt::ShiftModifier) {
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
    } else if(ingame && gameover && naming && !donenaming && name.length()<8 && event->modifiers()==Qt::ShiftModifier) {
        switch(key) {
        case Qt::Key_A: {
           name +="A";
           break;
        }
        case Qt::Key_B: {
           name +="B";
           break;
        }
        case Qt::Key_C: {
           name +="C";
           break;
        }
        case Qt::Key_D: {
           name +="D";
           break;
        }
        case Qt::Key_E: {
           name +="E";
           break;
        }
        case Qt::Key_F: {
           name +="F";
           break;
        }
        case Qt::Key_G: {
           name +="G";
           break;
        }
        case Qt::Key_H: {
           name +="H";
           break;
        }
        case Qt::Key_I: {
           name +="I";
           break;
        }
        case Qt::Key_J: {
           name +="J";
           break;
        }
        case Qt::Key_K: {
           name +="K";
           break;
        }
        case Qt::Key_L: {
           name +="L";
           break;
        }
        case Qt::Key_M: {
           name +="M";
           break;
        }
        case Qt::Key_N: {
           name +="N";
           break;
        }
        case Qt::Key_O: {
           name +="O";
           break;
        }
        case Qt::Key_P: {
           name +="P";
           break;
        }
        case Qt::Key_Q: {
           name +="Q";
           break;
        }
        case Qt::Key_R: {
           name +="R";
           break;
        }
        case Qt::Key_S: {
           name +="S";
           break;
        }
        case Qt::Key_T: {
           name +="T";
           break;
        }
        case Qt::Key_U: {
           name +="u";
           break;
        }
        case Qt::Key_V: {
           name +="V";
           break;
        }
        case Qt::Key_W: {
           name +="W";
           break;
        }
        case Qt::Key_X: {
           name +="X";
           break;
        }
        case Qt::Key_Y: {
           name +="Y";
           break;
        }
        case Qt::Key_Z: {
           name +="Z";
           break;
        }
        }
    }

    if(naming && gameover && key==Qt::Key_Space && !donenaming && name.length()>0) {
        naming = false;
        donenaming = true;
        setHighScores();
        score = 0;
    } else if(naming && gameover && key==Qt::Key_Backspace && !donenaming && name.length()>0) {
        name = name.substr(0, name.length()-1);
    }
    if(!naming && gameover) {
    }

    if(key==Qt::Key_Space) {
        //shoot
        if(ingame && !start) {
            start=true;
            music.play();
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    keys[e->key()] = false;
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
   int mousey = e->y();
   int mousex= e->x();
   if(mainScreen-1>=characters*2 && main) {
       if(e->buttons()==Qt::LeftButton && mousey>=9*40+30 && mousey<=9*40+110) {
           setFixedSize(gamewidth, gameheight);
           ingame = true;
           level = 0;
           makeLevel();
           start = false;
           gameover = false;
           gameOverScreen = 0;
           player_x = 250;
           lives = 3;
           player_y = gameheight-100;
           barriers.clear();
           makeBarriers();
           score = 0;
           donenaming = false;
           naming = false;
           leftdir = false;
           main = false;
           name ="";
           moveFrame = movement;
       } else if(e->buttons()==Qt::LeftButton && mousey>=11*40+30 && mousey<=11*40+110) {
           main = false;
           instructions = true;
           setFixedSize(1100, 800);
       } else if (e->buttons()==Qt::LeftButton && mousey>=13*40+30 && mousey<=13*40+110) {
           ingame = true;
           gameover = true;
           gameOverScreen = 100;
           main = false;
       } else if(e->buttons()==Qt::LeftButton && mousey>=15*40+30 && mousey<=15*40+110) {
           qApp->exit(0);
       }
   } else if (e->buttons()==Qt::LeftButton && instructions && mousey>=34 && mousey<=85+34 && mousex>=28 && mousex<=285+28) {
       MainMenu();
   } else if(e->buttons()==Qt::LeftButton && gameover && !naming && mousey>=34 && mousey<=85+34 && mousex>=28 && mousex<=285+28) {
       MainMenu();
   }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
   int mousey = e->y();
   if(mousey>=11*40+30 && mousey<11*40+110 && selected!=12) {
       selected = 12;
       Rain = QColor(0, 150, 0).hue();
    } else if(mousey>=13*40+30 && mousey<13*40+110 && selected!=14) {
        selected = 14;
        Rain = QColor(0, 150, 0).hue();
    } else if(mousey>=15*40+30 && mousey<15*40+110&& selected!=16) {
        selected = 16;
        Rain = QColor(0, 150, 0).hue();
    } if(mousey>=9*40+30 && mousey<9*40+110&& selected!=10) {
        selected = 10;
        Rain = QColor(0, 150, 0).hue();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MainMenu() {
    ingame = false;
    start = false;
    gameover = false;
    enemies.clear();
    shots.clear();
    enshots.clear();
    enemiesDown = false;
    instructions = false;
    main = true;
    mainScreen = characters*2+1;
    setFixedSize(start_width, start_height);
}
