# QSnake
&ensp;This project is project 1 of SE127@SJTU, written in QT5.
<pr>
## 0. Overview

&ensp;**Function:**</br>

- **Basic function.** You can move the snake, eat the food, and your scores and levels will increase accordingly.
- **Obstacles.** In `Obstacle Mode`, obstacles will be created.
- **Different Food.** In `Obstacle Mode`, differnent food will trigger different effects.
- **Battle.** In `Battle Mode`, two players can play together.
- **AI Snake.** In `Auto Mode`, the snake will eat foods automatically.
- **Login And Rank List.** In `Obstacle Mode`, you should create an account and login.A ranklist will show at [the game widget.

&ensp;**Files：**

- Classes: `Food`,&thinsp;`Snake`,&thinsp;`Wall`,&thinsp;`Scene`
- Widgets: `Widget`,&thinsp;`GameWidget`,&thinsp;`WallGameWidget`,&thinsp;`DoubleGameWidget`,&thinsp;`DoubleGameWidget`,&thinsp;`AutoGameWidget`
- Dialogs: `HelpDialog`,&thinsp;`LoginDialog`

## 1. Classes

### 2.1 Scene
&emsp;It contains the control button of the snake ,the `start` button, `stop` button and `change speed` button.&thinsp; It also contains the `score label`, the`level label`, the `hint label` and many parts of the scene. 

| Function | Usage 
| :-: | -
|createBasic(QWidget* w)| create basic scene ( background, size, icon, title .etc and the start and stop button)
|createControlButtons(QWidget* w) | create the control buttons of the snake ( move and change speed) 
|createScoreAndLevel(QWidget* w)| create the score and level label 
|createBattleScore(QWidget* w)| create the scores of the two players in the battle widget
|createHint(QWidget* w) | create the hint label ( warn you the effect of the current food and the time remain)
|updateScore()|update score and level, only for one player widget
|updateBattleScore()|update scores of the two player


### 2.2 Snake
&emsp;It use `int food_x`,&thinsp;`int food_y`,&thinsp;`int **snake` and `int **snake_before` to store a snake.Two functions:&thinsp;`createWall()` and `updateWall`can be called in other widgets.

### 2.3 Food
&emsp;It use `int direction`,&thinsp;`int bodycount`,&thinsp;`int food_type` and `int food_type_before` to store the location and type of the food. Two  overloaded functions:&thinsp;`createFood()` and `updateFood()`can be called in other widgets.

### 2.4 Wall
&emsp;It use `char** map`&thinsp;to store the obstacles ( means that if there is a wall at `point (x, y)` than `map[x][y]` stores `'w'`,chose map rather than store the location of obstacles as it save time when judging whelther the food or snake is created at the point where exsits the wall ). And use `int** map_auto`&thinsp;to store the location several randomized blocks. Two  overloaded functions:&thinsp;`createFood()` and `updateFood()`can be called in other widgets.

## 2. Widgets and Dialogs
### 2.1 Main Widget
&emsp;This Widget is the home page of the game. You can choose the game pattern you want here.
&emsp;It's realized by `QButton`. Once you click the button, the program will new relevant widget and show it.</br>

![2019092301.JPG](https://i.loli.net/2019/09/23/4WpoAV1jOZFq6Ub.jpg)

### 2.2 Classic Pattern Widget

&emsp;This pattern is a very sample one. You can control the move of the snake by either the keybroad or the mouse.
#### 2.2.1 `time_out()` function
&emsp;It use function `time_out()` to update the widget. Every certain time, the `time_out()` will be called. Once it is called, the snake will move a step ( by `bodycount++` and calling `Snake::updateSnake()`) and it will check whether the snake die after move this step. If the head of the snake meet the food, it will call `Food::updateFood()` and `Score::updateScore()` to update relevent parts.
#### 2.2.2 `paintEvent()` function
&emsp; Use `QPainter` to draw food and snake.
#### 2.2.3 `keyPressEvent()` function
&emsp; This function is reimplemented to receive key press events for the widget. Calls `grabKeyboard()` to enable this widget to receive all keyboard events.
![2019092302.JPG](https://i.loli.net/2019/09/23/HNAsPK72yULGBCi.jpg)

### 2.3 Obstacle Mode Widget
&emsp;This pattern is based on the classic mode. Besides, it will create walls and sevel blocks at the begining of the game. During the game, different types of food will appear at random. They have different special effects. What's more, this mode require you to register ang login in, a rank list will show.

![2019092304.JPG](https://i.loli.net/2019/09/23/Iwcb2mNGQPrvJEZ.jpg)

#### 2.2.1 Login Dialog
&emsp;After click the `Obstacle Mode` button, this `QDialog` will show first.
&emsp;It use a database to store the `name`, `password` and `score` of every user. Click the `Login ` button and it will call `loginbtn_clicked()` to verify your account. Write your name and password first and Click the `Login` Button, your information will add to the database. 
&emsp; A gobal varible `QString user` is declared here to pass the user name to the `wallgamewiget.cpp` so as to update the user's score later. 

![login](https://i.loli.net/2019/09/23/aUZN5eIiQu6H8AY.jpg)

#### 2.2.2 `updateAccordingToFoodType()` Function

     APPLE: score +10.
     BANANA: score +20.
     ORANGE: The snake can run into and out of the walls and blocks.
     GRAPE: The snake will keep running at a faster speed until it eat a food which type is not GRAPE.
            Warning: the CHANGE SPEED button is invalid after eat GRAPE.
     BOOM: The snake must eat another food in 10s.There will be a time counting at the side.
&emsp; Use a QTimer which refrush every 1 s to count 10 seconds.  int `timeCnt` records the  time remained ( so the remain time can be show at the `HINT` label ) , once it reach `0`, gameover.
### 2.4 Auto Pattern Widget

&emsp;The key point is to change the direction of the snake according to the location of the food. It use `A star` algorithm to find the shortest road. The pseudo code is show below. 


	    if( can find path to the food)
	         if(can find the spaces left by the tail after move )
	             move;
	         
	    if( can not find path to the food || can not find tail after move )
	        wander;

	    if( full all the window )
	        WIN;

&emsp; `wander` means chosing a direction to go, in order to ensure that the snake can full the window, it should satisfied the following conditions:

- **Special Case:** only one blank left. Update the snake without updating food and repaint the window. Game over.
- First try to find a direction to satisfy that you can find your tail after follow this direction. If there are many directions, choose the direction farest from the tail.
- If there is no such direction, find a direction which you still can go. Still, choose the direction farest from the tail.	<br>	

>**HINT :** check the four direction at random order, so you won't go the same way every time. This can avoid infinite loop.

To realize this, I use:

	    static int index[4][2]={{0,1},{0,-1},{-1,0},{1,0}};

	    int count = 0;
	    for (int var = qrand()%4; count < 4; var = (var+1)%4) {
		int x = head_x + index[var][0];
		int y = head_y + index[var][1];
		check this direction.
		count++
	    } 

The A star algorithm use the same way to find different shorest way every time.

### 2.5 Battle Pattern Widget
&emsp;In this Mode, two players use `w`, `a`, `s`, `d` or `⬅`, `➡`, `⬆`, `⬇` to control differnent snake.

![2019092305.JPG](https://i.loli.net/2019/09/23/mutqLiMdsARNJo1.jpg)

### 2.6 Help Document Widget

Once clicked, it will show the game help.

![2019092306.JPG](https://i.loli.net/2019/09/23/U6dVApZhMrqbjOi.jpg)
