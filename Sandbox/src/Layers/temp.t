#include "SceneLayer.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"

#include "Geometry/GeometryMaths.h"
#include "Geometry/Scene.h"

#include "glm/gtx/quaternion.hpp"


void init() {
	Ref<Shader> shaderProgram = Shader::create(Path("Sandbox/assets/shaders/shader.glsl"));

	// Create a VAO, or Vertex Array Object.
	Ref<VertexArray> renderQuadVA = VertexArray::create();

	// Create a VBO, or Vertex Buffer Object.
	float vertices[(3) * 4] = {
		-0.5f,  0.5f,  0.0f,    // Index 0 - Top left
		-0.5f, -0.5f,  0.0f,    // Index 1 - Bottom left
		 0.5f, -0.5f,  0.0f,    // Index 2 - Bottom right
		 0.5f,  0.5f,  0.0f,    // Index 3 - Top right
	};
	const Ref<VertexBuffer> renderQuadVB = VertexBuffer::create(vertices, sizeof(vertices));
	// Use the setLayout function to specify the layout of one vertex. Here, one vertex is just a
	// Float3, that we called 'aPosition'.
	renderQuadVB->setLayout({ { ShaderDataType::Float3, "aPosition"} });

	renderQuadVA->addVertexBuffer(renderQuadVB); // Add the VB to the VA.
	
	// Create an index buffer.
	unsigned int indices[2 * 3] = {
		0, 1, 2,    // Top left triangle.
		2, 3, 0     // Bottom right triangle.
	};
	auto renderQuadIB = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
	renderQuadVA->setIndexBuffer(renderQuadIB);

	// Upload default set of uniforms.
	shaderProgram->bind();
	shaderProgram->uploadUniformInt("someNumberUniform", 2);
}

void YourLayer::onUpdate(Timestep ts) {
	// Runs every frame.
	RenderCommand::clear();

	Renderer::beginScene(camera); // These are members of the layer initialized beforehand.
		Renderer::submit(shaderProgram, renderQuadVA,
				glm::scale(glm::mat4(1.0f), glm::vec3(mScale)));
	Renderer::endScene();
}
