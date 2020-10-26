#include <GL/glew.h>

#include <labyrinth_desktop/maze/maze_display.hpp>

void labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation::operate(
		MazeDisplay* display, const DisplayOptions& options) const {
	auto dur = decltype(display->lastOperate)::clock::now() -
			display->lastOperate;
	if (display->lastOperate ==
			decltype(decltype(display->lastOperate)::clock::now())()) {
		dur = decltype(dur)::zero();
	}
	double seconds = std::chrono::duration_cast<
			std::chrono::duration<double>>(dur).count();
	double velocity = options.getVelocity();
	double sensit = options.getRotatSensitivity();
	// IMPORTANT: WE IGNORE SQUARENESS HERE TOTALLY
	switch(type) {
	case MazeDisplayOperation::Operation::MOVE_FORWARD:
		display->viewer.moveForward(velocity * seconds);
		break;
	case MazeDisplayOperation::Operation::MOVE_UP:
		display->viewer.moveUp(velocity * seconds);
		break;
	case MazeDisplayOperation::Operation::MOVE_RIGHT:
		display->viewer.moveRight(velocity * seconds);
		break;
	case MazeDisplayOperation::Operation::MOVE_DOWN:
		display->viewer.moveDown(velocity * seconds);
		break;
	case MazeDisplayOperation::Operation::MOVE_LEFT:
		display->viewer.moveLeft(velocity * seconds);
		break;
	case MazeDisplayOperation::Operation::MOVE_BACKWARDS:
		display->viewer.moveBackwards(velocity * seconds);
		break;
	case MazeDisplayOperation::Operation::ROTATE_UP:
		display->viewer.rotateUp(sensit * seconds);
		break;
	case MazeDisplayOperation::Operation::ROTATE_RIGHT:
		display->viewer.rotateRight(sensit * seconds);
		break;
	case MazeDisplayOperation::Operation::ROTATE_DOWN:
		display->viewer.rotateDown(sensit * seconds);
		break;
	case MazeDisplayOperation::Operation::ROTATE_LEFT:
		display->viewer.rotateLeft(sensit * seconds);
		break;
	case MazeDisplayOperation::Operation::ROTATE_CLOCKWISE:
		display->viewer.rotateClockwise(sensit * seconds);
		break;
	case MazeDisplayOperation::Operation::ROTATE_COUNTERCLOCKWISE:
		display->viewer.rotateCounterClockwise(sensit * seconds);
		break;
	case MazeDisplayOperation::Operation::SWITCH_SLICE:
		display->viewer.setCurrSlice(slice1);
		break;
	case MazeDisplayOperation::Operation::TOGGLE_ROTAT_BINDING:
		display->viewer.toggleRotatBinding(slice1, slice2);
		break;
	case MazeDisplayOperation::Operation::SET_ROTAT_BINDING:
		display->viewer.setRotatBinding(slice1, slice2, binding);
		break;
	}
}

void labyrinth_desktop::maze::MazeDisplay::DisplayOptions::operate(
		MazeDisplay* display,
		const std::unordered_set<int>& keys) const {
	std::vector<int> vecKeys (keys.begin(), keys.end());
	std::vector<const MazeDisplayOperation*> toexec;
	std::unordered_set<int> tmp;
	size_t bitfield = 1 << vecKeys.size();
	for (size_t i = 1; i < bitfield; i++) {
		tmp.clear();
		for (size_t ind = 0, j = 1; j < bitfield; ind++, j <<= 1) {
			if (i & j) {
				tmp.insert(vecKeys[ind]);
			}
		}
		auto iter = controls.find(tmp);
		if (iter != controls.end()) {
			for (const MazeDisplayOperation& oper: iter->second) {
				MazeDisplayOperation::Operation type = oper.getType();
				bool isCompat = true;
				for (const MazeDisplayOperation* oper2: toexec) {
					if (!compatible(type, oper2->getType(), *this)) {
						isCompat = false;
						break;
					}
				}
				if (isCompat) {
					toexec.push_back(&oper);
				}
			}
		}
	}
	for (const MazeDisplayOperation* oper: toexec) {
		oper->operate(display, *this);
	}
	display->lastOperate =
			decltype(display->lastOperate)::clock::now();
}

bool labyrinth_desktop::maze::MazeDisplay::compatible(
		MazeDisplayOperation::Operation first,
		MazeDisplayOperation::Operation second,
		const DisplayOptions& options) {
	switch(options.getSquareness()) {
	case DisplayOptions::Squareness::SMOOTH:
		switch(first) {
		case MazeDisplayOperation::Operation::MOVE_FORWARD:
		case MazeDisplayOperation::Operation::MOVE_UP:
		case MazeDisplayOperation::Operation::MOVE_RIGHT:
		case MazeDisplayOperation::Operation::MOVE_DOWN:
		case MazeDisplayOperation::Operation::MOVE_LEFT:
		case MazeDisplayOperation::Operation::MOVE_BACKWARDS:
		case MazeDisplayOperation::Operation::SWITCH_SLICE:
		case MazeDisplayOperation::Operation::TOGGLE_ROTAT_BINDING:
		case MazeDisplayOperation::Operation::SET_ROTAT_BINDING:
			return true;
		case MazeDisplayOperation::Operation::ROTATE_UP:
		case MazeDisplayOperation::Operation::ROTATE_RIGHT:
		case MazeDisplayOperation::Operation::ROTATE_DOWN:
		case MazeDisplayOperation::Operation::ROTATE_LEFT:
		case MazeDisplayOperation::Operation::ROTATE_CLOCKWISE:
		case MazeDisplayOperation::Operation::ROTATE_COUNTERCLOCKWISE:
			switch(second) {
			case MazeDisplayOperation::Operation::MOVE_FORWARD:
			case MazeDisplayOperation::Operation::MOVE_UP:
			case MazeDisplayOperation::Operation::MOVE_RIGHT:
			case MazeDisplayOperation::Operation::MOVE_DOWN:
			case MazeDisplayOperation::Operation::MOVE_LEFT:
			case MazeDisplayOperation::Operation::MOVE_BACKWARDS:
			case MazeDisplayOperation::Operation::SWITCH_SLICE:
			case MazeDisplayOperation::Operation::TOGGLE_ROTAT_BINDING:
			case MazeDisplayOperation::Operation::SET_ROTAT_BINDING:
				return true;
			case MazeDisplayOperation::Operation::ROTATE_UP:
			case MazeDisplayOperation::Operation::ROTATE_RIGHT:
			case MazeDisplayOperation::Operation::ROTATE_DOWN:
			case MazeDisplayOperation::Operation::ROTATE_LEFT:
			case MazeDisplayOperation::Operation::ROTATE_CLOCKWISE:
			case MazeDisplayOperation::Operation::ROTATE_COUNTERCLOCKWISE:
				return false;
			}
		}
		// never going to happen
		break;
		case DisplayOptions::Squareness::JUST_ROTATION:
		case DisplayOptions::Squareness::SQUARE:
			switch(first) {
			case MazeDisplayOperation::Operation::SWITCH_SLICE:
			case MazeDisplayOperation::Operation::TOGGLE_ROTAT_BINDING:
			case MazeDisplayOperation::Operation::SET_ROTAT_BINDING:
				return true;
			case MazeDisplayOperation::Operation::MOVE_FORWARD:
			case MazeDisplayOperation::Operation::MOVE_UP:
			case MazeDisplayOperation::Operation::MOVE_RIGHT:
			case MazeDisplayOperation::Operation::MOVE_DOWN:
			case MazeDisplayOperation::Operation::MOVE_LEFT:
			case MazeDisplayOperation::Operation::MOVE_BACKWARDS:
				switch(second) {
				case MazeDisplayOperation::Operation::MOVE_FORWARD:
				case MazeDisplayOperation::Operation::MOVE_UP:
				case MazeDisplayOperation::Operation::MOVE_RIGHT:
				case MazeDisplayOperation::Operation::MOVE_DOWN:
				case MazeDisplayOperation::Operation::MOVE_LEFT:
				case MazeDisplayOperation::Operation::MOVE_BACKWARDS:
				case MazeDisplayOperation::Operation::SWITCH_SLICE:
				case MazeDisplayOperation::Operation::TOGGLE_ROTAT_BINDING:
				case MazeDisplayOperation::Operation::SET_ROTAT_BINDING:
					return true;
				case MazeDisplayOperation::Operation::ROTATE_UP:
				case MazeDisplayOperation::Operation::ROTATE_RIGHT:
				case MazeDisplayOperation::Operation::ROTATE_DOWN:
				case MazeDisplayOperation::Operation::ROTATE_LEFT:
				case MazeDisplayOperation::Operation::ROTATE_CLOCKWISE:
				case MazeDisplayOperation::Operation::ROTATE_COUNTERCLOCKWISE:
					return false;
				}
				break;
				case MazeDisplayOperation::Operation::ROTATE_UP:
				case MazeDisplayOperation::Operation::ROTATE_RIGHT:
				case MazeDisplayOperation::Operation::ROTATE_DOWN:
				case MazeDisplayOperation::Operation::ROTATE_LEFT:
				case MazeDisplayOperation::Operation::ROTATE_CLOCKWISE:
				case MazeDisplayOperation::Operation::ROTATE_COUNTERCLOCKWISE:
					switch(second) {
					case MazeDisplayOperation::Operation::SWITCH_SLICE:
					case MazeDisplayOperation::Operation::TOGGLE_ROTAT_BINDING:
					case MazeDisplayOperation::Operation::SET_ROTAT_BINDING:
						return true;
					case MazeDisplayOperation::Operation::MOVE_FORWARD:
					case MazeDisplayOperation::Operation::MOVE_UP:
					case MazeDisplayOperation::Operation::MOVE_RIGHT:
					case MazeDisplayOperation::Operation::MOVE_DOWN:
					case MazeDisplayOperation::Operation::MOVE_LEFT:
					case MazeDisplayOperation::Operation::MOVE_BACKWARDS:
					case MazeDisplayOperation::Operation::ROTATE_UP:
					case MazeDisplayOperation::Operation::ROTATE_RIGHT:
					case MazeDisplayOperation::Operation::ROTATE_DOWN:
					case MazeDisplayOperation::Operation::ROTATE_LEFT:
					case MazeDisplayOperation::Operation::ROTATE_CLOCKWISE:
					case MazeDisplayOperation::Operation::ROTATE_COUNTERCLOCKWISE:
						return false;
					}
			}
			// never going to happen
			break;
	}
	// also never going to happen
	return false;
}

namespace {

GLuint makeShader(const std::string& src, GLenum type, std::string* errMsg) {
	GLuint toreturn = glCreateShader(type);
	if (toreturn == 0) {
		return toreturn;
	}
	GLint len = src.size();
	const char* c_src = src.c_str();
	glShaderSource(toreturn, 1, &c_src, &len);
	glCompileShader(toreturn);
	GLint success;
	glGetShaderiv(toreturn, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logSize;
		glGetShaderiv(toreturn, GL_INFO_LOG_LENGTH, &logSize);
		char* infoLog = new char[logSize];
		glGetShaderInfoLog(toreturn, logSize, &logSize, infoLog);
		*errMsg = infoLog;
		delete[] infoLog;
		glDeleteShader(toreturn);
		return 0;
	}
	return toreturn;
}

}

labyrinth_desktop::maze::MazeDisplay::MazeDisplay(
		labyrinth_core::maze::Maze&& inMaze,
		const double* inCamera,
		const DisplayOptions& displayOptions,
		const labyrinth_core::maze::MazeViewer::ViewerOptions& viewerOptions,
		size_t width, size_t height):
			maze(inMaze, 0), viewer(inMaze, viewerOptions, inCamera),
			options(displayOptions),
			buf(0), program(0), posLoc(0), texcLoc(0),
			mouseX(0.0), mouseY(0.0) {
	inMaze.invalidate();
	if (options.getNumSlices() != viewer.getNumSlices()) {
		std::vector<std::tuple<double, double, double, double>> sliceLocs;
		for (size_t i = 0; i < viewer.getNumSlices(); i++) {
			sliceLocs.push_back(std::make_tuple(
					static_cast<double>(i) / viewer.getNumSlices(),
					0,
					1.0 / viewer.getNumSlices(),
					1.0));
		}
		options.setSliceLocs(sliceLocs);
	}
	setAspect(width, height);
	camera = new double[maze.getNumDims()];
	std::copy(inCamera, inCamera + maze.getNumDims(), camera);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (texture == 0) {
		errMsg = "Failed to make OpenGL texture";
		return;
	}
	glGenBuffers(1, &buf);
	if (buf == 0) {
		errMsg = "Failed to make OpenGL buffer";
		return;
	}
	GLuint vshad = makeShader("#version 110\n"
			"attribute vec2 pos; attribute vec2 texc; varying vec2 Texc;"
			"void main() {gl_Position = vec4(pos, 1.0, 1.0); Texc = texc;}",
			GL_VERTEX_SHADER, &errMsg);
	if (vshad == 0) {
		return;
	}
	GLuint fshad = makeShader("#version 110\n"
			"uniform sampler2D tex; varying vec2 Texc;"
			"void main() {gl_FragColor = texture2D(tex, Texc);}",
			GL_FRAGMENT_SHADER, &errMsg);
	if (fshad == 0) {
		return;
	}
	program = glCreateProgram();
	if (program == 0) {
		errMsg = "Failed to make OpenGL program";
		return;
	}
	glAttachShader(program, vshad);
	glAttachShader(program, fshad);
	glLinkProgram(program);
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* infoLog = new char[logSize];
		glGetProgramInfoLog(program, logSize, &logSize, infoLog);
		errMsg = infoLog;
		delete[] infoLog;
		glDetachShader(program, vshad);
		glDeleteShader(vshad);
		glDetachShader(program, fshad);
		glDeleteShader(fshad);
		glDeleteProgram(program);
		program = 0;
		return;
	}
	glDetachShader(program, vshad);
	glDeleteShader(vshad);
	glDetachShader(program, fshad);
	glDeleteShader(fshad);

	posLoc = glGetAttribLocation(program, "pos");
	texcLoc = glGetAttribLocation(program, "texc");
}

labyrinth_desktop::maze::MazeDisplay::~MazeDisplay() {
	glDeleteTextures(1, &texture);
	delete[] camera;
}

void labyrinth_desktop::maze::MazeDisplay::display() {
	options.operate(this, keyboardState);
	if (buf == 0) {
		return;
	}
	if (program == 0) {
		return;
	}
	if (texture == 0) {
		return;
	}
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(texcLoc);
	glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribPointer(texcLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
			static_cast<float*>(nullptr) + 2);
	std::vector<std::tuple<double, double, double, double>> locs =
			options.getSliceLocs();
	std::vector<std::pair<size_t, size_t>> sizes = viewer.getSliceSizes();
	std::vector<std::uint8_t*> result = viewer.render();
	float l, u, r, b;
	for (size_t i = 0; i < result.size(); i++) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizes[i].first, sizes[i].second,
				0, GL_RGBA, GL_UNSIGNED_BYTE, result[i]);
		l = std::get<0>(locs[i]);
		u = std::get<1>(locs[i]);
		r = std::get<2>(locs[i]);
		b = std::get<3>(locs[i]);
		float verts[] = {
				l, u, 0, 0,
				r, u, 1, 0,
				l, b, 0, 1,
				r, b, 1, 1
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		if (i == viewer.getCurrSlice()) {
			std::uint8_t select[] = {0xFF, 0xFF, 0, 0xFF};
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, select);
			float verts[] = {
					l, u, 0, 0,
					r, u, 0, 0,
					r, b, 0, 0,
					l, b, 0, 0,
					l, u, 0, 0
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
			// just in case this changes in the future
			glDrawArrays(GL_LINE_STRIP, 0, 5);
		}
	}
}

void labyrinth_desktop::maze::MazeDisplay::onWindowResize(size_t width, size_t height) {
	glViewport(0, 0, width, height);
	setAspect(width, height);
}
