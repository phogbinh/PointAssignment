// Assignment 1 19T3 COMP1511: CS Paint
// paint.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0 (2019-10-08): Assignment released.

#include <stdio.h>

// Note: you may find the square root function (sqrt) from the math
// library useful for drawing ellipses in Stage 3 and Stage 4.
#include <math.h>

// IF YOU NEED MORE LIBRARIES ADD THEM HERE
#include <stdlib.h>


// ADD YOUR DATA STRUCTURES HERE
typedef enum
{
    false = 0,
    true
}
bool;

typedef enum
{
    FAILURE = 0,
    SUCCESS,
    IGNORING
}
EStatus;

typedef enum
{
    BLACK = 0,
    DARK,
    GREY,
    LIGHT,
    WHITE
}
EShade;

typedef struct
{
    int m_nRow;
    int m_nColumn;
}
Point;

void SetPoint( Point* pPoint, const int nNewRow, const int nNewColumn )
{
    pPoint->m_nRow = nNewRow;
    pPoint->m_nColumn = nNewColumn;
}


// The dimensions of the canvas (20 rows x 36 columns).
#define N_ROWS 20
#define N_COLS 36

// Shades (assuming your terminal has a black background).
#define BLACK 0
#define WHITE 4

// IF YOU NEED MORE #defines ADD THEM HERE
//#define _CRT_SECURE_NO_WARNINGS 1 // For Visual Studio compiler.
// Canvas
#define MIN_ROW 0
#define MAX_ROW 19
#define MIN_COLUMN 0
#define MAX_COLUMN 35
// Angles
#define SIMPLIFIED_ANGLE_MAX_DEGREE 360
#define SIMPLIFIED_ANGLE_UP 0
#define SIMPLIFIED_ANGLE_RIGHT 90
#define SIMPLIFIED_ANGLE_DOWN 180
#define SIMPLIFIED_ANGLE_LEFT 270
#define SIMPLIFIED_ANGLE_RIGHT_UP 45
#define SIMPLIFIED_ANGLE_RIGHT_DOWN 135
#define SIMPLIFIED_ANGLE_LEFT_DOWN 225
#define SIMPLIFIED_ANGLE_LEFT_UP 315


// ADD YOUR GLOBAL VARIABLES HERE
EShade g_eCurrentShade = ( EShade ) BLACK;


// Provided helper functions:
// Display the canvas.
void displayCanvas( int canvas[ N_ROWS ][ N_COLS ] );

// Clear the canvas by setting every pixel to be white.
void clearCanvas( int canvas[ N_ROWS ][ N_COLS ] );

// Calculate the distance between two points.
// Note: you will only need this function for the Draw Ellipse command
// in Stages 3 and 4.
double distance( int row1, int col1, int row2, int col2 );


// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
void ProcessCommand( const int nCommand, int kCanvasReference[ N_ROWS ][ N_COLS ] );
void ProcessCommandOne( int kCanvasReference[ N_ROWS ][ N_COLS ] );
EStatus ReadAndProcessInput( int* const pStartRow, int* const pStartColumn, int* const pLength, Point* pDrawDirection );
EStatus ConvertAngleToDrawDirection( const int nAngle, Point* pDrawDirection );
int GetSimplifiedAngle( const int nAngle );
EStatus ConvertSimplifiedAngleToDirection( const int nSimplifiedAngle, Point* pDirection );
bool IsSimplifiedAngle( const int nAngle );
void InvertLengthAndDrawDirectionIfIsNegativeLength( int* const pLength, Point* const pDrawDirection );
void InvertDirection( Point* const pDirection );
bool DrawEndPointIsInCanvas( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection );
Point GetDrawEndPoint( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection );
bool PointIsInCanvas( const Point kPoint );
void DrawLine( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection, int kCanvasReference[ N_ROWS ][ N_COLS ] );
void ProcessCommandTwo( int kCanvasReference[ N_ROWS ][ N_COLS ] );
void DoCommandTwo( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection, int kCanvasReference[ N_ROWS ][ N_COLS ] );
bool IsPositive( int nNumber );
Point GetDrawRectangleTopLeft( const Point kDrawStartPoint, const Point kDrawEndPoint );
Point GetDrawRectangleBottomRight( const Point kDrawStartPoint, const Point kDrawEndPoint );
int GetMin( const int nFirstNumber, const int nSecondNumber );
int GetMax( const int nFirstNumber, const int nSecondNumber );
void DrawRectangle( const Point kRectangleTopLeft, const Point kRectangleBottomRight, int kCanvasReference[ N_ROWS ][ N_COLS ] );
Point GetRightDirection();
void ProcessCommandThree();


int main( void )
{
    int canvas[ N_ROWS ][ N_COLS ];
    clearCanvas( canvas );

    int nCommand;
    while ( scanf( "%d", &nCommand ) != EOF )
    {
        ProcessCommand( nCommand, canvas );
        displayCanvas( canvas );
    }

    return 0;
}


// ADD CODE FOR YOUR FUNCTIONS HERE
void ProcessCommand( const int nCommand, int kCanvasReference[ N_ROWS ][ N_COLS ] )
{
    switch ( nCommand )
    {
        case 1:
            ProcessCommandOne( kCanvasReference );
            break;
        case 2:
            ProcessCommandTwo( kCanvasReference );
            break;
        case 3:
            ProcessCommandThree();
            break;
        default:
            // TODO: Stage 3 and 4.
            break;
    }
}

void ProcessCommandOne( int kCanvasReference[ N_ROWS ][ N_COLS ] )
{
    int nStartRow;
    int nStartColumn;
    int nLength;
    Point kDrawDirection;
    if ( ReadAndProcessInput( &nStartRow, &nStartColumn, &nLength, &kDrawDirection ) == ( EStatus ) IGNORING )
    {
        return;
    }
    DrawLine( nStartRow, nStartColumn, nLength, kDrawDirection, kCanvasReference );
}

EStatus ReadAndProcessInput( int* const pStartRow, int* const pStartColumn, int* const pLength, Point* pDrawDirection )
{
    int nAngle;
    scanf( "%d %d %d %d", pStartRow, pStartColumn, pLength, &nAngle );
    if ( ( *pLength ) == 0 )
    {
        return ( EStatus ) IGNORING;
    }
    if ( ConvertAngleToDrawDirection( nAngle, pDrawDirection ) == ( EStatus ) IGNORING )
    {
        return ( EStatus ) IGNORING;
    }
    InvertLengthAndDrawDirectionIfIsNegativeLength( pLength, pDrawDirection );
    if ( !DrawEndPointIsInCanvas( ( *pStartRow ), ( *pStartColumn), ( *pLength ), ( *pDrawDirection ) ) )
    {
        return ( EStatus ) IGNORING;
    }
    return ( EStatus ) SUCCESS;
}

EStatus ConvertAngleToDrawDirection( const int nAngle, Point* pDrawDirection )
{
    return ConvertSimplifiedAngleToDirection( GetSimplifiedAngle( nAngle ), pDrawDirection );
}

int GetSimplifiedAngle( const int nAngle )
{
    return nAngle % SIMPLIFIED_ANGLE_MAX_DEGREE;
}

EStatus ConvertSimplifiedAngleToDirection( const int nSimplifiedAngle, Point* pDirection )
{
    if ( !IsSimplifiedAngle( nSimplifiedAngle ) )
    {
        printf( "The given angle is not a simplified angle." );
        exit( EXIT_FAILURE );
    }
    switch ( nSimplifiedAngle )
    {
        case SIMPLIFIED_ANGLE_UP:
            SetPoint( pDirection, -1, 0 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_RIGHT:
            SetPoint( pDirection, 0, 1 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_DOWN:
            SetPoint( pDirection, 1, 0 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_LEFT:
            SetPoint( pDirection, 0, -1 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_RIGHT_UP:
            SetPoint( pDirection, -1, 1 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_RIGHT_DOWN:
            SetPoint( pDirection, 1, 1 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_LEFT_DOWN:
            SetPoint( pDirection, 1, -1 );
            return ( EStatus ) SUCCESS;
        case SIMPLIFIED_ANGLE_LEFT_UP:
            SetPoint( pDirection, -1, -1 );
            return ( EStatus ) SUCCESS;
        default:
            return ( EStatus ) IGNORING;
    }
}

bool IsSimplifiedAngle( const int nAngle )
{
    return nAngle <= SIMPLIFIED_ANGLE_MAX_DEGREE;
}

void InvertLengthAndDrawDirectionIfIsNegativeLength( int* const pLength, Point* const pDrawDirection )
{
    if ( ( *pLength ) < 0 )
    {
        ( *pLength ) = abs( *pLength );
        InvertDirection( pDrawDirection );
    }
}

void InvertDirection( Point* const pDirection )
{
    ( *pDirection ).m_nRow = -( *pDirection ).m_nRow;
    ( *pDirection ).m_nColumn = -( *pDirection ).m_nColumn;
}

bool DrawEndPointIsInCanvas( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection )
{
    if ( !IsPositive( nPositiveLength ) )
    {
        printf( "The given length is not positive." );
        exit( EXIT_FAILURE );
    }
    return PointIsInCanvas( GetDrawEndPoint( nStartRow, nStartColumn, nPositiveLength, kDrawDirection ) );
}

Point GetDrawEndPoint( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection )
{
    if ( !IsPositive( nPositiveLength ) )
    {
        printf( "The given length is not positive." );
        exit( EXIT_FAILURE );
    }
    int nOffsetRow = ( nPositiveLength - 1 ) * kDrawDirection.m_nRow;
    int nOffsetColumn = ( nPositiveLength - 1 ) * kDrawDirection.m_nColumn;
    Point kDrawEndPoint;
    SetPoint( &kDrawEndPoint, nStartRow + nOffsetRow, nStartColumn + nOffsetColumn );
    return kDrawEndPoint;
}

bool PointIsInCanvas( const Point kPoint )
{
    return MIN_ROW <= kPoint.m_nRow && kPoint.m_nRow <= MAX_ROW && MIN_COLUMN <= kPoint.m_nColumn && kPoint.m_nRow <= MAX_COLUMN;
}

void DrawLine( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection, int kCanvasReference[ N_ROWS ][ N_COLS ] )
{
    if ( !IsPositive( nPositiveLength ) )
    {
        printf( "The given length is not positive." );
        exit( EXIT_FAILURE );
    }
    int nIndex;
    for ( nIndex = 0; nIndex < nPositiveLength; nIndex++ )
    {
        kCanvasReference[ nStartRow + nIndex * kDrawDirection.m_nRow ][ nStartColumn + nIndex * kDrawDirection.m_nColumn ] = ( int ) g_eCurrentShade;
    }
}

void ProcessCommandTwo( int kCanvasReference[ N_ROWS ][ N_COLS ] )
{
    int nStartRow;
    int nStartColumn;
    int nLength;
    Point kDrawDirection;
    if ( ReadAndProcessInput( &nStartRow, &nStartColumn, &nLength, &kDrawDirection ) == ( EStatus ) IGNORING )
    {
        return;
    }
    DoCommandTwo( nStartRow, nStartColumn, nLength, kDrawDirection, kCanvasReference );
}

void DoCommandTwo( const int nStartRow, const int nStartColumn, const int nPositiveLength, const Point kDrawDirection, int kCanvasReference[ N_ROWS ][ N_COLS ] )
{
    if ( !IsPositive( nPositiveLength ) )
    {
        printf( "The given length is not positive." );
        exit( EXIT_FAILURE );
    }
    Point kStartPoint;
    SetPoint( &kStartPoint, nStartRow, nStartColumn );
    Point kEndPoint          = GetDrawEndPoint( nStartRow, nStartColumn, nPositiveLength, kDrawDirection );
    Point kRectangleTopLeft     = GetDrawRectangleTopLeft( kStartPoint, kEndPoint );
    Point kRectangleBottomRight = GetDrawRectangleBottomRight( kStartPoint, kEndPoint );
    DrawRectangle( kRectangleTopLeft, kRectangleBottomRight, kCanvasReference );
}

bool IsPositive( int nNumber )
{
    return nNumber > 0;
}

Point GetDrawRectangleTopLeft( const Point kDrawStartPoint, const Point kDrawEndPoint )
{
    int nRectangleTopLeftRow    = GetMin( kDrawStartPoint.m_nRow,    kDrawEndPoint.m_nRow );
    int nRectangleTopLeftColumn = GetMin( kDrawStartPoint.m_nColumn, kDrawEndPoint.m_nColumn );
    Point kRectangleTopLeft;
    SetPoint( &kRectangleTopLeft, nRectangleTopLeftRow, nRectangleTopLeftColumn );
    return kRectangleTopLeft;
}

Point GetDrawRectangleBottomRight( const Point kDrawStartPoint, const Point kDrawEndPoint )
{
    int nRectangleBottomRightRow    = GetMax( kDrawStartPoint.m_nRow,    kDrawEndPoint.m_nRow );
    int nRectangleBottomRightColumn = GetMax( kDrawStartPoint.m_nColumn, kDrawEndPoint.m_nColumn );
    Point kRectangleBottomRight;
    SetPoint( &kRectangleBottomRight, nRectangleBottomRightRow, nRectangleBottomRightColumn );
    return kRectangleBottomRight;
}

int GetMin( const int nFirstNumber, const int nSecondNumber )
{
    return nFirstNumber < nSecondNumber ? nFirstNumber : nSecondNumber;
}

int GetMax( const int nFirstNumber, const int nSecondNumber )
{
    return nFirstNumber > nSecondNumber ? nFirstNumber : nSecondNumber;
}

void DrawRectangle( const Point kRectangleTopLeft, const Point kRectangleBottomRight, int kCanvasReference[ N_ROWS ][ N_COLS ] )
{
    int nRectangleWidth = abs( kRectangleTopLeft.m_nColumn - kRectangleBottomRight.m_nColumn ) + 1;
    int nRow;
    for ( nRow = kRectangleTopLeft.m_nRow; nRow <= kRectangleBottomRight.m_nRow; nRow++ )
    {
        DrawLine( nRow, kRectangleTopLeft.m_nColumn, nRectangleWidth, GetRightDirection(), kCanvasReference );
    }
}

Point GetRightDirection()
{
    Point kDirection;
    SetPoint( &kDirection, 0, 1 );
    return kDirection;
}

void ProcessCommandThree()
{
    int nNewShadeOffset;
    scanf( "%d", &nNewShadeOffset );
    int nNewShade = ( int ) g_eCurrentShade + nNewShadeOffset;
    if ( !IsValidShade( nNewShade ) )
    {
        return;
    }
    g_eCurrentShade = ( EShade ) nNewShade;
}

bool IsValidShade( const int nShade )
{
    return 0 <= nShade && nShade <= 4;
}


// Displays the canvas, by printing the integer value stored in
// each element of the 2-dimensional canvas array.
//
// You should not need to change the displayCanvas function.
void displayCanvas( int canvas[ N_ROWS ][ N_COLS ] )
{
    int row = 0;
    while ( row < N_ROWS )
    {
        int col = 0;
        while ( col < N_COLS )
        {
            printf( "%d ", canvas[ row ][ col ] );
            col++;
        }
        row++;
        printf( "\n" );
    }
}


// Sets the entire canvas to be blank, by setting each element in the
// 2-dimensional canvas array to be WHITE (which is #defined at the top
// of the file).
//
// You should not need to change the clearCanvas function.
void clearCanvas( int canvas[ N_ROWS ][ N_COLS ] )
{
    int row = 0;
    while ( row < N_ROWS )
    {
        int col = 0;
        while ( col < N_COLS )
        {
            canvas[ row ][ col ] = WHITE;
            col++;
        }
        row++;
    }
}

// Calculate the distance between two points (row1, col1) and (row2, col2).
// Note: you will only need this function for the Draw Ellipse command
// in Stages 3 and 4.
double distance( int row1, int col1, int row2, int col2 )
{
    int row_dist = row2 - row1;
    int col_dist = col2 - col1;
    return sqrt( ( row_dist * row_dist ) + ( col_dist * col_dist ) );
}