#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <thread>
#include <GL/glu.h>
#include <gtk/gtk.h>

using namespace std;

float frametime = 1.0 / 30.0;
int line = 0;
vector <float> l_entries;
vector <float> d_entries;
vector <pair<int, int>> conexiones;

thread z1_thread, z2_thread;
GtkWidget *window;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *l_brazo_l, *l_antebrazo_l, *l_muslo_l, *l_pierna_l, *l_pie_l, *r_brazo_l, *r_antebrazo_l, *r_muslo_l, *r_pierna_l, *r_pie_l, *tronco_l;
GtkWidget *l_brazo_d, *l_antebrazo_d, *l_muslo_d, *l_pierna_d, *l_pie_d, *r_brazo_d, *r_antebrazo_d, *r_muslo_d, *r_pierna_d, *r_pie_d, *tronco_d;
GtkLabel *largo, *distancia;
GtkLabel *l_brazo, *l_antebrazo, *l_muslo, *l_pierna, *l_pie, *r_brazo, *r_antebrazo, *r_muslo, *r_pierna, *r_pie, *tronco;

struct Point {
    float x, y, z;
};

float normalize(float originalValue,  float maxValue) {
    float valor = 2.0f * ((originalValue - 0.0f) / (maxValue - 0.0f)) - 1.0f;
    valor = -valor;
    return valor;
}

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
            point.x = value;
            stream.ignore();
            stream >> point.y;
            stream.ignore();
            stream >> point.z;
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

gboolean updateLenght(gpointer data) {
    // Update the GTK entry with the appropriate value
    static vector<float>* lista = reinterpret_cast<vector<float>*>(data);

    try {
        gtk_entry_set_text(GTK_ENTRY(l_brazo_l), g_strdup_printf("%.7f", (*lista)[0]));
        gtk_entry_set_text(GTK_ENTRY(l_antebrazo_l), g_strdup_printf("%.7f", (*lista)[1]));
        gtk_entry_set_text(GTK_ENTRY(l_muslo_l), g_strdup_printf("%.7f", (*lista)[2]));
        gtk_entry_set_text(GTK_ENTRY(l_pierna_l), g_strdup_printf("%.7f", (*lista)[3]));
        gtk_entry_set_text(GTK_ENTRY(l_pie_l), g_strdup_printf("%.7f", (*lista)[4]));
        gtk_entry_set_text(GTK_ENTRY(r_brazo_l), g_strdup_printf("%.7f", (*lista)[5]));
        gtk_entry_set_text(GTK_ENTRY(r_antebrazo_l), g_strdup_printf("%.7f", (*lista)[6]));
        gtk_entry_set_text(GTK_ENTRY(r_muslo_l), g_strdup_printf("%.7f", (*lista)[7]));
        gtk_entry_set_text(GTK_ENTRY(r_pierna_l), g_strdup_printf("%.7f", (*lista)[8]));
        gtk_entry_set_text(GTK_ENTRY(r_pie_l), g_strdup_printf("%.7f", (*lista)[9]));
        gtk_entry_set_text(GTK_ENTRY(tronco_l), g_strdup_printf("%.7f", (*lista)[10]));
    }
    catch(const std::bad_array_new_length &e) {
        cout << e.what() << endl;
    }

    //delete lista;
    // Returning FALSE removes the callback
    return FALSE;
}

void set_length_fields(vector<Point> landmark) {
    float x1, x2, y1, y2, z1, z2, z3, z4, valor, x_value, y_value;
    
    for (int i=0; i < conexiones.size() - 1; i++) {
        switch (i) {
            case 2:
                continue;
            case 5:
                continue;
            case 8:
                continue;
            case 13:
                continue;
            case 14:
                z1_thread = thread([&landmark, &x_value, &z3](){
                    float x1 = landmark[0].x < 0 ? landmark[0].x * -1 : landmark[0].x;
                    float x2 = landmark[3].x < 0 ? landmark[3].x * -1 : landmark[3].x;
                    float y1 = landmark[0].y < 0 ? landmark[0].y * -1 : landmark[0].y;
                    float y2 = landmark[3].y < 0 ? landmark[3].y * -1 : landmark[3].y;
                    float z1 = landmark[0].z == 0 ? 500 : landmark[0].z;
                    float z2 = landmark[3].z == 0 ? 500 : landmark[3].z;
                    float x_value = pow(x1-x2, 2);
                    float y_value = pow(y1-y2, 2);
                    z3 = sqrt((x_value + y_value)*(z1/z2));
                });
                
                z2_thread = thread([&landmark, &x_value, &z4](){
                    float x1 = landmark[8].x < 0 ? landmark[8].x * -1 : landmark[8].x;
                    float x2 = landmark[11].x < 0 ? landmark[11].x * -1 : landmark[11].x;
                    float y1 = landmark[8].y < 0 ? landmark[8].y * -1 : landmark[8].y;
                    float y2 = landmark[11].y < 0 ? landmark[11].y * -1 : landmark[11].y;
                    float z1 = landmark[8].z == 0 ? 500 : landmark[8].z;
                    float z2 = landmark[11].z == 0 ? 500 : landmark[11].z;
                    float x_value = pow(x1-x2, 2);
                    float y_value = pow(y1-y2, 2);
                    z4 = sqrt((x_value + y_value)*(z1/z2));
                });
                z1_thread.join();
                z2_thread.join();

                l_entries.push_back((z3+z4)/2);
                break;
            default:
                x1 = landmark[conexiones[i].first].x < 0 ? landmark[conexiones[i].first].x * -1 : landmark[conexiones[i].first].x;
                x2 = landmark[conexiones[i].second].x < 0 ? landmark[conexiones[i].second].x * -1 : landmark[conexiones[i].second].x;
                y1 = landmark[conexiones[i].first].y < 0 ? landmark[conexiones[i].first].y * -1 : landmark[conexiones[i].first].y;
                y2 = landmark[conexiones[i].second].y < 0 ? landmark[conexiones[i].second].y * -1 : landmark[conexiones[i].second].y;
                z1 = landmark[conexiones[i].first].z == 0 ? 500 : landmark[conexiones[i].first].z;
                z2 = landmark[conexiones[i].second].z == 0 ? 500 : landmark[conexiones[i].second].z;
                cout << "z1: " << z1 << " z2: " << z2 << endl;
                thread x_thread([&x1, &x2, &x_value](){x_value = pow(x1-x2, 2);});
                thread y_thread([&y1, &y2, &y_value](){y_value = pow(y1-y2, 2);});
                x_thread.join();
                y_thread.join();
                valor = sqrt(x_value + y_value);
                l_entries.push_back(valor*(z1/z2));
        }
    }
    // Schedule the update function to be called in the main thread
    g_idle_add(updateLenght, GINT_TO_POINTER(&l_entries));
    l_entries.clear();
}

gboolean updateDistance(gpointer data) {
    // Update the GTK entry with the appropriate value
    static vector<float>* lista = reinterpret_cast<vector<float>*>(data);

    try {
        gtk_entry_set_text(GTK_ENTRY(l_brazo_d), g_strdup_printf("%.7f", (*lista)[0]));
        gtk_entry_set_text(GTK_ENTRY(l_antebrazo_d), g_strdup_printf("%.7f", (*lista)[1]));
        gtk_entry_set_text(GTK_ENTRY(l_muslo_d), g_strdup_printf("%.7f", (*lista)[2]));
        gtk_entry_set_text(GTK_ENTRY(l_pierna_d), g_strdup_printf("%.7f", (*lista)[3]));
        gtk_entry_set_text(GTK_ENTRY(l_pie_d), g_strdup_printf("%.7f", (*lista)[4]));
        gtk_entry_set_text(GTK_ENTRY(r_brazo_d), g_strdup_printf("%.7f", (*lista)[5]));
        gtk_entry_set_text(GTK_ENTRY(r_antebrazo_d), g_strdup_printf("%.7f", (*lista)[6]));
        gtk_entry_set_text(GTK_ENTRY(r_muslo_d), g_strdup_printf("%.7f", (*lista)[7]));
        gtk_entry_set_text(GTK_ENTRY(r_pierna_d), g_strdup_printf("%.7f", (*lista)[8]));
        gtk_entry_set_text(GTK_ENTRY(r_pie_d), g_strdup_printf("%.7f", (*lista)[9]));
        gtk_entry_set_text(GTK_ENTRY(tronco_d), g_strdup_printf("%.7f", (*lista)[10]));
    }
    catch(const std::bad_array_new_length &e) {
        cout << e.what() << endl;
    }

    //delete lista;
    // Returning FALSE removes the callback
    return FALSE;
}

void set_distance_fields(vector<Point> landmark) {
    float z1, z2, z3, z4;
    
    for (int i=0; i < conexiones.size() - 1; i++) {
        z1 = landmark[conexiones[i].first].z == 0 ? 500 : landmark[conexiones[i].first].z;
        z2 = landmark[conexiones[i].second].z == 0 ? 500 : landmark[conexiones[i].second].z;
        switch (i) {
            case 2:
                continue;
            case 5:
                continue;
            case 8:
                continue;
            case 13:
                continue;
            case 14:
                z1 = landmark[0].z == 0 ? 500 : landmark[0].z;
                z2 = landmark[3].z == 0 ? 500 : landmark[3].z;
                z3 = landmark[8].z == 0 ? 500 : landmark[8].z;
                z4 = landmark[11].z == 0 ? 500 : landmark[11].z;
                d_entries.push_back((z1+z2+z3+z4)/4);
                break;
            default:
                d_entries.push_back((z1+z2)/2);
        }
    }
    // Schedule the update function to be called in the main thread
    g_idle_add(updateDistance, GINT_TO_POINTER(&d_entries));
    d_entries.clear();
}

void render_thread() {

    GLFWwindow* glWindow = glfwCreateWindow(800, 600, "3D Visualization", NULL, NULL);
    if (!glWindow) {
        glfwTerminate();
        //return NULL;
    }
    make_conexion();
    glfwMakeContextCurrent(glWindow);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    string csvFilePath = "./Test1.csv";
    vector<string> frames = readCSVFile(csvFilePath);

    for (const string& frame : frames) {
        vector<Point> points = parseCSV(frame);
        vector<Point> txt_points = points;
        thread norm([&points](){
            for (int i = 0; i < points.size(); i++) {
                points[i].x = normalize(points[i].x, 511.0f);
                points[i].y = normalize(points[i].y, 423.0f);
                points[i].z = normalize(points[i].z, 8500.0f);
            }
        });
        
        thread l_fields(set_length_fields, txt_points);
        thread d_fields(set_distance_fields, txt_points);
        norm.join();

        line++;
        cout << "Frame " << line << endl;

        auto startTime = chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(glWindow)) {
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

                glBegin(GL_LINES);
                for (int i = 0; i < conexiones.size(); ++i) {
                    glVertex3f(points[conexiones[i].second].x, points[conexiones[i].second].y, points[conexiones[i].second].z);
                    glVertex3f(points[conexiones[i].first].x, points[conexiones[i].first].y, points[conexiones[i].first].z);
                }
                glEnd();

                glfwSwapBuffers(glWindow);
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(1));
            glfwPollEvents();
        }
        if (glfwWindowShouldClose(glWindow)) break;
        d_fields.join();
        l_fields.join();
    }
}

// GTK Callback function when button is clicked
void button_clicked(GtkWidget* widget, gpointer data) {
    GtkEntry* entry1 = GTK_ENTRY(data);
    GtkGrid* grid = GTK_GRID(gtk_widget_get_parent(GTK_WIDGET(entry1)));
    GtkEntry* entry2 = GTK_ENTRY(gtk_grid_get_child_at(grid, 0, 1));

    const gchar* text1 = gtk_entry_get_text(entry1);
    const gchar* text2 = gtk_entry_get_text(entry2);

    g_print("Text 1: %s\n", text1);
    g_print("Text 2: %s\n", text2);
}

void initGTK() {

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    largo = GTK_LABEL(gtk_label_new("Largo "));
    distancia = GTK_LABEL(gtk_label_new("Distancia desde camara "));
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(largo), 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(distancia), 2, 0, 1, 1);

    l_brazo = GTK_LABEL(gtk_label_new("Brazo izquierdo"));
    l_antebrazo = GTK_LABEL(gtk_label_new("Antebrazo izquierdo"));
    l_muslo = GTK_LABEL(gtk_label_new("Muslo izquierdo"));
    l_pierna = GTK_LABEL(gtk_label_new("Pierna izquierda"));
    l_pie = GTK_LABEL(gtk_label_new("Pie izquierdo"));
    r_brazo = GTK_LABEL(gtk_label_new("Brazo derecho"));
    r_antebrazo = GTK_LABEL(gtk_label_new("Antebrazo derecho"));
    r_muslo = GTK_LABEL(gtk_label_new("Muslo derecho"));
    r_pierna = GTK_LABEL(gtk_label_new("Pierna derecha"));
    r_pie = GTK_LABEL(gtk_label_new("Pie derecho"));
    tronco = GTK_LABEL(gtk_label_new("Tronco"));

    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(l_brazo), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(l_antebrazo), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(l_muslo), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(l_pierna), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(l_pie), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(r_brazo), 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(r_antebrazo), 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(r_muslo), 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(r_pierna), 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(r_pie), 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(tronco), 0, 11, 1, 1);

    l_brazo_l = gtk_entry_new();
    l_antebrazo_l = gtk_entry_new();
    l_muslo_l = gtk_entry_new();
    l_pierna_l = gtk_entry_new();
    l_pie_l = gtk_entry_new();
    r_brazo_l = gtk_entry_new();
    r_antebrazo_l = gtk_entry_new();
    r_muslo_l = gtk_entry_new();
    r_pierna_l = gtk_entry_new();
    r_pie_l = gtk_entry_new();
    tronco_l = gtk_entry_new();

    l_brazo_d = gtk_entry_new();
    l_antebrazo_d = gtk_entry_new();
    l_muslo_d = gtk_entry_new();
    l_pierna_d = gtk_entry_new();
    l_pie_d = gtk_entry_new();
    r_brazo_d = gtk_entry_new();
    r_antebrazo_d = gtk_entry_new();
    r_muslo_d = gtk_entry_new();
    r_pierna_d = gtk_entry_new();
    r_pie_d = gtk_entry_new();
    tronco_d = gtk_entry_new();

    gtk_editable_set_editable(GTK_EDITABLE(l_brazo_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_antebrazo_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_muslo_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_pierna_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_pie_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_brazo_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_antebrazo_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_muslo_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_pierna_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_pie_l), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(tronco_l), FALSE);

    gtk_editable_set_editable(GTK_EDITABLE(l_brazo_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_antebrazo_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_muslo_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_pierna_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(l_pie_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_brazo_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_antebrazo_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_muslo_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_pierna_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(r_pie_d), FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(tronco_d), FALSE);

    gtk_grid_attach(GTK_GRID(grid), l_brazo_l, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_antebrazo_l, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_muslo_l, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_pierna_l, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_pie_l, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_brazo_l, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_antebrazo_l, 1, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_muslo_l, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_pierna_l, 1, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_pie_l, 1, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tronco_l, 1, 11, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), l_brazo_d, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_antebrazo_d, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_muslo_d, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_pierna_d, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), l_pie_d, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_brazo_d, 2, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_antebrazo_d, 2, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_muslo_d, 2, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_pierna_d, 2, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), r_pie_d, 2, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tronco_d, 2, 11, 1, 1);

    button = gtk_button_new_with_label("SuS");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), (gpointer)l_brazo_l);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 17, 1, 1);

    // Show GTK window
    gtk_widget_show_all(window);
    gtk_main();
}

int main(int argc, char *argv[]) {

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Initialize GLFW
    if (!glfwInit()) return -1;

    thread gtkThread(initGTK);
    thread glfwThread(&render_thread);
    
    gtkThread.join();
    glfwThread.join();

    gtk_main_quit();
    glfwTerminate();

    return 0;
}
