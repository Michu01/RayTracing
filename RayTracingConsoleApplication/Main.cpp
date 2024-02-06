#include <memory>

#include "PPM_ImageSaver.h"
#include "Renderer.h"
#include "SphereCollection.h"
#include "Materials.h"

static void Test1(size_t samplesPerPixel = 8)
{
	Material material1 = LambertianMaterial(glm::vec3(191, 21, 21), 1.0f);
	Material material2 = LambertianMaterial(glm::vec3(117, 76, 15), 1.0f);
	Material material3 = MetalMaterial(glm::vec3(131, 145, 201), 0.2f);
	Material material4 = DielectricMaterial(1.5f);

	SphereCollection collection
	{
		Sphere(glm::vec3(0, 0, -2), 0.5, material1),
		Sphere(glm::vec3(0, -100.5, -1), 100, material2),
		Sphere(glm::vec3(-1.1, 0, -2), 0.5, material3),
		Sphere(glm::vec3(1.1, 0, -2), 0.5, material4)
	};

    glm::uvec2 imageSize(800, 800);

    float vFov = 90;
    float aspectRatio = (float)imageSize.x / imageSize.y;

    glm::vec3 lookFrom(-2, 2, 1);
    glm::vec3 lookAt(0, 0, -1);

    Camera camera(vFov, aspectRatio, lookFrom, lookAt);

    Renderer renderer(samplesPerPixel, 8, std::chrono::seconds(10));

    Image image = renderer.Render(imageSize, camera, collection);

    PPM_ImageSaver().SaveImage(image, std::format("test1-{}.ppm", samplesPerPixel));
}

static void Test2(size_t samplesPerPixel = 8)
{
    SphereCollection collection;

    Material groundMaterial = LambertianMaterial(glm::vec3(0.5, 0.5, 0.5), 0.5);

    collection.Add(Sphere(glm::vec3(0, -1000, -3), 1000, groundMaterial));

    auto random = [](float max = 1.0) { return Random::Instance.Float(std::uniform_real_distribution(0.0f, max)); };

    int d = 9;

    for (int a = -d; a < d; a++)
    {
        for (int b = -d; b < d; b++)
        {
            glm::vec3 center(a + 0.9 * random(), 0.2, b + 0.9 * random());

            if (glm::length(center - glm::vec3(4, 0.2, 0)) < 0.9)
            {
                continue;
            }

            std::unique_ptr<Material> material;
            float choice = random();

            if (choice < 0.8) 
            {
                glm::vec3 color = Random::Instance.Color();
                material = std::make_unique<Material>(LambertianMaterial(color, 0.5));
            }
            else if (choice < 0.95) 
            {
                glm::vec3 albedo = Random::Instance.Color();
                float fuzz = random(0.5);
                material = std::make_unique<Material>(MetalMaterial(albedo, fuzz));
            }
            else 
            {
                material = std::make_unique<Material>(DielectricMaterial(1.5));
            }

            collection.Add(Sphere(center, 0.2f, *material));
        }
    }

    Material material1 = DielectricMaterial(1.5);
    collection.Add(Sphere(glm::vec3(0, 1, 0), 1.0, material1));

    Material material2 = LambertianMaterial(glm::vec3(0.4, 0.2, 0.1), 0.5);
    collection.Add(Sphere(glm::vec3(-4, 1, 0), 1.0, material2));

    Material material3 = MetalMaterial(glm::vec3(0.7, 0.6, 0.5), 0.0);
    collection.Add(Sphere(glm::vec3(4, 1, 0), 1.0, material3));

    float aspectRatio = 16.0f / 9.0f;

    glm::uvec2 imageSize(1920, 1080);

    float vFov = 20;

    glm::vec3 lookFrom(13, 2, 3);
    glm::vec3 lookAt(0, 0, 0);

    Camera camera(vFov, aspectRatio, lookFrom, lookAt);

    Renderer renderer(samplesPerPixel, 8, std::chrono::seconds(10));

    Image image = renderer.Render(imageSize, camera, collection);

    PPM_ImageSaver().SaveImage(image, std::format("test2-{}.ppm", samplesPerPixel));
}

//static void Test3()
//{
//    for (size_t n = 1; n <= 128; n *= 2)
//    {
//        Test2(n);
//    }
//}

int main()
{
    Test2(512);

	return 0;
}