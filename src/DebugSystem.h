#pragma once
#include "includes.h"
#include "Shader.h"
#include <vector>
#include "GraphicsSystem.h"
#include "ConsoleModule.h"


struct TransformNode {
	std::vector<TransformNode> children;
	int trans_id;
	int entity_owner;
	std::string ent_name;
	bool isTop = false;
};

class DebugSystem {
public:

	~DebugSystem();
	void init(GraphicsSystem* gs);
	void lateInit();
	void update(float dt);

	void setActive(bool a);

	//public imGUI functions
	bool isShowGUI() { return show_imGUI_; };
	void toggleimGUI() { show_imGUI_ = !show_imGUI_; };

	//public protoGUI functions
	bool isShowProtoGUI() { return show_protoGUI_; };
	void toggleprotoGUI() { show_protoGUI_ = !show_protoGUI_; };

	//public entitiesGUI functions
	bool isShowEntitiesGUI() { return show_entitiesGUI_; };
	void toggleentitiesGUI() { show_entitiesGUI_ = !show_entitiesGUI_; };

	//public materialsGUI functions
	bool isShowMaterialsGUI() { return show_materialsGUI_; };
	void togglematerialsGUI() { show_materialsGUI_ = !show_materialsGUI_; };

	//public consoleGUI functions
	bool isShowConsoleGUI() { return show_consoleGUI_; };
	void toggleconsoleGUI() { show_consoleGUI_ = !show_consoleGUI_; };

	//set picking ray
	void setPickingRay(int mouse_x, int mouse_y, int screen_width, int screen_height);

private:
	//graphics system pointer
	GraphicsSystem * graphics_system_;

	//bools to draw or not
	bool active_;
	bool draw_grid_;
	bool draw_icons_;
	bool draw_frustra_;
	bool draw_colliders_;
	bool draw_joints_;

	bool is_adding_new_material_;

	//cube for frustra and boxes
	void createCube_();
	GLuint cube_vao_;

	//colliders
	void createRay_();
	GLuint collider_ray_vao_;
	GLuint collider_box_vao_;

	//icons
	void createIcon_();
	GLuint icon_vao_;
	GLuint icon_light_texture_;
	GLuint icon_camera_texture_;

	//grid
	void createGrid_();
	GLuint grid_vao_;
	GLuint grid_num_indices;
	float grid_colors[12] = {
		0.7f, 0.7f, 0.7f, //grey
		1.0f, 0.5f, 0.5f, //red
		0.5f, 1.0f, 0.5f, //green
		0.5f, 0.5f, 1.0f }; //blue

							//shaders
	Shader* grid_shader_;
	Shader* icon_shader_;

	//drawing methods
	void drawGrid_();
	void drawIcons_();
	void drawFrusta_();
	void drawColliders_();
	void drawJoints_();

	//imGUI
	void imGuiRenderTransformNode(TransformNode& trans);
	bool show_imGUI_ = false;
	void updateimGUI_(float dt);

	//protoGui
	bool show_protoGUI_ = false;
	void updateprotoGUI_(float dt);

	//EntitiesGui
	//void entitiesGuiRenderTransformNode(TransformNode& trans);
	bool show_entitiesGUI_ = false;
	void update_entitiesGUI_(float dt);

	// addEntitieGui
	bool show_addEntitiesGUI_ = false;
	void update_addEntitiesGUI_(float dt);
	lm::vec3 position_;
	bool addTransform_ = false;
	bool addMesh_ = false;
	bool addLight_ = false;
	bool addCollider_ = false;
	int selected_geo_;
	int selected_mat_;
	std::string entity_name_;
	lm::vec3 light_direction_;
	lm::vec3 light_color_;
	int light_type_;
	float light_linear_att_;
	float light_quadratic_att_;
	float light_spot_inner_;
	float light_spot_outer_;
	bool light_cast_shadow_;
	int light_resolution_;
	int collider_type_;
	lm::vec3 collider_local_center_;
	lm::vec3 collider_local_halfwidth_;
	lm::vec3 collider_direction_;
	void resetAddEntity();

	// Add Component
	bool show_newComponentGUI_ = false;
	void update_NewComponentGUI_(float dt);
	int newComponent_entity_id_;
	const char* components_[3] = {"Mesh", "Light", "Collider"};
	int selected_component_ = 0;

	//MaterialsGui
	//void entitiesGuiRenderTransformNode(TransformNode& trans);
	bool show_materialsGUI_ = false;
	void update_materialsGUI_(float dt);

	//ConsoleGui
	//void entitiesGuiRenderTransformNode(TransformNode& trans);
	bool show_consoleGUI_ = false;
	void update_consoleGUI_(float dt);

	//picking
	bool can_fire_picking_ray_ = true;
	int ent_picking_ray_;

	//bones
	std::vector<GLuint> joints_vaos_;
	std::vector<GLuint> joints_chain_counts_;
	void createJointGeometry_();
	void getJointWorldMatrices_(Joint* current,
		lm::mat4 current_model,
		std::vector<float>& all_matrices,
		int& joint_count);
	Shader* joint_shader_;

	class ConsoleModule * console_module_;

};

