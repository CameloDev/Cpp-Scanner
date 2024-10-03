//ImGui 
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

// Custom ImGui Addition

#include "imguipp_v2.h"

// Bytes
#include "logo.h"

// Important
#include "main.h"
#include "globals.h"
#include <winsvc.h>

// Font and Icon related
#include "font.h"
#include "icons.h"

// D3DX


// Variables
#include <codecvt>
#include <locale> 
#include <wchar.h>
#include <TlHelp32.h>  
#include <string>
#include <winhttp.h>
#include <vector>
#include <random>
#include <math.h>
#include "imgui_custom.hpp"
#include <filesystem>
#include "imgui/imgui_internal.h"
#pragma comment(lib, "urlmon.lib")

#include <Shlwapi.h>
#include <dwmapi.h>
#include "ImGui/imstb_truetype.h"
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <Psapi.h>
#include <thread>
#include <stdio.h>
#include <tchar.h>
#include<fstream>
#include <regex>
#include <stdlib.h>
#include <string.h>
#include <dbghelp.h>
#include <curl/curl.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment (lib, "dbghelp.lib")
#pragma comment(lib, "Shlwapi.lib")


namespace fs = std::filesystem;
//  ⬇ Particulas ⬇
bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = size_arg;
	size.x -= style.FramePadding.x * 2;

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	const float circleStart = size.x * 0.7f;
	const float circleEnd = size.x;
	const float circleWidth = circleEnd - circleStart;

	ImU32 gradient_start = ImColor(255, 255, 255, 255); 
	ImU32 gradient_end = ImColor(0, 0, 0, 255); 


	window->DrawList->AddRectFilledMultiColor(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), gradient_start, gradient_start, gradient_end, gradient_end);


	window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

	const float t = g.Time;
	const float r = size.y / 2;
	const float speed = 6.5f;

	const float a = speed * 0;
	const float b = speed * 0.333f;
	const float c = speed * 0.666f;

	const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
	const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
	const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

	window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
	window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
	window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

	return true;
}

bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;
	window->DrawList->PathClear();

	int num_segments = 30;
	int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

	const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

	for (int i = 0; i < num_segments; i++) {
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
			centre.y + ImSin(a + g.Time * 8) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);
	return true;
}

std::vector<ImVec2> circles_pos;
std::vector<ImVec2> circles_dir;
std::vector<int> circles_radius;

void draw_circle(ImVec2 pos, int radius, ImU32 color)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	unsigned char* color_ptr = (unsigned char*)&color;
	draw_list->AddCircleFilled(pos, radius - 1, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], 75));
}

void draw_line(ImVec2 pos1, ImVec2 pos2, ImU32 color, int radius)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float distance = std::sqrt(
		std::pow(pos2.x - pos1.x, 2) +
		std::pow(pos2.y - pos1.y, 2)
	);
	float alpha;
	if (distance <= 20.0f) {
		alpha = 255.0f;
	}
	else {
		alpha = (1.0f - ((distance - 20.0f) / 25.0f)) * 255.0f;
	}

	int r = (color & 0xFF0000) >> 16;
	int g = (color & 0x00FF00) >> 8;
	int b = (color & 0x0000FF);

	unsigned char* color_ptr = (unsigned char*)&color;

	draw_list->AddLine(pos1, pos2, IM_COL32(color_ptr[9], color_ptr[9], color_ptr[9], static_cast<int>(alpha)), 1.0f);
	if (distance >= 40.0f) {
		draw_list->AddCircleFilled(pos1, radius - 0.96f, IM_COL32(color_ptr[9], color_ptr[9], color_ptr[9], alpha * 200.0f));
		draw_list->AddCircleFilled(pos2, radius - 0.96f, IM_COL32(color_ptr[9], color_ptr[9], color_ptr[9], alpha * 200.0f));
	}
	else if (distance <= 20.0f) {
		draw_list->AddCircleFilled(pos1, radius, IM_COL32(color_ptr[2], color_ptr[2], color_ptr[2], alpha * 200.0f));
		draw_list->AddCircleFilled(pos2, radius, IM_COL32(color_ptr[9], color_ptr[9], color_ptr[9], alpha * 200.0f));
	}
	else {
		float radius_factor = 1.0f - ((distance - 20.0f) / 20.0f);
		float offset_factor = 1.0f - radius_factor;
		float offset = (radius - radius * radius_factor) * offset_factor;
		draw_list->AddCircleFilled(pos1, radius - offset, IM_COL32(color_ptr[9], color_ptr[9], color_ptr[9], alpha * 200.0f));
		draw_list->AddCircleFilled(pos2, radius - offset, IM_COL32(color_ptr[9], color_ptr[9], color_ptr[9], alpha * 200.0f));
	}
}

void move_circles(ImVec2 aaaa)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 360);

	ImVec2 window_size = ImGui::GetWindowSize();

	for (int i = 0; i < circles_pos.size(); i++)
	{
		ImVec2& pos = circles_pos[i];
		ImVec2& dir = circles_dir[i];
		int radius = circles_radius[i];

		pos.x += dir.x * 0.4;
		pos.y += dir.y * 0.4;

		if (pos.x - radius < 0 || pos.x + radius > aaaa.x + window_size.x)
		{
			dir.x = -dir.x;
			dir.y = dis(gen) % 2 == 0 ? -1 : 1;
		}

		if (pos.y - radius < 0 || pos.y + radius >  aaaa.y + window_size.y)
		{
			dir.y = -dir.y;
			dir.x = dis(gen) % 2 == 0 ? -1 : 1;
		}
	}
}

void draw_circles_and_lines(ImU32 color, ImVec2 aaa)
{
	move_circles(aaa);

	for (int i = 0; i < circles_pos.size(); i++)
	{
		draw_circle(circles_pos[i], circles_radius[i], color);

		for (int j = i + 1; j < circles_pos.size(); j++)
		{
			float distance = ImGui::GetIO().FontGlobalScale * std::sqrt(
				std::pow(circles_pos[j].x - circles_pos[i].x, 2) +
				std::pow(circles_pos[j].y - circles_pos[i].y, 2)
			);

			if (distance <= 45.0f)
			{
				draw_line(circles_pos[i], circles_pos[j], color, circles_radius[i]);
			}
		}
	}
}

void setup_circles()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	std::uniform_int_distribution<> pos_dis(0, static_cast<int>(1920));
	std::uniform_int_distribution<> pos_dis_y(0, static_cast<int>(1080));

	for (int i = 0; i < 300; i++)
	{
		circles_pos.push_back(ImVec2(pos_dis(gen), pos_dis_y(gen)));
		circles_dir.push_back(ImVec2(dis(gen) == 0 ? -1 : 1, dis(gen) == 0 ? -1 : 1));
		circles_radius.push_back(3);
	}
}
// ⬆ Particulas ⬆ trem dificil de fazer
std::wstring conversor(const std::string& narrowString) {
	int length = MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, nullptr, 0);
	if (length == 0) {
	
		return L"";
	}
	std::wstring wideString(length, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, &wideString[0], length);
	return wideString;
}

namespace idk
{
	static int Tab = 0;
}

const char* label = "Seja Bem Vindo"; //IMGUI Funcionando :D
float radius = 30.0f; 
float thickness = 3.0f;  
ImColor color = ImColor(0, 0, 0); 
float speed = 3.0f; 
float angle = IM_PI / 2.0f; 

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	std::ofstream* outFile = static_cast<std::ofstream*>(userp);
	size_t totalSize = size * nmemb;
	outFile->write(static_cast<char*>(contents), totalSize);
	return totalSize;
}

void StringColetor() {
	system("curl https://github.com/CameloDev/testing/raw/main/strings.exe --output C:\\strings.exe"); // tenho que fazer mensagem de erro !!
} //curl nao ta me ajudando :(
DWORD GetServiceProcessId(const std::wstring& serviceName, SC_HANDLE serviceManager) {
	if (!serviceManager)
		return 0;

	SC_HANDLE service = OpenService(serviceManager, serviceName.c_str(), SERVICE_QUERY_STATUS);
	if (!service) {
		CloseServiceHandle(serviceManager);
		return 0;
	}
	DWORD bytesNeeded;
	SERVICE_STATUS_PROCESS status;
	if (!QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&status), sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
		CloseServiceHandle(service);
		CloseServiceHandle(serviceManager);
		return 0;
	}

	DWORD processId = status.dwProcessId;

	CloseServiceHandle(service);
	CloseServiceHandle(serviceManager);

	return processId;
}

void executacomandocmd(const std::wstring& command) {
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if (CreateProcessW(NULL, const_cast<LPWSTR>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
void coletorzin() {
	
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

	
	if (!hSCManager) {
		std::cerr << "Nao instalou o bglh" << std::endl;
		return;
	}


	int explorerPID = 0, lsassPID = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pe32)) {
		do {
			if (_wcsicmp(pe32.szExeFile, L"explorer.exe") == 0) {
				explorerPID = pe32.th32ProcessID;
			}
			else if (_wcsicmp(pe32.szExeFile, L"lsass.exe") == 0) {
				lsassPID = pe32.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);


	int dnscache = 0, dps = 0, diagtrack = 0, sysmain = 0, pcasvc = 0;
	dnscache = GetServiceProcessId(L"Dnscache", hSCManager);
	dps = GetServiceProcessId(L"DPS", hSCManager);
	diagtrack = GetServiceProcessId(L"DiagTrack", hSCManager);
	sysmain = GetServiceProcessId(L"SysMain", hSCManager);
	pcasvc = GetServiceProcessId(L"PcaSvc", hSCManager);


	std::wstring commandDnscache = L"cd C:\\ && strings.exe -pid " + std::to_wstring(dnscache) + L" -raw -nh > C:\\Dnscache.txt";
	std::wstring commandDps = L"cd C:\\ && strings.exe -pid " + std::to_wstring(dps) + L" -raw -nh > C:\\dps.txt";
	std::wstring commandDiagtrack = L"cd C:\\ && strings.exe -pid " + std::to_wstring(diagtrack) + L" -raw -nh > C:\\Diagtrack.txt";
	std::wstring commandSysMain = L"cd C:\\ && strings.exe -pid " + std::to_wstring(sysmain) + L" -raw -nh > C:\\sysmain.txt";
	std::wstring commandPcaSvc = L"cd C:\\ && strings.exe -pid " + std::to_wstring(pcasvc) + L" -raw -nh > C:\\PcaSvc.txt";
	std::wstring commandExplorer = L"cd C:\\ && strings.exe -pid " + std::to_wstring(explorerPID) + L" -raw -nh > C:\\explorer.txt";
	std::wstring commandLsass = L"cd C:\\ && strings.exe -pid " + std::to_wstring(lsassPID) + L" -raw -nh > C:\\Lsass.txt";

	executacomandocmd(commandDnscache);
	executacomandocmd(commandDps);
	executacomandocmd(commandDiagtrack);
	executacomandocmd(commandSysMain);
	executacomandocmd(commandPcaSvc);
	executacomandocmd(commandExplorer);
	executacomandocmd(commandLsass);


	std::vector<std::wstring> historyFiles = {
		L"LocalAppData\\Microsoft\\Edge\\User Data\\Default\\History",
		L"LocalAppData\\Google\\Chrome\\User Data\\Default\\History",
		L"AppData\\Opera Software\\Opera Stable\\History" //Pegar historico dos navegadores
	};

	std::wstring filesaida = L"C:\\Historico.txt";

	
std::wofstream ofs(filesaida);
if (ofs.is_open()) {
    for (const auto& file : historyFiles) {
        std::wifstream ifs(file);
        if (ifs.is_open()) {
            std::wstring line;
            while (std::getline(ifs, line)) {
                ofs << line << std::endl;
            }
            ifs.close(); 
        } else {
            std::wcerr << L"Não foi possível abrir o arquivo: " << file << std::endl;
        }
    }
    ofs.close(); 
} else {
    std::wcerr << L"Não foi possível abrir o arquivo de saída: " << filesaida << std::endl;
}


	Sleep(3000);
}
void excluirarquivos() 
{

}
std::wstring pegardatetime() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	wchar_t buffer[50];
	swprintf_s(buffer, L"%02d/%02d/%04d %02d:%02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	return buffer;
}
std::wstring Result = L"https://discord.com/api/webhooks/1248735294208081921/hMJ8gNKLoxXc-l4EqmQEUCrNGqeQ1hn_A0yLqMPUmVpF7mZEKbtHdioe47TmCUTgnBcu";
std::wstring Pin = L"https://discord.com/api/webhooks/1245600643926982768/KkuTY-6zTc2vY4us6AHTd0PkA0bUw7ue7ucOmMH4-tOg9lgjcye035PY0llLLyt2LmC4";

void enviarwebhook(const std::wstring& pin) {
	
	std::wstring pinFormatado = L"*||" + pin + L"||*";
	std::wstring HWID = L"__HWID_AQUI__"; 
	std::wstring PCNAME = L"__NOME_DO_PC_AQUI__"; 
	std::wstring UserS = L"__USUARIO_AQUI__"; 
	std::wstring currentDateTime = pegardatetime();


	HINTERNET hSession = WinHttpOpen(L"React Scanner/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,
		0);
	if (!hSession) {
		std::cerr << "o WinHTTP nao funfa." << std::endl;
		return;
	}

	HINTERNET hConnect = WinHttpConnect(hSession,
		L"discordapp.com",
		INTERNET_DEFAULT_HTTPS_PORT,
		0);
	if (!hConnect) {
		std::cerr << "nao mandou webhook." << std::endl;
		WinHttpCloseHandle(hSession);
		return;
	}

	std::wstring requestBody = L"{\"username\": \"React Scanner\","
		L"\"avatar_url\": \"https://cdn.discordapp.com/attachments/1232015922424713267/1236330340323295343/pngwing.com_3.png\","
		L"\"embeds\": [{"
		L"\"title\": \"Pin - React Scanner\","
		L"\"description\": \">  **Usuario:** " + UserS + L"\\n > **Nome PC:** " + PCNAME +
		L"\\n > ***Data:*** " + currentDateTime + L"\\n > ***Seu Pin:*** " + pinFormatado + L"\","
		L"\"color\": 4886754,"
		L"\"footer\": {"
		L"\"text\": \"React Scanner - Gerador\","
		L"\"icon_url\": \"https://cdn.discordapp.com/attachments/1232015922424713267/1236330340323295343/pngwing.com_3.png\""
		L"}"
		L"}]"
		L"}";

	HINTERNET hRequest = WinHttpOpenRequest(hConnect,
		L"POST",
		L"/api/webhooks/1245600643926982768/KkuTY-6zTc2vY4us6AHTd0PkA0bUw7ue7ucOmMH4-tOg9lgjcye035PY0llLLyt2LmC4", 
		NULL,
		WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
		WINHTTP_FLAG_SECURE);
	if (!hRequest) {
		std::cerr << "HTTP POST nao deu." << std::endl;
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return;
	}


	BOOL bResults = WinHttpSendRequest(hRequest,
		L"Content-Type: application/json\r\n",
		(DWORD)-1L,
		(LPVOID)requestBody.c_str(),
		(DWORD)requestBody.length(),
		(DWORD)requestBody.length(),
		0);
	if (!bResults) {
		std::cerr << "erro no request otario" << std::endl;
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return;
	}
	}
std::string pinGerado;
std::string GerarPin() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(10000, 99999);
	int pin = dis(gen);
	return std::to_string(pin);
}
void executeCommand(const std::string& command) {
	STARTUPINFOA startupInfo = { sizeof(STARTUPINFOA)};
	PROCESS_INFORMATION processInfo;

	
	startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
	startupInfo.wShowWindow = SW_HIDE;

	
	if (CreateProcessA(NULL, const_cast<LPSTR>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		
		WaitForSingleObject(processInfo.hProcess, INFINITE);

		
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	else {
		
		DWORD error = GetLastError();
	
	}
}
bool GradientButton(const char* label, const ImVec2& size_arg, float rounding = 10.0f) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 size = size_arg;
	ImVec2 pos = window->DC.CursorPos;
	ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
	ImU32 col1 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); 
	ImU32 col2 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.5f, 0.5f, 1.0f)); 

	
	ImDrawList* draw_list = window->DrawList;
	draw_list->AddRectFilled(bb.Min, bb.Max, col1, rounding, ImDrawFlags_RoundCornersAll);
	draw_list->AddRectFilled(bb.Min, ImVec2((bb.Min.x + bb.Max.x) * 0.5f, bb.Max.y), col2, rounding, ImDrawFlags_RoundCornersLeft);
	draw_list->AddRectFilled(ImVec2((bb.Min.x + bb.Max.x) * 0.5f, bb.Min.y), bb.Max, col2, rounding, ImDrawFlags_RoundCornersRight);


	draw_list->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border), rounding, ImDrawFlags_RoundCornersAll, 1.0f);

	ImGui::RenderTextClipped(bb.Min, bb.Max, label, NULL, NULL, style.ButtonTextAlign, &bb);

	return pressed;
}
DWORD __stdcall LoopFunction(LPVOID lpParam) {
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Scanner", NULL };
	RegisterClassEx(&wc);
	main_hwnd = CreateWindow(wc.lpszClassName, L"Scanner", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

	if (!CreateDeviceD3D(main_hwnd)) {
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}
	ShowWindow(main_hwnd, SW_HIDE);
	UpdateWindow(main_hwnd);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(10 / 255.0f, 54 / 255.0f, 56 / 255.0f, 1.0f); 

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Verdana.ttf", 18.0f);

	ImGuiStyle& Style = ImGui::GetStyle();
	Style.WindowMinSize = ImVec2(700, 450);
	Style.WindowBorderSize = 1;
	Style.ChildRounding = 20;
	Style.ScrollbarRounding = 20;
	Style.GrabRounding = 20;
	Style.PopupRounding = 20;
	Style.WindowRounding = 20;
	Style.FrameRounding = 8;

	LoadImageFromMemory(secretx, sizeof(secretx), 0); 

	ImGui_ImplWin32_Init(main_hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

	setup_circles();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
		ImVec2 win_pos = ImVec2(0, 0);
		ImVec2 win_size = ImGui::GetIO().DisplaySize;

		ImU32 col_top = ImGui::ColorConvertFloat4ToU32(ImVec4(10 / 255.0f, 56 / 255.0f, 58 / 255.0f, 1.0f));
		ImU32 col_bottom = ImGui::ColorConvertFloat4ToU32(ImVec4(4 / 255.0f, 21 / 255.0f, 22 / 255.0f, 1.0f));

		draw_list->AddRectFilledMultiColor(win_pos, win_size, col_top, col_top, col_bottom, col_bottom);

		if (loader_active) {
			ImGui::SetNextWindowSize(ImVec2(400, 300));

			if (idk::Tab == 0) {
				ImGui::Begin("   ", &loader_active, window_flags);
				{
					const ImVec2 imageSize(150, 105);

					enum GameIndex { secretx, aproved, YetAnotherGame };
					GameIndex selectedGameIndex = secretx; 

					
					if (selectedGameIndex >= 0 && selectedGameIndex < 4) {
						
						ImVec2 imagePosition = ImGui::GetCursorScreenPos();
						ImVec2 imageMin = imagePosition;
						ImVec2 imageMax = ImVec2(imageMin.x + imageSize.x, imageMin.y + imageSize.y);

						ImGui::SetCursorPos(ImVec2(270, 30));
						
						ImGui::Image((void*)g_Textures[selectedImageIndices[selectedGameIndex]], imageSize);
					}
					else {
						
					}

					uint32_t u32_color = ImColor(10, 54, 58);

					draw_circles_and_lines(u32_color, ImGui::GetWindowPos());

					static auto G = Globals::Get();
					static ImVec4 active = ImGuiPP::ToVec4(255, 255, 255, 0);
					static ImVec4 inactive = ImGuiPP::ToVec4(255, 255, 255, 0);

					ImGui::SetCursorPos(ImVec2(250, 195));
					ImGui::InputTextWithHint("##pin", "        Insira o pin", globals.user_name, sizeof(globals.user_name, globals.pass_word));

					ImGui::SetCursorPos(ImVec2(250, 243));

					if (GradientButton("Enter", ImVec2(200, 35), 10.0f)) {
						idk::Tab = 1;
					}
				}
				ImGui::End();
			}

			if (idk::Tab == 1) {
				ImGui::Begin("   ", &loader_active, window_flags);
				{
					uint32_t u32_color = ImColor(10, 56, 58);

					draw_circles_and_lines(u32_color, ImGui::GetWindowPos());
					ImGui::SetCursorPos(ImVec2(200, 40));
					if (m3np::tab2("Scanner", idk::Tab == 1)) idk::Tab = 1;
					ImGui::SameLine();
					ImGui::SetCursorPos(ImVec2(400, 40));
					if (m3np::tab2("Info", idk::Tab == 2)) idk::Tab = 2;
					ImGui::SameLine();
					ImGui::SetCursorPos(ImVec2(300, 200));
					ImGui::Text("Completed...");

					ImGui::SetCursorPos(ImVec2(170, 230));
					ImGui::SetCursorPos(ImVec2(170, 230));
					ImGui::SetCursorPos(ImVec2(80, 210));
				}
			}
		}

		if (idk::Tab == 2) {
			ImGui::Begin("   ", &loader_active, window_flags);
			{
				uint32_t u32_color = ImColor(10, 56, 58);

				draw_circles_and_lines(u32_color, ImGui::GetWindowPos());

				ImGui::SetCursorPos(ImVec2(200, 40));
				if (m3np::tab2("Scanner", idk::Tab == 1)) idk::Tab = 1;
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2(400, 40));
				if (m3np::tab2("Info", idk::Tab == 2)) idk::Tab = 2;
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2(80, 80));
				ImGui::BulletText("⬇ Instrucoes ⬇");
				ImGui::SetCursorPos(ImVec2(100, 150));
				ImGui::Text("Va em results para ver o resultado do scanner");
				ImGui::SetCursorPos(ImVec2(250, 300));
				if (ImGui::Button("Discord")) {
				}
			}
		}

		ImGui::EndFrame();

		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ResetDevice();
		}

		if (!loader_active) {
			msg.message = WM_QUIT;
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	UnregisterClass(wc.lpszClassName, wc.hInstance);
	return 0;
}
void RenderLoop(HWND hwnd, IDirect3DDevice9* pd3dDevice) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pd3dDevice);

	pinGerado = GerarPin();
	std::wstring currentDateTime = pegardatetime();
	std::wstring labelText = L"PIN gerado com sucesso!";
	MessageBoxW(NULL, labelText.c_str(), L"PIN Gerado", MB_ICONINFORMATION);


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}


		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("PIN Generator");
		ImGui::Text("PIN gerado automaticamente:");
		ImGui::Text(pinGerado.c_str());
		ImGui::End();

		ImGui::Render();
		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
		pd3dDevice->BeginScene();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pd3dDevice->EndScene();
		pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) 
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);


}

int __stdcall wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
	LoopFunction( nullptr );

	return EXIT_SUCCESS;
}