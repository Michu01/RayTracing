#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>

#include <memory>
#include <thread>
#include <mutex>

#include "Renderer.h"
#include "Conversions.h"
#include "Sphere.h"
#include "Renderer.h"
#include "DefaultSettings.h"
#include "SettingsProvider.h"

class RenderView : public wxWindow
{
private:
	size_t bounceLimit = 8;
	size_t samplesTaken = 0;

	Renderer renderer;
	Camera camera;
	SphereCollection collection;
	Image image;

	std::unique_ptr<std::jthread> renderThread;

	std::mutex imageMutex;

private:
	void OnPaint(wxPaintEvent& event);

	void OnResize(wxSizeEvent& event);

	void Render();

	void RenderLoop(std::stop_token stopToken, Image imageCopy, Camera cameraCopy);

	std::unique_ptr<std::jthread> CreateRenderThread();

public:
	RenderView(wxWindow* parent, wxWindowID id = -1, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize);
};

