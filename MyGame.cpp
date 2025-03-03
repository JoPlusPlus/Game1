#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
#include "MainMenu.h"
#include<thread>
#include<chrono>

using namespace std;
using namespace sf;

// -----------------------------------------
//    -1 => Exit
//     0 => play (0 => pName) / continue
//     1 => settings
//     3 => GamePlay
//     4 => GameOver
//  1000 => MainMenu
//
//
//
//
//
//
SoundBuffer healthbuff;
Sound healthsound;
SoundBuffer gow;
Sound gows;
SoundBuffer bfOver;
Sound SOver;
SoundBuffer bfCutscene;
Sound sCutscene;
int y = 0;
bool cutScene = 0;

// global 

RenderWindow window(VideoMode(1920, 1080), "SFML works!");
Texture tback;
Sprite sback;


// menu handling
MainMenu this_mainmenu;
int pagenum = 1000;


//pausemenu handling
PauseMenu this_pausemenu;
bool entered_pausemenu = false;


// overmenu handling
GameOverMenu thisGameOverMenu;
string PlayerName;
// Drops
Texture tDrops[4];
Sprite sDrops[4];

Font playerFont;

Text playerHealthText;
String playerHealthString;

Text ScoreText;
String ScoreString;

Text EkilledText;
String EkilledString;

Text ProtectionText;
String ProtectionString;

int highestScore = -1;
// _______________________________________________________________________________________________________________

struct Enemy {
    Sprite sEnemy;
    int speed;
    RectangleShape enemyRealcollider;

};

struct Drop {
    Sprite sDropshape;
    RectangleShape dropCollider, targetCollider;
    int type;
   
};

struct Bullet {
    int speed = 10;
    int coolDown = 3;
    int magazine = 3;
    int move_to_right = -1;
    CircleShape bulletCircle;

};

class Player {
public:
    Sprite Ninja;

    bool Protected = 0;
    float  ProtectionCooldown = 0;

    float speed = 360.0f;       // Movement speed
    bool facingRight = true;    // Character direction
    bool onground = 1;
    bool moving = false;
    bool shooting = 0;
    float gravity = 1000.0f;    // Gravity
    float verticalVelocity = 0.0f; // Vertical velocity for jumping
    float groundHeight = 1000.0f;   // Ground level
    const int jumpSpeed = -580.0f; // Jump strength
    RectangleShape playerRealcollider;
    CircleShape playerProtection;
    int health = 5;
    int score = 0;
    bool bulletShooting = 0;
    int bulletIndex = -1;
    int bulletCooldown;
    vector<Bullet> bullets;

    int enemiesKilled = 0;
    Clock AnimRight, AnimLeft, AnimIdle;
    float time = 0;

    // Animation frame counters
    int WidthFrame_shoting = 0;
    int WidthFrame_idle = 0;
    int widthFrameRun = 0;

    // Textures
    Texture idleTex;
    Texture shootingUpTex;
    Texture RunTex;



    Player() {
        // Load textures
        if (!idleTex.loadFromFile("Chosen/Soldier_1/Idle.png")) {
            cerr << "Error loading idle texture" << endl;
        }
        if (!shootingUpTex.loadFromFile("Chosen/Soldier_1/Shot_2.png")) {
            cerr << "Error loading shooting texture" << endl;
        }
        if (!RunTex.loadFromFile("Chosen/Soldier_1/Run.png")) {
            cerr << "Error loading running texture" << endl;
        }
        Ninja.setTexture(idleTex);
        Ninja.setOrigin(128 / 2, 128 / 2);
        Ninja.setPosition(500, groundHeight);



        playerRealcollider.setSize(Vector2f(40, 70));
        playerRealcollider.setFillColor(Color::Green);

        playerRealcollider.setPosition(Ninja.getPosition());
        playerRealcollider.setOrigin(Ninja.getOrigin());

        playerProtection.setRadius(35);
        playerProtection.setFillColor(Color::White);
        playerProtection.setPosition(Ninja.getPosition());
        playerProtection.setOrigin(playerProtection.getGlobalBounds().width / 2, playerProtection.getGlobalBounds().height / 2);

    }

    void move(float deltaTime) {
        
        moving = false;
        shooting = 0;
        // Horizontal movement
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            Ninja.move(speed * deltaTime, 0);
            facingRight = true; // Facing right
            moving = true;



        }
        else if (Keyboard::isKeyPressed(Keyboard::A)) {
            Ninja.move(-speed * deltaTime, 0);
            facingRight = false; // Facing left
            moving = true;



        }
        



        // Jumping
        if (Keyboard::isKeyPressed(Keyboard::W) && onground) {
            verticalVelocity = jumpSpeed;
            onground = 0;
        }

        // Apply gravity

        //if (!onground) {
        verticalVelocity += gravity * deltaTime;
        Ninja.move(0, verticalVelocity * deltaTime);
        //}




        // Prevent falling through the ground
        if (Ninja.getPosition().y >= groundHeight) {
            Ninja.setPosition(Ninja.getPosition().x, groundHeight);
            verticalVelocity = 0.0f;
            onground = 1;
        }

        // prevent passing through screen edges
        for (int i = 0; i < 5; i++) {
            if (Ninja.getPosition().x > 1900) Ninja.setPosition(1900, Ninja.getPosition().y);
            if (Ninja.getPosition().x < 10) Ninja.setPosition(10, Ninja.getPosition().y);
        }



        // Shooting 

        // Idle animation


        // Flip the sprite based on facing direction
        Ninja.setScale(facingRight ? 1.0f : -1.0f, 1.0f);

        playerRealcollider.setPosition(Ninja.getPosition().x + 45, Ninja.getPosition().y + 60);
        playerRealcollider.setOrigin(Ninja.getOrigin());

        playerProtection.setPosition(Ninja.getPosition().x + 25 , Ninja.getPosition().y + 60);
        playerProtection.setOrigin(Ninja.getOrigin());
    }

    void PlayerAnimation() {
        if (moving && facingRight) {
            if (AnimRight.getElapsedTime().asSeconds() >= 0.03) {
                Ninja.setTexture(RunTex);
                Ninja.setTextureRect(IntRect(widthFrameRun * 128, 0, 128, 128));
                widthFrameRun = (widthFrameRun + 1) % 8;
                AnimRight.restart();
            }
        }
        if (moving && !facingRight) {
            if (AnimLeft.getElapsedTime().asSeconds() >= 0.03) {
                Ninja.setTexture(RunTex);
                Ninja.setTextureRect(IntRect(widthFrameRun * 128, 0, 128, 128));
                widthFrameRun = (widthFrameRun + 1) % 8;
                AnimLeft.restart();
            }
        }
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Ninja.setTexture(shootingUpTex);
            Ninja.setTextureRect(IntRect(WidthFrame_shoting * 128, 0, 128, 128));
            WidthFrame_shoting = (WidthFrame_shoting + 1) % 4;
            shooting = 1;
        }
        else if (!moving) {

            if (AnimIdle.getElapsedTime().asSeconds() >= 0.1) {
                Ninja.setTexture(idleTex);
                Ninja.setTextureRect(IntRect(WidthFrame_idle * 128, 0, 128, 128));
                WidthFrame_idle = (WidthFrame_idle + 1) % 7;
                AnimIdle.restart();
            }

        }
    }
};

vector<Drop> dropBag;

// Functions Declration
void Pname();

void setStatus(Player& player);
void updateStatus(Player& player);
void displayStatus();

void GamePlay();
void pauseMenu();
void blocks_fun(Sprite blocks[], RectangleShape blocks_Real_collider[]);
void GameOver();
void SetDrops();
void ChooseDrop(RectangleShape blocks_real_collider[], Clock& Addtimer, Clock& DeleteTimer);
void DropADrop(float dt, Player& player);
void bulletCooldown(Player& player);
void Bulletmovement(vector<Bullet> &bullets);
void LeaderBoard();
void Cutscene(Player& player, Sprite blocks[], Enemy enemies[]);
void BackGroundEnemies(Player& player);
void AnimateBackgroundEnemies(Enemy Enemies, Texture tbackgroundenemies);
void LevelTwo(Player& player, Sprite Sprites[]);


int main() {
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) {
                    this_mainmenu.move_up();
                }
                if (event.key.code == Keyboard::Down) {
                    this_mainmenu.move_down();
                }
                if (event.key.code == Keyboard::Enter) {
                    if (this_mainmenu.pressed() == 0) {
                        Pname();
                    }
                    if (this_mainmenu.pressed() == 1) {
                        
                    }
                    if (this_mainmenu.pressed() == 2) {
                        window.close();
                    }
                }
            }
            window.clear();
            this_mainmenu.draw(window);
            window.display();

        }


    }

    return 0;
}


void Pname() {
    

    if (!PlayerName.empty()) {
        PlayerName.clear();
    }

    Font player_name_font;
    Font system_font;
    player_name_font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");
    system_font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");
    Text t1, t2;
    t1.setString("Enter your name");
    t1.setFont(system_font), t2.setFont(player_name_font);
    t1.setCharacterSize(70), t2.setCharacterSize(70);
    t1.setFillColor(Color(128, 128, 128)), t2.setFillColor(Color::White);
    t1.setPosition(250, 100), t2.setPosition(250, 300);



    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                break;
            }
            else if (Keyboard::isKeyPressed(Keyboard::BackSpace) && PlayerName.size() > 0) {
                PlayerName.resize(PlayerName.size() - 1);
            }
            else if ((Keyboard::isKeyPressed(Keyboard::Enter)) && (PlayerName.size() >= 1)) {
                GamePlay();
            }
            else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                return;
            }
            else if (event.type == Event::TextEntered) {
                PlayerName += static_cast<char>(event.text.unicode);
            }

        }
        t2.setString(PlayerName);
        window.clear();
        window.draw(t1);
        window.draw(t2);
        window.display();

    }


}


void GamePlay() {


    tback.loadFromFile("Chosen/Backgrounds/castle.png");
    sback.setTexture(tback);
    sback.setScale(0.5, 0.5);

    Clock clock;

    

    Player player;

    setStatus(player);

    gow.loadFromFile("Chosen/Sound/Deliverance _ God of War OST(MP3_320K).mp3");
    gows.setBuffer(gow);
    bfOver.loadFromFile("Chosen/Sound/DeathMenu/Wardruna - Einar Selvik - Snake Pit Poetry  [FULL EP](MP3_320K)-[AudioTrimmer.com]-[AudioTrimmer.com].mp3");
    SOver.setBuffer(bfOver);
    gows.play();

    // loading enemy tex

    vector <Texture> tEnemy;
    Texture t;
    for (int i = 1; i <= 6; i++) {
        if (!t.loadFromFile("Chosen/enemies/PNG/lizard/Walk" + to_string(i) + ".png")) {
            cerr << "Failed to load " << i;
        }
        else {
            tEnemy.push_back(t);
        }
    }

    Enemy enemies[6];



    for (int i = 0; i < 6; i++) {
        enemies[i].sEnemy.setTexture(tEnemy[i]);
        enemies[i].speed = rand() % (8) + 8;
        int y = rand() % (744) + 80;

        enemies[i].sEnemy.setOrigin(enemies[i].sEnemy.getGlobalBounds().width / 2, enemies[i].sEnemy.getGlobalBounds().height / 2);

        enemies[i].enemyRealcollider.setSize(Vector2f(90, 60));
        enemies[i].enemyRealcollider.setFillColor(Color::Cyan);


        enemies[i].enemyRealcollider.setPosition(enemies[i].sEnemy.getPosition());
        enemies[i].enemyRealcollider.setOrigin(enemies[i].enemyRealcollider.getGlobalBounds().width / 2, enemies[i].enemyRealcollider.getGlobalBounds().height / 2);



        if (i < 3) {
            enemies[i].sEnemy.setPosition(0, y);
            enemies[i].sEnemy.setTextureRect(IntRect(0, 0, 256, 256));
        }
        else {
            enemies[i].sEnemy.setPosition(1900, y);
            enemies[i].sEnemy.setTextureRect(IntRect(0, 0, 256, 256));
        }
    }


    // load blocks tex

    Sprite blocks[15];
    RectangleShape blocks_real_collider[15];
    Texture blockTexture;
    blockTexture.loadFromFile("Chosen/objects/blocks/pngtree-game-ground-asset-seamless-level-png-image_6373714.png");

    blocks_fun(blocks, blocks_real_collider);

    for (int i = 0; i < 13; i++) {
        blocks[i].setTexture(blockTexture);
        blocks[i].setTextureRect(IntRect(22, 255, 290, 100));
    }

    // RectangleShape collider(Vector2f(290, 44)), collider2(Vector2f(145,45));

    // Drops
    Clock addTimer;
    Clock deleteTimer;
    dropBag.clear();
    SetDrops();

    // sounds






    while (window.isOpen()) {

        window.setFramerateLimit(60);
        float deltaTime = clock.restart().asSeconds();

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
                break;
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    gows.pause();
                    pauseMenu();
                }
            }
            
        }
        //if (Mouse::isButtonPressed(Mouse::Right)) {
        //    Vector2i mousePos = Mouse::getPosition(window);
        //    cout << "X: " << mousePos.x << " Y: " << mousePos.y << endl;
        //}
        if (Mouse::isButtonPressed(Mouse::Left) && player.bulletIndex >= 0 && player.bulletShooting) {

            player.bullets[player.bulletIndex].bulletCircle.setPosition(player.Ninja.getPosition());
            if (player.facingRight) player.bullets[player.bulletIndex].move_to_right = 1;
            else player.bullets[player.bulletIndex].move_to_right = 0;
            player.bulletCooldown = player.bullets[player.bulletIndex].coolDown;
            player.bulletIndex--;
            player.bulletShooting = 0;

        }        


        bulletCooldown(player);
        Bulletmovement(player.bullets);
        player.move(deltaTime);
        player.PlayerAnimation();



        // enemy move, handle collision between enemy & player

        for (int i = 0; i < 6; i++) {

            if (i < 3) {
                enemies[i].sEnemy.move(enemies[i].speed, 0);
                if (enemies[i].sEnemy.getPosition().x > 1900) {
                    int y = rand() % (921) + 80;
                    enemies[i].sEnemy.setPosition(-200, y);
                    enemies[i].speed = rand() % (8) + 8;
                }
                // enemy hit player
                
                if (!player.Protected) {
                    if (player.playerRealcollider.getGlobalBounds().intersects(enemies[i].enemyRealcollider.getGlobalBounds())) {
                        int y = rand() % (921) + 80;
                        enemies[i].sEnemy.setPosition(0, y);
                        enemies[i].speed = rand() % (8) + 8;
                        player.health--;
                    }
                }

                // player shoot enemy
                for (int j = 0; j < player.bullets.size(); j++) {
                    if (player.bullets[j].bulletCircle.getGlobalBounds().intersects(enemies[i].enemyRealcollider.getGlobalBounds())) {
                        int y = rand() % (921) + 80;
                        enemies[i].sEnemy.setPosition(0, y);
                        enemies[i].speed = rand() % (8) + 8;
                        player.enemiesKilled++;
                        player.score++;
                    }
                }

                enemies[i].enemyRealcollider.setOrigin(enemies[i].sEnemy.getOrigin().x - 80, enemies[i].sEnemy.getOrigin().y - 100);
            }
            else {
                enemies[i].sEnemy.setScale(-1, 1);
                enemies[i].sEnemy.move(-enemies[i].speed, 0);
                if (enemies[i].sEnemy.getPosition().x < 0) {
                    int y = rand() % (744) + 80;
                    enemies[i].sEnemy.setPosition(1900, y);
                    enemies[i].speed = rand() % (8) + 8;
                }
                // enemy hit player
                if (!player.Protected) {
                    if (player.playerRealcollider.getGlobalBounds().intersects(enemies[i].enemyRealcollider.getGlobalBounds())) {
                        int y = rand() % (921) + 80;
                        enemies[i].sEnemy.setPosition(1900, y);
                        enemies[i].speed = rand() % (8) + 8;
                        player.health--;
                }

                }
                // player shoot enemy
                for (int j = 0; j < player.bullets.size(); j++) {
                    if (player.bullets[j].bulletCircle.getGlobalBounds().intersects(enemies[i].enemyRealcollider.getGlobalBounds())) {
                        int y = rand() % (921) + 80;
                        enemies[i].sEnemy.setPosition(1900, y);
                        enemies[i].speed = rand() % (8) + 8;
                        player.enemiesKilled++;
                        player.score++;
                    }
                }
                enemies[i].enemyRealcollider.setOrigin(enemies[i].sEnemy.getOrigin().x - 90, enemies[i].sEnemy.getOrigin().y - 100);
            }

            enemies[i].enemyRealcollider.setPosition(enemies[i].sEnemy.getPosition());

        }
        
        

        // handle collision between player & blocks

        for (int i = 0; i < 13; i++) {
            if (player.playerRealcollider.getGlobalBounds().intersects(blocks_real_collider[i].getGlobalBounds())) {



                if (i != 8 && i != 9 && i != 11) {

                    if (player.playerRealcollider.getGlobalBounds().left <= blocks_real_collider[i].getPosition().x && Keyboard::isKeyPressed(Keyboard::D)) { // left collision
                        player.Ninja.setPosition(blocks_real_collider[i].getPosition().x, player.Ninja.getPosition().y);
                    }
                    else if (player.playerRealcollider.getGlobalBounds().left >= blocks_real_collider[i].getPosition().x + 240 && Keyboard::isKeyPressed(Keyboard::A)) {
                        player.Ninja.setPosition(blocks_real_collider[i].getPosition().x + 290, player.Ninja.getPosition().y);
                    }
                    else {
                        if (player.playerRealcollider.getGlobalBounds().top >= blocks_real_collider[i].getPosition().y) {
                            player.verticalVelocity = 0.0f;


                            player.Ninja.setPosition(player.Ninja.getPosition().x, blocks_real_collider[i].getPosition().y + 70);
                        }
                        else if (player.playerRealcollider.getGlobalBounds().top + 70 >= blocks_real_collider[i].getPosition().y && player.playerRealcollider.getGlobalBounds().left >= blocks_real_collider[i].getPosition().x) {
                            player.Ninja.setPosition(player.Ninja.getPosition().x, blocks_real_collider[i].getPosition().y - 70);
                            player.onground = 1;
                            player.verticalVelocity -= player.gravity * deltaTime;
                            player.playerProtection.setPosition(player.Ninja.getPosition().x + 25, player.Ninja.getPosition().y + 60);
                            

                        }
                    }
                    if (player.playerRealcollider.getGlobalBounds().left + 20 <= blocks_real_collider[i].getPosition().x) {
                        player.onground = 0;
                    }

                }
                else {

                    if (player.playerRealcollider.getGlobalBounds().left <= blocks_real_collider[i].getPosition().x && Keyboard::isKeyPressed(Keyboard::D)) { // left collision
                        player.Ninja.setPosition(blocks_real_collider[i].getPosition().x, player.Ninja.getPosition().y);
                        player.playerRealcollider.setPosition(blocks_real_collider[i].getPosition().x, player.Ninja.getPosition().y);
                    }
                    else if (player.playerRealcollider.getGlobalBounds().left >= blocks_real_collider[i].getPosition().x + 120 && Keyboard::isKeyPressed(Keyboard::A)) {
                        player.Ninja.setPosition(blocks_real_collider[i].getPosition().x + 145, player.Ninja.getPosition().y);
                        player.playerRealcollider.setPosition(blocks_real_collider[i].getPosition().x + 145, player.Ninja.getPosition().y);
                    }
                    else {
                        if (player.playerRealcollider.getGlobalBounds().top >= blocks_real_collider[i].getPosition().y) {
                            player.verticalVelocity = 0.0f;


                            player.Ninja.setPosition(player.Ninja.getPosition().x, blocks_real_collider[i].getPosition().y + 70);
                            player.playerRealcollider.setPosition(player.Ninja.getPosition().x, blocks_real_collider[i].getPosition().y + 70);
                        }
                        else if (player.playerRealcollider.getGlobalBounds().top + 70 >= blocks_real_collider[i].getPosition().y && player.playerRealcollider.getGlobalBounds().left >= blocks_real_collider[i].getPosition().x) {
                            player.Ninja.setPosition(player.Ninja.getPosition().x, blocks_real_collider[i].getPosition().y - 70);
                            player.playerRealcollider.setPosition(player.Ninja.getPosition().x, blocks_real_collider[i].getPosition().y - 70);
                            player.onground = 1;
                            player.verticalVelocity -= player.gravity * deltaTime;
                            player.playerProtection.setPosition(player.Ninja.getPosition().x + 25, player.Ninja.getPosition().y + 60);

                        }
                    }


                }
            }

        }

        // drops updating
        ChooseDrop(blocks_real_collider, addTimer, deleteTimer);
        DropADrop(deltaTime, player);
        player.playerRealcollider.setPosition(player.Ninja.getPosition().x + 45, player.Ninja.getPosition().y + 60);
        player.playerRealcollider.setOrigin(player.Ninja.getOrigin());



        
        updateStatus(player);
        

        if (player.health <= 0) {

            

            ofstream offile;
            offile.open("LeaderBoard.txt", ios::app);
            if (player.score > highestScore) {
                highestScore = player.score;
                offile << PlayerName << "          " << player.score << '*' << endl;
                
            }
            

           gows.stop();
           SOver.play();
           
           this_thread::sleep_for(chrono::milliseconds(2900));
            
            GameOver();
        }


        if (player.score >= 5) {
            bfCutscene.loadFromFile("Chosen/Sound/Cutscene/[LIVE] God of War (2023) - Hollywood Bowl(MP3_320K)-[AudioTrimmer.com].mp3");
            sCutscene.setBuffer(bfCutscene);
            sCutscene.play();
            Cutscene(player, blocks, enemies);
        }
        

        window.clear();
        window.draw(sback);
        for (int i = 0; i < 13; i++) {

            //window.draw(blocks_real_collider[i]);
            window.draw(blocks[i]);
        }
        for (int i = 0; i < dropBag.size(); i++) {
            //window.draw(dropBag[i].dropCollider);
            window.draw(dropBag[i].sDropshape);
        }

        for (int i = 0; i < 6; i++) {
            //window.draw(enemies[i].enemyRealcollider);
            window.draw(enemies[i].sEnemy);

        }
        for (int i = 0; i < player.bullets.size(); i++) {
           window.draw(player.bullets[i].bulletCircle);
        }
        if (player.Protected) window.draw(player.playerProtection);
        
        //window.draw(player.playerRealcollider);
        window.draw(player.Ninja);
        displayStatus();

        window.display();
    }

}

void setStatus(Player& player) {
    //health

    playerFont.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");

    playerHealthText.setFont(playerFont);

    String playerHealthString = to_string(player.health);
    playerHealthText.setString("Health : " + playerHealthString);

    playerHealthText.setCharacterSize(40);
    playerHealthText.setFillColor(Color::Green);

    // EnemiesKilled

    EkilledText.setFont(playerFont);

    String EkilledString = to_string(player.enemiesKilled);
    EkilledText.setString("Enemies killed : " + EkilledString);
    EkilledText.setCharacterSize(40);
    EkilledText.setPosition(0, 75);
    EkilledText.setFillColor(Color::White);

    //Score

    ScoreText.setFont(playerFont);

    String ScoreString = to_string(player.score);

    ScoreText.setPosition(0, 150);
    ScoreText.setString("Score : " + ScoreString);
    ScoreText.setCharacterSize(40);
    ScoreText.setFillColor(Color::White);

    

    ProtectionText.setFont(playerFont);
    ProtectionString = to_string(player.ProtectionCooldown);
    ProtectionText.setPosition(0, 225);
    ProtectionText.setString("Sheild cooldown : " + ProtectionString);
    ProtectionText.setCharacterSize(40);
    ProtectionText.setFillColor(Color::White);





}

void updateStatus(Player& player) {
    // health and score updates on screen
    playerHealthString = to_string(player.health);
    playerHealthText.setString("Health : " + playerHealthString);

    EkilledString = to_string(player.enemiesKilled);
    EkilledText.setString("Enemies killed : " + EkilledString);

    ScoreString = to_string(player.score);
    ScoreText.setString("Score : " + ScoreString);

    ProtectionString = to_string(player.ProtectionCooldown);
    ProtectionText.setString("Sheild cooldown : " + ProtectionString);

}

void displayStatus() {
    window.draw(playerHealthText);
    window.draw(EkilledText);
    window.draw(ScoreText);
    //window.draw(ProtectionText);
}

void pauseMenu() {

    Texture tbackground;
    tbackground.loadFromFile("Chosen/Backgrounds/origbig.png");
    Sprite sbackground(tbackground);

    while (window.isOpen()) {

        Event e;
        while (window.pollEvent(e)) {

            if (e.type == Event::Closed) {
                window.close();
                break;
            }

            if (e.type == Event::KeyPressed) {

                if (e.key.code == Keyboard::Escape) {
                    gows.play();
                    return;
                }
                if (e.key.code == Keyboard::Up) {
                    this_pausemenu.move_up();
                }
                if (e.key.code == Keyboard::Down) {
                    this_pausemenu.move_down();
                }
                if (e.key.code == Keyboard::Enter) {
                    int selectedItem = this_pausemenu.pressed();
                    if (selectedItem == 0) { // Resume
                        gows.play();
                        return;
                    }
                    else if (selectedItem == 1) { // Options (example)
                        pagenum = 1;
                    }
                    else if (selectedItem == 2) { // Exit to main menu
                        pagenum = 1000;
                        this_pausemenu.set_selected(0);
                        main();
                    }
                }
            }

        }

        window.clear();

        // Draw pause menu
        window.draw(sbackground);
        this_pausemenu.draw(window);

        window.display();


    }
}

void blocks_fun(Sprite blocks[], RectangleShape blocks_Real_collider[]) {

    int height = 23;

    blocks[0].setPosition(1400, 640);
    blocks[0].setScale(1, 0.5);
    blocks_Real_collider[0].setPosition(blocks[0].getPosition().x, blocks[0].getPosition().y + 20);
    blocks_Real_collider[0].setSize(Vector2f(290, height));

    blocks[1].setPosition(1100, 500);
    blocks[1].setScale(1, 0.5);
    blocks_Real_collider[1].setPosition(blocks[1].getPosition().x, blocks[1].getPosition().y + 20);
    blocks_Real_collider[1].setSize(Vector2f(290, height));

    blocks[2].setPosition(340, 380);
    blocks[2].setScale(1, 0.5);
    blocks_Real_collider[2].setPosition(blocks[2].getPosition().x, blocks[2].getPosition().y + 20);
    blocks_Real_collider[2].setSize(Vector2f(290, height));

    blocks[3].setPosition(-10, 500);
    blocks[3].setScale(1, 0.5);
    blocks_Real_collider[3].setPosition(blocks[3].getPosition().x, blocks[3].getPosition().y + 20);
    blocks_Real_collider[3].setSize(Vector2f(290, height));

    blocks[4].setPosition(1430, 370);
    blocks[4].setScale(1, 0.5);
    blocks_Real_collider[4].setPosition(blocks[4].getPosition().x, blocks[4].getPosition().y + 20);
    blocks_Real_collider[4].setSize(Vector2f(290, height));

    blocks[5].setPosition(650, 255);
    blocks[5].setScale(1, 0.5);
    blocks_Real_collider[5].setPosition(blocks[5].getPosition().x, blocks[5].getPosition().y + 20);
    blocks_Real_collider[5].setSize(Vector2f(290, height));

    blocks[6].setPosition(510, 780);
    blocks[6].setScale(1, 0.5);
    blocks_Real_collider[6].setPosition(blocks[6].getPosition().x, blocks[6].getPosition().y + 20);
    blocks_Real_collider[6].setSize(Vector2f(290, height));


    blocks[7].setPosition(850, 910);
    blocks[7].setScale(1, 0.5);
    blocks_Real_collider[7].setPosition(blocks[7].getPosition().x, blocks[7].getPosition().y + 20);
    blocks_Real_collider[7].setSize(Vector2f(290, height));

    blocks[8].setPosition(1750, 500);
    blocks[8].setScale(0.5, 0.5);
    blocks_Real_collider[8].setPosition(blocks[8].getPosition().x, blocks[8].getPosition().y + 20);
    blocks_Real_collider[8].setSize(Vector2f(145, height));

    blocks[9].setPosition(340, 640);
    blocks[9].setScale(0.5, 0.5);
    blocks_Real_collider[9].setPosition(blocks[9].getPosition().x, blocks[9].getPosition().y + 20);
    blocks_Real_collider[9].setSize(Vector2f(145, height));

    blocks[10].setPosition(680, 590);
    blocks[10].setScale(1, 0.5);
    blocks_Real_collider[10].setPosition(blocks[10].getPosition().x, blocks[10].getPosition().y + 20);
    blocks_Real_collider[10].setSize(Vector2f(290, height));

    blocks[11].setPosition(1200, 780);
    blocks[11].setScale(0.5, 0.5);
    blocks_Real_collider[11].setPosition(blocks[11].getPosition().x, blocks[11].getPosition().y + 20);
    blocks_Real_collider[11].setSize(Vector2f(145, height));

    blocks[12].setPosition(1050, 255);
    blocks[12].setScale(1, 0.5);
    blocks_Real_collider[12].setPosition(blocks[12].getPosition().x, blocks[12].getPosition().y + 20);
    blocks_Real_collider[12].setSize(Vector2f(290, height));
}

void GameOver() {

    ScoreText.setPosition(1200, 250);
    ScoreText.setFillColor(Color::Yellow);
    ScoreText.setCharacterSize(100);

    while (window.isOpen()) {
        Event e;

        while (window.pollEvent(e)) {

            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {

                if (e.key.code == Keyboard::Up) {
                    thisGameOverMenu.move_up();
                }
                else if (e.key.code == Keyboard::Down) {
                    thisGameOverMenu.move_down();
                }
                else if (e.key.code == Keyboard::Enter) {

                    if (thisGameOverMenu.pressed() == 0) {
                        SOver.stop();
                        GamePlay();
                    }
                    else if (thisGameOverMenu.pressed() == 1) {
                        SOver.stop();
                        Pname();
                    }
                    else if (thisGameOverMenu.pressed() == 2) {
                        LeaderBoard();
                    }
                    else if (thisGameOverMenu.pressed() == 3) {
                        SOver.stop();
                        pagenum = 1000;
                        main();
                    }
                    else if (thisGameOverMenu.pressed() == 4) {
                        window.close();
                    }
                }
            }
        }
        window.clear();
        thisGameOverMenu.draw(window);
        window.draw(ScoreText);
        window.display();
    }

}

void SetDrops() {
    tDrops[0].loadFromFile("Chosen/objects/Crystals/9.png");
    tDrops[1].loadFromFile("Chosen/objects/Crystals/14.png");
    tDrops[2].loadFromFile("Chosen/objects/Crystals/13.png");
    tDrops[3].loadFromFile("Chosen/Weapons/vecteezy_ai-generated-weapon-isolated-on-transparent-background-png_36743659.png");

    for (int i = 0; i < 4; i++) {
        sDrops[i].setTexture(tDrops[i]);
    }

    // adjust scales
    sDrops[0].setScale(0.2, 0.2);
    sDrops[1].setScale(0.2, 0.2);
    sDrops[2].setScale(0.2, 0.2);
    sDrops[3].setScale(0.05, 0.05);
}

void ChooseDrop(RectangleShape blocks_real_collider[], Clock& Addtimer, Clock& DeleteTimer) {

    if (Addtimer.getElapsedTime().asSeconds() >= 6) {
        y++;
        int indexDrop = rand() % 4;
        int targetIndex = rand() % 13;

        Drop thisDrop;

        thisDrop.sDropshape = sDrops[indexDrop]; // set a drop
        thisDrop.targetCollider = blocks_real_collider[targetIndex]; // set a block to collide with
        thisDrop.targetCollider.setPosition(blocks_real_collider[targetIndex].getPosition().x, blocks_real_collider[targetIndex].getPosition().y - 17);

        if (targetIndex != 8 && targetIndex != 9 && targetIndex != 11) {
            thisDrop.sDropshape.setPosition(thisDrop.targetCollider.getPosition().x + 110, -20); // allgn drop above block
        }
        else {
            thisDrop.sDropshape.setPosition(thisDrop.targetCollider.getPosition().x, -20); // allgn drop above block
        }



        // initialze dropCollider properties

            // Rifle
        if (indexDrop == 3) {
            thisDrop.dropCollider.setSize(Vector2f(93, 45));

        }
        // Crystal, health, protection
        else if (indexDrop == 0 || indexDrop == 1 || indexDrop == 2) {
            thisDrop.dropCollider.setSize(Vector2f(50, 50));
        }

        thisDrop.dropCollider.setFillColor(Color::Yellow);

        // allign dropCollider on drop

            // Rifle
        if (indexDrop == 3) {
            thisDrop.dropCollider.setPosition(thisDrop.sDropshape.getPosition().x, thisDrop.sDropshape.getPosition().y + 25);
        }
        // Crystal, health, protection
        else if (indexDrop == 0 || indexDrop == 1 || indexDrop == 2) {
            thisDrop.dropCollider.setPosition(thisDrop.sDropshape.getPosition().x + 23, thisDrop.sDropshape.getPosition().y + 20);
        }


        thisDrop.type = indexDrop;        // 0 => crystal, 1 => protection , 2 => health
        dropBag.push_back(thisDrop);
        Addtimer.restart().asSeconds();

    }

    if (DeleteTimer.getElapsedTime().asSeconds() >= 30) {
        if (!dropBag.empty()) {
            dropBag.erase(dropBag.begin());
            DeleteTimer.restart().asSeconds();
            Addtimer.restart().asSeconds();
        }

    }

}

void DropADrop(float dt, Player& player) {


    for (int i = 0; i < dropBag.size(); i++) {

        // continous allign dropCollider on the drop

            //Rifle
        if (dropBag[i].type == 3) {
            dropBag[i].dropCollider.setPosition(dropBag[i].sDropshape.getPosition().x, dropBag[i].sDropshape.getPosition().y + 25);
        }
        // Crystal, health, protection

        else if (dropBag[i].type == 0 || dropBag[i].type == 1 || dropBag[i].type == 2) {
            dropBag[i].dropCollider.setPosition(dropBag[i].sDropshape.getPosition().x + 23, dropBag[i].sDropshape.getPosition().y + 20);
        }



        // moving the drop always down
        dropBag[i].sDropshape.move(0, 500 * dt);

        // handle drop Collsion with block
        if (dropBag[i].dropCollider.getGlobalBounds().intersects(dropBag[i].targetCollider.getGlobalBounds())) {
            dropBag[i].sDropshape.move(0, -500 * dt);
        }
        // handle player Collsion with drop
        if (player.playerRealcollider.getGlobalBounds().intersects(dropBag[i].dropCollider.getGlobalBounds())) {

            if (dropBag[i].type == 0) {
                player.score += 4;
                dropBag.erase(dropBag.begin() + i);
            }
            else if (dropBag[i].type == 1) {
                player.health++;
                dropBag.erase(dropBag.begin() + i);

                healthbuff.loadFromFile("Chosen/Sound/mixkit-winning-a-coin-video-game-2069.wav");
                healthsound.setBuffer(healthbuff);
                healthsound.play();

            }
            else if (dropBag[i].type == 2) {
                player.ProtectionCooldown = 10;
                player.Protected = 1;
                
                dropBag.erase(dropBag.begin() + i);
            }
            else if (dropBag[i].type == 3) {
                Bullet bullet;
                bullet.bulletCircle.setRadius(10);
                bullet.bulletCircle.setPosition(-30, 0);
                bullet.bulletCircle.setFillColor(Color::White);
                bullet.coolDown = 9;
                bullet.magazine = 100;
                bullet.speed = 30;
                player.bulletShooting = 1;

                for (int j = 0; j < bullet.magazine; j++) {
                    player.bullets.push_back(bullet);
                    player.bulletIndex++;
                }
                dropBag.erase(dropBag.begin() + i);

            }
        }

    }
}

void bulletCooldown(Player& player) {
    if (player.bulletCooldown > 0) {
        player.bulletCooldown -= 0.01;
    }
    else if (player.bulletCooldown <= 0 && player.bulletShooting == 0) {
        player.bulletShooting = 1;
    }

    if (player.ProtectionCooldown > 0) {
        player.ProtectionCooldown -= 0.01;
    }
    else {
        player.Protected = 0;
    }
}

void Bulletmovement(vector<Bullet> &bullets) {

    for (int i = 0; i < bullets.size(); i++) {

        

        if (bullets[i].move_to_right == 1) bullets[i].bulletCircle.move(bullets[i].speed, 0);
        else if(bullets[i].move_to_right == 0) bullets[i].bulletCircle.move(-1 * bullets[i].speed, 0);

        //if (bullets[i].bulletCircle.getPosition().x >= 1920 || bullets[i].bulletCircle.getPosition().x <= 0) {
        //    bullets.erase(bullets.begin() + i);
        // 
        //}

    }
}

void LeaderBoard() {

    ifstream infile;
    infile.open("LeaderBoard.txt", ios::in);

    vector<string> lines;
    string line;

    while (getline(infile, line, '*')) {
        lines.push_back(line);
    }

    Font font;
    font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");

    vector<Text> text;
    

    for (int i = 0; i < lines.size(); i++) {

        Text t;
        t.setFont(font);
        t.setFillColor(Color::White);
        t.setCharacterSize(40);
        text.push_back(t);

    }



    for (int i = 0; i < lines.size(); i++) {
        text[i].setString(lines[i]);
        text[i].setPosition(100, 100 * 2 * i);
    }

    while (window.isOpen()) {

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    GameOver();
                }
            }
        }
        window.clear();
        
        for (int i = 0; i < lines.size(); i++) {
            window.draw(text[i]);
        }
        window.display();
    }

}

void Cutscene(Player& player,Sprite blocks[], Enemy enemies[]) {

    player.moving = 0;
    gows.stop();

    // loading texures && sounds


    Texture tex[4];
    Sprite Sprites[4];
    for (int i = 3; i <= 5; i++) {
        tex[i - 3].loadFromFile("Chosen/Backgrounds/2/" + to_string(i) + ".png");
        Sprites[i - 3].setTexture(tex[i - 3]);

    }

    Sprites[0].scale(0.658, 1.34);
    Sprites[0].setPosition(0, -1060);
    Sprites[1].scale(0.5, 1.14);
    Sprites[1].setPosition(0,-1900);
    Sprites[2].scale(0.5, 0.8787);
    Sprites[2].setPosition(0, -2990);

    tex[3].loadFromFile("Chosen/Backgrounds/2/throne room.png");
    Sprites[3].setTexture(tex[3]);
    Sprites[3].setScale(0.5, 0.5);
    Sprites[3].setPosition(0, -4080);


    // text
    Text levelOne;
    Font font;
    font.loadFromFile("Chosen/font/BreatheFireIv-3zAVZ.ttf");
    levelOne.setFont(font);

    levelOne.setPosition(650, -550);
    levelOne.setCharacterSize(80);
    levelOne.setString("LEVEL 1 COMPLETED");

    bool Acend = 1;



    View PlayerCam(Vector2f(0, 0), Vector2f(window.getSize().x, window.getSize().y));


    player.Ninja.setPosition(950, 540 );

    cutScene = 1;

    while (window.isOpen()) {



        Event e;
        
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    pauseMenu();
                }
                if (e.key.code == Keyboard::H) {
                    cout << player.Ninja.getPosition().x << " " << player.Ninja.getPosition().y << endl;
                }

            }
            
            
        }
        if (player.Ninja.getPosition().y <= -3550) {
            player.Ninja.setPosition(player.Ninja.getPosition().x, -3550);
            Acend = 0;

        }
        else {
            player.Ninja.move(0, -5);
            window.setView(PlayerCam);
            PlayerCam.setCenter(player.Ninja.getPosition());
        }
        if (!Acend) {
            
            if (PlayerCam.getCenter().y <= -4080)
            PlayerCam.move(0, -5);
            else {
                LevelTwo(player, Sprites);
            }
        }



        player.PlayerAnimation();






        // draw ____________________________________________________________________________________________________________________
        window.clear();

        if (player.Ninja.getPosition().y >= -552 ) {
            window.draw(sback);
            for (int i = 0; i < 13; i++) {

                //window.draw(blocks_real_collider[i]);
                window.draw(blocks[i]);
            }
            for (int i = 0; i < dropBag.size(); i++) {
                //window.draw(dropBag[i].dropCollider);
                window.draw(dropBag[i].sDropshape);
            }

            for (int i = 0; i < 6; i++) {
                //window.draw(enemies[i].enemyRealcollider);
                window.draw(enemies[i].sEnemy);

            }
            for (int i = 0; i < player.bullets.size(); i++) {
                window.draw(player.bullets[i].bulletCircle);
            }
            if (player.Protected) window.draw(player.playerProtection);

            //window.draw(player.playerRealcollider);
            window.draw(player.Ninja);
            //displayStatus();
        }
        for (int i = 0; i <= 3; i++) {
            window.draw(Sprites[i]);
        }
        window.draw(levelOne);
        window.draw(player.Ninja);
        window.display();
    }



}

void BackGroundEnemies(Player &player) {

    Enemy Enemies[12];

    

    Texture tbackgroundEnemy[3];
    for (int i = 0; i < 3; i++) {
        tbackgroundEnemy[i].loadFromFile("Chosen/enemies/PNG/demon/Idle" + to_string(i + 1) + ".png");

    }
    
    int left_x = -20, right_x = 1900;
    int start_y = -3200;

    Enemies[0].sEnemy.setPosition(left_x, -3250); // 0
    Enemies[1].sEnemy.setPosition(left_x, -3350); // 1 
    Enemies[2].sEnemy.setPosition(left_x, -3450); // 2
    Enemies[3].sEnemy.setPosition(left_x, -3900); // 7 
    Enemies[4].sEnemy.setPosition(left_x, -4000); // 8
    Enemies[5].sEnemy.setPosition(left_x, -4100); // 9
    Enemies[6].sEnemy.setPosition(right_x, -3250); // 10
    Enemies[7].sEnemy.setPosition(right_x, -3350); // 11
    Enemies[8].sEnemy.setPosition(right_x, -3450); // 12
    Enemies[9].sEnemy.setPosition(right_x, -3900); // 17
    Enemies[10].sEnemy.setPosition(right_x, -4000); // 18
    Enemies[11].sEnemy.setPosition(right_x, -4100); // 19

    for (int i = 0; i < 12; i++) {
        Enemies[i].sEnemy.setTexture(tbackgroundEnemy[0]);
        if (i > 5) {
            Enemies[i].sEnemy.setScale(-1, 1);
        }

        window.draw(Enemies[i].sEnemy);
    }
    
}


void LevelTwo(Player& player, Sprite Sprites[]) {


    while (window.isOpen()) {

        Event e;

        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    pauseMenu();
                }
            }
        }
        window.clear();
        window.draw(Sprites[3]);
        window.draw(player.Ninja);
        BackGroundEnemies(player);
        window.display();
    }
}