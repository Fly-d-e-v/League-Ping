#include <Windows.h>

#pragma comment (lib,"Gdiplus.lib")

#include <iostream>

#include "Overlay.h"
#include "../Ping/Ping.h"


Overlay::Overlay(LPCWSTR windowName, Ping* p) {
	glfwInit();
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	windowWidth = 30;
	windowHeight = 15;

	window = glfwCreateWindow(windowWidth, windowHeight, "League Ping", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window\n";
		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(window);

	winWindow = glfwGetWin32Window(window);
	windowInitialized = true;

	////////////////////////////////////////////////////////////////////////////

	name = windowName;
	ping = p;

	if (UpdateWindow()) {
		SetParent(winWindow, targetWindow);
		// TODO: ASPECT RATIO
		windowPosX = 1290;
		windowPosY = 100;
		glfwSetWindowPos(window, windowPosX, windowPosY);
	}

	Gdiplus::GdiplusStartup(&gdiToken, &gdiStartInput, NULL);

	hdc = GetWindowDC(winWindow);
	graphics = new Gdiplus::Graphics(hdc);

	// Ping draw text Data
	textData.solidBrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 44, 222, 44));
	textData.fontFamily = new Gdiplus::FontFamily(L"Arial");
	textData.font = new Gdiplus::Font(textData.fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	textData.point = new Gdiplus::PointF(1.0f, 1.0f);
	textData.pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 255));
}

int Overlay::ShouldClose() {
	return glfwWindowShouldClose(window);
}

void Overlay::Update(float dt) {
	elapsedTime += dt;

	if (lastPress >= 0) {
		lastPress += dt;

		if (lastPress > 1000)
			lastPress = -1.f;
	}

	HandleInput();

	if (elapsedTime >= 200) {
		Draw();
		elapsedTime = 0;
	}

	glfwPollEvents();
}

void Overlay::Draw() {
	graphics->Clear(Gdiplus::Color(255, 0, 0, 0));

	switch (drawState) {
	case DRAW_STATE_PING:
		graphics->DrawString(std::to_wstring(ping->GetLogPing()).c_str(), -1, textData.font, *textData.point, textData.solidBrush);
		break;
	case DRAW_STATE_GRAPH:
		graphics->DrawLine(textData.pen, 4, 4, 4, 140);
		break;
	}
		
}

void Overlay::HandleInput() {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS && lastPress < 0)
	{
		lastPress = 0.f;

		switch (drawState) {
		case DRAW_STATE_PING:
			glfwSetWindowSize(window, 220, 160);
			glfwSetWindowPos(window, windowPosX - (220 - windowWidth), windowPosY);
			glfwSwapBuffers(window);
			drawState = DRAW_STATE_GRAPH;
			break;
		case DRAW_STATE_GRAPH:
			glfwSetWindowSize(window, windowWidth, windowHeight);
			glfwSetWindowPos(window, windowPosX, windowPosY);
			glfwSwapBuffers(window);
			drawState = DRAW_STATE_PING;
			break;
		}

		hdc = GetWindowDC(winWindow);
		delete(graphics);
		graphics = new Gdiplus::Graphics(hdc);
	}
}

bool Overlay::UpdateWindow() {
	targetWindow = FindWindow(NULL, name);

	std::cout << targetWindow << std::endl;

	return (targetWindow != nullptr);
}


void Overlay::Shutdown() {
	glfwTerminate();
}