Tamjeed Zunayed, 101269330
Assignment 3

OS: Windows
	
Changes:

- Game::Render() changes:
  - Added continuous scrolling of the game, by adding a playerTransformation to the view_matrix

- GameObject Class changes:
  - Created ghost_mode, tiling_ and isOrbiting bools
  - Created getter and setters
  - GameObject::Render() changes:
    - created orbit matrix
    - changed transformation matrix based on its orbiting state
    - created uniforms in the shader based on tiling_ and ghost_mode
    - implemented grayscale and tiling rendering in the fragment shader
  - GameObject::Update() changes:
    - implemented orbiting function by top left of image (rough but it works)
  - Updated Constructor
  - Scaling automaically finds textures dimensions and applies accordingly

- PlayerGameObject class Changes:
  - created velocity_ vector
  - created addForce() method, with maximum velocity of 2
  - PlayerGameObject::Update() now uses physics based motion
  - Game::HandleControls() now uses AddForce method of the player

- Game::Setup() changes:
  - scaled background up by a factor of 3
  - set its tiling to true
  - added swinging axe at cords(2.0, -2.0)
  - set its orbiting to true

- Game::Update() changes:
  - collectibles now turn to grayscale when collided with the player

Citations:
A1:
player's sprite taken from Website "https://millionthvector.blogspot.com/2013/07/more-free-top-down-spaceship-sprites.html"
enemy's sprite taken from Website "https://millionthvector.blogspot.com/2013/07/more-free-top-down-spaceship-sprites.html"
Explosion sprite taken from Website "https://www.cleanpng.com/png-pixel-art-nyan-cat-4515197/download-png.html"

A2:
collectible sprite taken from Website "https://iconduck.com/icons/313561/gold-coin"
Invincible Player sprite edited from player sprite

A3:
Swinging axe from: https://www.vecteezy.com/vector-art/24212725-pixel-art-axe-strong-lumberjack-axe-pixelated-design-for-logo-web-mobile-app-badges-and-patches-video-game-sprite-8-bit-isolated-vector-illustration