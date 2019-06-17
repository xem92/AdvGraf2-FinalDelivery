//
//  Copyright � 2018 Alun Evans. All rights reserved.
//
#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"

class ParticleEmitter {
public:
	void init();
	void init(int num_particles);
	void update();
private:
	GLuint vaoA_, vaoB_;
	GLuint tfA_, tfB_;
	int vaoSource = 0;
	Shader* particle_shader_;
	GLuint texture_id_;
};