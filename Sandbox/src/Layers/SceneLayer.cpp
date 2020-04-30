#include "SceneLayer.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"

#include "Geometry/GeometryMaths.h"
#include "Geometry/Scene.h"

#include "glm/gtx/quaternion.hpp"

namespace App
{

	void SceneLayer::onAttach() {
		this->mShaderProgram = Shader::create(Path("Sandbox/assets/shaders/vertex.glsl"), Path("Sandbox/assets/shaders/fragment.glsl"));

		this->mRenderQuadVA = VertexArray::create();

		float vertices[(3) * 4] = {
			-0.5f,  0.5f,  0.0f,    // Index 0 - Top left
			-0.5f, -0.5f,  0.0f,    // Index 1 - Bottom left
			 0.5f, -0.5f,  0.0f,    // Index 2 - Bottom right
			 0.5f,  0.5f,  0.0f,    // Index 3 - Top right
		};

		const Ref<VertexBuffer> renderQuadVB = VertexBuffer::create(vertices, sizeof(vertices));

		renderQuadVB->setLayout({ { ShaderDataType::Float3, "aPosition"} });
		mRenderQuadVA->addVertexBuffer(renderQuadVB);
		// Aulys::VertexArray keeps a reference (with Aulys::Ref<T> = std::shared_ptr<T>).

		unsigned int indices[2 * 3] = {
			0, 1, 2,    // Top left triangle.
			2, 3, 0     // Bottom right triangle.
		};
		auto renderQuadIB = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
		this->mRenderQuadVA->setIndexBuffer(renderQuadIB);
		// Aulys::VertexArray will keep a reference (with Aulys::Ref<T> = std::shared_ptr<T>),
		// so we don't bother holding onto it ourselves, hence why we create both the IB and VB
		// just in this init function, wo/ making them members. Yay for good design.

		// Upload default set of uniforms.
		mShaderProgram->bind();

		/**************************************************************************************************/
		/*** Default (or non-variable) uniforms ***/
		/**************************************************************************************************/
		auto& app = Application::get();

		{
			bool debugInfo = false;
			DebugInfoChangedEvent e("debugInfo", &debugInfo); app.onEvent(e);
		}
		{
			glm::vec2 screenResolutionDefault = { 1920.0f, 1080.0f };
			SizeChangedEvent e("screenResolution", &screenResolutionDefault); app.onEvent(e);
		}
		{
			float fovDefault = 90.0f;
			FOVChangedEvent e(&fovDefault); app.onEvent(e);
		}
		{

			uint32_t maxStepsDefault = 29;
			MaxStepsChangedEvent e(&maxStepsDefault); app.onEvent(e);
		}
		{
			float maxDistDefault = 6.4f;
			MaxDistChangedEvent e(&maxDistDefault); app.onEvent(e);
		}
		{
			int attnModelDefault = 1;
			AttenuationChangedEvent e(&attnModelDefault); app.onEvent(e);
		}
		/* Scene Geometry */
		{
			float tubeRadDefault = 0.15f;
			TubeRadiusChangedEvent e(&tubeRadDefault); app.onEvent(e);
		}
		{
			UploadObjectsDataEvent<4> e; app.onEvent(e);
			// This takes care of NUM_OBJECTS and all globalObjects uniforms.
		}
		{
			CellBoostChangedEvent e(&cellBoost); app.onEvent(e);
		}
		{
			invCellBoostChangedEvent e(&invCellBoost); app.onEvent(e);
		}
		{
			CurrentBoostChangedEvent e(&currentBoost); app.onEvent(e);
		}
		{
			auto [lightPos, lightInt] = initLights(Geometry::Hyperbolic);
			LightPositionsChangedEvent eP(lightPos.get()); app.onEvent(eP);
			LightIntensitiesChangedEvent eI(lightInt.get()); app.onEvent(eI);
		}
		/* Texture */
		{
			this->mTexture = Texture2D::create(Path("Sandbox/assets/textures/brick.png"));
			TextureChangedEvent e(mTexture.get()); app.onEvent(e);
		}

		mFrameBuffer->bind();
		mFrameBuffer->attachTexture(mFrameBufferTexture);

		RequestUpdateUniformsFromUIEvent e;
		app.onEvent(e);

		// Current color = 282A36
		RenderCommand::setClearColor({ 40.0f / 256.0f, 42.0f / 256.0f, 54.0 / 256.0f, 1 });

	};

	void SceneLayer::onDetach() { }

	void SceneLayer::onUpdate(Timestep ts) {
		RenderCommand::clear();

		Renderer::beginScene(mCamera);
			if(*mFrameBuffer) {
				mFrameBuffer->bind();
				Renderer::submit(mShaderProgram, mRenderQuadVA,
						glm::scale(glm::mat4(1.0f), glm::vec3(mScale)));
				// RenderCommand::drawIndexed(this->mRenderQuadVA); TOUCH removed, repeat
				mFrameBuffer->unbind();
			}
		Renderer::endScene();

		float speed = Input::isKeyPressed(AU_KEY_LEFT_SHIFT) ? 2 * mSpeed : mSpeed;

		// deltaPosState contains information about what keys are being pressed, to indicate where
		// the user intends to move. This is set by a pair of event handler lambdas, see the bottom
		// of the onEvent member function below.
		auto deltaPos = deltaPosState * speed * ts.getMilliSeconds();
		// Similar to the above.
		auto deltaRot = deltaRotState * speed * ts.getMilliSeconds();

		bool change = false; bool posChange = false;
		if (deltaRot != glm::vec3{ 0.0f }) {
			change = true;
			glm::fquat deltaRotQ(1.0f, deltaRot.x * mRotSpeed * ts.getMilliSeconds(),
				deltaRot.y * mRotSpeed * ts.getMilliSeconds(),
				deltaRot.z * mRotSpeed * ts.getMilliSeconds());
			currentBoost = glm::toMat4(deltaRotQ) * currentBoost;
		}

		if (deltaPos != glm::vec3{ 0.0f }) {
			change = true;
			posChange = true;
			auto m = translateByVector(g, eToHScale * deltaPos);

			currentBoost = gramSchmidt(g, m * currentBoost);
			if (auto fixIndex = fixOutsideCentralCell(currentBoost, invGens); fixIndex != -1) {
				cellBoost = gramSchmidt(g, invGens[fixIndex] * cellBoost);
				invCellBoost = inverse(cellBoost);
			}

		}

		if (change) {
			auto& app = Application::get();
			{
				CurrentBoostChangedEvent e(&currentBoost);
				app.onEvent(e);
			}
			if (posChange) {
				{
					CellBoostChangedEvent e(&cellBoost);
					app.onEvent(e);
				}
				{
					invCellBoostChangedEvent e(&invCellBoost);
					app.onEvent(e);
				}
			}
		}
		deltaPos = glm::vec3{ 0.0f };
	}

	void SceneLayer::onEvent(Event& event) {
		EventDispatcher disp(event);

		disp.dispatch<DebugInfoChangedEvent>(
			[this](DebugInfoChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved DebugInfoChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformBool(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<SizeChangedEvent>(
			[this](SizeChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved SizeChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat2(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<FOVChangedEvent>(
			[this](FOVChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved FOVChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<MaxStepsChangedEvent>(
			[this](MaxStepsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved MaxStepsChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformInt(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<MaxDistChangedEvent>(
			[this](MaxDistChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved MaxDistChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<ScaleChangedEvent>(
			[this](ScaleChangedEvent& e) {
			LT("Recieved ScaleChangedEvent val={0} : {1}", e.val(), e);
			this->mScale = e.val();
			return true;
		}
		);

		disp.dispatch<AttenuationChangedEvent>(
			[this](AttenuationChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved AttenuationChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformInt(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<RenderShadsChangedEvent>(
			[this](RenderShadsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved RenderShadsChangedEvent val={0}, {1} : {2}",
				e.valptr()->data()[0], e.valptr()->data()[1], e);
			this->mShaderProgram->uploadUniform2Bool(e.name(), e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<ShadSoftChangedEvent>(
			[this](ShadSoftChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved ShadSoftChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<TubeRadiusChangedEvent>(
			[this](TubeRadiusChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved TubeRadiusChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<CellPositionChangedEvent>(
			[this](CellPositionChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved CellPositionChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat4(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<CellSurfaceOffsetChangedEvent>(
			[this](CellSurfaceOffsetChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved CellSurfaceOffsetChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return false;
		}
		);

		disp.dispatch<VertexPositionChangedEvent>(
			[this](VertexPositionChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved VertexPositionChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat4(e.name(), *e.valptr());
			return false;
		}
		);

		disp.dispatch<VertexSurfaceOffsetChangedEvent>(
			[this](VertexSurfaceOffsetChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved VertexSurfaceOffsetChangedEvent val={0}, : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return false;
		}
		);

		disp.dispatch<UseSimplexChangedEvent>(
			[this](UseSimplexChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved UseSimplexChangedEvent val={0}, : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformBool(e.name(), *e.valptr());
			return false;
		}
		);

		disp.dispatch<SimplexMirrorsChangedEvent>(
			[this](SimplexMirrorsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved SimplexMirrorsChangedEvent val=\n{0}, : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat4(e.name(), 4, e.valptr()->data());
			return false;
		}
		);

		disp.dispatch<SimplexDualPointsChangedEvent>(
			[this](SimplexDualPointsChangedEvent& e) {
				this->mShaderProgram->bind();
				LT("Recieved SimplexDualPointsChangedEvent val=\n{0}, : {1}", *e.valptr(), e);
				this->mShaderProgram->uploadUniformFloat4(e.name(), 4, e.valptr()->data());
				return false;
			}
		);

		disp.dispatch<CutChangedEvent>(
			[this](CutChangedEvent& e) {
				this->mShaderProgram->bind();
				LT("Recieved CutChangedEvent val={0} : {1}", *e.valptr(), e);
				this->mShaderProgram->uploadUniformInt(e.name(), *e.valptr());
				return false;
			}
		);

		disp.dispatch<TextureChangedEvent>(
			[this](TextureChangedEvent& e) {
				this->mShaderProgram->bind();
				LT("Recieved TextureChangedEvent val=hardtoprint, : {0}", e);
				e.valptr()->bind();
				this->mShaderProgram->uploadUniformInt("uTexture", 0); // Texture slot 0
				return true;
			}
		);

		disp.dispatch<NumObjectsChangedEvent>(
			[this](NumObjectsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved NumObjectsChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformInt(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<UploadObjectsDataEvent<4>>(
			[this](UploadObjectsDataEvent<4>& e) {
			this->mShaderProgram->bind();
			auto objects = *e.valptr();
			LT("Recieved UploadObjectsDataEvent val="
				"\nglobalObjectsBoosts={0}"
				"\ninvGlobalObjectBoosts={1}"
				"\nglobalObjectRadii={2}"
				"\nNUM_OBJECTS={3}"
				"\n : {4}",
				objects->objectBoosts, objects->invObjectBoosts, objects->objectRadii,
				objects->getCount(), e);
			this->mShaderProgram->uploadUniformMat4("globalObjectBoosts", 4, objects->objectBoosts.data());
			this->mShaderProgram->uploadUniformMat4("invGlobalObjectBoosts", 4, objects->invObjectBoosts.data());
			this->mShaderProgram->uploadUniformFloat3("globalObjectRadii", 4, objects->objectRadii.data());
			this->mShaderProgram->uploadUniformInt("NUM_OBJECTS", objects->getCount());
			return true;
		}
		);

		disp.dispatch<CellBoostChangedEvent>(
			[this](CellBoostChangedEvent& e) {
			this->mShaderProgram->bind();
			// LT("Recieved CellBoostChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformMat4(e.name(), *e.valptr());
			this->cellBoost = *e.valptr();
			return true;
		}
		);

		disp.dispatch<invCellBoostChangedEvent>(
			[this](invCellBoostChangedEvent& e) {
			this->mShaderProgram->bind();
			// LT("Recieved invCellBoostChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformMat4(e.name(), *e.valptr());
			this->invCellBoost = *e.valptr();
			return true;
		}
		);

		disp.dispatch<invGeneratorsChangedEvent>(
			[this](invGeneratorsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received invGeneratorsChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformMat4(e.name(), 6, e.valptr()->data());
			this->invGens = *e.valptr();
			return true;
		}
		);

		disp.dispatch<CurrentBoostChangedEvent>(
			[this](CurrentBoostChangedEvent& e) {
			this->mShaderProgram->bind();
			// LT("Received CurrentBoostChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformMat4(e.name(), *e.valptr());
			this->currentBoost = *e.valptr();
			return true;
		}
		);

		disp.dispatch<LightPositionsChangedEvent>(
			[this](LightPositionsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received LightPositionsChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat4(e.name(), 4, e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<LightIntensitiesChangedEvent>(
			[this](LightIntensitiesChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received LightIntensitiesChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat4(e.name(), 5, e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<GlobalObjectBoostsChangedEvent>(
			[this](GlobalObjectBoostsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received GlobalObjectsBoostsChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformMat4(e.name(), 4, e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<invGlobalObjectBoostsChangedEvent>(
			[this](invGlobalObjectBoostsChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received invGlobalObjectsBoostsChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformMat4(e.name(), 4, e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<GlobalObjectRadiiChangedEvent>(
			[this](GlobalObjectRadiiChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received GlobalObjectsRadiiChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat3(e.name(), 4, e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<HalfCubeDualPointChangedEvent>(
			[this](HalfCubeDualPointChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Received HalfCubeDualPointChangedEvent val={0} : {1}", *e.valptr(), e);
			float a = 0.70710678120f;
			float b = 1.22474487140f;
			std::array<glm::vec4, 3> buf = {
				glm::vec4{ b, 0.0f, 0.0f, a },
				glm::vec4{ 0.0f, b, 0.0f, a },
				glm::vec4{ 0.0f, 0.0f, b, a }
			};
			this->mShaderProgram->uploadUniformFloat4(e.name(), 3, e.valptr()->data());
			return true;
		}
		);

		disp.dispatch<HalfCubeWidthChangedEvent>(
			[this](HalfCubeWidthChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved HalfCubeWidthChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<DebugVector2ChangedEvent>(
			[this](DebugVector2ChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved DebugVector2ChangedEvent val={0} : {1}", *e.valptr(), e);
			this->mShaderProgram->uploadUniformFloat2(e.name(), *e.valptr());
			return true;
		}
		);

		disp.dispatch<AmbientChangedEvent>(
			[this](AmbientChangedEvent& e) {
				this->mShaderProgram->bind();
				LT("Recieved AmbientChangedEvent val={0} : {1}", *e.valptr(), e);
				this->mShaderProgram->uploadUniformFloat(e.name(), *e.valptr());
				return true;
			}
		);

		disp.dispatch<GeometryChangedEvent>(
			[this](GeometryChangedEvent& e) {
			this->mShaderProgram->bind();
			LT("Recieved GeometryChangedEvent val={0}, {1}, {2} : {3}", e.pqr[0], e.pqr[1], e.pqr[2], e);
			this->g = getGeometry(e.pqr[0], e.pqr[1], e.pqr[2]);
			return true;
		}
		);

		disp.dispatch<MovementSpeedChangedEvent>(
			[this](MovementSpeedChangedEvent& e) {
				this->mSpeed = e.val();
				return true;
			}
		);

		disp.dispatch<RotSpeedChangedEvent>(
			[this](RotSpeedChangedEvent& e) {
				this->mRotSpeed = e.val();
				return true;
			}
		);
		
		disp.dispatch<TagSwapRequestedEvent>(
			[this](TagSwapRequestedEvent& e) {
				return this->mShaderProgram->setTag(e.tag, e.val);
			}
		);

		disp.dispatch<KeyPressedEvent>(
			[this](KeyPressedEvent& e) {
				if (e.getRepeatCount()) {
					// This is a repeat press, so we don't care e.g. holding a button down.
					return false;
				}
				switch (e.getKeyCode()) {
				case AU_KEY_W:
					this->deltaPosState.z = -1.0f;
					break;
				case AU_KEY_A:
					this->deltaPosState.x = -1.0f;
					break;
				case AU_KEY_S:
					this->deltaPosState.z = 1.0f;
					break;
				case AU_KEY_D:
					this->deltaPosState.x = 1.0f;
					break;
				case AU_KEY_SPACE:
					this->deltaPosState.y = 1.0f;
					break;
				case AU_KEY_LEFT_CONTROL:
					this->deltaPosState.y = -1.0f;
					break;
				case AU_KEY_UP:
					this->deltaRotState.x = -1.0f;
					break;
				case AU_KEY_LEFT:
					this->deltaRotState.y = -1.0f;
					break;
				case AU_KEY_DOWN:
					this->deltaRotState.x = 1.0f;
					break;
				case AU_KEY_RIGHT:
					this->deltaRotState.y = 1.0f;
					break;
				case AU_KEY_RIGHT_SHIFT:
					this->deltaRotState.z = 1.0f;
					break;
				case AU_KEY_RIGHT_CONTROL:
					this->deltaRotState.z = -1.0f;
					break;
				default:
					return false;
				}
				e.handled = true;
				return true;
			}
		);

		disp.dispatch<KeyReleasedEvent>(
			[this](KeyReleasedEvent& e) {
				switch (e.getKeyCode()) {
				case AU_KEY_W:
					this->deltaPosState.z = 0.0f;
					break;
				case AU_KEY_A:
					this->deltaPosState.x = 0.0f;
					break;
				case AU_KEY_S:
					this->deltaPosState.z = 0.0f;
					break;
				case AU_KEY_D:
					this->deltaPosState.x = 0.0f;
					break;
				case AU_KEY_SPACE:
					this->deltaPosState.y = 0.0f;
					break;
				case AU_KEY_LEFT_CONTROL:
					this->deltaPosState.y = 0.0f;
					break;
				case AU_KEY_UP:
					this->deltaRotState.x = 0.0f;
					break;
				case AU_KEY_LEFT:
					this->deltaRotState.y = 0.0f;
					break;
				case AU_KEY_DOWN:
					this->deltaRotState.x = 0.0f;
					break;
				case AU_KEY_RIGHT:
					this->deltaRotState.y = 0.0f;
					break;
				case AU_KEY_RIGHT_SHIFT:
					this->deltaRotState.z = 0.0f;
					break;
				case AU_KEY_RIGHT_CONTROL:
					this->deltaRotState.z = 0.0f;
					break;
				default:
					return false;
				}
				e.handled = true;
				return true;
			}
		);

		disp.dispatch<DragAndDropEvent>(
			[this](DragAndDropEvent& e) {
				if (e.extension == "png") {
					auto p = Path(e.mPath, true);
					mTexture = Texture2D::create(p);
					TextureChangedEvent e(mTexture.get());
					Application::get().onEvent(e);
					e.handled = true;
					return true;
				}
				return true;
			}
		);

	}

}; // namespace App
