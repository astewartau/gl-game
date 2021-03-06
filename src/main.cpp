// Standard library
#include <iostream>

// GLEW
#include <GL/glew.h>

// SFML
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

// Image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window handles
#include <windows.h>

// My classes
#include "Shader.hpp"
#include "Camera.hpp"

// Window settings
unsigned int screenWidth = 1280;
unsigned int screenHeight = 720;
bool fullScreen = false;

void toggle_fullscreen(sf::Window& window) {
    fullScreen = !fullScreen;

    HWND handle = window.getSystemHandle();
    DWORD win32Style = WS_VISIBLE;

    if (fullScreen) {
	win32Style |= WS_POPUP;
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	screenWidth = desktopMode.width;
	screenHeight = desktopMode.height;
    } else {
	win32Style |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION;
	sf::VideoMode windowMode = sf::VideoMode(1280, 720);
	screenWidth = windowMode.width;
	screenHeight = windowMode.height;
	window.setSize(sf::Vector2u(screenWidth, screenHeight));
    }

    SetWindowLongPtr(handle, GWL_STYLE, win32Style);

    // Force changes to take effect
    SetWindowPos(
	handle,
	NULL,
	0,
	0,
	0,
        0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME
    );

    window.setSize(sf::Vector2u(screenWidth, screenHeight));
    
    if (fullScreen) {
	window.setPosition(sf::Vector2i(0, 0));
    } else {
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	window.setPosition(
	    sf::Vector2i(
	        desktopMode.width / 2 - screenWidth / 2,
	        desktopMode.height / 2 - screenHeight / 2
	    )
        );
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
}

int main() {
    // create the window
    sf::Window window(
        sf::VideoMode(screenWidth, screenHeight),
    	"OpenGL",
    	sf::Style::Default,
    	sf::ContextSettings(24)
    );

    window.setVerticalSyncEnabled(true);

    // initialise glew
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << glewGetErrorString(err);
        window.close();
        return -1;
    }

    // activate the window as the current OpenGL target
    window.setActive(true);

    // build and compile our shader program
    Shader ourShader("data/shaders/shader.vs", "data/shaders/shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    // indices
    /* unsigned int indices[] = {
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
       };*/

    // generate buffer objects and vertex array
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // setup vertex array object
    {
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(
	    GL_ARRAY_BUFFER,
	    sizeof(vertices),
	    vertices,
	    GL_STATIC_DRAW
	);

	/*
	glBufferData(
	    GL_ELEMENT_ARRAY_BUFFER,
	    sizeof(indices),
	    indices,
	    GL_STATIC_DRAW
	);
	*/

	glVertexAttribPointer(
	    0,
	    3,
	    GL_FLOAT,
	    GL_FALSE,
	    5 * sizeof(float),
	    (void*)0
	);

	glVertexAttribPointer(
	    1,
	    2,
	    GL_FLOAT,
	    GL_FALSE,
	    5 * sizeof(float),
	    (void*)(3 * sizeof(float))
	);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
    }

    // prepare for texture loading

    // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture1, texture2;

    // load texture 1
    {
	// texture 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load(
	    "data/textures/container.jpg",
	    &width,
	    &height,
	    &nrChannels,
	    0
        );

	if (data) {
	    glTexImage2D(
	        GL_TEXTURE_2D,
		0,
		GL_RGB,
		width,
		height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data
	    );

	    glGenerateMipmap(GL_TEXTURE_2D);
	} else {
	    std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
    }

    // load texture 2
    {
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load(
	    "data/textures/awesomeface.png",
	    &width,
	    &height,
	    &nrChannels,
	    0
	);

	if (data) {

	    // note that the awesomeface.png has transparency and thus an alpha
	    // channel, so make sure to tell OpenGL the data type is of GL_RGBA
	    glTexImage2D(
	        GL_TEXTURE_2D,
		0,
		GL_RGB,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data
	    );

	    glGenerateMipmap(GL_TEXTURE_2D);
	} else {
	    std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
    }

    // setup shader uniforms
    {
	ourShader.use();

	// textures
	ourShader.setInt("fTexture1", 0);
	ourShader.setInt("fTexture2", 1);

	glUseProgram(0);
    }

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // cube positions
    glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // time
    sf::Clock clock;
    float currentTime = clock.getElapsedTime().asSeconds();
    float previousTime = 0;
    float deltaTime = 0;

    // mouse
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);
    float middleX = screenWidth / 2.0f;
    float middleY = screenHeight / 2.0f;
    sf::Mouse::setPosition(sf::Vector2i(middleX, middleY), window);

    while (window.isOpen()) {
	// get time
	previousTime = currentTime;
	currentTime = clock.getElapsedTime().asSeconds();
	deltaTime = currentTime - previousTime;

	// input processing
	sf::Event event;
	while (window.pollEvent(event)) {
	    switch (event.type) {
	    case sf::Event::Closed:
		window.close();
		break;
	    case sf::Event::Resized:
		screenWidth = event.size.width;
		screenHeight = event.size.height;
		glViewport(0, 0, screenWidth, screenHeight);
		break;
	    case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Key::F11) {
		    toggle_fullscreen(window);
		} else if (event.key.code == sf::Keyboard::Key::Escape) {
		    window.close();
		}
		break;
	    case sf::Event::MouseWheelScrolled:
		camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
		break;
	    default:

		break;
	    }
	}

	// mouse controlled camera
	if (window.hasFocus()) {
	    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	    // check whether the cursor is in the screen (it should be if it has
	    // been 'grabbed')
	    if ((mousePos.x > 0 && mousePos.x < screenWidth) &&
		(mousePos.y > 0 && mousePos.y < screenHeight)) {

		// find how much the cursor moved
		float xoffset = mousePos.x - middleX;
		float yoffset = middleY - mousePos.y;

		// handle the movement
		camera.ProcessMouseMovement(xoffset, yoffset);

		// move the cursor back to the centre
		sf::Mouse::setPosition(sf::Vector2i(middleX, middleY), window);
	    }
	}

	// keyboard movement
	if (window.hasFocus()) {
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	    }
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	    }
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	    }
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	    }
	}

	// clear screen (color buffer and depth buffer)
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// setup per-frame uniforms
	ourShader.use();

	// model matrix (translations, scaling, rotations to transform vertices
	// into world space)
	glm::mat4 model;
	model = glm::rotate(
	    model,
	    (float)currentTime * glm::radians(50.0f),
	    glm::vec3(0.5f, 1.0f, 0.0f)
	);

	// move the camera backwards (by pushing the scene forwards into the
	// negative z direction)
	glm::mat4 view = camera.GetViewMatrix();

	// perspective projection
	glm::mat4 projection;
	projection = glm::perspective(
	    glm::radians(camera.Zoom),
	    (float)screenWidth / (float)screenHeight,
	    0.1f,
	    100.0f
        );

	int modelLoc = glGetUniformLocation(
	    ourShader.getID(),
	    "vModel"
	);

	glUniformMatrix4fv(
	    modelLoc,
	    1,
	    GL_FALSE,
	    glm::value_ptr(model)
	);

	int viewLoc = glGetUniformLocation(
	    ourShader.getID(),
	    "vView"
	);

	glUniformMatrix4fv(
	    viewLoc,
	    1,
	    GL_FALSE,
	    glm::value_ptr(view)
	);

	int projectionLoc = glGetUniformLocation(
	    ourShader.getID(),
	    "vProjection"
	);

	glUniformMatrix4fv(
	    projectionLoc,
	    1,
	    GL_FALSE,
	    glm::value_ptr(projection)
	);

	// draw
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++) {
	    glm::mat4 model;
	    model = glm::translate(model, cubePositions[i]);
	    float angle = 20.0f * i + currentTime * 50;
	    model = glm::rotate(
	        model,
		glm::radians(angle),
		glm::vec3(1.0f, 0.3f, 0.5f)
	    );
	    ourShader.setMat4("vModel", model);

	    glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// unbind
	glBindVertexArray(0);
	glUseProgram(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	// swap buffers
	window.display();

	sf::sleep(sf::milliseconds(5));
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}
