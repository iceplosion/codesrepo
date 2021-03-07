#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
void Camera::rotate(float hori, float vert) {
	hori *= SPEED_OF_ROTATE;
	vert *= SPEED_OF_ROTATE;
	na = alpha + vert;
	na = na < 0.5f ? 0.5f : na;
	na = na > (PI - 0.5f) ? (PI - 0.5f) : na;
	nb = beta + hori;
}
void Camera::push(float delta) {
	delta *= SPEED_OF_PUSH;
	nr = rho + delta < 1.f ? 1.f : rho + delta;
}
void Camera::update() {
	alpha = na;
	beta = nb;
	rho = nr;
}
glm::mat4 Camera::getView() {
	float theta = nb + PI / 4.f;
	float Sa = sin(na), Ca = cos(na);
	float Sb = sin(nb), Cb = cos(nb);
	float St = sin(theta), Ct = cos(theta);
	glm::vec3 D = glm::vec3(nr * Sa * Sb, nr * Ca, nr * Sa * Cb);
	glm::vec3 R = glm::vec3(nr * Sa * St, nr * Ca, nr * Sa * Ct) - D;
	glm::vec3 U = glm::cross(D, R);
	return glm::lookAt(D, glm::vec3(0.f, 0.f, 0.f), U);
}