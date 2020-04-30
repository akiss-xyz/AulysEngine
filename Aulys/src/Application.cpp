#include "src/pch.h"

#include <Application.h>

#include "memory.h"

#include "imgui/imgui.h"

#include "Renderer/Renderer.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"

#include "Events/Event.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Aulys
{
	Application* Application::sInstance = nullptr;

	// The universal constructor for all applications - runs before any client code is executed.
	Application::Application(const WindowProps& windowSettings) : mCamera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		// Create the instance only if it doesn't already exist. We shouldn't be able to create more
		// than one instance.
		AU_CORE_ASSERT(!sInstance, "Error: Application already exists!");
		sInstance = this;

		mWindow = Window::Create(windowSettings);
		mWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		// Create our renderer.
		Renderer::init();

		// Create the universal, engine-managed imgui layer. The client application also has access
		// to this, as it is a protected member of the Application base class.
		this->mImGuiLayer = new ImGuiLayer("Engine-managed ImGui Layer");
		this->pushOverlay(mImGuiLayer);
		this->mImGuiLayer->swapFont("Roboto Mono Medium", Path("Aulys\\assets\\fonts\\RobotoMono-Medium.ttf"));
	};

	bool Application::onWindowClose(WindowCloseEvent& e) {
		if(this->decideIfWindowCanClose(e)) {
			this->closeWindow(e);
			this->mRunning = false;
		}
		return true;
	};

	void Application::onEvent(Event& e) noexcept
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(
			[this](WindowCloseEvent& e){ return this->onWindowClose(e); }
		);

		// Go backwards through the layer stack (top first), and tell them about events, stopping if
		// a layer's onEvent function informs us that it has been handled.
		for (auto it = this->mLayerStack.end(); it != this->mLayerStack.begin(); ){
			(*(--it))->onEvent(e);

			if (e.handled == true) {
				break;
			}
		}
	};
	
	/* Application::pushLayer(Layer* layer)
	 * This is the intended, safe way to push a general layer. Events will propagate down the layer
	 * stack, first in recieves the events first - except for overlays. Overlays are guaranteed to
	 * be above all non-overlay layers, and are then sorted by when they're pushed.
	 *
	 * You can listen to events by overriding the Layer's onEvent function, and then from there, use
	 * EventDispatchers as above. To block an event, set the event's handled arg.
	 */
	void Application::pushLayer(Layer* layer)
	{
		this->mLayerStack.pushLayer(layer);
		layer->onAttach();
	};
	
	/* Application::pushOverlay(Layer* overlay)
	 * This is the intended way to push e.g. a GUI-focused layer. These are guaranteed to be above 
	 * all non-overlay layers in the layer stack - events propagate from top to bottom in the layer
	 * stack, so overlay layers have a priviledge like that.
	 *
	 * See also: Application::pushLayer(Layer* layer), LayerStack.h, Layer.h, Event.h
	 */
	void Application::pushOverlay(Layer* overlay)
	{
		this->mLayerStack.pushOverlay(overlay);
		overlay->onAttach();
	};

/**************************************************************************************************/
/*** Default run - a little demo for the people who forgot to override Application::Run. ***/
/**************************************************************************************************/

	void setUpTriangleVA(float col1[4], float col2[4], float col3[4], 
			Ref<VertexArray>& vertexArray, Ref<IndexBuffer>& indexBuffer,
			Ref<VertexBuffer>& vertexBuffer) {
		// Set the vertexArray pointer to a newly created VertexArray.
		// This argument vertexArray holds a set of VertexBuffers.
		vertexArray = Aulys::VertexArray::create();

		//// So now we need to make our VertexBuffer.
		//
		// These are the vertices to our triangle, each vertex composed of a Float3 position and a
		// Float4 (RGBA) colour.
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, col1[0],col1[1],col1[2],col1[3], // Bottom left
			 0.5f, -0.5f, 0.0f, col2[0],col2[1],col2[2],col2[3], // Bottom right
			 0.0f,  0.5f, 0.0f, col3[0],col3[1],col3[2],col3[3], // Centre top
		};

		// Set the vertexBuffer pointer argument to a newly created VertexBuffer, using these
		// vertices.
		vertexBuffer = Aulys::VertexBuffer::create(vertices, sizeof(vertices));

		// This layout models how our VertexBuffer is laid out.
		Aulys::BufferLayout layout = {
			{
				Aulys::ShaderDataType::Float3, "aPosition"
			},
			{
				Aulys::ShaderDataType::Float4, "aColor"
			},
		};

		// And so, let's pass it in!
		vertexBuffer->setLayout(layout);
		// Now that our VertexBuffer is complete, let's add it to the VertexArray.
		vertexArray->addVertexBuffer(vertexBuffer);

		//// Now we need to create an IndexBuffer.
		unsigned int indices[3] = {
			0, 1, 2 // Bottom left -> Bottom right -> Centre top
		}; 	// We have 3 corners on our triangle, and 3 vertices. We go through them anticlockwise.
		
		// Then let's set the argument indexBuffer pointer to a newly created IndexBuffer
		indexBuffer = Aulys::IndexBuffer::create(indices, 3);
		// And then bind it!
		indexBuffer->bind();

		// Finally, we attach the IndexBuffer to the VertexArray.
		vertexArray->setIndexBuffer(indexBuffer);
	}

	// This is the default Application::Run function that should be overridden by the user
	// application. IF THIS CODE IS RUNNING, YOU HAVEN'T OVERRIDDEN APPLICATION::RUN().
	//
	// If it's not, we run this code here, which does some funky stuff to show off some features.
	void Application::Run()
	{
		// This is the data we need to create the default scene. It's stored for the entire length
		// of this function.
		Ref<VertexArray>  mVertexArray;
		Ref<VertexBuffer> mVertexBuffer;
		Ref<IndexBuffer> mIndexBuffer;
		Ref<Shader> mGradientShader;
		Ref<Shader> mFlatShader;

		glm::vec3 objPos = {0.0f, 0.0f, 0.0f};
		glm::mat4 objTransform = glm::translate(glm::mat4(1.0f), objPos);
		float objVelocity = 0.2f;
		float objRot = 0.05f;

		glm::vec3 camAcc = { 0.0f, 0.0f, 0.0f };
		float camRot = { 0.0f };
		float rotConst = 0.05f;
		float camAccConst = 0.001f;

		mGradientShader = Shader::create(Path("/Aulys/assets/shaders/def/gradient-shader.glsl"));

		// The flat colour shader uses the same vertex shader.
		mFlatShader = Shader::create(
				Path("/Aulys/assets/shaders/def/flat-color-shader.glsl"));

		float bgCol[4] = { 22.0f/256.0f, 25.0f/256.0f, 37.0f/256.0f, 1 };
		float col1[4] =  { 0.75f, 0.05f, 0.75f, 1.0f, };
		float col2[4] =  { 0.05f, 0.05f, 0.75f, 1.0f, };
		float col3[4] =  { 0.75f, 0.05f, 0.05f, 1.0f, };
		// R G B A
		float flatColour[4] = {0.2f, 0.3f, 0.8f, 1.0f, };

		bool vsync = true;
		
		int count = 0;
		while (this->mRunning) {
			
			Timestep deltatime = mWindow->calculateDeltaTime(mLastFrameTime);

			//// Handle input
			// Moving the camera - acceleration based
			if (Input::isKeyPressed(AU_KEY_W)) {
				camAcc.y -= camAccConst;
			}

			if (Input::isKeyPressed(AU_KEY_S)) {
				camAcc.y += camAccConst;
			}

			if (Input::isKeyPressed(AU_KEY_A)) {
				camAcc.x += camAccConst;
			}

			if (Input::isKeyPressed(AU_KEY_D)) {
				camAcc.x -= camAccConst;
			}

			// Rotating the camera - acceleration based
			if (Input::isKeyPressed(AU_KEY_R)) {
				camRot += rotConst;
			}

			if (Input::isKeyPressed(AU_KEY_F)) {
				camRot -= rotConst;
			}

			// Clear the camera velocity.
			if (Input::isKeyPressed(AU_KEY_SPACE)) {
				camAcc -= camAcc;
			}

			// Clear the camera rotation speed when V is pressed.
			if (Input::isKeyPressed(AU_KEY_V)) {
				camRot -= camRot;
			}

			// Moving the object - non-acceleration
			if (Input::isKeyPressed(AU_KEY_I)) {
				objPos.y += objVelocity * deltatime.getSeconds();
			}

			if (Input::isKeyPressed(AU_KEY_K)) {
				objPos.y -= objVelocity * deltatime.getSeconds();
			}

			if (Input::isKeyPressed(AU_KEY_J)) {
				objPos.x -= objVelocity * deltatime.getSeconds();
			}

			if (Input::isKeyPressed(AU_KEY_L)) {
				objPos.x += objVelocity * deltatime.getSeconds();
			}

			// Update the object's transform matrix.
			objTransform = glm::translate(glm::mat4(1.0f), objPos);

			if (Input::isKeyPressed(AU_KEY_Q)) {
				/// Clear everything!
				// Reset all our velocities.
				camAcc -= camAcc; camRot -= camRot;
				// Reset the camera.
				mCamera.setPosition({0.0f, 0.0f, 0.0f}); mCamera.setRotation(0.0f);
			}

			glm::vec3 camOut = { camAcc.x * deltatime.getSeconds() 
				+ camAcc.x * deltatime.getSeconds() * deltatime.getSeconds(),camAcc.y 
					* deltatime.getSeconds() +  camAcc.y * deltatime.getSeconds() 
					* deltatime.getSeconds(), 
				camAcc.z * deltatime.getSeconds() + camAcc.z * deltatime.getSeconds() * deltatime.getSeconds() };

			// Set camera pos based on the input we received.
			mCamera.setPosition(mCamera.getPosition() + camOut);
			mCamera.setRotation(mCamera.getRotation() + camRot * deltatime.getSeconds());

			// Clear the screen with the background colour.
			RenderCommand::setClearColor({ bgCol[0], bgCol[1], bgCol[2], bgCol[3] });
			RenderCommand::clear();

			/// Set up the scene.
			Renderer::beginScene(mCamera);

			// We need to set up the TriangleVA once each update, in case the user changed the
			// colours.
			setUpTriangleVA(col1, col2, col3, mVertexArray, mIndexBuffer, mVertexBuffer);

			mGradientShader->bind();
			// Submit for rendering.
			Renderer::submit(mGradientShader, mVertexArray, objTransform);

			mFlatShader->bind();
				// As it turns out, you need to bind a shader before you can upload uniforms.

			// Colours for the background triangle.
			mFlatShader->uploadUniformFloat4("uColor", flatColour);

			// Background one as well.
			Renderer::submit(mFlatShader, mVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));

			Renderer::endScene();

			/// Create default welcome and warning window.
			{
				mImGuiLayer->begin();

				ImGui::TextWrapped("Hello! It looks like your Sandbox application, that inherits from Aulys::Application, didn't override the Run method! If you want to do much of anything, including get rid of me, you should override that method!");

				ImGui::ColorEdit4("Edit the background bgColour here!", bgCol);

				ImGui::ColorEdit4("Edit the first vertex's colour here!", col1);
				ImGui::ColorEdit4("Edit the second vertex's colour here!", col2);
				ImGui::ColorEdit4("Edit the third vertex's colour here!", col3);
				ImGui::ColorEdit4("Flat colour editor.", flatColour);

				ImGui::DragFloat("Camera Acceleration Value (Keys: W, A, S, D)", &camAccConst, 0.01f);
				ImGui::DragFloat("Camera Rotation Value (Keys: R, F, V)", &camRot, 0.01f);

				ImGui::DragFloat("Object Velocity Value (Keys: I, K, J, L)", &objVelocity, 0.01f);
				ImGui::DragFloat("Camera Rotation Value (Keys: R, F, V)", &objRot, 0.01f);

				if (ImGui::Checkbox("VSync", &vsync)) {
					mWindow->setVSync(vsync);
				}

				// for (Layer* layer : this->mLayerStack)
				//	layer->onImGuiRender();

				mImGuiLayer->end();
			}

			/// This is supposed to be ran each update, but listen, we have no layers - this is just
			/// as an example.
			// for (Layer* layer : this->mLayerStack)
			//	layer->onUpdate(timestep);

			mWindow->onUpdate();
		}
	};

}; // namespace Aulys
