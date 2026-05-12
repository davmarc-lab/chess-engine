#include "ui/app_gui.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <string>

#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"

const auto em = EntityManager::instance();
const auto ed = EventManager::instance();

unsigned int npent = 0;
Shared<RenderComponent> rc = nullptr;
Shared<BufferComponent> bc = nullptr;
Shared<VertexComponent> vc = nullptr;

static unsigned int lightCompSel = 0;

void ImGuiEntityTree::onRender() {
	ImGui::Begin("Entities", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
	for (auto id : em->getEntities()) {
		if (em->entityHasComponent<HideTreeComponent>(id))
			continue;
		if (ImGui::TreeNode(systems::ecs::getEntityName(id).c_str())) {
			ImGui::PushID(&id);
			ImGui::Text("(ECS) Entity Id: %s", std::to_string(id).c_str());

			auto ivc = em->getComponentFromId<VertexComponent>(id);
			// the entity has the component
			if (ivc != nullptr) {
				if (!ivc->getNormalsCoords().empty()) {
					if (ImGui::Button("Normals##0")) {
						npent = id;
						ed->post(NORMAL_VIEW_OPEN);
						rc = em->getComponentFromId<RenderComponent>(id);
						ASSERT(rc != nullptr);
						bc = em->getComponentFromId<BufferComponent>(id);
						ASSERT(bc != nullptr);
						vc = em->getComponentFromId<VertexComponent>(id);
						ASSERT(vc != nullptr);
					}
				}
			}

			if (em->entityHasComponent<Transform>(id)) {
				if (ImGui::CollapsingHeader("Transform##0")) {
					auto pos = systems::transform::getPosition(id);
					if (ImGui::DragFloat3("Position##0", &pos.x, 0.2f)) {
						systems::transform::updatePosition(id, pos);
					}

					auto scale = systems::transform::getScale(id);
					if (ImGui::DragFloat3("Scale##0", &scale.x, 0.2f)) {
						systems::transform::updateScale(id, scale);
					}

					auto rot = glm::degrees(systems::transform::getRotation(id));
					if (ImGui::DragFloat3("Rotation##0", &rot.x)) {
						systems::transform::updateRotation(id, rot);
					}
				}
			}

			if (em->entityHasComponent<TextureComponent>(id)) {
				if (ImGui::CollapsingHeader("Texture##3")) {
					ImGui::Text("%s", systems::texture::getTexturePath(id).c_str());
					ImGui::SameLine();
					if (ImGui::Button("Change")) {
						std::cout << "CHANGE TEXTURE\n";
					}
				}
			}

			if (em->entityHasComponent<CameraComponent>(id)) {
				if (ImGui::Button("Reset##4")) {
					ed->post(CAMERA_RESET_POSITION);
				}
				if (ImGui::CollapsingHeader("Camera##4")) {
					auto camera = systems::camera::getCamera(id);
					if (ImGui::CollapsingHeader("World Info##4")) {
						auto pos = camera->getCameraPosition();
						if (ImGui::DragFloat3("Position##4", &pos.x)) {
							camera->setCameraPosition(pos);
							ed->post(CAMERA_UPDATE_DATA);
						}
					}
					if (ImGui::CollapsingHeader("Camera Info##4")) {
						auto speed = camera->getCameraVelocity();
						if (ImGui::SliderFloat("Velocity##4", &speed, 0.01f, 1.f)) {
							camera->setCameraVelocity(speed);
						}
						auto sens = camera->getMouseSensitivity();
						if (ImGui::SliderFloat("Sensitivity##4", &sens, 0.01f, 1.f)) {
							camera->setMouseSensitivity(sens);
						}
						auto zoom = camera->getCameraZoom();
						if (ImGui::SliderFloat("Zoom##4", &zoom, 20.f, 100.f)) {
							camera->setCameraZoom(zoom);
						}
					}

					if (ImGui::CollapsingHeader("Camera Rotation##4")) {
						auto rot = camera->getCameraRotation();
						if (ImGui::DragFloat("Yaw##4", &rot.yaw)) {
							camera->setCameraYaw(rot.yaw);
						}
						if (ImGui::DragFloat("Pitch##4", &rot.pitch)) {
							camera->setCameraPitch(rot.pitch);
						}
					}
				}
			}

			if (em->entityHasComponent<ColliderComponent>(id)) {
				if (ImGui::CollapsingHeader("Collider##5")) {
					auto cc = em->getComponentFromId<ColliderComponent>(id);
					auto bot = cc->botLeft;
					ImGui::DragFloat3("Bot Left##5", &bot.x);
					auto top = cc->topRight;
					ImGui::DragFloat3("Top Right##5", &top.x);
					auto pos = cc->position;
					ImGui::DragFloat3("Position##5", &pos.x);
				}
			}

			if (em->entityHasComponent<ParentComponent>(id)) {
				if (ImGui::CollapsingHeader("Parent##6")) {
					ImGui::Text("%zu", em->getComponentFromId<ParentComponent>(id)->children.size());
				}
			}

			ImGui::PopID();
			ImGui::TreePop();
		}
	}

	ImGui::Separator();
	ImGui::Text("Collisions -> %zu", systems::collision::getCollisions().size());
	ImGui::Text("FPS -> %f", ImGui::GetIO().Framerate);
	ImGui::End();
}

ImGuiNormalView::ImGuiNormalView() {
	FBConfig fconf{};
	fconf.height = 20;
	fconf.width = 20;
	fconf.operation = GL_FRAMEBUFFER;

	RBConfig rconf{};
	rconf.height = 20;
	rconf.width = 20;
	rconf.format = GL_DEPTH24_STENCIL8;
	rconf.attachment = GL_DEPTH_STENCIL_ATTACHMENT;

	TextureParams tconf{};
	tconf.target = GL_TEXTURE_2D;
	tconf.border = 0;
	tconf.format = GL_RGB;
	tconf.internalFormat = GL_RGB;
	tconf.dataType = GL_UNSIGNED_BYTE;
	tconf.level = 0;

	this->m_fbo = CreateUnique<ogl::FrameBuffer>(fconf);
	this->m_fbo->onAttach();
	this->m_fbo->bind();

	this->m_text = ogl::Texture{tconf, 20, 20};
	this->m_text.onAttach();
	this->m_text.createTexture2D(NULL);
	this->m_text.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->m_text.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->m_fbo->setRenderBuffer(rconf);
	this->m_fbo->setColorTexture(this->m_text);

	auto res = this->m_fbo->createFrameBuffer();
	this->m_fbo->unbind();
	ASSERT(res);

	this->m_npcam = CreateUnique<ogl::Camera>();
	this->m_npcam->setCameraVelocity(0.05f);
	this->m_npcam->setCameraPosition(glm::vec3{0, 0, 3});
	this->m_npcam->setMouseSensitivity(0.3f);

	this->m_shader = CreateUnique<ogl::ShaderProgram>("basicVS.glsl", "basicFS.glsl");
	this->m_shader->createShaderProgram();
	this->m_nshader = CreateUnique<ShaderProgram>("normalVertShader.glsl", "normalFragShader.glsl", "normalGeomShader.glsl");
	this->m_nshader->createShaderProgram();
}

double npxpos, npypos;
bool npmfirst = true;

void ImGuiNormalView::processInput() {
	if (ImGui::IsKeyDown(ImGuiKey_W)) {
		this->m_npcam->moveCamera(this->m_npcam->getCameraFront());
	}
	if (ImGui::IsKeyDown(ImGuiKey_S)) {
		this->m_npcam->moveCamera(-this->m_npcam->getCameraFront());
	}
	if (ImGui::IsKeyDown(ImGuiKey_A)) {
		this->m_npcam->moveCamera(-this->m_npcam->getCameraRight());
	}
	if (ImGui::IsKeyDown(ImGuiKey_D)) {
		this->m_npcam->moveCamera(this->m_npcam->getCameraRight());
	}
	if (ImGui::IsKeyDown(ImGuiKey_Space)) {
		this->m_npcam->moveCamera(this->m_npcam->getCameraUp());
	}
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		this->m_npcam->moveCamera(-this->m_npcam->getCameraUp());
	}

	auto nx = ImGui::GetMousePos().x;
	auto ny = ImGui::GetMousePos().y;

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right, false) || ImGui::IsMouseClicked(ImGuiMouseButton_Right, true)) {
		if (npmfirst) {
			npmfirst = false;
			npxpos = nx;
			npypos = ny;
		}
		auto xo = nx - npxpos;
		auto yo = ny - npypos;
		this->m_npcam->processMouseMovement(xo, yo);
	}
	npxpos = nx;
	npypos = ny;
}

ImVec2 npsize;
glm::mat4 npmodel{};
glm::vec3 npscale{0.3};
glm::vec3 nprot{};
ImGuiTableFlags tflag = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter;

const auto rd = Renderer::instance();

void ImGuiNormalView::onRender() {
	// framebuffer space
	ImGui::Begin("Normals");
	ImGui::BeginChild("Render");
	// resize the framebuffer
	npsize = ImGui::GetWindowSize();
	this->m_fbo->rescaleFrameBuffer(npsize.x, npsize.y);
	ImGui::Image((ImTextureID)this->m_text.getId(), npsize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();

	if (rc != nullptr) {
		this->m_npcam->updatePerspProjection(45.f, npsize.x / npsize.y, 0.01f, 10.f);

		npmodel = glm::translate(glm::mat4{1}, glm::vec3{0});
		npmodel = glm::scale(npmodel, npscale);

		// draw into the framebuffer
		this->m_fbo->bind();
		this->m_shader->use();
		this->m_shader->setMat4("viewProj", this->m_npcam->getViewProjMatrix());
		this->m_shader->setMat4("model", npmodel);

		bc->vao.bind();
		rd->drawElements(bc->vao.getId(), GL_LINE_STRIP, vc->getIndexCoords().size(), GL_UNSIGNED_INT);

		this->m_nshader->use();
		this->m_nshader->setMat4("view", this->m_npcam->getViewMatrix());
		this->m_nshader->setMat4("proj", this->m_npcam->getProjMatrix());
		this->m_nshader->setMat4("model", npmodel);
		rc->call();
		this->m_fbo->unbind();
	}

	ImGui::End();

	ImGui::Begin("Coords");
	if (ImGui::Button("Close")) {
		ed->post(NORMAL_VIEW_CLOSE);
		this->resetCamera();
		npmfirst = true;
	}
	ImGui::SameLine();
	ImGuiHelpMarker("This window shows all the normals of each point stored in the Buffers, also you can move the camera using WASD and while keeping the MOUSE2 button you can turn around");
	auto nv = em->getComponentFromId<VertexComponent>(npent)->getNormalsCoords();
	ImGui::Text("Normals Coords");
	ImGui::BeginTable("Normal Coords", 4, tflag);
	ImGui::TableSetupColumn("n", ImGuiTableColumnFlags_WidthFixed, 35.0f);
	ImGui::TableSetupColumn("x");
	ImGui::TableSetupColumn("y");
	ImGui::TableSetupColumn("z");
	ImGui::TableHeadersRow();
	for (auto i = 0; i < nv.size(); i++) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%d", i + 1);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", std::to_string(nv[i].x).c_str());
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%s", std::to_string(nv[i].y).c_str());
		ImGui::TableSetColumnIndex(3);
		ImGui::Text("%s", std::to_string(nv[i].z).c_str());
	}
	ImGui::EndTable();
	ImGui::End();
}

void ImGuiNormalView::resetCamera() {
	this->m_npcam = CreateUnique<ogl::Camera>();
	this->m_npcam->setCameraVelocity(0.05f);
	this->m_npcam->setCameraPosition(glm::vec3{0, 0, 3});
	this->m_npcam->setMouseSensitivity(0.1f);
}

void ImGuiSimulationPanel::onRender() {
	ImGui::Begin("Simulation Config");

	ImGui::End();
}

void ImGuiMeshLoader::onRender() {
	// not working in windows
}

void ImGuiFilePicker::onRender() {
}
