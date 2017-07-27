//
// Gmsh input
//

lc = 1; // element size in the domain

// Rectangular section
Point(1) = {0,0,0,lc};
Point(2) = {50,0,0,lc};
Point(3) = {50,100,0,lc};
Point(4) = {0,100,0,lc};
Line(1) = {1,2};
Line(3) = {3,4};
Line(4) = {4,1};

// Semicircle section
Point(5) = {50,50,0,lc}; // center
Circle(2) = {2,5,3};

Line Loop(1) = {1,2,3,4};
Plane Surface(1) = {1};

Physical Line("top") = {3};
Physical Line("bottom") = {1};
Physical Line("right") = {2};
Physical Line("left") = {4};
Physical Surface(0) = {1};
