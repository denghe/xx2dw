#include <iostream>
#include <GLFW/glfw3.h>

// Include the EMSCRIPTEN specific headers
#include <emscripten.h>
#include <emscripten/html5.h>

// For example purpose we use global variables
#define SPEED 0.005f
GLFWwindow* window = nullptr;
float red = 0.0f;
bool increase = true;

/**
 * @brief This function just increases and decreases the red value.
 *
 * @return The red value.
 */
float getRed() {
    if (increase) {
        red += SPEED;
        if (red > 1.0f) {
            red = 1.0f;
            increase = false;
        }
    } else {
        red -= SPEED;
        if (red < 0.0f) {
            red = 0.0f;
            increase = true;
        }
    }

    return red;
}

/**
 * @brief This function is called every frame.
 */
EM_BOOL mainLoop(double time, void *userData) {
    // Clear the screen with a color
    glClearColor(getRed(), 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap the buffers of the window
    glfwSwapBuffers(window);

    // Poll for the events
    glfwPollEvents();
	
	std::cout << "mainLoop... time = " << time << "" << std::endl;
	return EM_TRUE;
}

/**
 * @brief The normal main() function.
 *
 * @return Exit code.
 */
int main() {
    std::cout << "Starting" << std::endl;

    // Initialize glfw
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Create the window
    window = glfwCreateWindow(640, 480, "Emscripten webgl example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make this window the current context
    glfwMakeContextCurrent(window);

    std::cout << "Going into loop" << std::endl;

    // Define a mail loop function, that will be called as fast as possible
    emscripten_request_animation_frame_loop(&mainLoop, 0);

    // Tear down the system
//    std::cout << "Loop ended" << std::endl;
//    glfwDestroyWindow(window);
//    glfwTerminate();

//    return EXIT_SUCCESS;
}
