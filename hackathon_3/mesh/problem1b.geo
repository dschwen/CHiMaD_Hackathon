//
// Gmsh input
//
Mesh.ElementOrder = 2;

periphery_size = 1; // element size elsewhere in the domain
obstruction_size = 0.3; // element size around the obstruction

If (Exists(scale))
  periphery_size = periphery_size * scale;
  obstruction_size = obstruction_size * scale;
EndIf

/*
Point(1) = {0,0,0,periphery_size};
Point(2) = {30,0,0,periphery_size};
Point(3) = {30,6,0,periphery_size};
Point(4) = {0,6,0,periphery_size};
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};
Line Loop(1) = {1,2,3,4};
*/

Point(10) = {0,0,0,periphery_size};
Point(11) = {5,0,0,obstruction_size};
Point(12) = {9,0,0,obstruction_size};
Point(20) = {30,0,0,periphery_size};
Point(30) = {30,6,0,periphery_size};
Point(31) = {9,6,0,obstruction_size};
Point(32) = {5,6,0,obstruction_size};
Point(40) = {0,6,0,periphery_size};
Line(11) = {10,11};
Line(12) = {11,12};
Line(13) = {12,20};
Line(14) = {20,30};
Line(15) = {30,31};
Line(16) = {31,32};
Line(17) = {32,40};
Line(18) = {40,10};
Line Loop(1) = {11,12,13,14,15,16,17,18};


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
Physical Line("top") = {15,16,17};
Physical Line("bottom") = {11,12,13};
Physical Line("right") = {14};
Physical Line("left") = {18};
Physical Surface(0) = {2};
