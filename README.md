# SkullKing

## Overview

Skull King is a card game where players compete to win hands by playing the highest-ranking card in each round. each player receiving a set of cards that they use strategically to outplay their opponents.
The game is implemented using C++ and the Qt framework.

## Table of Contents

- [SkullKing](#SkullKing)
  - [Overview](#Overview)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Installation](#installation)
    - [From binary](#from-binary)
      - [Windows](#windows)
    - [From source](#from-source)
  - [Screenshots](#screenshots)
  - [Notes](#Notes)

## Features
**Scoreboard:**

- Keep track of scores across rounds and display the final winner.

**Interactive UI:**

- User-friendly interface for smooth gameplay experience.

**User Login and Registration:**

- Players can create new accounts or log into existing ones.

**Server-Client Architecture:**

- One player acts as the server, hosting the game, while others join as clients.
- The server manages game state and player interactions.

**Game Menu:**

- The game menu includes options to start a new game, view player statistics, and change account settings.

**Game Start:**

- A random player is chosen to start the game each round.
- The starting player rotates based on previous hand winners.

**Stop & Resume, Exit Options:**

- Players can pause the game, When players press the pause button, the game stops for 60 seconds for both players. After 60 seconds, the game resumes automatically, or the player who pressed the pause button can resume the game for both players by pressing the resume button.
- Players can exit the game, which affects their score and game state.
  
## Installation

### From binary

#### Windows
You can download **SkullKing-release.rar** from [link](https://iutbox.iut.ac.ir/index.php/s/SxCPzfRfSpJidko).

After downloading the rar file first extract it, then start `skullKing.exe` from Skullking folder and finally you can play .

### From source
You should have [Qt Creator](https://www.qt.io/download) installed on your system to build this project.

1- Open the project in Qt Creator.

2- Build and run the project using the appropriate build settings for your platform.

3- Enjoy the game.

## Screenshots
![menu](https://s8.uupload.ir/files/screenshot_2024-08-04_152651_hezf.png)

![chenge info](https://s8.uupload.ir/files/screenshot_2024-08-04_190102_h0v3.png)

![skullKing ](https://s8.uupload.ir/files/screenshot_2024-08-04_152542_ie7j.png)

![client or server](https://s8.uupload.ir/files/screenshot_2024-08-04_152709_a1xt.png)

![game1](https://s8.uupload.ir/files/screenshot_2024-08-04_153014_emqr.png)

![game 2](https://s8.uupload.ir/files/screenshot_2024-08-04_153144_rndl.png)

## Notes
- Ensure that the server and client are running on the same network or that the necessary ports are open if running on different networks.
