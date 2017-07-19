//
// Gmsh input
//

periphery_size = 1; // element size elsewhere in the domain
obstruction_size = 0.3; // element size around the obstruction

If (Exists(scale))
  periphery_size = periphery_size * scale;
  obstruction_size = obstruction_size * scale;
EndIf

Point(1) = {0,0,0,periphery_size};
Point(2) = {30,0,0,periphery_size};
Point(3) = {30,6,0,periphery_size};
Point(4) = {0,6,0,periphery_size};
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};
Line Loop(1) = {1,2,3,4};

Point(5) = {7,2.5,0,obstruction_size}; // Ellipse center
Point(6) = {6,2.5,0,obstruction_size}; // Ellipse left
Point(7) = {7,4,0,obstruction_size}; // Ellipse top
Point(8) = {8,2.5,0,obstruction_size}; // Ellipse right
Point(9) = {7,1,0,obstruction_size}; // Ellipse bottom
Ellipse(5) = {6,5,7,7};
Ellipse(6) = {7,5,8,8};
Ellipse(7) = {8,5,9,9};
Ellipse(8) = {9,5,6,6};
Line Loop(2) = {5,6,7,8};

Plane Surface(2) = {1,2};

Physical Line("obstruction") = {5,6,7,8};
Physical Line("top") = {3};
Physical Line("bottom") = {1};
Physical Line("right") = {2};
Physical Line("left") = {4};
Physical Surface(0) = {2};
