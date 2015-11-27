// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"
#include "pointObj.h"
#include "mat.h"
#include "vec.h"

#include <FL/gl.h>
#include <stdlib.h>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1

static Mat4f CameraMatrix;
Mat4f getModelViewMatrix();
void SpawnParticles(Mat4f CameraTransforms);
void AddParticleStartingAt(Vec4<float> WorldPoint);
Mat4f getModelViewMatrix(){
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	return matMV.transpose(); // convert to row major
}
void SpawnParticles(Mat4f CameraTransforms)
{

	Mat4f ModelTransforms = CameraTransforms.inverse() * getModelViewMatrix();
	Vec4<float> WorldPoint = ModelTransforms * Vec4f(0, 0, 0, 1);
	AddParticleStartingAt(WorldPoint);
	return;

}

// This is a list of the controls for the RobotArm
// We'll use these constants to access the values 
// of the controls from the user interface.
enum RobotArmControls
{ 
    BASE_ROTATION=0, LOWER_TILT, UPPER_TILT, CLAW_ROTATION,
        BASE_LENGTH, LOWER_LENGTH, UPPER_LENGTH, PARTICLE_COUNT, 
		X_WIND, Y_WIND, Z_WIND, WIND_MAGNITUDE, GRAVITY,
		NUMCONTROLS, 
};
// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

void AddParticleStartingAt(Vec4<float> WorldPoint) {
	ParticleSystem* ps = ModelerApplication::Instance()->GetParticleSystem();
	for (int i = 0; i < VAL(PARTICLE_COUNT) * 5 ; i++) {

		float mag = rand() % 10 / 10.0 + 0.2;
		float theta = rand() % 360 / 57.3;
		float yVelocity = rand() % 10 / 10.0 + 2;
		float xVelocity = cos(theta) * mag;
		float zVelocity = sin(theta) * mag * 3;
		printf("add: %f, %f, %f\n", WorldPoint[0], WorldPoint[1], WorldPoint[2]);
		Vec3f position(WorldPoint[0], WorldPoint[1], WorldPoint[2]);
		Vec3f velocity(xVelocity, 15, zVelocity);
		Particle* p = new PointObj(1.0f, position, velocity);
		printf("added: %f, %f, %f\n", p->getPosition()[0], p->getPosition()[1], p->getPosition()[2]);

		ps->addParticle(p);
	}
}

void ground(float h);
void base(float h);
void rotation_base(float h);
void lower_arm(float h);
void upper_arm(float h);
void claw(float h);
void y_box(float h);

// To make a RobotArm, we inherit off of ModelerView
class RobotArm : public ModelerView 
{
public:
    RobotArm(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) {}
    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createRobotArm(int x, int y, int w, int h, char *label)
{ 
    return new RobotArm(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotArm
void RobotArm::draw()
{
	/* pick up the slider values */

	float theta = VAL( BASE_ROTATION );
	float phi = VAL( LOWER_TILT );
	float psi = VAL( UPPER_TILT );
	float cr = VAL( CLAW_ROTATION );
	float h1 = VAL( BASE_LENGTH );
	float h2 = VAL( LOWER_LENGTH );
	float h3 = VAL( UPPER_LENGTH );
	float pc = VAL( PARTICLE_COUNT );


    // This call takes care of a lot of the nasty projection 
    // matrix stuff
	ModelerView::draw();
	static GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	Mat4f temp = getModelViewMatrix();

	// define the model

	ground(-0.2);
	//ariou();
	base(0.8);

    glTranslatef( 0.0, 0.8, 0.0 );			// move to the top of the base
    glRotatef( theta, 0.0, 1.0, 0.0 );		// turn the whole assembly around the y-axis. 
	rotation_base(h1);						// draw the rotation base

    glTranslatef( 0.0, h1, 0.0 );			// move to the top of the base
    glRotatef( phi, 0.0, 0.0, 1.0 );		// rotate around the z-axis for the lower arm
	glTranslatef( -0.1, 0.0, 0.4 );
	lower_arm(h2);							// draw the lower arm

    glTranslatef( 0.0, h2, 0.0 );			// move to the top of the lower arm
    glRotatef( psi, 0.0, 0.0, 1.0 );		// rotate  around z-axis for the upper arm
	upper_arm(h3);							// draw the upper arm

	glTranslatef( 0.0, h3, 0.0 );
	glRotatef(cr, 0.0, 0.0, 1.0);
	claw(1.0);
	SpawnParticles(temp);

	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
	endDraw();
}

void Ariou() {
	Mat4f temp = getModelViewMatrix();
	glPushMatrix();
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
		glScaled(VAL(XSCALE), VAL(YSCALE), VAL(ZSCALE));
		glRotated(VAL(ROTATE), 0, 1, 0);
		setDiffuseColor(COLOR_YELLOW);

		// Torus
		if (VAL(DETAIL_LEVEL) > 1) {
			glPushMatrix();
				glTranslated(.0f, 6, .0f);
				drawTorus(VAL(TORUS_R), VAL(TORUS_r));
			glPopMatrix();
		}
		
		//head
		glPushMatrix();
			glTranslated(0, VAL(LEG_LENGTH) + 0.05 + VAL(HEIGHT) + 0.05 + VAL(HEAD_SIZE), 0);
			glRotated(VAL(HEAD_ROTATE), 0.0, 1.0, 0.0);
			drawSphere(VAL(HEAD_SIZE));
			if (VAL(DETAIL_LEVEL) > 2) {

				// Nose
				drawRoundCylinder(VAL(HEAD_SIZE) * 1.1, 0.2, 0.2);

				// Ear
				glPushMatrix();
				glTranslated(0.9 / sqrt(0.9*0.9 + 1.1*1.1) * VAL(HEAD_SIZE), 1.1 / sqrt(0.9*0.9 + 1.1*1.1) * VAL(HEAD_SIZE), 0);
				glRotated(-20, 0, 0, 1);
				SpawnParticles(temp);
				drawPyramid(VAL(EAR_SIZE));
				glPopMatrix();

				glPushMatrix();
				glTranslated(-0.9 / sqrt(0.9*0.9 + 1.1*1.1) * VAL(HEAD_SIZE), 1.1 / sqrt(0.9*0.9 + 1.1*1.1) * VAL(HEAD_SIZE), 0);
				glRotated(20, 0, 0, 1);
				SpawnParticles(temp);
				drawPyramid(VAL(EAR_SIZE));
				glPopMatrix();
				
				// Eyes
				glPushMatrix();
				setDiffuseColor(COLOR_RED);
				glTranslated(-0.5 / sqrt(0.5*0.5 * 2) * VAL(HEAD_SIZE) * 0.5, 0.5 / sqrt(0.5*0.5 * 2) * VAL(HEAD_SIZE) * 0.5, VAL(HEAD_SIZE) - 0.9);
					drawRoundCylinder( 0.9, 0.2, 0.2);
				glPopMatrix();
				glPushMatrix();
				setDiffuseColor(COLOR_RED);
				glTranslated( 0.5 / sqrt(0.5*0.5 * 2) * VAL(HEAD_SIZE) * 0.5, 0.5 / sqrt(0.5*0.5 * 2) * VAL(HEAD_SIZE) * 0.5, VAL(HEAD_SIZE) - 0.9);
					drawRoundCylinder( 0.9, 0.2, 0.2);
				glPopMatrix();
			}
		glPopMatrix();

		if (VAL(DETAIL_LEVEL) > 1) {
			//body
			// a.k.a. torso/trunk
			glPushMatrix();
			setDiffuseColor(COLOR_YELLOW);
			glTranslated(0, 0.05 + VAL(LEG_LENGTH), 0);
			glRotated(-90, 1.0, 0.0, 0.0);
			drawRoundCylinder(VAL(HEIGHT), 0.7, 0.6);
			glPushMatrix();
			glTranslated(-0.8, 0, VAL(HEIGHT) - 0.4);
			glRotated(90, 0, 1, 0);
			// the shoulder
			if (VAL(DETAIL_LEVEL) > 2) {
				drawRoundCylinder(1.6, 0.2, 0.2);
			}
			glPopMatrix();

			// the waist
			if (VAL(DETAIL_LEVEL) > 3) {
				glPushMatrix();
				glTranslated(0, 0, 0.5);
				glRotated(90, 1, 0, 0);
				drawTorus(0.7, 0.08);
				glPopMatrix();
			}

			glPopMatrix();

			if (VAL(DETAIL_LEVEL) > 2) {
				//right arm
				glPushMatrix();
				glTranslated(-0.7 - 0.20, VAL(LEG_LENGTH) + 0.05 + VAL(HEIGHT) * 0.9f, 0);
				glTranslated(0.15, 0, 0);
				glRotated(VAL(RIGHT_UPPER_ARM_ROTATE_X), 1.0, 0.0, 0.0);
				glRotated(VAL(RIGHT_UPPER_ARM_ROTATE_Y), 0.0, 1.0, 0.0);
				glTranslated(-0.15, 0, 0);
				drawRoundCylinder(VAL(UPPER_ARM_LENGTH), 0.22, 0.15);

				// lower arm
				glTranslated(0, 0, VAL(UPPER_ARM_LENGTH) - 0.1);
				glRotated(VAL(RIGHT_LOWER_ARM_ROTATE) - 180, 1, 0, 0);
				drawRoundCylinder(VAL(LOWER_ARM_LENGTH), 0.15, 0.20);

				// hand
				glPushMatrix();
				glTranslated(-0.03, -0.15, VAL(LOWER_ARM_LENGTH) - 0.1);
				glRotated(VAL(RIGHT_HAND_ANGLE), 0, 1, 0);
				drawCylinder(0.8, 0.15, 0.0001);
				glPopMatrix();

				glPushMatrix();
				glTranslated(0.03, -0.15, VAL(LOWER_ARM_LENGTH) - 0.1);
				glRotated(-VAL(RIGHT_HAND_ANGLE), 0, 1, 0);
				drawCylinder(0.8, 0.15, 0.0001);
				glPopMatrix();

				glPopMatrix();

				//left arm
				glPushMatrix();
				glTranslated(0.7 + 0.20, VAL(LEG_LENGTH) + 0.05 + VAL(HEIGHT) * 0.9f, 0);
				glTranslated(-0.15, 0, 0);
				glRotated(VAL(LEFT_UPPER_ARM_ROTATE_X), 1.0, 0.0, 0.0);
				glRotated(VAL(LEFT_UPPER_ARM_ROTATE_Y), 0.0, 1.0, 0.0);
				glTranslated(0.15, 0, 0);
				drawRoundCylinder(VAL(UPPER_ARM_LENGTH), 0.22, 0.15);

				glTranslated(0, 0, VAL(UPPER_ARM_LENGTH) - 0.1);
				glRotated(VAL(LEFT_LOWER_ARM_ROTATE) - 180, 1, 0, 0);
				drawRoundCylinder(VAL(LOWER_ARM_LENGTH), 0.15, 0.20);

				// hand
				glPushMatrix();
				glTranslated(-0.03, 0, VAL(LOWER_ARM_LENGTH) - 0.1);
				glRotated(VAL(LEFT_HAND_ANGLE), 0, 1, 0);
				drawBox(0.03, 0.25, 0.5);
				glPopMatrix();

				glPushMatrix();
				glTranslated(0.03, 0, VAL(LOWER_ARM_LENGTH) - 0.1);
				glRotated(-VAL(LEFT_HAND_ANGLE), 0, 1, 0);
				drawBox(0.03, 0.25, 0.5);
				if (VAL(DETAIL_LEVEL) > 3) {
					glRotated(90, 0, 0, 1);
					drawCylinder(5, 0.02, 0.02);
					if (VAL(DETAIL_LEVEL) > 4) {
						glTranslated(0, 0, 4);

						glPushMatrix();
						setDiffuseColor(COLOR_GREEN);
						glTranslated(0, -0.5, 1);
						SpawnParticles(temp);
						glPushMatrix();
						glRotated(90, 1.0, 0.0, 0.0);
						glPopMatrix();
						glPopMatrix();
					}
				}
				glPopMatrix();

				glPopMatrix();
			}

			//right leg
			glPushMatrix();
			setDiffuseColor(COLOR_YELLOW);
			glTranslated(-0.5, VAL(LEG_LENGTH), 0);
			glRotated(VAL(RIGHT_LEG_ROTATE_X), 1.0, 0.0, 0.0);
			glRotated(VAL(RIGHT_LEG_ROTATE_Y), 0.0, 1.0, 0.0);
			drawRoundCylinder(VAL(LEG_LENGTH) - 0.15, 0.3, 0.4);
			glTranslated(0, 0, VAL(LEG_LENGTH) * 0.85f);
			glRotated(70, 1, 0, 0);
			drawTorus(VAL(FEET_SIZE), VAL(FEET_SIZE) / 4);

			glPopMatrix();

			//left leg
			glPushMatrix();
			glTranslated(0.5, VAL(LEG_LENGTH), 0);
			glRotated(VAL(LEFT_LEG_ROTATE_X), 1.0, 0.0, 0.0);
			glRotated(VAL(LEFT_LEG_ROTATE_Y), 0.0, 1.0, 0.0);
			drawRoundCylinder(VAL(LEG_LENGTH) - 0.15, 0.3, 0.4);
			glTranslated(0, 0, VAL(LEG_LENGTH) * 0.85f);
			glRotated(70, 1, 0, 0);
			drawTorus(VAL(FEET_SIZE), VAL(FEET_SIZE) / 4);

			glPopMatrix();
		}
	glPopMatrix();
}

void ground(float h) 
{
	glDisable(GL_LIGHTING);
	glColor3f(0.65,0.45,0.2);
	glPushMatrix();
	glScalef(30,0,30);
	y_box(h);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void base(float h) {
	setDiffuseColor( 0.25, 0.25, 0.25 );
	setAmbientColor( 0.25, 0.25, 0.25 );
	glPushMatrix();
		glPushMatrix();
			glTranslatef(1.0, h / 2.0, 0.75);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.0, h / 2.0, -1.0);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0, h / 2.0, 0.75);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0, h / 2.0, -1.0);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
	glScalef(4.0f, h, 4.0f);
	y_box(1.0f);
	glPopMatrix();
}

void rotation_base(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	glPushMatrix();
		glPushMatrix();
			glScalef(4.0, h, 4.0);
			y_box(1.0f); // the rotation base
		glPopMatrix();
		setDiffuseColor( 0.15, 0.15, 0.65 );
		setAmbientColor( 0.15, 0.15, 0.65 );
		glPushMatrix();
			glTranslatef(-0.5, h, -0.6);
			glScalef(2.0, h, 1.6);
			y_box(1.0f); // the console
		glPopMatrix();
		setDiffuseColor( 0.65, 0.65, 0.65 );
		setAmbientColor( 0.65, 0.65, 0.65 );
		glPushMatrix();
			glTranslatef( 0.5, h, 0.6 );
			glRotatef( -90.0, 1.0, 0.0, 0.0 );
			drawCylinder( h, 0.05, 0.05 ); // the pipe
		glPopMatrix();
	glPopMatrix();
}

void lower_arm(float h) {					// draw the lower arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	y_box(h);
}

void upper_arm(float h) {					// draw the upper arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	glPushMatrix();
	glScalef( 1.0, 1.0, 0.7 );
	y_box(h);
	glPopMatrix();
}

void claw(float h) {
	setDiffuseColor( 0.25, 0.25, 0.85 );
	setAmbientColor( 0.25, 0.25, 0.85 );

	glBegin( GL_TRIANGLES );

	glNormal3d( 0.0, 0.0, 1.0);		// +z side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d(-0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);

	glNormal3d( 0.0, 0.0, -1.0);	// -z side
	glVertex3d( 0.5, 0.0, -0.5);
	glVertex3d(-0.5, 0.0, -0.5);
	glVertex3d( 0.5,   h, -0.5);

	glEnd();
	glBegin( GL_QUADS );

	glNormal3d( 1.0,  0.0,  0.0);	// +x side
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);
	glVertex3d( 0.5,   h,-0.5);

	glNormal3d( 0.0,-1.0, 0.0);		// -y side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0, 0.5);

	glEnd();
}

void y_box(float h) {

	glBegin( GL_QUADS );

	glNormal3d( 1.0 ,0.0, 0.0);			// +x side
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d( 0.25,  h, 0.25);

	glNormal3d( 0.0 ,0.0, -1.0);		// -z side
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d( 0.25,  h,-0.25);

	glNormal3d(-1.0, 0.0, 0.0);			// -x side
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,  h, 0.25);
	glVertex3d(-0.25,  h,-0.25);

	glNormal3d( 0.0, 0.0, 1.0);			// +z side
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d( 0.25,0.0,-0.25);

	glEnd();
}

int main()
{
    ModelerControl controls[NUMCONTROLS ];

	controls[BASE_ROTATION] = ModelerControl("base rotation (theta)", -180.0, 180.0, 0.1, 0.0 );
    controls[LOWER_TILT] = ModelerControl("lower arm tilt (phi)", 15.0, 95.0, 0.1, 55.0 );
    controls[UPPER_TILT] = ModelerControl("upper arm tilt (psi)", 0.0, 135.0, 0.1, 30.0 );
	controls[CLAW_ROTATION] = ModelerControl("claw rotation (cr)", -30.0, 180.0, 0.1, 0.0 );
    controls[BASE_LENGTH] = ModelerControl("base height (h1)", 0.5, 10.0, 0.1, 0.8 );
    controls[LOWER_LENGTH] = ModelerControl("lower arm length (h2)", 1, 10.0, 0.1, 3.0 );
	controls[UPPER_LENGTH] = ModelerControl("upper arm length (h3)", 1, 10.0, 0.1, 2.5);
	controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0);
	controls[X_WIND] = ModelerControl("X_WIND", 0.0, 10.0, 0.1, 4.6);
	controls[Y_WIND] = ModelerControl("Y_WIND", 0.0, 10.0, 0.1, 5.0);
	controls[Z_WIND] = ModelerControl("Z_WIND", 0.0, 10.0, 0.1, 5.2);
	controls[WIND_MAGNITUDE] = ModelerControl("WIND_MAGNITUDE", 0.0, 10.0, 0.1, 6.0);
	controls[GRAVITY] = ModelerControl("GRAVITY", -10.0, 10.0, 1.0, 9.8);
    


	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.

	ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);
	ParticleSystem *ps = new ParticleSystem();
	ModelerApplication::Instance()->SetParticleSystem(ps);
	Force* g = new Gravity((float)-VAL(GRAVITY));
	//Force* g = new Gravity(9.8f);
	ps->addForce(g);
	Wind *wind = new Wind();
	wind->setDirection(Vec3f((float)-VAL(X_WIND) - 5.0, (float)VAL(Y_WIND) - 5.0, (float)VAL(Y_WIND) - 5.0));
	//wind->setDirection(Vec3f(-0.4, 0, 0.2));
	wind->setMagnitube((float)VAL(WIND_MAGNITUDE));
	//wind->setMagnitube(6.0);
	ps->addForce(wind);
	

    return ModelerApplication::Instance()->Run();
}
