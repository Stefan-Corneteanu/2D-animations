2D animations

A C++ project using the openGL API for realising 2D animations. Based on user input, on the screen a square, a hexagon or an octagon is drawn in a random position. The shape can be translated, scaled, rotated or erased. This is an academic project oriented on understanding the basics of openGL: the notions of VBO, EBO(or IBO), VAO, animations and user-input

User input and its result:
1) By pressing 4, a rectangle is generated on the screen in a random position
2) By pressing 6, a hexagon is generated on the screen in a random position
3) By pressing 8, an octagon is generated on the screen in a random position
4) By pressing O, the current shape is erased
5) By pressing T, the current shape is translated to the right border of the window and back to the original position in a perpetual loop. Pressing T again while this translation is performed will cancel the animation and keep the shape in the current position
6) By pressing R, the current shape is rotated counterclockwise. Pressing R again will cancel the animation and keep the object in the current position
7) By pressing S, the current shape will scale till it covers the whole screen

Notes: 
Changing the shape will not stop the animation
If the user initiates an animation while another is performing, only the most recent animation called will perform
Erasing a shape will also act as a stop for all animations