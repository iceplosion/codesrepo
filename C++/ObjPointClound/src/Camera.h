#include <glm/glm.hpp>
class Camera {
private:
	const float PI = (float)acos(-1.);
	const float SQRT2 = (float)sqrt(2.);
	const float SPEED_OF_ROTATE = 0.005f;
	const float SPEED_OF_PUSH = 0.005f;
	float rho, alpha, beta;
	float nr, na, nb;
public:
	Camera() : rho(2.f), alpha(PI / 2.f), beta(0.f), nr(2.f), na(PI / 2.f), nb(0.f) {}
	// degree in radian system of horizental rotation and vertical rotation
	void rotate(float hori, float vert);
	void push(float delta);
	void update();
	// construct view matrix
	glm::mat4 getView();
};