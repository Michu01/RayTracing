#include "RenderView.h"

RenderView::RenderView(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size) : 
	wxWindow(parent, id, position, size)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	Bind(wxEVT_PAINT, &RenderView::OnPaint, this);
	Bind(wxEVT_SIZE, &RenderView::OnResize, this);
	Bind(wxEVT_MOTION, &RenderView::OnMotion, this);
	Bind(wxEVT_ENTER_WINDOW, &RenderView::OnEnterWindow, this);
	Bind(wxEVT_LEAVE_WINDOW, &RenderView::OnLeaveWindow, this);
	Bind(wxEVT_KEY_DOWN, &RenderView::OnKeyDown, this);
	Bind(wxEVT_KEY_UP, &RenderView::OnKeyUp, this);
	Bind(wxEVT_MIDDLE_DOWN, &RenderView::OnMiddleDown, this);
	Bind(wxEVT_MIDDLE_UP, &RenderView::OnMiddleUp, this);
	Bind(wxEVT_MOUSEWHEEL, &RenderView::OnMouseWheel, this);

	wxSize clientSize = GetClientSize();

	double aspectRatio = (double)clientSize.x / clientSize.y;

	renderer = Renderer(SettingsProvider::Instance.SamplesPerPixel.Get(), bounceLimit, std::chrono::duration<long long>::max());
	SpheresGenerator generator;
	collection = generator.Generate({});
	/*collection = SphereCollection
	{
		Sphere(glm::vec3(0, 0, 1), 0.5, LambertianMaterial(glm::vec3(0.7, 0.5, 0.8), 0.5)),
		Sphere(glm::vec3(1.5, 0, 1), 0.5, MetalMaterial(glm::vec3(0.42, 0.76, 0.23), 0.2)),
		Sphere(glm::vec3(-1.5, 0, 1), 0.5, DielectricMaterial(1.5)),
		Sphere(glm::vec3(0, -1000.5, 1), 1000, LambertianMaterial(glm::vec3(0.3, 0.6, 0.45), 0.6))
	};*/
	camera = Camera(90, aspectRatio, SettingsProvider::Instance.GetLookFrom(), glm::vec3(0, 0, -1));
	image = Image(glm::uvec2(clientSize.x, clientSize.y));

	renderThread = CreateRenderThread();

	SettingsProvider::Instance.Bind(EVT_PROPERTY_CHANGE, [this](PropertyChangeEvent& event) 
		{
			const auto& settings = SettingsProvider::Instance;

			switch (event.GetSettings())
			{
			case Settings::SamplesPerPixel:
				renderer.SetSamplesPerPixel(event.GetValue<size_t>());
				break;
			}

			renderThread = {};
			renderThread = CreateRenderThread();
		});
}

void RenderView::OnPaint(wxPaintEvent& event)
{
	Render();
}

void RenderView::OnResize(wxSizeEvent& event)
{
	wxSize size = GetClientSize();

	double aspectRatio = (double)size.x / size.y;

	camera.SetAspectRatio(aspectRatio);

	renderThread = {};
	renderThread = CreateRenderThread();
}

void RenderView::OnMotion(wxMouseEvent& event)
{
	wxPoint point = event.GetPosition();

	if (!lastPoint)
	{
		lastPoint = point;
		return;
	}

	wxPoint offset = point - lastPoint.value();
	lastPoint = point;

	wxSize size = GetClientSize();

	glm::vec3 offsetScaled((float)offset.x / size.x, (float)offset.y / size.y, 0);

	if (rotationCenter)
	{
		glm::vec3 rotationCenterScaled(rotationCenter.value().x / size.x, rotationCenter.value().y / size.y, 0);

		camera.Rotate(rotationCenterScaled, offsetScaled);

		renderThread = {};
		renderThread = CreateRenderThread();
	}
	else if (event.ShiftDown())
	{
		camera.Move(offsetScaled);

		renderThread = {};
		renderThread = CreateRenderThread();
	}
}

void RenderView::OnEnterWindow(wxMouseEvent& event)
{
	lastPoint = event.GetPosition();
}

void RenderView::OnLeaveWindow(wxMouseEvent& event)
{
	lastPoint = {};
}

void RenderView::OnKeyDown(wxKeyEvent& event)
{
	
}

void RenderView::OnKeyUp(wxKeyEvent& event)
{
	
}

void RenderView::OnMiddleUp(wxMouseEvent& event)
{
	rotationCenter = {};
}

void RenderView::OnMiddleDown(wxMouseEvent& event)
{
	rotationCenter = event.GetPosition();
}

void RenderView::OnMouseWheel(wxMouseEvent& event)
{
	camera.Zoom(-(float)event.GetWheelRotation() / event.GetWheelDelta());

	renderThread = {};
	renderThread = CreateRenderThread();
}

void RenderView::Render()
{
	Image imageCopy;
	size_t samplesTakenCopy;

	{
		std::lock_guard imageLock(imageMutex);

		imageCopy = image;
		samplesTakenCopy = samplesTaken;
	}

	glm::uvec2 size = imageCopy.GetSize();

	std::vector<unsigned char> colors(size.x * size.y * 3, 0);

	if (samplesTakenCopy != 0)
	{
		for (size_t n = 0; n != (size_t)size.x * size.y; ++n)
		{
			glm::vec3 color = glm::sqrt((imageCopy[n] / (float)samplesTakenCopy));
			colors[3 * n] = round(glm::clamp(color.r, 0.0f, 1.0f) * 255);
			colors[3 * n + 1] = round(glm::clamp(color.g, 0.0f, 1.0f) * 255);
			colors[3 * n + 2] = round(glm::clamp(color.b, 0.0f, 1.0f) * 255);
		}
	}

	wxAutoBufferedPaintDC dc(this);
	wxImage wxImage(wxSize(size.x, size.y), colors.data(), nullptr, true);
	wxBitmap bitmap(wxImage, dc);

	dc.DrawBitmap(bitmap, wxPoint(0, 0));
}

void RenderView::RenderLoop(std::stop_token stopToken)
{
	auto clientSize = GetClientSize();
	Image localImage(glm::uvec2(clientSize.x, clientSize.y));
	auto render = renderer.GetRenderFunction(localImage.GetSize(), camera, collection);
	size_t samplesPerPixel = renderer.GetSamplesPerPixel();

	samplesTaken = 0;

	while (samplesTaken != samplesPerPixel)
	{
		render(localImage);

		if (stopToken.stop_requested())
		{
			return;
		}

		{
			std::lock_guard imageLock(imageMutex);

			image = localImage;
			++samplesTaken;
		}

		Refresh();
	}
}

std::unique_ptr<std::jthread> RenderView::CreateRenderThread()
{
	return std::make_unique<std::jthread>([this](std::stop_token stopToken) { RenderLoop(stopToken); });
}
