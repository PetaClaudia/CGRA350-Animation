
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "skeleton_model.hpp"


// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and modified for adding in extra information for drawing
// including textures for texture mapping etc.
struct basic_model {
	GLuint shader = 0;
	cgra::gl_mesh mesh;
	glm::vec3 color{0.7};
	glm::mat4 modelTransform{1.0};
	GLuint texture;

    void draw(const glm::mat4 &view, const glm::mat4 proj, const std::vector<glm::vec3> &sliderPts, const float &start);
    void draw1(const glm::mat4 &view, const glm::mat4 proj);
    void draw3(const glm::mat4 &view, const glm::mat4 proj, std::vector<glm::vec3> &controlPts, std::vector<glm::vec3> &interpolatedPts);
    void draw4(const glm::mat4 &view, const glm::mat4 proj, std::vector<glm::vec3> &controlPts, std::vector<glm::vec3> &interpolatedPts);
};

struct splineSeg {
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 d;
};


// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// oribital camera
	float m_pitch = .37;
	float m_yaw = -.73;
	float m_distance = 9.24;

	// last input
	bool m_leftMouseDown = false;
	glm::vec2 m_mousePosition;

	// drawing flags
	bool m_show_axis = false;
	bool m_show_grid = false;
	bool m_showWireframe = false;

	// geometry
	basic_model m_model;
    basic_model m_sphere_r;
    basic_model m_sphere_g;
    basic_model m_sphere_b;
    basic_model m_spline;
    basic_model m_cam_spline;
    std::vector<glm::vec3> controlPts;
    std::vector<glm::vec3> interpolatedPoints;
    std::vector<glm::vec3> camControlPts;
    std::vector<glm::vec3> camInterpolatedPoints;
    std::vector<glm::vec3> sliderPts;
    std::vector<glm::vec3> camSliderPts;
    
    
public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
    
    void calculateSpline(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, std::vector<glm::vec3> &intPoints, std::vector<glm::vec3> &sliderPoints);
    void animateTeapot();
    void reparameterize(std::vector<glm::vec3> &samplePoints, float sampleDist);
    
    int start = 0;
    int camStart = 0;
    int syncStart = 0;
    bool animate = false;
    bool camTrack = false;
};
