#pragma once

#include <windows.h>
#include <string.h>
#include <iostream>

#include <commdlg.h>
using namespace std;

// Returns an empty string if dialog is canceled
string openfilename(HWND owner = NULL) {
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";
    LPWSTR filter = L"Text (*.txt*)\0*.txt*\0";
    LPWSTR FileExit = L"txt";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = FileExit;

    string fileNameStr;

    if (GetOpenFileName(&ofn))
    {
        wstring ws(fileName);
        string str(ws.begin(), ws.end());
        fileNameStr = str;
    }

    return fileNameStr;
}


string temp()
{
    string filename = "../ ObjectMatrix / GameLevel1.txt";

    return filename;
}

