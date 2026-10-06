#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

//	Define Function to make rect creation easier
PhysicsRectangle makeRect(Vector2f size, Vector2f pos) {
	PhysicsRectangle wall;

	wall.setSize(size);
	wall.setCenter(pos);
	wall.setStatic(true);
	
	return wall;
}

int main() {
	//	Instantiate the window
	Vector2f dims(800, 600);

	RenderWindow window(VideoMode(dims.x, dims.y), "Bounce");
	World world(Vector2f(0, 1));

	//	Global Vars
	float wallWidth = 20;
	int thudCount(0), bangCount(0);

	//	Create Ball
	PhysicsCircle ball;
	ball.setCenter(Vector2f(dims.x - wallWidth * 2, dims.y / 2));
	ball.setRadius(20);

	world.AddPhysicsBody(ball);
	
	//	Create Walls
	PhysicsRectangle leftWall(makeRect(
		Vector2f(wallWidth, dims.y), 
		Vector2f(wallWidth / 2, dims.y / 2)));
	PhysicsRectangle rightWall(makeRect(
		Vector2f(wallWidth, dims.y),
		Vector2f(dims.x - wallWidth / 2, dims.y / 2)));
	PhysicsRectangle ceiling(makeRect(
		Vector2f(dims.x, wallWidth),
		Vector2f(dims.x / 2, wallWidth / 2)));
	PhysicsRectangle floor(makeRect(
		Vector2f(dims.x, wallWidth),
		Vector2f(dims.x / 2, dims.y - wallWidth / 2)));

	//	Wall Collision Callbacks
	leftWall.onCollision = [&thudCount](PhysicsBodyCollisionResult r) {
		//	Flipped these to have the first msg be thud 1
		thudCount++;
		cout << "thud " << thudCount << endl;
	};
	rightWall.onCollision = [&thudCount](PhysicsBodyCollisionResult r) {
		thudCount++;
		cout << "thud " << thudCount << endl;
	};
	ceiling.onCollision = [&thudCount](PhysicsBodyCollisionResult r) {
		thudCount++;
		cout << "thud " << thudCount << endl;
	};
	floor.onCollision = [&thudCount](PhysicsBodyCollisionResult r) {
		thudCount++;
		cout << "thud " << thudCount << endl;
	};

	//	Add walls to world
	world.AddPhysicsBody(leftWall);
	world.AddPhysicsBody(rightWall);
	world.AddPhysicsBody(ceiling);
	world.AddPhysicsBody(floor);

	//	Add obstacle
	PhysicsRectangle obstacle(makeRect(
		Vector2f(200, 40),
		Vector2f(dims.x / 2, dims.y / 2)));
	
	//	Obstacle Callback
	obstacle.onCollision = [&bangCount](PhysicsBodyCollisionResult r) {
		bangCount++;
		cout << "bang " << bangCount << endl;
	};

	//	Give arbitrary velocity
	ball.applyImpulse(Vector2f(.5, .5));

	//	Add to world
	world.AddPhysicsBody(obstacle);

	//	Game loop
	Clock clock;
	Time lastTime(clock.getElapsedTime());
	while (true) {
		Time currentTime(clock.getElapsedTime());
		Time deltaTime(currentTime - lastTime);
		int deltaTimeMS(deltaTime.asMilliseconds());
		if (deltaTimeMS > 0) {
			world.UpdatePhysics(deltaTimeMS);
			lastTime = currentTime;
		}
		window.clear(Color(0, 0, 0));
		
		//	Draw everything
		window.draw(ball);
		window.draw(obstacle);

		window.draw(leftWall);
		window.draw(rightWall);
		window.draw(ceiling);
		window.draw(floor);
		
		window.display();

		//	Exit after 3 "bangs"
		if (bangCount >= 3)
			exit(0);
	}
}