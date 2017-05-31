#include "cinder/app/AppScreenSaver.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class PixelPushSaverApp : public AppScreenSaver {
  public:
	void setup() override;
	void update() override;
	void draw() override;
	
  protected:
	ci::Color	mColor, mBackgroundColor;
	float		mRadius;
};

void PixelPushSaverApp::setup()
{
	mColor = Color( 1.0f, 0.5f, 0.25f );
	mBackgroundColor = Color( 0.25f, 0.0f, 0.0f );
}

void PixelPushSaverApp::update()
{
	mRadius = abs( cos( getElapsedSeconds() ) * 200 );
}

void PixelPushSaverApp::draw()
{
	gl::clear( mBackgroundColor );
	gl::color( mColor );
	gl::drawSolidCircle( getWindowCenter(), mRadius );
}

CINDER_APP_SCREENSAVER( PixelPushSaverApp, RendererGl )
