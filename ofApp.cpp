#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(0.5);

	this->hexagon_height = 8;
	this->hexagon_width = 5;

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 300, true, true, true);
	/* https://github.com/kazesawa/kazesawa */

	font.drawString("13", font.stringWidth("13") * -0.5, font.stringHeight("13") * -0.05);
	font.drawString("00", font.stringWidth("00") * -0.5, font.stringHeight("00") * 1);

	fbo.end();

	ofPixels pixels;
	fbo.readToPixels(pixels);

	ofColor pix_color;
	for (float x = 24; x <= ofGetWidth() - 24; x += 12) {

		for (float y = 24; y <= ofGetHeight() - 24; y += 24) {

			pix_color = pixels.getColor(x, y);
			if (pix_color != ofColor(0, 0)) {

				this->number_list.push_back(make_pair<glm::vec2, bool>(glm::vec2(x, y), true));
			}
			else {

				this->number_list.push_back(make_pair<glm::vec2, bool>(glm::vec2(x, y), false));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (auto& number : this->number_list) {

		if (number.second) {

			int number_index = ofMap(ofNoise(glm::vec3(number.first * 0.05, ofGetFrameNum() * 0.05)), 0, 1, 0, 10);
			this->draw_digital(number.first, number_index, ofColor(39), ofColor(239));
		}
		else {

			this->draw_digital(number.first, 0, ofColor(150), ofColor(239));
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw_digital(glm::vec2 location, int number_index, ofColor fill_color, ofColor frame_color) {

	vector<pair<glm::vec2, float>> part_list = {
		make_pair<glm::vec2, float>(location + glm::vec2(0, -this->hexagon_height), 90),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * -0.5, this->hexagon_height * 0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * 0.5, this->hexagon_height * 0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(0, 0), 90),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * -0.5, this->hexagon_height * -0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * 0.5, this->hexagon_height * -0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(0, this->hexagon_height), 90)
	};

	vector<vector<int>> index_list = {
		{ 0, 1, 2, 4, 5, 6 },
		{ 2, 5 },
		{ 0, 1, 3, 5, 6 },
		{ 0, 2, 3, 5, 6 },
		{ 2, 3, 4, 5 },
		{ 0, 2, 3, 4, 6 },
		{ 0, 1, 2, 3, 4, 6 },
		{ 0, 2, 5 },
		{ 0, 1, 2, 3, 4, 5, 6 },
		{ 0, 2, 3, 4, 5, 6 },
	};

	for (auto& index : index_list[number_index]) {

		this->draw_hexagon(part_list[index].first, part_list[index].second, fill_color, frame_color);
	}
}

//--------------------------------------------------------------
void ofApp::draw_hexagon(glm::vec2 location, float deg, ofColor fill_color, ofColor frame_color) {

	ofPushMatrix();
	ofTranslate(location);
	ofRotate(deg);

	vector<glm::vec2> vertices;
	vertices.push_back(glm::vec2(this->hexagon_width * -0.4, this->hexagon_height * -0.4));
	vertices.push_back(glm::vec2(this->hexagon_width * -0.4, this->hexagon_height * 0.4));
	vertices.push_back(glm::vec2(0, this->hexagon_height * 0.5));
	vertices.push_back(glm::vec2(this->hexagon_width * 0.4, this->hexagon_height * 0.4));
	vertices.push_back(glm::vec2(this->hexagon_width * 0.4, this->hexagon_height * -0.4));
	vertices.push_back(glm::vec2(0, this->hexagon_height * -0.5));

	ofFill();
	ofSetColor(fill_color);

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	ofNoFill();
	ofSetColor(frame_color);

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}