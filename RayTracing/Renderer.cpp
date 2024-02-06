#include "Renderer.h"

void Renderer::LogProgress(std::atomic_uint& i, glm::uvec2 imageSize, std::stop_token stopToken) const
{
    std::cout << "Render started...\n";

    unsigned n = imageSize.x * imageSize.y * (unsigned)samplesPerPixel;
    auto start = std::chrono::steady_clock::now();

    auto logProgressLocal = [&i, n, start]()
        {
            float progress = 100.0f * i / n;
            auto elapsed = std::chrono::steady_clock::now() - start;
            auto left = i == 0 ? std::chrono::steady_clock::duration::max() : elapsed * ((float)n / i - 1.0);

            std::cout << i << "/" << n << " (" << std::setprecision(4) << progress << "%), time left: " 
                << std::chrono::duration<float>(left) << "\n";
        };

    SleepFor(logDelay, stopToken);

    while (!stopToken.stop_requested())
    {
        logProgressLocal();

        SleepFor(logDelay, stopToken);
    }

    std::cout << "Render finished! Total time: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - start) << "\n";
}

glm::vec3 Renderer::GetRayColor(const Ray& ray, const SphereCollection& sphereCollection, size_t bounces) const
{
    if (bounces == bounceLimit)
    {
        return glm::vec3(0, 0, 0);
    }

    std::optional<HitResult> hitResult = sphereCollection.Hit(ray, Range(0.001f, std::numeric_limits<float>::max()));

    if (hitResult)
    {
        const HitResult& resultValue = hitResult.value();

        auto scatterResult = std::visit([&ray, &resultValue](auto&& material) { return material.Scatter(ray, resultValue); }, *resultValue.material);

        if (scatterResult)
        {
            auto&& [color, scatteredRay] = scatterResult.value();

            return color * GetRayColor(scatteredRay, sphereCollection, bounces + 1);
        }

        return glm::vec3(0, 0, 0);
    }

    float a = 0.5f * (glm::normalize(ray.direction).y + 1.0f);

    glm::vec3 color = (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + a * glm::vec3(0.5, 0.7, 1.0);

    return color;
}

Renderer::Renderer(size_t samplesPerPixel, size_t bounceLimit, std::chrono::duration<long long> logDelay) :
    samplesPerPixel{ samplesPerPixel }, bounceLimit{ bounceLimit }, logDelay{ logDelay }
{
}

Renderer::Renderer() :
    samplesPerPixel{}, bounceLimit{}, logDelay{}
{
}

Image Renderer::Render(glm::uvec2 imageSize, const Camera& camera, const SphereCollection& sphereCollection) const
{
    //std::chrono::steady_clock::time_point start;
    //start = std::chrono::steady_clock::now();

    std::atomic_uint i{};

    std::jthread logProgressThread([this, &i, &imageSize](std::stop_token stopToken) { LogProgress(i, imageSize, stopToken); });

    Image image(imageSize);

    auto createRay = [&camera, imageSize](size_t row, size_t column) { return camera.CreateRay(imageSize, row, column); };
    auto getRayColor = [this, &sphereCollection](Ray&& ray) { return GetRayColor(ray, sphereCollection); };

    std::vector<std::tuple<size_t, size_t, size_t>> coords(imageSize.x * imageSize.y);
    
    size_t index = 0;

    for (size_t row = 0; row != imageSize.y; ++row)
    {
        for (size_t col = 0; col != imageSize.x; ++col, ++index)
        {
            coords[index] = { row, col, index };
        }
    }

    //std::cout << "Phase 1: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - start) << "\n";
    //start = std::chrono::steady_clock::now();

    for (size_t n = 0; n != samplesPerPixel; ++n)
    {
        std::for_each(std::execution::par_unseq, coords.cbegin(), coords.cend(),
            [&i, &image, createRay, getRayColor](const std::tuple<size_t, size_t, size_t>& coords)
            {
                Ray ray = createRay(std::get<0>(coords), std::get<1>(coords));

                glm::vec3 color = getRayColor(std::move(ray));

                image[std::get<2>(coords)] += color;

                ++i;
            });
    }

    //std::cout << "Phase 2: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - start) << "\n";
    //start = std::chrono::steady_clock::now();

    std::vector<glm::vec3>& pixels = image.GetPixels();

    std::for_each(std::execution::par_unseq, pixels.begin(), pixels.end(), [this](glm::vec3& color)
        {
            color = glm::sqrt(color / (float)samplesPerPixel);
        });

    //std::cout << "Phase 3: " << std::chrono::duration<float>(std::chrono::steady_clock::now() - start) << "\n";

    return image;
}

void Renderer::SetSamplesPerPixel(size_t samplesPerPixel)
{
    this->samplesPerPixel = samplesPerPixel;
}

void Renderer::Render(Image& image, const Camera& camera, const SphereCollection& sphereCollection) const
{
    auto imageSize = image.GetSize();

    auto createRay = [&camera, imageSize](size_t row, size_t column) { return camera.CreateRay(imageSize, row, column); };
    auto getRayColor = [this, &sphereCollection](Ray&& ray) { return GetRayColor(ray, sphereCollection); };

    std::vector<std::tuple<size_t, size_t, size_t>> coords(imageSize.x * imageSize.y);

    size_t index = 0;

    for (size_t row = 0; row != imageSize.y; ++row)
    {
        for (size_t col = 0; col != imageSize.x; ++col, ++index)
        {
            coords[index] = { row, col, index };
        }
    }

    std::for_each(std::execution::par_unseq, coords.cbegin(), coords.cend(),
        [&image, createRay, getRayColor](const std::tuple<size_t, size_t, size_t>& coords)
        {
            Ray ray = createRay(std::get<0>(coords), std::get<1>(coords));

            glm::vec3 color = getRayColor(std::move(ray));

            image[std::get<2>(coords)] += color;
        });
}

size_t Renderer::GetSamplesPerPixel() const
{
    return samplesPerPixel;
}

std::function<void(Image&)> Renderer::GetRenderFunction(glm::uvec2 imageSize, const Camera& camera, const SphereCollection& sphereCollection) const
{
    auto createRay = [&camera, imageSize](size_t row, size_t column) { return camera.CreateRay(imageSize, row, column); };
    auto getRayColor = [this, &sphereCollection](Ray&& ray) { return GetRayColor(ray, sphereCollection); };

    std::vector<std::tuple<size_t, size_t, size_t>> coords(imageSize.x * imageSize.y);

    size_t index = 0;

    for (size_t row = 0; row != imageSize.y; ++row)
    {
        for (size_t col = 0; col != imageSize.x; ++col, ++index)
        {
            coords[index] = { row, col, index };
        }
    }

    return [coords, createRay, getRayColor](Image& image)
        {
            std::for_each(std::execution::par_unseq, coords.cbegin(), coords.cend(),
                [&image, createRay, getRayColor](const std::tuple<size_t, size_t, size_t>& coords)
                {
                    Ray ray = createRay(std::get<0>(coords), std::get<1>(coords));

                    glm::vec3 color = getRayColor(std::move(ray));

                    image[std::get<2>(coords)] += color;
                });
        };
}
