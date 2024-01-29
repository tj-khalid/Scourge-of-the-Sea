Tamjeed Zunayed, 101269330
Assignment 1

OS: Windows
	
Changes:
- Created EnemyGameObject class, inherits GameObject class
- Enemies are replaced with EnemyGameObjects, Game::Setup()
- Added & implemented Timer Class
- GameObject Class changes:
  - Split Scale member into 2 variables (x, y).
  - Created collision_ bool for whether the Object has collision or not
  - Created IsPlayer_ bool to define whether the Object is the player or not
  - Created hp_ variable to represent the GameObjects Health
  - Created a timer_ member
  - Created a TakeDamage() function
  - Updated Get Functions
  - Updated Constructor
- EnemyGameObjects have 1 HP, declared in its constructor
- PlayerGameObject has 3 HP, declared in its constructor
- Game::Update() chages:
  - Implemented the circle to circle collision, both objects take 1 damage upon collision
  - Implemented deletion of an object, if its Hp is 0 or if its timer is finished
  - EnemyGameObjects and PlayerGameObject create Explosion game object upon their deletion
  - Game Over when there are 3 game objects left (temporary solution)
- Game::HandleControls() chages:
  - Created a check at the start to see whether the object being controlled is the player object

Citations:
player's sprite taken from Website "https://millionthvector.blogspot.com/2013/07/more-free-top-down-spaceship-sprites.html"
enemy's sprite taken from Website "https://millionthvector.blogspot.com/2013/07/more-free-top-down-spaceship-sprites.html"
Explosion sprite taken from Website "https://www.cleanpng.com/png-pixel-art-nyan-cat-4515197/download-png.html"
https://iconduck.com/icons/313561/gold-coin