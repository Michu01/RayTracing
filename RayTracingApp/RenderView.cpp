#include "RenderView.h"

RenderView::RenderView(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size) : 
	wxWindow(parent, id, position, size)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	Bind(wxEVT_PAINT, &RenderView::OnPaint, this);
	Bind(wxEVT_SIZE, &RenderView::OnResize, this);

	wxSize clientSize = GetClientSize();

	double aspectRatio = (double)clientSize.x / clientSize.y;

	renderer = Renderer(SettingsProvider::Instance.SamplesPerPixel.Get(), bounceLimit, std::chrono::duration<long long>::max());
	collection = SphereCollection
	{
		Sphere(Point3d(0, 0, 1), 0.5, LambertianMaterial(Color(0.7, 0.5, 0.8), 0.5)),
		Sphere(Point3d(1.5, 0, 1), 0.5, MetalMaterial(Color(0.42, 0.76, 0.23), 0.2)),
		Sphere(Point3d(-1.5, 0, 1), 0.5, DielectricMaterial(1.0)),
		Sphere(Point3d(0, -1000.5, 1), 1000, LambertianMaterial(Color(0.3, 0.6, 0.45), 0.6))
	};
	camera = Camera(90, aspectRatio, SettingsProvider::Instance.GetLookFrom(), Point3d(0, 0, 0));
	image = Image(SizeU(clientSize.x, clientSize.y));

	renderThread = CreateRenderThread();

	SettingsProvider::Instance.Bind(EVT_PROPERTY_CHANGE, [this](PropertyChangeEvent& event) 
		{
			const auto& settings = SettingsProvider::Instance;

			switch (event.GetSettings())
			{
			case Settings::SamplesPerPixel:
				renderer.SetSamplesPerPixel(event.GetValue<size_t>());
				break;
			case Settings::LookFromX:
			case Settings::LookFromY:
			case Settings::LookFromZ:
				camera.SetLookFrom(SettingsProvider::Instance.GetLookFrom());
				break;
			}

			renderThread = {};
			image.SetColor(Color(0, 0, 0));
			renderThread = CreateRenderThread();
		});
}

void RenderView::OnPaint(wxPaintEvent& event)
{
	Render();
}

void RenderView::OnResize(wxSizeEvent& event)
{
	renderThread = {};

	wxSize size = GetClientSize();

	double aspectRatio = (double)size.x / size.y;

	camera.SetAspectRatio(aspectRatio);

	image = Image(SizeU(size.x, size.y));

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

	SizeU size = imageCopy.GetSize();

	std::vector<unsigned char> colors(size.height * size.width * 3, 0);

	if (samplesTakenCopy != 0)
	{
		for (size_t n = 0; n != (size_t)size.height * size.width; ++n)
		{
			Color color = (imageCopy[n] / samplesTakenCopy).GetGammaCorrected();
			colors[3 * n] = round(color.r * 255);
			colors[3 * n + 1] = round(color.g * 255);
			colors[3 * n + 2] = round(color.b * 255);
		}
	}

	wxAutoBufferedPaintDC dc(this);
	wxImage wxImage(wxSize(size.width, size.height), colors.data(), nullptr, true);
	wxBitmap bitmap(wxImage, dc);

	dc.DrawBitmap(bitmap, wxPoint(0, 0));
}

void RenderView::RenderLoop(std::stop_token stopToken, Image imageCopy, Camera cameraCopy)
{
	auto render = renderer.GetRenderFunction(imageCopy.GetSize(), cameraCopy, collection);
	size_t samplesPerPixel = renderer.GetSamplesPerPixel();

	samplesTaken = 0;

	while (samplesTaken != samplesPerPixel)
	{
		render(imageCopy);

		if (stopToken.stop_requested())
		{
			return;
		}

		{
			std::lock_guard imageLock(imageMutex);

			image = imageCopy;
			++samplesTaken;
		}

		Refresh();
	}
}

std::unique_ptr<std::jthread> RenderView::CreateRenderThread()
{
	return std::make_unique<std::jthread>([this](std::stop_token stopToken) { RenderLoop(stopToken, image, camera); });
}
