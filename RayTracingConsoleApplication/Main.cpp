#include <memory>

#include "PPM_ImageSaver.h"
#include "Renderer.h"
#include "SphereCollection.h"
#include "Materials.h"
#include "Vector3.h"

static void Test1(size_t samplesPerPixel = 8)
{
	Material material1 = LambertianMaterial(Color(191, 21, 21), 1.0);
	Material material2 = LambertianMaterial(Color(117, 76, 15), 1.0);
	Material material3 = MetalMaterial(Color(131, 145, 201), 0.2);
	Material material4 = DielectricMaterial(1.5);

	SphereCollection collection
	{
		Sphere(Point3d(0, 0, -2), 0.5, material1),
		Sphere(Point3d(0, -100.5, -1), 100, material2),
		Sphere(Point3d(-1.1, 0, -2), 0.5, material3),
		Sphere(Point3d(1.1, 0, -2), 0.5, material4)
	};

    SizeU imageSize(800, 800);

    double vFov = 90;
    double aspectRatio = GetAspectRatio(imageSize);

    Point3d lookFrom(-2, 2, 1);
    Point3d lookAt(0, 0, -1);

    Camera camera(vFov, aspectRatio, lookFrom, lookAt);

    Renderer renderer(samplesPerPixel, 8, std::chrono::seconds(10));

    Image image = renderer.Render(imageSize, camera, collection);

    PPM_ImageSaver().SaveImage(image, std::format("test1-{}.ppm", samplesPerPixel));
}

static void Test2(size_t samplesPerPixel = 8)
{
    SphereCollection collection;

    Material groundMaterial = LambertianMaterial(Color(0.5, 0.5, 0.5), 0.5);

    collection.Add(Sphere(Point3d(0, -1000, -3), 1000, groundMaterial));

    auto random = [](double max = 1.0) { return Random::Instance.RandomDouble(std::uniform_real_distribution(0.0, max)); };

    int d = 9;

    for (int a = -d; a < d; a++)
    {
        for (int b = -d; b < d; b++)
        {
            Point3d center(a + 0.9 * random(), 0.2, b + 0.9 * random());

            if ((center - Point3d(4, 0.2, 0)).GetLength() < 0.9)
            {
                continue;
            }

            std::unique_ptr<Material> material;
            double choice = random();

            if (choice < 0.8) 
            {
                Color color = Random::Instance.RandomColor();
                material = std::make_unique<Material>(LambertianMaterial(color, 0.5));
            }
            else if (choice < 0.95) 
            {
                Color albedo = Random::Instance.RandomColor();
                double fuzz = random(0.5);
                material = std::make_unique<Material>(MetalMaterial(albedo, fuzz));
            }
            else 
            {
                material = std::make_unique<Material>(DielectricMaterial(1.5));
            }

            collection.Add(Sphere(center, 0.2, *material));
        }
    }

    Material material1 = DielectricMaterial(1.5);
    collection.Add(Sphere(Point3d(0, 1, 0), 1.0, material1));

    Material material2 = LambertianMaterial(Color(0.4, 0.2, 0.1), 0.5);
    collection.Add(Sphere(Point3d(-4, 1, 0), 1.0, material2));

    Material material3 = MetalMaterial(Color(0.7, 0.6, 0.5), 0.0);
    collection.Add(Sphere(Point3d(4, 1, 0), 1.0, material3));

    double aspectRatio = 16.0 / 9.0;

    SizeU imageSize;
    imageSize.width = 1920;
    imageSize.height = 1080;

    double vFov = 20;

    Point3d lookFrom(13, 2, 3);
    Point3d lookAt(0, 0, 0);

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