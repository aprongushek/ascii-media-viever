#ifndef CRENDER_H
#define CRENDER_H

#include <windows.h>
#include <cstdio>

class ConsoleRender {
private:
	HANDLE mConsoleHandle;
	// HWND mWindowHandle;

	int mWidth;
	int mHeight;

	int mCurrent;
	char **mBuffers;

	void allocateBuffer ();
	void freeBuffer ();

public:
	ConsoleRender ();
	~ConsoleRender ();

	int getWidth ();
	int getHeight ();
	void getFontSize (int *width, int *height);

	void resize ();
	void clear ();
	void show ();
	
	void drawPoint (char filler, int x, int y);
	void drawBuffer (char *buffer, int width, int height, int x, int y);
};

#endif