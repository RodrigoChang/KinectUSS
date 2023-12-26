#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <GL/glu.h>

using namespace std;

float frametime = 1.0 / 30.0;
int line = 0;

struct Point {
    float x, y, z;
};

float normalize(float originalValue,  float maxValue) {
    float valor = 2.0f * ((originalValue - 0.0f) / (maxValue - 0.0f)) - 1.0f;
    valor = -valor;
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

vector<Point> parseCSV(const string& csv) {
    vector<Point> points;
    istringstream stream(csv);
    float value;
    while (stream >> value) {
        Point point;
        point.x = normalize(value, 511.0f);
        stream.ignore();
        stream >> value;
        point.y = normalize(value, 423.0f);
        stream.ignore();
        stream >> value;
        point.z = normalize(value, 8500.0f);
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
    make_conexion();
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    string csvFilePath = "./Test1.csv";
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

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();

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

            // Check for user input events
            glfwPollEvents();
        }

        // Check if the user closed the window
        if (glfwWindowShouldClose(window)) {
            break;
        }
    }

    glfwTerminate();
    return 0;
}