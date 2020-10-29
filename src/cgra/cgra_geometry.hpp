
#pragma once

// std
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;
namespace cgra {
	
	// creates a mesh for a unit sphere (radius of 1)
	// immediately draws the sphere mesh, assuming the shader is set up
	void drawSphere();

	// creates a mesh for a unit cylinder (radius and hieght of 1) along the z-axis
	// immediately draws the sphere mesh, assuming the shader is set up
	void drawCylinder();
	
	// creates a mesh for a unit cone (radius and hieght of 1) along the z-axis
	// immediately draws the sphere mesh, assuming the shader is set up
	void drawCone();

    void drawLines(std::vector<glm::vec3> &controlPts, std::vector<glm::vec3> &interpolatedPts);
    void drawLines2(std::vector<glm::vec3> &controlPts, std::vector<glm::vec3> &intPts);

	// sets up a shader and draws an axis straight to the current framebuffer
	void drawAxis(const glm::mat4 &view, const glm::mat4 &proj);

	// sets up a shader and draws a grid straight to the current framebuffer
	void drawGrid(const glm::mat4 &view, const glm::mat4 &proj);
}
