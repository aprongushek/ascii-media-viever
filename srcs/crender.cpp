#include "crender.h"

#define DEFAULT_WIDTH 120
#define DEFAULT_HEIGHT 30

void ConsoleRender::allocateBuffer ()
{
	mBuffers = new char*[2];
	for (int i = 0; i < 2; i++) {
		mBuffers[i] = new char[mHeight * mWidth];
	}

	memset(mBuffers[0], ' ', mHeight * mWidth * sizeof(char));
}

void ConsoleRender::freeBuffer ()
{
	if (mBuffers != nullptr) {
		for (int i = 0; i < 2; i++) {
			delete[] mBuffers[i];
			mBuffers[i] = nullptr;
		}
		delete[] mBuffers;
	
		mBuffers = nullptr;
	}
}

ConsoleRender::ConsoleRender () : 
	mCurrent(0),
	mBuffers(nullptr)
{
	mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	// mWindowHandle = GetConsoleWindow();

	resize();

	system("cls");
}

ConsoleRender::~ConsoleRender () 
{
	CONSOLE_CURSOR_INFO	cursorInfo;
	GetConsoleCursorInfo(mConsoleHandle, &cursorInfo);
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(mConsoleHandle, &cursorInfo);

	freeBuffer();

	system("cls");
}

int ConsoleRender::getWidth ()
{
	return mWidth;
}

int ConsoleRender::getHeight ()
{
	return mHeight;
}

void ConsoleRender::getFontSize (int *width, int *height)
{
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(mConsoleHandle, false, &fontInfo);
	*width = fontInfo.dwFontSize.X;
	*height = fontInfo.dwFontSize.Y;
}

void ConsoleRender::resize ()
{
	freeBuffer();

	CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(mConsoleHandle, &scrBufferInfo);
    
    mWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
    mHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
	
	SetConsoleScreenBufferSize(mConsoleHandle, {mWidth, mHeight});

	CONSOLE_CURSOR_INFO	cursorInfo;
	GetConsoleCursorInfo(mConsoleHandle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(mConsoleHandle, &cursorInfo);

	allocateBuffer();
}

void ConsoleRender::clear ()
{
	memset(mBuffers[mCurrent], ' ', mHeight * mWidth * sizeof(char));
}

void ConsoleRender::show ()
{
	SetConsoleCursorPosition(mConsoleHandle, {0, 0});
	fwrite(mBuffers[mCurrent], mHeight * mWidth * sizeof(char), 1, stderr);

	mCurrent = 1 - mCurrent;
}

void ConsoleRender::drawPoint (char filler, int x, int y)
{
	mBuffers[mCurrent][(y * mWidth) + x] = filler;
}

void ConsoleRender::drawBuffer (char *buffer, int width, int height, int x, int y)
{
	for (int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			mBuffers[mCurrent][((y + i) * mWidth) + x + j] = buffer[(i * width) + j];
}