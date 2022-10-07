#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <sstream>

/* Simulation Vars */
const double STEP_TIME = 0.05;

/* Game Vars */
const int LEFT_MARGIN = 70;
const int RIGHT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void move_big_bubbles(vector<Bubble> &bigbubbles){
    // move all bigbubbles
    for (unsigned int i=0; i < bigbubbles.size(); i++)
    {
        bigbubbles[i].nextStep(STEP_TIME);
    }
}

bool bubble_bullet_collision(vector<Bubble> &bubbles,vector<Bullet> &bullets){
    //  Check for any collision between bubble and bullet
    int collision_detector = 0;
    for (unsigned int i=0; i < bubbles.size();i++)
    {
        for(unsigned int j=0;j < bullets.size();j++)
        {
            if ((abs(bullets[j].get_center_x() - bubbles[i].get_center_x()) < bullets[j].get_width()/2 + bubbles[i].get_radius()) // range of x separation of centre of bubble and bullet to consider it a collision
               &&
               (bullets[j].get_center_y() - bubbles[i].get_center_y() < bullets[j].get_height()/2 + bubbles[i].get_radius())  // upper limit of y separation of centre of bubble and bullet to consider it a collision
               &&
               (bullets[j].get_center_y() - bubbles[i].get_center_y() > 0))  // lower limit for the y separation of centre of bubble and bullet to consider it a collision
            {
                // erase the bullet and particular bubble and detect the collision
                bullets.erase(bullets.begin()+j);
                bubbles.erase(bubbles.begin()+i);
                collision_detector += 1;
            }
        }
    }
    if (collision_detector != 0)      // report that there is a collision if it is detected
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool big_bubble_splitter(vector<Bubble> &bigbubbles,vector<Bubble> &bubbles,vector<Bullet> &bullets){
    //  Check for any collision between bigbubble and bullet and split the big bubble into two small bubbles
    int collision_detector = 0;
    for(unsigned int i = 0;i < bigbubbles.size();i++)
    {
        for(unsigned int j = 0;j < bullets.size();j++)
        {
            if ((abs(bullets[j].get_center_x() - bigbubbles[i].get_center_x()) < bullets[j].get_width()/2 + bigbubbles[i].get_radius())  // range of x separation of centre of bigbubble and bullet to consider it a collision
                &&
                (bullets[j].get_center_y() - bigbubbles[i].get_center_y() < bullets[j].get_height()/2 + bigbubbles[i].get_radius())    // upper limit of y separation of centre of bigbubble and bullet to consider it a collision
                &&
               (bullets[j].get_center_y() - bigbubbles[i].get_center_y() > 0))   // lower limit for the y separation of centre of bubble and bullet to consider it a collision
            {
                bullets.erase(bullets.begin()+j);  // erase the bullet
                // create two small bubbles moving in opposite directions with default vx
                bubbles.push_back(Bubble(bigbubbles[i].get_center_x(), bigbubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(255,0,255)));
                bubbles.push_back(Bubble(bigbubbles[i].get_center_x(), bigbubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(255,0,255)));
                bigbubbles.erase(bigbubbles.begin()+i);    //erase the bigbubble
                collision_detector += 1;    //detect a collision
            }
        }
    }
    if (collision_detector != 0)      // report that there is a collision if it is detected
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool bubble_shooter_collision(vector<Bubble> &bubbles,Shooter shooter){
    int collision_detector = 0;
    for (unsigned int i=0; i < bubbles.size();i++)
    {
        if((abs(bubbles[i].get_center_x() - shooter.get_body_center_x()) < shooter.get_body_width()/2 + bubbles[i].get_radius())      // range of x separation of centre of bubble and shooter to consider it a collision
          &&
          (abs((shooter.get_body_center_y() + shooter.get_body_height()/2) - bubbles[i].get_center_y()) < shooter.get_body_height() + 2*bubbles[i].get_radius()))    // range of y separation of centre of bubble and shooter to consider it a collision
        {
            collision_detector += 1;    // detects the collision through change of value
        }
    }
    if (collision_detector != 0)      // report that there is a collision if it is detected
    {
        return true;
    }
    else
    {
        return false;
    }  
}

bool big_bubble_shooter_collision(vector<Bubble> &bigbubbles,Shooter shooter){
    int collision_detector = 0;
    for (unsigned int i=0; i < bigbubbles.size();i++)
    {
        if((abs(bigbubbles[i].get_center_x() - shooter.get_body_center_x()) <= shooter.get_body_width()/2 + bigbubbles[i].get_radius())       // range of x separation of centre of bigbubble and shooter to consider it a collision
          &&
          (abs((shooter.get_body_center_y() + shooter.get_body_height()/2) - bigbubbles[i].get_center_y()) < shooter.get_body_height() + 2*bigbubbles[i].get_radius()))     // range of y separation of centre of bigbubble and shooter to consider it a collision
        {
            collision_detector +=1;       // detect the collision
        }
    }
    if (collision_detector != 0)        // report that there is a collision if it is detected
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(0,0,255)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(0,0,255)));
    bubbles.push_back(Bubble(WINDOW_X/3.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(0,0,255)));
    return bubbles;
}

vector<Bubble> create_big_bubbles()
{
    //  creates initial bigbubbles in the game
    vector<Bubble> bigbubbles;
    bigbubbles.push_back(Bubble(WINDOW_X, BUBBLE_START_Y, BUBBLE_RADIUS_THRESHOLD, BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(255,0,255)));
    bigbubbles.push_back(Bubble(WINDOW_X/5.0, BUBBLE_START_Y, BUBBLE_RADIUS_THRESHOLD, -BUBBLE_DEFAULT_VX, 0, BUBBLE_Y_ACCELERATION, COLOR(255,0,255)));
    return bigbubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    bool WIN, SCORE_INCREASE, HEALTH_DECREASE;   // bool variables controlling the game

    double health = 30, time = 0, total = 0;  // initial values of health,time and score

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    Text SCORE_UPDATE( WINDOW_X - RIGHT_MARGIN, BOTTOM_MARGIN,"Score: 0");   //  text variable which displays score

    Text TIME_LEFT(LEFT_MARGIN,TOP_MARGIN,"Time:0/50");                      //  text variable which displays time left in the game

    Text HEALTH_LEFT( WINDOW_X - RIGHT_MARGIN,TOP_MARGIN,"Health:30/30");      //  text variable which displays health left

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bigbubbles
    vector<Bubble> bigbubbles = create_big_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while ((health != 0) && !(WIN) && (floor(time) < 50))
    {
        time += STEP_TIME;   // updating time by a time step in every iteration

        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bigbubbles
        move_big_bubbles(bigbubbles);

        // Update the bullets
        move_bullets(bullets);

        //  Check for any collision of bubble of any kind with a bullet
        SCORE_INCREASE = ((bubble_bullet_collision(bubbles,bullets)) || (big_bubble_splitter(bigbubbles,bubbles,bullets)));

        // Check for a collision of bubble of any kind with the shooter
        HEALTH_DECREASE = ((bubble_shooter_collision(bubbles,shooter)) || (big_bubble_shooter_collision(bigbubbles,shooter)));

        if(SCORE_INCREASE)                             // code executed only if bullet hits a bubble
        {
            total += 1;                                // score increased by 1
            stringstream total_score;                  // a stringstream variable is defined
            total_score << total;                      // total is stored in this variable
            string SCORE = "Score:", score;            // two string variables are created
            total_score >> score;                      // stringstream variable is stored in a string variable to convert it in string
            SCORE_UPDATE.setMessage(SCORE + score);    //text displaying score is updated
        }

        if(HEALTH_DECREASE)                                // code executed only if a bubble hits shooter
        {
            health -= 1;                                   // health decreased by 1
            stringstream total_health;                     // a stringstream variable is defined
            total_health << health;                        // health is stored in this variable
            string Health = "Health:", health;             // two string variables are created
            total_health >> health;                        // stringstream variable is stored in a string variable to convert it in string
            HEALTH_LEFT.setMessage(Health + health + "/30");     //text displaying health is updated
        }

        if(floor(time) > 0)                           // code executed only when certain time has passed
        {
            stringstream total_time;                  // a stringstream variable is defined
            total_time << floor(time);                // time is stored in this variable
            string Time = "Time:", time;              // two string variables are created
            total_time >> time;                       // stringstream variable is stored in a string variable to convert it in string
            TIME_LEFT.setMessage(Time + time + "/50");     //text displaying time is updated
        }

        // Check whether all the bubbles have been destroyed
        WIN = (bubbles.size() == 0) && (bigbubbles.size() == 0);

        wait(STEP_TIME);
    }
    if((health == 0) || (floor(time) >= 50))  // condition to be checked to display appropriate message at the end of the game
    {
        Text GAME_OVER_ALERT(250,250,"GAME OVER");GAME_OVER_ALERT.setColor(COLOR(255,0,0));
        wait(10);
    }
    else
    {
        Text WIN_ALERT(250,250,"CONGRATULATIONS!!");WIN_ALERT.setColor(COLOR(0,255,0));
        wait(10);
    }
}
