#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <GL/glu.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

float frametime = 1.0 / 30.0;
int line = 0;

float cameraSpeed = 0.1f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 direction;
float yaw, pitch;
bool firstMouse = true;
static double lastX = 400, lastY = 300;

struct Point {
    float x, y, z;
};

float normalize(float originalValue,  float maxValue, bool flip) {
    float valor = 2.0f * (originalValue / maxValue - 0.0f) - 1.0f;
    if (flip) valor = -valor;
    return valor;
}

vector <pair<int, int>> conexiones;

//debe haber una forma mas eficiente de hacer esto
void make_conexion() {
    conexiones.push_back(make_pair(0, 1));   //('left shoulder', 'left elbow')
    conexiones.push_back(make_pair(1, 2));   //('left elbow', 'left wrist')
    conexiones.push_back(make_pair(0, 3));   //('left shoulder', 'left hip')

    conexiones.push_back(make_pair(3, 4));   //('left hip', 'left knee')
    conexiones.push_back(make_pair(4, 5));   //('left knee', 'left ankle')
    conexiones.push_back(make_pair(5, 6));   //('left ankle', 'left heel')
    conexiones.push_back(make_pair(6, 7));   //('left heel', 'left foot index')

    conexiones.push_back(make_pair(8, 9));   //('right shoulder', 'right elbow')
    conexiones.push_back(make_pair(9, 10));  //('right elbow', 'right wrist')
    conexiones.push_back(make_pair(8, 11));  //('right shoulder', 'right hip')

    conexiones.push_back(make_pair(11, 12)); //('right hip', 'right knee')
    conexiones.push_back(make_pair(12, 13)); //('right knee', 'right ankle')
    conexiones.push_back(make_pair(13, 14)); //('right ankle', 'right heel')
    conexiones.push_back(make_pair(14, 15)); //('right heel', 'right foot index')

    conexiones.push_back(make_pair(0, 8));   //('left shoulder', 'right shoulder')
    conexiones.push_back(make_pair(3, 11));  //('left hip', 'right hip')
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    pitch = pitch > 89.0F ? 89.0F : pitch;
    pitch = pitch < -89.0F ? -89.0F : pitch;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}  

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPos += cameraSpeed * cameraFront;
        cout << "W" << endl;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPos -= cameraSpeed * cameraFront;
        cout << "S" << endl;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        cout << "A" << endl;
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        cout << "D" << endl;
    }
    if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        glfwSetWindowShouldClose(window, true);
    }
}

vector<Point> parseCSV(const string& csv) {
    vector<Point> points;
    istringstream stream(csv);
    float value;
    while (stream >> value) {
        Point point;
        point.x = normalize(value, 511.0f, false);
        stream.ignore();
        stream >> value;
        point.y = normalize(value, 423.0f, true);
        stream.ignore();
        stream >> value;
        point.z = normalize(value, 8500.0f, false);
        points.push_back(point);
        stream.ignore(); 
    }
    return points;
}

vector<string> readCSVFile(const string& filename) {
    ifstream file(filename);
    vector<string> frames;
    string line;
    while (getline(file, line)) {
        frames.push_back(line);
    }
    return frames;
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Visualization", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);  
    
    make_conexion();
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    string csvFilePath = "./sus.csv";
    vector<string> frames = readCSVFile(csvFilePath);

    for (const string& frame : frames) {
        vector<Point> points = parseCSV(frame);

        line++;
        cout << "Frame " << line << endl;

        auto startTime = chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(window)) {
            auto currentTime = chrono::high_resolution_clock::now();
            float deltaTime = chrono::duration<float>(currentTime - startTime).count();

            if (deltaTime >= frametime) {
                startTime = currentTime;

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glPointSize(5.0f);

                glfwPollEvents();
   
                glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 
                        cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z, 
                        cameraUp.x, cameraUp.y, cameraUp.z);

                //updateCamera();
                glBegin(GL_POINTS);
                for (const auto& point : points) {
                    glVertex3f(point.x, point.y, point.z);
                }
                glEnd();

                // Draw lines between consecutive points
                glBegin(GL_LINES);
                for (int i = 0; i < conexiones.size(); ++i) {
                    glVertex3f(points[conexiones[i].second].x, points[conexiones[i].second].y, points[conexiones[i].second].z);
                    glVertex3f(points[conexiones[i].first].x, points[conexiones[i].first].y, points[conexiones[i].first].z);
                }
                glEnd();
                glfwSwapBuffers(window);
                break;
            }

            // Introduce a small delay to control frame rate
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        // Check if the user closed the window
        if (glfwWindowShouldClose(window)) {
            break;
        }
    }

    glfwTerminate();
    return 0;
}