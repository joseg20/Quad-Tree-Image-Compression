#ifndef __TAREA2__
#define __TAREA2__

// 
// JOSE GUILLEN ROL USM 201921076-9
typedef struct qtree
{
    int PosX; // Top-left point
    int PosY;
    int Level; // Node level in the quadtree
    int Width; // Square width
    int Color; // 0: black, 1: white, 2:gray
    struct qtree * nodes[4]; // Children nodes
}Quadtree;

//--------------------------------------------------------------------------------------------------------------
// squareImageDimensions : This function analyzes the input image dimensions and returns an ideal width or height to make the image a square
int squareImageDimensions(int h, int w); // Input parameters (Height, Width)
// Returns Fixed_Dimension (The fixed dimension to make the image a perfect square based on its original dimensions)
//--------------------------------------------------------------------------------------------------------------
// SetNode : Sets a node's parameters and initializes memory for its 4 child nodes
void SetNode(Quadtree* newNode, int PosX, int PosY, int Width, int Color, int Level); // Input parameters (Node, PosX_topleft, PosY_topleft, square Width, square Color, Node Level)
// Void function that modifies my Quadtree through pointers
//--------------------------------------------------------------------------------------------------------------
// BuildTree : Builds a Quadtree based on an initial square, its top-left point, its width, and its color. If the Quadtree color is gray, 4 new QuadTrees are generated based on the top-left point, which is moved and used to generate the 4 new squares, updating the widths for each one.
void BuildQuadTree(Quadtree *node, int ** Temp, int levels); // Input parameters (Node, Image array, permitted levels)
// Void function that modifies the Quadtree through pointers
//--------------------------------------------------------------------------------------------------------------
// CheckColour : Checks a square's color by examining all pixels within it and based on the average determines whether it is black, gray or white. Returns the color.
int CheckColour (int PosX, int PosY, int Width, int ** Temp); // Input parameters (TopLeftPosX, TopLeftPosY, square width, Image array)
// Returns the square's color
//--------------------------------------------------------------------------------------------------------------
// BuildNewImage : Constructs the new image based on the input square and its color from the image array.
void BuildNewImage(Quadtree * Node, int ** image); // Input parameters (Node, Image array)
// Void function that modifies the image array through pointers based on the node being read
//----------------------------------------------------

#endif // __TAREA1__
