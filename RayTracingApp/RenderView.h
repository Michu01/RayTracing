#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>

#include <memory>
#include <thread>
#include <mutex>

#include "Renderer.h"
#include "Sphere.h"
#include "Renderer.h"
#include "DefaultSettings.h"
#include "SettingsProvider.h"
#include "SpheresGenerator.h"

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

	std::optional<wxPoint> lastPoint;
	std::optional<wxPoint> rotationCenter;

	bool isShiftPressed;

private:
	void OnPaint(wxPaintEvent& event);

	void OnResize(wxSizeEvent& event);

	void OnMotion(wxMouseEvent& event);

	void OnEnterWindow(wxMouseEvent& event);

	void OnLeaveWindow(wxMouseEvent& event);

	void OnKeyDown(wxKeyEvent& event);

	void OnKeyUp(wxKeyEvent& event);

	void OnMiddleUp(wxMouseEvent& event);

	void OnMiddleDown(wxMouseEvent& event);

	void OnMouseWheel(wxMouseEvent& event);

	void Render();

	void RenderLoop(std::stop_token stopToken);

	std::unique_ptr<std::jthread> CreateRenderThread();

public:
	RenderView(wxWindow* parent, wxWindowID id = -1, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize);
};

