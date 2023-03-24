// Include necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Use STB libraries for image I/O
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

// Define number of color channels in the image (in this case, 1 for grayscale)
#define CHANNEL_NUM 1

// Include custom header file for functions used in this program
#include "../include/image_compression.h"

int main(int argc, char **argv)
{
    // To initialize variables
    int width, height, bpp;
    char *imgname = argv[1];
    int levels = atoi(argv[2]);

    // To load the input image and save it into input_image
    uint8_t* input_image = stbi_load(imgname, &width, &height, &bpp, 1);
    printf("Image %s loaded! width %d, height %d, bpp %d\n", imgname, width,height, bpp);

    // I adjust the input dimensions to form a perfect square 2^n based on the entered dimensions. More detailed explanation can be found in the function.
    int fixed_dimension = squareImageDimensions(height, width);
    printf("The temporary dimensions are: %d x %d \n", fixed_dimension, fixed_dimension);
    
    int ** ArrayImage; // A double pointer array is being created to make image manipulation easier. The array will have the dimensions of a square.
    int i,j, cont_temp = 0; // Variables i and j are being initialized for use in a for loop, and cont_temp is being initialized to keep track of how many positions have been moved with a pointer.
    uint8_t * temp = input_image; // A temporary pointer is being created to the image so that the original pointer's direction is not lost.
    int max_permitted_levels;

    for(i = 0; fixed_dimension > pow(2,i); i++) // Maximum levels allowed in the QuadTree are being calculated by the user.
    {
        max_permitted_levels = i;
    }
    max_permitted_levels = max_permitted_levels + 2;
    printf("The maximum permitted levels are: %d \n", max_permitted_levels);

    //--------------------------------------------------------------------------------------------------------------
    // Initializing a 2D array where the input image will be stored.

    ArrayImage = (int**) malloc(fixed_dimension*sizeof(int*)); // An array of size i is being initialized.
                for(i = 0; i < fixed_dimension; i++)
                {
                    ArrayImage[i] = (int*) malloc(fixed_dimension*sizeof(int)); // Initializing the second dimension of the array with array[i][j].
                    for ( j = 0; j < fixed_dimension; j++ )
                    {
                        if ( i < height && j < width) // Copying the original image into a perfect square with its original dimensions.
                        {
                            ArrayImage[i][j] = *temp; 
                            temp++;
                            cont_temp++;

                        }else {
                            ArrayImage[i][j] = 128;  // If the original dimensions are exceeded, fill the excess areas with white pixels.
                        }
                    }
                }
    //--------------------------------------------------------------------------------------------------------------
    // Performing the Quad Tree operation.

    if (levels <= max_permitted_levels) // Performing the QuadTree operation if the allowed levels are respected.
    {
    Quadtree RootNode; // The QuadTree root is being initialized.
    SetNode(&RootNode, 0, 0, fixed_dimension, 2, 1); // The first node is being Set by calling (MyRootNode, PosX_0, PosY_0, Width_0, Color_0, Level_0_Quadtree(1)).
    BuildQuadTree(&RootNode, ArrayImage, levels); // A QuadTree is being built using the following parameters: (MyRootNode, Fixed_Dimension_Array, Input_Levels).
    } else { // Otherwise, the image remains unchanged.
        printf("The level entered exceeds the permitted limit, please enter a correct value. (The image is the same as the input)");
    }
    //--------------------------------------------------------------------------------------------------------------
    // The image is restored to its original size, and the necessary parts are copied from the temporary array to finalize the image for export.

    temp = temp - cont_temp; // The temporary copy of the pointer starting at input_image is returned.
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            *temp = ArrayImage[i][j]; //The expanded image with white padding is being cut down to its original size by modifying input_image using the temp pointer.
            temp++;
        }
    }

    //--------------------------------------------------------------------------------------------------------------
    // To save the resulting image, which is stored in input_image, as out.jpg
    stbi_write_jpg("bin/out.jpg", width, height, CHANNEL_NUM, input_image, 100);
    //--------------------------------------------------------------------------------------------------------------
    // The memory space used by the created image is being freed up.
    stbi_image_free(input_image);
    // The temporary array created is being freed to release memory space.
    for ( i=0; i < fixed_dimension; i++)
    {
        free(ArrayImage[i]);
    }
    free(ArrayImage);
    return 0;
}


//DEVELOPMENT OF FUNCTIONS
//--------------------------------------------------------------------------------------------------------------
// squareImageDimensions : By analyzing the dimensions of the input image, the function calculates an ideal width or height to make the image square.

int squareImageDimensions(int h, int w) // Input variables are (Height, Width).
{
    int f_width, f_height,v_for;

    for(v_for = 0; w > pow(2,v_for); v_for++) // This function employs a for loop to search for a value greater than Width that is a power of 2**n.
    {
        f_width = v_for;
    }
    f_width++;
    f_width = pow(2,f_width);

    for(v_for = 0; h > pow(2,v_for); v_for++) // This function employs a for loop to search for a value greater than Height that is a power of 2**n.
    {
        f_height = v_for;
    }
    f_height++;
    f_height = pow(2,f_height);

    if (f_width > f_height) f_height = f_width; // By setting the dimensions equal to the larger of Height and Width, the image is transformed into a square shape.
    if (f_height > f_width) f_width = f_height;

    return f_width; // The function returns the ideal dimension of 2^N.
}

//--------------------------------------------------------------------------------------------------------------
// SetNode : This function sets a node using the input parameters and allocates memory for the next four nodes.
//--------------------------------------------------------------------------------------------------------------
void SetNode(Quadtree* newNode, int PosX, int PosY, int Width, int Color, int Level)
{
    int k; // A variable is initialized for the for loop.
    newNode->PosX=PosX; // Top-left corner point. 
    newNode->PosY=PosY;
    newNode->Width= Width; // The square's width.
    newNode->Color= Color; // The square's width.
    newNode->Level = Level; // The current level of the function within the Quadtree.

    for (k = 0; k < 4; k++) // Memory is initialized for the next four nodes.
        newNode->nodes[k] = (Quadtree*) malloc(sizeof(Quadtree));
}

//--------------------------------------------------------------------------------------------------------------
// BuildTree : This function builds the Quadtree based on an initial square, its top-left corner point, width, and color. If a Quadtree's color is gray, four new QuadTrees are generated based on the top-left corner point, which is moved to generate the four new squares, updating their widths as needed.
//--------------------------------------------------------------------------------------------------------------
void BuildQuadTree (Quadtree * Node, int ** Temp, int levels)
{
    int PreviousPosX, PreviousPosY, PosX, PosY; // Top-left corner point
    int Width, PreviousWidth; // Width
    int Level, PreviousLevel; // The level of the Quadtree
    int Color, PreviousColor; // Color
    int x; // This function initializes a variable to execute the function on the four generated squares.

    // Values of the previous node (the one that enters the function and generates new nodes).
    PreviousPosX = Node->PosX;
    PreviousPosY = Node->PosY;
    PreviousWidth = Node->Width;
    PreviousLevel = Node->Level;
    PreviousColor = Node->Color;
    
    // Values for the next nodes (new width and updated level).
    Width = (PreviousWidth/2);
    Level = PreviousLevel + 1;

    if (levels == PreviousLevel) // If the specified level in the console input is reached, the squares are painted with their respective colors and the output is generated.
    {
        BuildNewImage(Node, Temp);
    }
    

    if (PreviousLevel<levels)  // If the levels of the QuadTree are less than those specified in the console input, more levels are generated.
    {
        for (x=0; x<4; x++) // A for loop is used to access the four cases in the switch statement, which correspond to the four squares generated by the Quadtree.
        {
            switch(x) // Based on calculations and drawings, the top-left corner point is moved to the top-left corner of the four new squares.
            {
                case 0: // Top-left square.
                        PosX = PreviousPosX; // PX is defined for the top-left square.
                        PosY = PreviousPosY; // PY is defined for the top-left square.
                        Color = CheckColour(PosX, PosY, Width, Temp); // The color of the top-left square is checked based on its point, width, and the image array.
                        SetNode(Node->nodes[0], PosX, PosY, Width, Color, Level); // The new node is set based on the new information.
                        if (Color!=0 && Color != 1) // If the color is gray (2), a new Quadtree is generated in this square.
                            BuildQuadTree(Node->nodes[0], Temp, levels);                        
                        break; // The process is repeated for the bottom squares, only the point values change.
                case 1: // Top-right square.
                        PosX = PreviousPosX + Width;
                        PosY = PreviousPosY;
                        Color = CheckColour(PosX, PosY, Width, Temp);
                        SetNode(Node->nodes[1], PosX, PosY, Width, Color, Level);
                        if (Color!=0 && Color != 1)
                            BuildQuadTree(Node->nodes[1], Temp, levels);                           
                        break;
                case 2: // Bottom-left square.
                        PosX = PreviousPosX;
                        PosY = PreviousPosY + Width;
                        Color = CheckColour(PosX, PosY, Width, Temp);
                        SetNode(Node->nodes[2], PosX, PosY, Width, Color, Level);
                        if (Color!=0 && Color != 1)
                            BuildQuadTree(Node->nodes[2], Temp, levels);                          
                        break;
                case 3: // Bottom-right square.
                        PosX = PreviousPosX + Width;
                        PosY = PreviousPosY + Width;
                        Color = CheckColour(PosX, PosY, Width, Temp);
                        SetNode(Node->nodes[3], PosX, PosY, Width, Color, Level); 
                        if (Color!=0 && Color != 1)
                            BuildQuadTree(Node->nodes[3], Temp, levels);                                                  
                        break;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------
// CheckColour : The color of a square is checked by iterating through all the pixels within the square, and based on the average, it is determined if it is white, gray, or black. The function then returns the color.
//--------------------------------------------------------------------------------------------------------------
int CheckColour (int PosX, int PosY, int Width, int ** Temp)
{
    int i,j; // Variables for the for loop.
    unsigned int sumArray  = 0, cont = 0; // A variable is defined for the sum of the array, and another for the counter.
    int PosX_BottomRight = PosX + (Width-1); // Based on the input's top-left point, the bottom-right point of the square being analyzed is calculated.
    int PosY_BottomRight = PosY + (Width-1);

    for ( i = PosX; i <= PosX_BottomRight; i++ ) // The corresponding square is iterated through within the array.
    {
        for ( j = PosY; j <= PosY_BottomRight; j++ )
        {
            sumArray = sumArray + Temp[j][i]; // The pixel values are summed as the iteration progresses.
            cont++; // The number of pixels is counted as the iteration progresses.
        }
    }

    sumArray = sumArray/cont; // The average pixel value within the square is calculated.
    
    if (sumArray==0){ // If the average pixel value is 0, it means the square is completely black.
        return 0;
    }
    if (sumArray==255){ // If the average pixel value is 255, it means the square is completely white.
        return 255;
    }
    return sumArray;  // If the average pixel value is neither 0 nor 255, it means the square is gray.
}

//--------------------------------------------------------------------------------------------------------------
// BuildNewImage : Builds the new image around the input square and its colors based on the image array.
//--------------------------------------------------------------------------------------------------------------

void BuildNewImage(Quadtree * Node, int ** image)
{
    int PosX = Node->PosX; // Upper left point
    int PosY = Node->PosY;
    int Color = Node->Color; // Node color
    int Width = Node->Width; // Width of the node.
    int i,j; // Variables to be used in the for loop.
    int PosX_BottomRight = PosX + (Width-1); // Lower-right point.
    int PosY_BottomRight = PosY + (Width-1);    
    for ( i = PosX; i <= PosX_BottomRight; i++ ) // I iterate from the upper-left point to the bottom-right point of the square being analyzed.
    {
        for ( j = PosY; j <= PosY_BottomRight; j++ )
        {
            image[j][i] = Color; // The image is painted.
        }
    }
}
