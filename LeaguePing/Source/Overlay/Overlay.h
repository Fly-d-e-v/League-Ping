#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32

#define DRAW_STATE_PING 0
#define DRAW_STATE_GRAPH 1

#include <string>
#include <glfw3.h>
#include <glfw3native.h>
#include <gdiplus.h>

class Ping;

struct PingTextData {
	Gdiplus::SolidBrush* solidBrush;
	Gdiplus::FontFamily* fontFamily;
	Gdiplus::Font* font;
	Gdiplus::PointF* point;
	Gdiplus::Pen* pen;
};

class Overlay {
public:
	Overlay(LPCWSTR windowName, Ping* p);

	int ShouldClose();
	void Update(float dt);
	void Draw();
	void HandleInput();

	void Shutdown();

public:
	bool windowInitialized = false;

private:
	bool UpdateWindow();

private:
	GLFWwindow* window;
	HWND winWindow;
	HWND targetWindow;
	LPCWSTR name;

	int windowWidth;
	int windowHeight;
	int windowPosX;
	int windowPosY;

	PingTextData textData;
	HDC hdc;
	Gdiplus::Graphics* graphics;

	int drawState = DRAW_STATE_PING;

	Gdiplus::GdiplusStartupInput gdiStartInput;
	ULONG_PTR gdiToken;

	float elapsedTime = 0.f;
	bool callonce = true;

	float lastPress = -1.f;

	Ping* ping;
};