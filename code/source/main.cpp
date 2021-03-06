// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include "../headers/ImageFile.hpp"
#include "../headers/Ray.hpp"
#include "../headers/Camera.hpp"
#include "../headers/Render.hpp"
#include <ctime>

const vec3 blue(0, 0, 255);
const vec3 green(0, 255, 0);
const vec3 red(255, 0, 0);
const vec3 floorColor(63, 122, 77);


Scene* createRandomScene(int seed = 0) {

	if (seed == 0)
		srand(static_cast <unsigned> (time(NULL)));
	else
		srand(static_cast <unsigned> (seed));

	//The scene object
	Scene* myscene = new Scene();
	//The Light
	myscene->light = new Light(vec3(0, 10, 6), 0.25f);
	myscene->ambientLight = vec3(1,1,1);
	//The Plane
	myscene->objectsToTrace.push_back(new Plane(vec3(0, 1, 0), vec3(0, -1.2f, 0), floorColor));


	for (int j = 0; j < 60; j++) {

		vec3 position = vec3((float)(rand() % 15 + (-7)), (-1) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - (-1)))), (-6) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-1 - (-6)))));
		vec3 color = vec3((float)(rand() % 255), (float)(rand() % 255), (float)(rand() % 255));
		float size = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
		myscene->objectsToTrace.push_back(new Sphere(position, size, color));

		myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.diffuse = ((double) rand() / (RAND_MAX));
		myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.specular = 1 - 	myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.diffuse;

	}


	return myscene;
}

Scene* createSimpleScene() {


	//The scene object
	Scene* myscene = new Scene();

	//The Light
	myscene->light = new Light(vec3(2, 5, 3), 0.25f);

	//Vector that saves all the objects that are going to be rendered
	myscene->objectsToTrace.push_back(new Plane(vec3(0, 1, 0), vec3(0, -1.2f, 0), floorColor));
	myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.diffuse = 0.9f;

	myscene->objectsToTrace.push_back(new Sphere(vec3(0.f, 0.2f, -3.f), 0.5f, green));
	myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.diffuse = 0.1f;
	myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.specular = 0.9f;

	myscene->objectsToTrace.push_back(new Sphere(vec3(1.f, 0.2f, -2.f), 0.5f, blue));
	myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.diffuse = 1.f;

	myscene->objectsToTrace.push_back(new Sphere(vec3(-1.f, 0.2f, -2.f), 0.5f, red));
	myscene->objectsToTrace[myscene->objectsToTrace.size() - 1]->myMaterial.diffuse = 1.f;


	return myscene;


}

int main()
{
	//initialization const values as the resolution or colors
	const int xResolution = 100;
	const int yResolution = 100;

	//The camera 
	Camera* camera = new Camera(vec3(0, 0, 0));

	//The class that will be responsible for rendering all objects
	Render render;
	render.myScene = createRandomScene(44);//createSimpleScene();

	//Output file .ppm
	ImageFile myFile;
	myFile.openImage("../../../output/ProgramOutput.ppm", "overwrite");
	myFile.addText("P3\n" + to_string(xResolution) + " " + to_string(yResolution) + "\n255\n");


	//Ray tracing algorithm
	for (int i = yResolution - 1; i > 0; i--)
		for (int j = 0; j < xResolution; j++) {

			vec3 currentColor;
			float x = (float)(j) / (float)xResolution;
			float y = (float)(i) / (float)yResolution;
			Ray cameraRay = camera->getRay(x, y);
			currentColor += render.getColor(cameraRay);

			cout << 100 - ((i * 100) / yResolution) << "%" << endl;
			myFile.addText(to_string((int)currentColor.r) + " " + to_string((int)currentColor.g) + " " + to_string((int)currentColor.b) + "\n");

		}

	//Delete obj in the heap 
	delete camera;
	delete render.myScene;

	cout << "Image completed, press enter...";

}

