#include <iostream>

#include <Windows.h>
#include <chrono>

#include <glfw3.h>

#include "Overlay/Overlay.h"
#include "Ping/Ping.h"

int main()
{

	Ping* ping = new Ping;
	Overlay* overlay = new Overlay(L"League of Legends", ping);

	ping->LoadPingData();

	if (!overlay->windowInitialized)
		return -1;

	auto t1 = std::chrono::steady_clock::now();
	auto t2 = t1;
	auto elapsed = t2 - t1;
	float dt = 1000.f;

	while (!overlay->ShouldClose()) {
		t1 = std::chrono::steady_clock::now();

		overlay->Update(dt);

		t2 = std::chrono::steady_clock::now();
		elapsed = t2 - t1;
		dt = (float)elapsed.count() / 1000000;
	}

	overlay->Shutdown();
	return 0;
}