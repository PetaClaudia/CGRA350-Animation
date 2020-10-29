
// std
#include <iostream>
#include <string>
#include <chrono>

 #define GLM_ENABLE_EXPERIMENTAL

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/spline.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


void basic_model::draw(const glm::mat4 &view, const glm::mat4 proj, const std::vector<glm::vec3> &sliderPts, const float &start) {
//	mat4 modelview = view * modelTransform;
//    translate(modelview, sliderPts[start]);
//    modelview = glm::scale(modelview, vec3(0.1));
    mat4 modelview(view);
    modelview = glm::scale(modelview, vec3(0.1));
	
	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

	mesh.draw(); // draw
}

void basic_model::draw1(const glm::mat4 &view, const glm::mat4 proj) {
    mat4 modelview = view * modelTransform;
    modelview = glm::scale(modelview, vec3(0.1));
    
    glUseProgram(shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
    glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

    drawSphere();
}

void basic_model::draw3(const glm::mat4 &view, const glm::mat4 proj, std::vector<glm::vec3> &controlPts, std::vector<glm::vec3> &interpolatedPts) {
    mat4 modelview = view * modelTransform;
    //modelview = glm::scale(modelview, vec3(0.2));
    
    glUseProgram(shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
    glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

    mesh.draw(); // draw
    drawLines(controlPts, interpolatedPts);
}

void basic_model::draw4(const glm::mat4 &view, const glm::mat4 proj, std::vector<glm::vec3> &controlPts, std::vector<glm::vec3> &interpolatedPts) {
    mat4 modelview = view * modelTransform;
    //modelview = glm::scale(modelview, vec3(0.2));
    
    glUseProgram(shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
    glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

    mesh.draw(); // draw
    drawLines2(controlPts, interpolatedPts);
}

void Application::calculateSpline(vec3 &p0, vec3 &p1, vec3 &p2, vec3 &p3, std::vector<glm::vec3> &intPoints, std::vector<glm::vec3> &sliderPoints){
    float tension = 0;
    float alpha = 0.5;
    float t0 = 0.0f;
    float t1 = t0 + pow(distance(p0, p1), alpha);
    float t2 = t1 + pow(distance(p1, p2), alpha);
    float t3 = t2 + pow(distance(p2, p3), alpha);
    
    vec3 m1 = (1.0f - tension) * (t2 - t1) *
    ((p1 - p0) / (t1 - t0) - (p2 - p0) / (t2 - t0) + (p2 - p1) / (t2 - t1));
    vec3 m2 = (1.0f - tension) * (t2 - t1) *
    ((p2 - p1) / (t2 - t1) - (p3 - p1) / (t3 - t1) + (p3 - p2) / (t3 - t2));
    
    splineSeg segment;
    segment.a = 2.0f * (p1 - p2) + m1 + m2;
    segment.b = -3.0f * (p1 - p2) - m1 - m1 - m2;
    segment.c = m1;
    segment.d = p1;
    
        for(float t = 0; t<1; t = t+0.1){
            vec3 point = segment.a * t * t * t +
             segment.b * t * t +
             segment.c * t +
             segment.d;
            
            intPoints.push_back(point);
        }
    
    for(float t = 0; t<1; t = t+0.01){
        vec3 point = segment.a * t * t * t +
         segment.b * t * t +
         segment.c * t +
         segment.d;
        
        sliderPoints.push_back(point);
    }
}

void Application::animateTeapot(){
    cout<<"animate "<<animate<<endl;
    if(animate){
        int i = 0;
        while (i<(int)sliderPts.size()-1){
            start = i;
            i++;
            if(i == (int)sliderPts.size()-1){
                start = 0;
                i = 0;
            }
        }
    }
}

void Application::reparameterize(vector<vec3> &samplePoints, float sampleDist){
    //reparameterize the t value
    vector<float> uVals;
    vector<float> sVals;
    for(float i = 0; i<1; i = i+sampleDist){
        uVals.push_back(i);
    }
}

Application::Application(GLFWwindow *window) : m_window(window) {
	
	shader_builder sb;
    sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	GLuint shader = sb.build();

	m_model.shader = shader;
	m_model.mesh = load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//teapot.obj")).build();
	m_model.color = vec3(1, 0, 1);
    
    m_sphere_r.shader = shader;
    m_sphere_r.color = vec3(1, 0, 0);
    m_sphere_g.shader = shader;
    m_sphere_g.color = vec3(0, 1, 0);
    m_sphere_b.shader = shader;
    m_sphere_b.color = vec3(0, 1, 1);
    m_spline.shader = shader;
    m_spline.color = vec3(1,1,1);
    m_spline.mesh.mode = GL_LINES;
    m_cam_spline.shader = shader;
    m_cam_spline.color = vec3(1,1,1);
    m_cam_spline.mesh.mode = GL_LINES;
   
    
    //k+2 control points
    controlPts.push_back(vec3(0,0,0));
    controlPts.push_back(vec3(1,0.5,0));
    controlPts.push_back(vec3(2,-0.5,0));
    controlPts.push_back(vec3(3,0.2,0));
    controlPts.push_back(vec3(3.5,1,0));
    controlPts.push_back(vec3(4.5,0.8,0));
    controlPts.push_back(vec3(5,0.5,0));

    camControlPts.push_back(vec3(0, -0.5, 4));
    camControlPts.push_back(vec3(1,0.5,4));
    camControlPts.push_back(vec3(2,2,4));
    camControlPts.push_back(vec3(3,0.6,4));
    camControlPts.push_back(vec3(3.9,1.5,4));
    camControlPts.push_back(vec3(4.5,0.5,4));
    camControlPts.push_back(vec3(5,0,4));
    
    for(int i = 0; i<(int)controlPts.size()-2; i++){
        cout<<i<<" "<<controlPts[i+3].x<<endl;
        calculateSpline(controlPts[i], controlPts[i+1], controlPts[i+2], controlPts[i+3], interpolatedPoints, sliderPts);
    }
     interpolatedPoints.push_back(controlPts[controlPts.size()-1]);
    
    for(int i = 0; i<(int)camControlPts.size()-2; i++){
        calculateSpline(camControlPts[i], camControlPts[i+1], camControlPts[i+2], camControlPts[i+3], camInterpolatedPoints, camSliderPts);
    }
     camInterpolatedPoints.push_back(camControlPts[camControlPts.size()-1]);
    
    for(int i = 0; i<(int)interpolatedPoints.size(); i++){
        cout<<"interpolated "<<interpolatedPoints[i].x<<" "<<interpolatedPoints[i].y<<" "<<interpolatedPoints[i].z<<endl;
    }
    cout<<"interpolated size "<<interpolatedPoints.size()<<endl;
    cout<<"cam interpolated size "<<camInterpolatedPoints.size()<<endl;
    
    
    
//    for(int i = 0; i<(int)controlPts.size()-3; i++){
//        interpolatedPoints.push_back(glm::catmullRom(controlPts[i], controlPts[i+1], controlPts[i+2], controlPts[i+3], 0));
//    }
//    for(int i = 0; i<(int)interpolatedPoints.size(); i++){
//        std::cout<<"interpolatedPoints "<<interpolatedPoints[i].x <<"  "<<interpolatedPoints[i].y <<"  " <<interpolatedPoints[i].z <<endl;
//    }
   
//    glLineWidth(2.0);
//    for(int i = 0; i < (int)controlPts.size()-1; i++){
//        glBegin(GL_LINES);
//        glVertex3f(controlPts[i].x, controlPts[i].y, controlPts[i].z);
//        glVertex3f(controlPts[i+1].x, controlPts[i+1].y, controlPts[i+1].z);
//        glEnd();
//    }
//    glfwSwapBuffers(window);
}


void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	// projection matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);

	// view matrix
    mat4 view;
	view = translate(mat4(1), vec3(0, 0, -m_distance))
		* rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0));
    
    if(camTrack){
        m_pitch = -0.03;
        m_yaw = - 0.08;
        //view = translate(mat4(1), -camSliderPts[start]);
        view = glm::lookAt(camSliderPts[camStart], sliderPts[start], vec3(0, 1, 0));
    }

	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);


	// draw the model
    mat4 modelTrans(view);
    modelTrans = glm::translate(modelTrans, sliderPts[start]);
	m_model.draw(modelTrans, proj, sliderPts, start);
    m_sphere_g.draw1(glm::translate(view, controlPts[1]), proj);
    m_sphere_b.draw1(glm::translate(view, controlPts[2]), proj);
    m_sphere_b.draw1(glm::translate(view, controlPts[3]), proj);
    m_sphere_b.draw1(glm::translate(view, controlPts[4]), proj);
    m_sphere_r.draw1(glm::translate(view, controlPts[5]), proj);
    
//    m_sphere_g.draw1(glm::translate(view, camControlPts[1]), proj);
//     m_sphere_b.draw1(glm::translate(view, camControlPts[2]), proj);
//     m_sphere_b.draw1(glm::translate(view, camControlPts[3]), proj);
//     m_sphere_b.draw1(glm::translate(view, camControlPts[4]), proj);
//     m_sphere_r.draw1(glm::translate(view, camControlPts[5]), proj);
    //for(int i = 0; i<(int)controlPts.size()-1; i++){
        //m_spline.draw2(glm::translate(view, controlPts[0]), proj, controlPts, 1);
    //m_spline.draw2(glm::translate(view, controlPts[0]), proj, controlPts, 2);
    //}
    m_spline.draw3(view, proj, controlPts, interpolatedPoints);
    m_cam_spline.draw4(view, proj, camControlPts, camInterpolatedPoints);
}



void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);

	// helpful drawing options
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);

	
	ImGui::Separator();
    
    ImGui::SliderInt("Teapot Slider", &start, 0, sliderPts.size()-101);
    ImGui::SliderInt("Camera Slider", &camStart, 0, camSliderPts.size()-101);
    if(ImGui::SliderInt("Sync Slider", &syncStart, 0, camSliderPts.size()-101)){
        camStart = syncStart;
        start = syncStart;
    }
    if (ImGui::Button("Camera Track")){
        int count = 0;
        if(camTrack && count == 0){
            camTrack = false;
            count++;
        }
        else if(camTrack == false && count == 0){
            camTrack = true;
            count++;
        }
    }
    
//    if (ImGui::Button("Start animation")){
//        animate=true;
//        animateTeapot();
//        cout<<"animate button"<<endl;
//    }
    

	// example of how to use input boxes
	static float exampleInput;
	if (ImGui::InputFloat("example input", &exampleInput)) {
		cout << "example input changed to " << exampleInput << endl;
	}

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
			- acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
			- acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_yaw > pi<float>()) m_yaw -= float(2 * pi<float>());
		else if (m_yaw < -pi<float>()) m_yaw += float(2 * pi<float>());
	}

	// updated mouse position
	m_mousePosition = vec2(xpos, ypos);
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)mods; // currently un-used

	// capture is left-mouse down
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_leftMouseDown = (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset; // currently un-used
	m_distance *= pow(1.1f, -yoffset);
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
