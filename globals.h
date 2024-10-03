#pragma once
#include "includes.h"

// D3DX

#include <d3dx9.h>

struct Globals
{
	static Globals* Get()
	{
		static auto* instance = new Globals();
		return instance;
	}

	int MenuTab = 0;

	std::vector<std::string> Games = { "FiveM"};
	int Game = 0;

	bool AutoInject = false;
	bool DelayInjection = false;
};

// Variables Added them in here to keep main.cpp clean

const int MaxGames = 4; // Set the maximum number of games

int selectedImageIndices [MaxGames] = { 0 }; // Index of the selected image for each game

bool loader_active = true;

// Injection code

bool Injection = false;

float InjectionMessageTimer = 0.0f;

const float InjectionMessageDuration = 1.5f; // Set the duration in seconds

// Code to load Images through memory 

LPDIRECT3DTEXTURE9 g_Textures[MaxGames] = { nullptr };

void LoadImageFromMemory(const unsigned char* imageData, int imageSize, int imageIndex)
{
	// Check if the imageIndex is within bounds
	if (imageIndex >= 0 && imageIndex < MaxGames)
	{
		// Release the existing texture if it exists
		if (g_Textures[imageIndex])
		{
			g_Textures[imageIndex]->Release();
			g_Textures[imageIndex] = nullptr;
		}

		// Create a texture from image bytes using DirectX 9
		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(
			g_pd3dDevice,        // Your DirectX 9 device
			imageData,           // Pointer to image bytes
			imageSize,           // Size of the image bytes
			D3DX_DEFAULT,        // Width
			D3DX_DEFAULT,        // Height
			D3DX_DEFAULT,        // Mip levels
			0,                   // Usage
			D3DFMT_UNKNOWN,      // Format (let DirectX detect it)
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,                   // Color key
			nullptr,             // Image info (optional)
			nullptr,             // Palette (optional)
			&g_Textures[imageIndex] // Output texture
		)))
		{
			// Handle error
		}
	}
}

#pragma once
#include <unordered_map>
#include <string>

struct c_globals {
	bool active = true;
	char user_name[255];
	char pass_word[255];
	char license[255];
	std::string version = "v1.0";
} globals;

