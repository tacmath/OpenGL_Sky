#include "camera.h"

// Default constructor
Camera::Camera() {
	width = 1;
	height = 1;
	fov = 80;
	pitch = 0;
	yaw = 90;
	position = glm::vec3(0.0f);
	direction = glm::vec3(0.0f, 0.0f, 1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
};

// init the camera
void Camera::Init(float windowWidth, float windowHeight, glm::vec3 pos) {
	position = pos;
	view = glm::lookAt(position, position + direction, up);
	ChangePerspective(fov, windowWidth, windowHeight, 0.1f, 10.0f);
}

// treat inputs to change the camera
void Camera::Update() {
	view = glm::lookAt(position, position + direction, up);
}

// treat inputs to change the camera view
void Camera::UpdateView() {
	view = glm::lookAt(position, position + direction, up);
}


// set the position of the camera with 3 float
void Camera::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

// set the position of the camera with a vec3
void Camera::SetPosition(glm::vec3 pos) {
	position = pos;
}

//set the divection of the camera
void Camera::SetDirection(glm::vec3 direction) {
	this->direction = direction;
	pitch = asin(-direction.y) * 180 / 3.14f;
	yaw = atan2(direction.z, direction.x) * 180 / 3.14f;
}

// change the prespective matrix
void Camera::ChangePerspective(float FOV, float windowWidth, float windowHeight, float near, float far) {
	if (FOV) //maybe add a SetFOV and UpdatePerspective instead
		fov = FOV;
	if (windowWidth)
		width = windowWidth;
	if (windowHeight)
		height = windowHeight;
	if (far)
		this->far = far;
	projection = glm::perspective(glm::radians(fov), (float)(width / height), near, this->far);
}

void Camera::Rotate(float _yaw, float _pitch) {
	pitch += _pitch;
	yaw += _yaw;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = -sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction = glm::normalize(front);
}

float Camera::GetFOV() {
	return fov;
}

float Camera::GetScreenRatio() {
	return width / height;
}
