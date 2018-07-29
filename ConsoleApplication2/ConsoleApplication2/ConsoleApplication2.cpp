// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifndef WINVER  
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <atlstr.h>
#include <locale>
using namespace std;

INPUT ip;
string currentItem = "Tunguska";
string ourItem = "";
string stop = "";
static int howmanytimes = 0;
static int skipWait = 0;
static int linePrev;
string passedItem = "";
string killFiletxt = "";
queue <string> ourqueue;
static string killFile = "C:\\Users\\blacktavius\\Desktop\\Bot\\KillFile.txt";
static string gunFile = "C:\\Users\\blacktavius\\Desktop\\Bot\\guns.txt";
static string gunListFile = "C:\\Users\\blacktavius\\Desktop\\Bot\\gunList.txt";
static string toText = "D:\\Steam\\SteamApps\\common\\Borderlands 2\\Binaries\\toText.txt";
static string OBS = "C:\\Users\\blacktavius\\Desktop\\Bot\\OBS.txt";
static string poppedgun;
string hello;


string readLine(string path, int loops) {
	string line = "";
	ifstream in(path);
	for (int i = 0; i <= loops; i++) {
		getline(in, line);
	}
	return line;
}

string toLowerCase(string upper) {
	locale loc;
	string lower;
	for (string::size_type i = 0; i < upper.length(); i++) {
		lower = tolower(upper[i], loc);
	}
	return lower;
}

int readFileGun(string path, string item) {
	int lineNum = 0;
	string line = "";
	ifstream in(path);
	while (!in.eof()) {
		lineNum++;
		getline(in, line);
		//cout << line + "+" + item + "  ";
		//line = toLowerCase(line);
		//item = toLowerCase(item);
		if (line.compare(item) == 0) {
			return lineNum;
			//cout << "*********FOUND********";
		}
	}
	return -1;

}

void writeFile(string path, string write) {
	ofstream file;
	file.open(path);
	file << write;
	file.close();
}

void keyPress(int key) {
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = key; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}



void Console() {   //should see if we can type things
	keyPress(0xC0);
	keyPress(0x26);
	keyPress(0x0D);
	keyPress(0xC0);
}

void gunSlot(int i) {
	keyPress(0x49);
	Sleep(550);
	for (int j = 0; j < 5; j++) {
		keyPress(0x26);
	}
	keyPress(0x0D);
	keyPress(0x0D);
	keyPress(0x0D);
	for (int j = 0; j<i; j++) {
		keyPress(0x28);
	}
	keyPress(0x0D);
	keyPress(0x1B);
}

void shieldSlot(int i) {
	keyPress(0x49);
	Sleep(550);
	for (int j = 0; j < 5; j++) {
		keyPress(0x26);
	}
	for (int j = 0; j < 4; j++) {
		keyPress(0x28);
	}
	keyPress(0x0D);
	keyPress(0x0D);
	keyPress(0x0D);
	for (int j = 0; j<i; j++) {
		keyPress(0x28);
	}
	keyPress(0x0D);
	keyPress(0x1B);
}

void grenadeSlot(int i) {
	keyPress(0x49);
	Sleep(550);
	for (int j = 0; j < 5; j++) {
		keyPress(0x26);
	}
	for (int j = 0; j < 10; j++) {
		keyPress(0x28);
	}
	keyPress(0x0D);
	keyPress(0x0D);
	keyPress(0x0D);
	for (int j = 0; j<i; j++) {
		keyPress(0x28);
	}
	keyPress(0x0D);
	keyPress(0x1B);
}

string GetActiveWindowTitle()
{
	TCHAR wnd_title[256];
	HWND hwnd = GetForegroundWindow();
	GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
	string myString = CW2A(wnd_title);
	return myString;
}

void gunSwap() {
	if (skipWait == 0) {
		for (int counter = 0; counter < 100; counter++) {
			hello = GetActiveWindowTitle();
			ourItem = readLine(gunFile, 0);
			string itemPart1 = ourItem.substr(ourItem.find(",") + 1, ourItem.length() - 1);
			int pass = readFileGun(gunListFile, itemPart1);
			if (ourItem.compare(passedItem) != 0) {
				if (pass != -1) {
					passedItem = ourItem;
					ourqueue.push(ourItem);
				}
			}
			Sleep(140);
		}
	}

	queue <string> fakequeue;
	fakequeue = ourqueue;
	int textList = 0;
	string fullTerm;
	while (!fakequeue.empty()) {
		cout << fakequeue.front() + "\n";
		string newTerm = fakequeue.front();
		fakequeue.pop();
		if (textList < 4) {
			fullTerm = fullTerm + "\n" + newTerm;
			textList++;
		}
		writeFile(OBS, fullTerm);

	}
	if (ourqueue.empty()) {
		writeFile(OBS, "\n" + poppedgun);
	}
	cout << "------------------------ \n";

	if (hello.compare("Borderlands 2 (32-bit, DX9)") == 0) {
		if (!ourqueue.empty()) {
			string popGun = ourqueue.front();
			poppedgun = popGun;
			ourqueue.pop();
			string namePart = popGun.substr(0, popGun.find(","));
			string itemPart = popGun.substr(popGun.find(",")+1, popGun.length()-1);
			int lineMatch = readFileGun(gunListFile, itemPart) - 1;
			//cout << "MADEITHERE ";
			if ((lineMatch >= 0) && (lineMatch != linePrev)) {
				string say = "say " + itemPart + " was chosen by " + namePart;
				writeFile(toText, say);
				if (lineMatch < 76) {
					currentItem = itemPart;
					Console();
					gunSlot(lineMatch);
					skipWait = 0;
					cout << " GunSlot";
				}
				else if ((lineMatch > 76) && (lineMatch < 91)) {
					currentItem = itemPart;
					Console();
					shieldSlot(lineMatch - 76);
					skipWait = 0;
					cout << " ShieldSlot";
				}
				else {
					currentItem = itemPart;
					Console();
					grenadeSlot(lineMatch - 91);
					skipWait = 0;
				}
			}
			else {
				itemPart = itemPart;
				skipWait = 0;
				cout << "Gun not on list";
			}
			linePrev = lineMatch;
		}
		else {
			skipWait = 0;
		}
	}
}

int main()
{
	int x = 1;
	cout << "hello";
	while (x == 1) {
		gunSwap();
	}
	return 0;
}
