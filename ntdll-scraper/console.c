
#include "console.h"

void printColor(const WORD wColor, const LPSTR fmt,... ){
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(
		hOutput,
		&info);
	SetConsoleTextAttribute(
		hOutput,
		wColor);
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	SetConsoleTextAttribute(
		hOutput,
		info.wAttributes);
}

void PrintError(const LPSTR fmt, ...){
	printColor(CONSOLE_WHITE, "[");
	printColor(CONSOLE_RED, "-");
	printColor(CONSOLE_WHITE,"] ");
	va_list args;
	va_start(args, fmt);
	printColor(CONSOLE_WHITE,fmt,args);
	va_end(args);
}
void PrintSuccess(const LPSTR fmt, ...){
	printColor(CONSOLE_WHITE, "[");
	printColor(CONSOLE_GREEN, "+");
	printColor(CONSOLE_WHITE,"] ");
	va_list args;
	va_start(args, fmt);
	printColor(CONSOLE_WHITE,fmt,args);
	va_end(args);
}
