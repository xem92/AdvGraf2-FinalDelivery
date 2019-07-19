//
//  Game.cpp
//
//  Copyright � 2018 Alun Evans. All rights reserved.
//

#include "Game.h"
#include "Shader.h"
#include "extern.h"
#include "Parsers.h"


Game::Game() {

}

//Nothing here yet
void Game::init(int w, int h) {

	window_width_ = w; window_height_ = h;
	//******* INIT SYSTEMS *******

	//init systems except debug, which needs info about scene
	control_system_.init();
	graphics_system_.init(window_width_, window_height_, "data/assets/");
	debug_system_.init(&graphics_system_);
    script_system_.init(&control_system_);
	gui_system_.init(window_width_, window_height_);
    animation_system_.init();
    
    graphics_system_.screen_background_color = lm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    createFreeCamera_(13.614, 16, 32, -0.466, -0.67, -0.579);
    
	Shader* phong_shader = graphics_system_.loadShader("data/shaders/phong.vert", "data/shaders/phong.frag");
	//int sphere_geom = graphics_system_.createGeometryFromFile("data/assets/sphere.obj");
	int floor_geom = graphics_system_.createGeometryFromFile("data/assets/floor_40x40.obj");

	int mat_red_check_index = graphics_system_.createMaterial();
	Material& mat_red_check = graphics_system_.getMaterial(mat_red_check_index);
	mat_red_check.shader_id = phong_shader->program;
	mat_red_check.name = "Mat red";
	mat_red_check.ambient = lm::vec3(1.0f, 1.0f, 1.0f);
	mat_red_check.diffuse = lm::vec3(1.0f, 0.0f, 0.0f);
	mat_red_check.specular = lm::vec3(0, 0, 0);

	int mat_green_check_index = graphics_system_.createMaterial();
	Material& mat_green_check = graphics_system_.getMaterial(mat_green_check_index);
	mat_green_check.shader_id = phong_shader->program;
	mat_green_check.name = "Mat green";
	mat_green_check.ambient = lm::vec3(1.0f, 1.0f, 1.0f);
	mat_green_check.diffuse = lm::vec3(0.0f, 1.0f, 0.0f);
	mat_green_check.specular = lm::vec3(0, 0, 0);

	int mat_blue_check_index = graphics_system_.createMaterial();
	Material& mat_blue_check = graphics_system_.getMaterial(mat_blue_check_index);
	mat_blue_check.shader_id = phong_shader->program;
	mat_blue_check.name = "Mat blue";
	mat_blue_check.diffuse_map = Parsers::parseTexture("data/assets/block_blue.tga");
	mat_blue_check.specular = lm::vec3(0, 0, 0);

	int sphere_entity = ECS.createEntity("phong_sphere");
	Transform& st = ECS.getComponentFromEntity<Transform>(sphere_entity);
	st.translate(0.0f, 0.0f, 0.0f);
	Mesh& sphere_mesh = ECS.createComponentForEntity<Mesh>(sphere_entity);
	sphere_mesh.geometry = graphics_system_.sphere_volume_geom_;
	sphere_mesh.material = mat_red_check_index;
	sphere_mesh.render_mode = RenderModeForward;

	int sphere_entity2 = ECS.createEntity("phong_sphere2");
	Transform& st2 = ECS.getComponentFromEntity<Transform>(sphere_entity2);
	st2.translate(0.0f, 5.0f, 0.0f);
	Mesh& sphere_mesh2 = ECS.createComponentForEntity<Mesh>(sphere_entity2);
	sphere_mesh2.geometry = graphics_system_.sphere_volume_geom_;
	sphere_mesh2.material = mat_green_check_index;
	sphere_mesh2.render_mode = RenderModeForward;

	int ent_light_dir = ECS.createEntity("light_dir");
	ECS.getComponentFromEntity<Transform>(ent_light_dir).translate(5, 9, 10);
	Light& light_comp_dir = ECS.createComponentForEntity<Light>(ent_light_dir);
	light_comp_dir.color = lm::vec3(0.99f, 0.99f, 0.99f);
	light_comp_dir.direction = lm::vec3(-0.5, -0.5, -0.5);
	light_comp_dir.type = LightTypeSpot; //change for direction or spot
	light_comp_dir.linear_att = 0.027f;
	light_comp_dir.quadratic_att = 0.0028f;
	light_comp_dir.spot_inner = 50.0f;
	light_comp_dir.spot_inner = 60.0f;
	light_comp_dir.position = lm::vec3(5, 9, 10);
	light_comp_dir.forward = light_comp_dir.direction.normalize();
	light_comp_dir.setPerspective(60 * DEG2RAD, 1, 5, 50);
	light_comp_dir.update();
	light_comp_dir.cast_shadow = true;

    //******* LATE INIT AFTER LOADING RESOURCES *******//
    graphics_system_.lateInit();
    script_system_.lateInit();
    animation_system_.lateInit();
    debug_system_.lateInit();

	debug_system_.setActive(true);

}

//update each system in turn
void Game::update(float dt) {

	if (ECS.getAllComponents<Camera>().size() == 0) {print("There is no camera set!"); return;}

	//update input
	control_system_.update(dt);

	//collision
	collision_system_.update(dt);

    //animation
    animation_system_.update(dt);
    
	//scripts
	script_system_.update(dt);

	//render
	graphics_system_.update(dt);
    
    //particles
    //particle_emitter_->update();
    
	//gui
	gui_system_.update(dt);

	//debug
	debug_system_.update(dt);
   
}
//update game viewports
void Game::update_viewports(int window_width, int window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	auto& cameras = ECS.getAllComponents<Camera>();
	for (auto& cam : cameras) {
		cam.setPerspective(60.0f*DEG2RAD, (float)window_width_ / (float) window_height_, 0.01f, 10000.0f);
	}

	graphics_system_.updateMainViewport(window_width_, window_height_);
}

Material& Game::createMaterial(GLuint shader_program) {
    int mat_index = graphics_system_.createMaterial();
    Material& ref_mat = graphics_system_.getMaterial(mat_index);
    ref_mat.shader_id = shader_program;
    return ref_mat;
}

int Game::createFreeCamera_(float px, float py, float pz, float fx, float fy, float fz) {
	int ent_player = ECS.createEntity("PlayerFree");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
    lm::vec3 the_position(px, py, px);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(the_position);
	player_cam.position = the_position;
	player_cam.forward = lm::vec3(fx, fy, fz);
	player_cam.setPerspective(60.0f*DEG2RAD, (float)window_width_/(float)window_height_, 0.1f, 1000.0f);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	control_system_.control_type = ControlTypeFree;

	return ent_player;
}

int Game::createPlayer_(float aspect, ControlSystem& sys) {
	int ent_player = ECS.createEntity("PlayerFPS");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
	lm::vec3 the_position(0.0f, 3.0f, 5.0f);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(the_position);
	player_cam.position = the_position;
	player_cam.forward = lm::vec3(0.0f, 0.0f, -1.0f);
	player_cam.setPerspective(60.0f*DEG2RAD, aspect, 0.01f, 10000.0f);

	//FPS colliders 
	//each collider ray entity is parented to the playerFPS entity
	int ent_down_ray = ECS.createEntity("Down Ray");
	Transform& down_ray_trans = ECS.getComponentFromEntity<Transform>(ent_down_ray);
	down_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& down_ray_collider = ECS.createComponentForEntity<Collider>(ent_down_ray);
	down_ray_collider.collider_type = ColliderTypeRay;
	down_ray_collider.direction = lm::vec3(0.0, -1.0, 0.0);
	down_ray_collider.max_distance = 100.0f;

	int ent_left_ray = ECS.createEntity("Left Ray");
	Transform& left_ray_trans = ECS.getComponentFromEntity<Transform>(ent_left_ray);
	left_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& left_ray_collider = ECS.createComponentForEntity<Collider>(ent_left_ray);
	left_ray_collider.collider_type = ColliderTypeRay;
	left_ray_collider.direction = lm::vec3(-1.0, 0.0, 0.0);
	left_ray_collider.max_distance = 1.0f;

	int ent_right_ray = ECS.createEntity("Right Ray");
	Transform& right_ray_trans = ECS.getComponentFromEntity<Transform>(ent_right_ray);
	right_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& right_ray_collider = ECS.createComponentForEntity<Collider>(ent_right_ray);
	right_ray_collider.collider_type = ColliderTypeRay;
	right_ray_collider.direction = lm::vec3(1.0, 0.0, 0.0);
	right_ray_collider.max_distance = 1.0f;

	int ent_forward_ray = ECS.createEntity("Forward Ray");
	Transform& forward_ray_trans = ECS.getComponentFromEntity<Transform>(ent_forward_ray);
	forward_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& forward_ray_collider = ECS.createComponentForEntity<Collider>(ent_forward_ray);
	forward_ray_collider.collider_type = ColliderTypeRay;
	forward_ray_collider.direction = lm::vec3(0.0, 0.0, -1.0);
	forward_ray_collider.max_distance = 1.0f;

	int ent_back_ray = ECS.createEntity("Back Ray");
	Transform& back_ray_trans = ECS.getComponentFromEntity<Transform>(ent_back_ray);
	back_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& back_ray_collider = ECS.createComponentForEntity<Collider>(ent_back_ray);
	back_ray_collider.collider_type = ColliderTypeRay;
	back_ray_collider.direction = lm::vec3(0.0, 0.0, 1.0);
	back_ray_collider.max_distance = 1.0f;

	//the control system stores the FPS colliders 
	sys.FPS_collider_down = ECS.getComponentID<Collider>(ent_down_ray);
	sys.FPS_collider_left = ECS.getComponentID<Collider>(ent_left_ray);
	sys.FPS_collider_right = ECS.getComponentID<Collider>(ent_right_ray);
	sys.FPS_collider_forward = ECS.getComponentID<Collider>(ent_forward_ray);
	sys.FPS_collider_back = ECS.getComponentID<Collider>(ent_back_ray);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	sys.control_type = ControlTypeFPS;

	return ent_player;
}

