#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>
#include <map>
#include "UsefulFunctions.h"

using namespace std;
// Global variables
#define IDM_FILE_SAVE 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define IDM_EDIT_Greyscale 4
#define IDM_EDIT_FlipHorizontal 5
#define IDM_EDIT_FlipVertical 6
#define IDM_EDIT_FilterRed 7
#define IDM_EDIT_FilterGreen 8
#define IDM_EDIT_FilterBlue 9
#define IDM_EDIT_Reset 10
#define IDM_EDIT_Blur 11
string current_file;
// The main window class name.
Image image;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

    HMENU hMenubar = CreateMenu(); // The main menu bar
    HMENU hMenu = CreateMenu(); // the file menu
    HMENU Alter = CreateMenu(); // the edit menu

    AppendMenuW(hMenu, MF_STRING, 2, L"&Open");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_SAVE, L"&Save As");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(Alter, MF_STRING, IDM_EDIT_Greyscale, L"&Greyscale"); // Copy this line to add
                                                                                                 // more items to the Edit menu;
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_FlipHorizontal, L"&Flip Horizontal"); // Copy this line to add
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_FlipVertical, L"&Flip Vertical"); // Copy this line to add
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_FilterRed, L"&Show Only Red"); // Copy this line to add
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_FilterGreen, L"&Show Only Green"); // Copy this line to add
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_FilterBlue, L"&Show Only Blue"); // Copy this line to add
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_Blur, L"&Blur"); // Copy this line to add
    AppendMenuW(Alter, MF_STRING, IDM_EDIT_Reset, L"&Reset"); // Copy this line to add



    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)Alter, L"&Edit");

    return createWindow(hInstance, nCmdShow, hMenubar);


}

void processMenu(HWND hWnd, WPARAM wParam)
{
    switch(LOWORD(wParam)) {
        case IDM_FILE_OPEN:
            current_file = openfilename("Image(*.ppm)", hWnd);
            image.load(current_file);
            break;
        case IDM_FILE_SAVE: {
            string f = saveFilename("Image(*.ppm)", hWnd);
            image.savePPM(f);
            break;
        }
        case IDM_EDIT_Greyscale:
            image.load(current_file);
            image.greyScale();
             break;
        case IDM_EDIT_FlipHorizontal:
            //image.load(current_file);
            image.flipHorizontal();
            break;
        case IDM_EDIT_FlipVertical:
             image.flipVertically();
             break;
        case IDM_EDIT_FilterRed:
             image.load(current_file);
             image.filterRed();
             break;
        case IDM_EDIT_FilterGreen:
            image.load(current_file);
            image.filterGreen();
            break;
        case IDM_EDIT_FilterBlue:
            image.load(current_file);
            image.filterBlue();
            break;
        case IDM_EDIT_Blur:
           // image.load(current_file);
            image.blur();
            break;
        case IDM_EDIT_Reset:
            image.load(current_file);

            break;
        case IDM_FILE_QUIT:

            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
    }
    SetWindowPos(hWnd, NULL, 0,0,image.getWidth()+20, image.getHeight()+60,SWP_NOMOVE);
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");
    int r, c;
    Rgb* pixels;
    RECT *rect = new RECT();


    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
/*
*/
            rect->top = 0;
            rect->left=0;
            rect->bottom = image.getHeight();
            rect->right = image.getWidth();
            dblBuffer(hWnd, hdc, rect, image);



// delete memory DC since it is no longer required

            EndPaint(hWnd, &ps);
            break;
        case WM_MENUCOMMAND:
        case WM_COMMAND:

            processMenu(hWnd, wParam);

            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }

    return 0;
}