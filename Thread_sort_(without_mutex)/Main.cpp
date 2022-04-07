#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <ctime>
#include <algorithm>
#include <string>
#include "resource.h"

#define WM_MYUPDATE WM_USER +1

HINSTANCE hInst;
HWND hMainDlg = nullptr;
HANDLE hMutex = nullptr;

int* data = nullptr;
const int dataSize = 100000;
int check;

TCHAR BubbleSortText[256] = TEXT("Not running");
TCHAR SelectSortText[256] = TEXT("Not running");
TCHAR QuickSortText[256] = TEXT("Not running");
char buff1[10];
char buff2[10];
char buff3[10];
char* time_of_Bubble;
char* time_of_Select;
char* time_of_Quick;

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void FillArray(int* arr, int size)
{
	for (int i = 0; i < size; ++i)
		arr[i] = rand() % 100000;
}

void BubbleSort(int* arr, int size)
{
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = 0; j < size - 1; ++j)
		{
			if (arr[j + 1] < arr[j])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void SelectSort(int* arr, int size)
{
	int tmp;
	for (int i = 0; i < size; ++i)
	{
		int pos = i;
		tmp = arr[i];
		for (int j = i + 1; j < size; ++j)
		{
			if (arr[j] < tmp)
			{
				pos = j;
				tmp = arr[j];
			}
		}
		arr[pos] = arr[i];
		arr[i] = tmp; 
	}
}

void QuickSort(int* a, int N)
{
	int i = 0, j = N;    
	int temp, p;
	p = a[N >> 1];
	
	do {
		while (a[i] < p) i++;
		while (a[j] > p) j--;
		
		if (i <= j) {
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++; j--;
		}
	} while (i <= j);

	if (j > 0) QuickSort(a, j);
	if (N > i) QuickSort(a + i, N - i);
}

DWORD WINAPI BubbleSortFunc(LPVOID lpParam)
{
	_tcscpy_s(BubbleSortText, TEXT("running..."));
	PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
	FillArray(data, dataSize);

	clock_t start = clock();
	BubbleSort(data, dataSize);
	clock_t stop = clock();
	
	int time = static_cast<int>(stop - start);
	time_of_Bubble = _itoa(time, buff1, 10);
	
	if (std::is_sorted(data, data + dataSize))
	{
		_tcscpy_s(BubbleSortText, TEXT("Succeded.   "));
		check++;
	}
	else 
		_tcscpy_s(BubbleSortText, TEXT("Failed. "));

	PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
	return 0;
}

DWORD WINAPI SelectSortFunc(LPVOID lpParam)
{
	_tcscpy_s(SelectSortText, TEXT("running..."));
	PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
	FillArray(data, dataSize);

	clock_t start = clock();
	SelectSort(data, dataSize);
	clock_t stop = clock();

	int time = static_cast<int>(stop - start);
	time_of_Select = _itoa(time, buff2, 10);

	if (std::is_sorted(data, data + dataSize))
	{
		_tcscpy_s(SelectSortText, TEXT("Succeded.   "));
		check++;
	}
	else
		_tcscpy_s(SelectSortText, TEXT("Failed. "));

	PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
	return 0;
}

DWORD WINAPI QuickSortFunc(LPVOID lpParam)
{
	_tcscpy_s(QuickSortText, TEXT("running..."));
	PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
	FillArray(data, dataSize);

	clock_t start = clock();
	SelectSort(data, dataSize);
	clock_t stop = clock();

	int time = static_cast<int>(stop - start);
	time_of_Quick = _itoa(time, buff3, 10);

	if (std::is_sorted(data, data + dataSize))
	{
		_tcscpy_s(QuickSortText, TEXT("Succeded.   "));
		check++;
	}
	else
		_tcscpy_s(QuickSortText, TEXT("Failed. "));

	PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
	return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hMainDlg = hwnd;
		data = new int[dataSize];
		PostMessage(hMainDlg, WM_MYUPDATE, 0, 0);
		break;

	case WM_MYUPDATE:
		SetDlgItemText(hwnd, 1004, BubbleSortText);
		SetDlgItemText(hwnd, 1005, SelectSortText);
		SetDlgItemText(hwnd, 1008, QuickSortText);
		SetDlgItemText(hwnd, 1012, time_of_Bubble);
		SetDlgItemText(hwnd, 1013, time_of_Select);
		SetDlgItemText(hwnd, 1014, time_of_Quick);
		
		if (check == 3)
		{
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), 1);
			check = 0;
		}
		break;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwnd, LOWORD(wParam));
			break;
		case IDC_BUTTON1:
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), 0);
			
			CreateThread(0, 0, BubbleSortFunc, 0, 0, 0);
			CreateThread(0, 0, SelectSortFunc, 0, 0, 0);
			CreateThread(0, 0, QuickSortFunc, 0, 0, 0);
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return FALSE;
	}
	return FALSE;
}