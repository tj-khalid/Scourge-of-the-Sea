Tamjeed Zunayed, 101269330
Elijah Molina, 101272342
Final Project

OS: Windows
	
Instructions:

The goal of the game is collect 100 coins to become the King Of The Pirates

Controls:

[w] - Move Forward
[A] - Move Left
[S] - Move Right
[D] - Move Backwards
[Left Click] - Shoot Harpoon At Mouse's Location
[Space Bar] - Shoot Cannons on the Sides of the Boat

Requirements:

Game Mechanics:

- Different Weapons To Help Destory Other Ships (Cannons & Harpoons)
- Gather Coins From The Ground And Destroying Other Ships
- Gather Powers Ups (Invincibilty)
- Gather 100 Coins In Order To Win


Enemies:
- Shark AI
	- Chase AI 
	- Rams Into The Player
- Cannon Ship
	- Intercept AI
	- Goes To The Side Of The Ship to Shoot It's Cannons 
- Harpoon Ship
	- Chase AI
	- Once Close Enough To Ship Shoot Harpoon

Weapons:
- Cannon
	- Comes From The Sides of the Ship Press [SPACE] to fire.
- Harpoon
	- Comes From The Centre of The Ship and Goes In The Direction of the Mouse Cursor Press [LMB]

Collcetible Items:
- Chest -> Gives 5 Coins
- Coin -> Gives 1 Coin
- Barrel -> Collect 5 And Become Invincible For 10 Seconds

Movement And Transformation:
- Player/Enemy Ships -> Physics Based
- Cannon/Harpoon/Shark -> Parametric Based

Collision Detection:
- Performs Circle to Circle Collision when 2 ships are close
- Performs Ray Collision When There is a Bullet or Harpoon hitting the ship

Game World
- Map is 21x21 Tiles created using texture tiling 

Particle System
- Cannon Fire -> caused when you Press [SPACE] and fire a Cannon 
- Explosion -> Caused when you hit a enemy ship

UI
- Displays Score and HP

Advanced Method
- Intercept AI that goes to a position to the side of the Player and fires a Cannon once close enough
     	
Gameplay Notes
- Harpoon does not work as expected while in Fullscreen!

Citations:
All Sprites Taken From opengameart.org under CC License
