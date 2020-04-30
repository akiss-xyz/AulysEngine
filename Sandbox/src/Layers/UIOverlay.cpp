#include "UIOverlay.h"

#include "Events/AppEvent.h"

#include "Geometry/GeometryMaths.h"
#include "Geometry/Simplex.h"
#include "Geometry/Honeycomb.h"
#include "Geometry/Scene.h"

#include "Log/ConsoleSink.h"

namespace App 
{
	void UIOverlay::onAttach() { 
		consoleSink->set_level(spdlog::level::trace);

		consoleSink->commands.push_back(ConsoleSink::CommandItem("swapTexture", [this](std::vector<std::string> argv, ConsoleSink* sink) {
			if (argv.size() >= 2) {
				this->mCurrentTexture = Texture2D::create(Path(argv[1]));
				TextureChangedEvent e(mCurrentTexture.get());
				Application::get().onEvent(e);
				return 0;
			}
			return 1; // Error code for not enough parameters.
		}));

		consoleSink->commands.push_back(ConsoleSink::CommandItem("do", [this](std::vector<std::string> argv, ConsoleSink* sink) {
			float v = 0.3f;
			LOG_WARN("In source file {FILE} in function {FUNC}, my variable has value {0}", v);
			return 0;
		}));

		consoleSink->commands.push_back(ConsoleSink::CommandItem("log", [=](std::vector<std::string> argv, ConsoleSink* sink){
			if (argv.size() >= 3) {
				std::stringstream ss;
				for (int i = 2; i < argv.size(); i++) {
					ss << argv[i] << " ";
				}
				switch (toupper(argv[1][0])) {
				case 'T':
					LOG_TRACE(ss.str());
					break;
				case 'I':
					LOG_INFO(ss.str());
					break;
				case 'W':
					LOG_WARN(ss.str());
					break;
				case 'E':
					LOG_ERROR(ss.str());
					break;
				default:
					LOG_WARN("Command \"log\": log level parameter (the first argument) not recognised.");
					return 2;
				}
				return 0;
			}
			return 1;
		}));

		Log::mClientLogset->header->sinks().push_back(consoleSink);
		Log::mClientLogset->logger->sinks().push_back(consoleSink);

		Log::mCoreLogset->header->sinks().push_back(consoleSink);
		Log::mCoreLogset->logger->sinks().push_back(consoleSink);

		constexpr char guibuf[1025] = 
R"(
[Window][Dock]
Pos=0,0
Size=1920,1080
Collapsed=0

[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Window][ConsoleSink]
Pos=8,867
Size=1513,205
Collapsed=0
DockId=0x00000004,0

[Window][Uniform editor]
Pos=1523,8
Size=389,857
Collapsed=0
DockId=0x00000005,0

[Window][General]
Pos=1523,867
Size=389,205
Collapsed=0
DockId=0x00000006,0

[Window][Viewport]
Pos=8,8
Size=1513,857
Collapsed=0
DockId=0x00000003,0

[Docking][Data]
DockSpace     ID=0x16C4D719 Window=0xE2091CDF Pos=8,8 Size=1904,1064 Split=X
  DockNode    ID=0x00000001 Parent=0x16C4D719 SizeRef=1513,1064 Split=Y
    DockNode  ID=0x00000003 Parent=0x00000001 SizeRef=1513,857 CentralNode=1 Selected=0x995B0CF8
    DockNode  ID=0x00000004 Parent=0x00000001 SizeRef=1513,205 Selected=0x5E05F8B0
  DockNode    ID=0x00000002 Parent=0x16C4D719 SizeRef=389,1064 Split=Y Selected=0x8C45687A
    DockNode  ID=0x00000005 Parent=0x00000002 SizeRef=389,857 Selected=0x8C45687A
    DockNode  ID=0x00000006 Parent=0x00000002 SizeRef=389,205 Selected=0x01940FD6)";
		ImGui::LoadIniSettingsFromMemory(guibuf, 1025);
	};


	void UIOverlay::onImGuiRender(Timestep ts) {
		auto& app = Application::get();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight()));
		auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
							| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("Dock", nullptr, window_flags);
		ImGui::DockSpace(ImGui::GetID("Dock"), ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();
		ImGui::PopStyleVar(2);

		this->consoleSink->draw();

		ImGui::Begin("Viewport");
			{
			auto windowSize = ImGui::GetWindowSize();
			auto windowPos = ImGui::GetWindowPos();
			ImGui::GetWindowDrawList()->AddImage((void*)mRenderTexture->getID(), 
				ImVec2(windowSize.x + windowPos.x, windowSize.y + windowPos.y),
				windowPos,
				ImVec2(1, 0), ImVec2(0, 1));
			}
		ImGui::End();

		ImGui::Begin("Uniform editor");
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if(ImGui::CollapsingHeader("View Controls")) {
				if(ImGui::DragFloat2("Size", mSize.data(), mSizeSpd)) {
					glm::vec2 res = {mSize[0], mSize[1]};
					SizeChangedEvent e("screenResolution", &res);
					app.onEvent(e);
				}

				if(ImGui::DragFloat("FOV", &mFov, mSizeSpd, 1.0f, 360.0f)) {
					FOVChangedEvent e("fov", &mFov);
					app.onEvent(e);
				}

			}

			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if(ImGui::CollapsingHeader("Raymarch Settings")) {
				if(ImGui::DragInt("Max steps", &mSteps, mMaxStepsSpd)) {
					auto val = static_cast<uint32_t>(mSteps);
					MaxStepsChangedEvent e("maxSteps", &val);
					app.onEvent(e);
				}

				if(ImGui::DragFloat("Max distance", &mMaxDist, 0.01f))
				{
					MaxDistChangedEvent e("maxDist", &mMaxDist);
					app.onEvent(e);
				}
			}

			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if(ImGui::CollapsingHeader("Lighting Controls")) {
				if (ImGui::DragFloat("Ambient", &mAmbient, 0.001f, 0.0f, 10.0f)) {
					AmbientChangedEvent e("ambient", &mAmbient);
					app.onEvent(e);
				}
			}
		ImGui::End();

		ImGui::Begin("General");
			if (ImGui::Button("Toggle fullscreen")) {

				auto w = &Application::get().getWindow();
				bool isFull = w->isFullscreen();
				if (isFull) {
					w->setFullscreen(false);
				}
				else {
					w->setFullscreen();
				}
			}
			ImGui::SameLine();

			if (ImGui::Button("Reset Position")) {
				const glm::mat4 currentBoost{ 1.0f };
				const glm::mat4 cellBoost{ 1.0f };
				const glm::mat4 invCellBoost{ 1.0f };
				{
					CellBoostChangedEvent e(&cellBoost); app.onEvent(e);
				}
				{
					invCellBoostChangedEvent e(&invCellBoost); app.onEvent(e);
				}
				{
					CurrentBoostChangedEvent e(&currentBoost); app.onEvent(e);
				}
			}
			ImGui::SameLine();
			ImGui::Text("%.0f fps", 1/ts.getSeconds());

			if(ImGui::DragFloat("Scale", &mScale, 0.001f, 0.0f, 20.0f)) {
				ScaleChangedEvent e(mScale);
				app.onEvent(e);
			}
			//if(ImGui::DragInt3("p, q, r", pqr.data(), pqrSpd, pqrMin, pqrMax)) {
			//	GeometryChangedEvent e(pqr);
			//	app.onEvent(e);
			//}
			if(ImGui::DragFloat("Tube Radius", &mTubeRad, 0.00007f, 0.0f, 3.0f)) {
				TubeRadiusChangedEvent e("tubeRad", &mTubeRad);
				app.onEvent(e);
			}
		ImGui::End();
	}

	bool UIOverlay::updateUniformsFromUI(GeometryChangedEvent& e) {
		Geometry::V g = getGeometry(e.p, e.q, e.r);
		bool isCubical = (e.p == 4) && (e.q == 3);
		// Here, we could rebuild the shader as necessary in the future.

		float inRadius = inRadiusCalc(e.p, e.q, e.r, g);
		float midRadius = midRadiusCalc(e.p, e.q, e.r, g, inRadius);
		this->hCWH = this->hCWK = inRadius;
		if (g == Geometry::Spherical) {
			this->hCWK = stereographicToGnomonic(sphericalToStereographic(inRadius));
		}
		if(g == Geometry::Hyperbolic) {
			this->hCWK = poincareToKlein(hyperbolicToPoincare(inRadius));
		}

		float tubeRad = this->mTubeRad;
		float hOffset = tubeRad;

		Geometry::V cut1 = getGeometry2D(e.p, e.q);
		Geometry::V cut4 = getGeometry2D(e.q, e.r);

		switch(cut1) {
			case Geometry::Euclidean:
			{
				this->mCellPosition = {0.0f, 0.0f, 1.0f, 1.0f};

				auto facetsUHS = simplexFacetsUHS(e.p, e.q, e.r);
				auto a = getTrianglePSide(e.q, e.p);
				// facetsUHS[3]
				auto c = facetsUHS.cellMirror.radius;
				auto b = sqrt(c*c - a*a);
				glm::vec3 vUHS(0.0f, 0.0f, b);
				auto poincare = UHSToPoincare(vUHS);
				this->mCellSurfaceOffset = poincareToHyperbolic(-poincare.z) - hOffset;
			}
			case Geometry::Hyperbolic:
			{
				this->mCellPosition = {0.0f, 0.0f, 1.0f, 0.0f};

				float a = getTrianglePSide(e.q, e.p);
				float c = sinh(sin(a) / cos(piOver(e.r)));
				float b = acosh(cosh(c) / cosh(a));
				glm::vec3 poincare = UHSToPoincare({0.0f, 0.0f, hyperbolicToPoincare(b)});
				this->mCellSurfaceOffset = poincareToHyperbolic(-poincare.z) - hOffset;
			}
			case Geometry::Spherical:
		   	{
				this->mCellPosition = {0.0f, 0.0f, 0.0f, 1.0f};
				this->mCellSurfaceOffset = midRadius - hOffset;
			}
			default:
				AU_ASSERT(true, "Geometry value g=\"{0}\" (calculated from p={1}, q={2}, "
					"r={3}) isn't appropriate for this function. Check your arguments.");
		}

		auto midEdge = constructPointInGeometry(g,
				glm::vec3(cos(PI/4.0f), cos(PI/4.0f), 1),
				this->mCellSurfaceOffset);

		this->mVertexPosition = glm::vec4( hCWK, hCWK, hCWK, 1.0f );
		if(g != Geometry::Euclidean) {
			this->mVertexPosition = normalize(g, mVertexPosition);
		}

		switch(cut4) {
			case Geometry::Euclidean:
			{
				auto distToMidEdge = horosphereHSDF(g, midEdge, IDEALCUBECORNERKLEIN, 
						-mCellSurfaceOffset);
				this->mVertexPosition = IDEALCUBECORNERKLEIN;
				this->mVertexSurfaceOffset = -(mCellSurfaceOffset - distToMidEdge);
				break;
			}
			case Geometry::Hyperbolic:
				this->mVertexSurfaceOffset = geodesicPlaneHSDF(g, midEdge, mVertexPosition, 0.0f);
				break;
			case Geometry::Spherical:
				this->mVertexSurfaceOffset = length(g, mVertexPosition - midEdge);
				break;
		}

		if(isCubical) {
			this->mTargetFPS = 27.5f;
			this->mMaxSteps = 31;
		}
		else {
			this->mVertexSurfaceOffset = 0.0f;
			cut4 = Geometry::Invalid;

			this->mTargetFPS = 17.0f;
			this->mMaxSteps = 55;
		}

		float maxDist = 10.0f;
		if(g == Geometry::Euclidean) {
			maxDist = 50.0f;
		}

		auto gens = initGenerators(e.p, e.q, e.r, defhCWK);
		auto lights = initLights(g);

		// Upload literally every uniform here.
		auto& app = Application::get();

		{
			LightPositionsChangedEvent e(lights.first.get());
			app.onEvent(e);
		}
		{
			LightIntensitiesChangedEvent e(lights.second.get());
			app.onEvent(e);
		}
		{
			CellPositionChangedEvent e("cellPosition", &mCellPosition);
			app.onEvent(e);
		}
		{
			CellSurfaceOffsetChangedEvent e("cellSurfaceOffset", &mCellSurfaceOffset);
			app.onEvent(e);
		}
		{
			VertexPositionChangedEvent e("vertexPosition", &mVertexPosition);
			app.onEvent(e);
		}
		{
			VertexSurfaceOffsetChangedEvent e("vertexSurfaceOffset", &mVertexSurfaceOffset);
			app.onEvent(e);
		}
		{
			bool useSimplex = !isCubical;
			UseSimplexChangedEvent e("useSimplex", &useSimplex);
			app.onEvent(e);
		}
		{
			SimplexMirrorsChangedEvent e("simplexMirrorsKlein", &gens.simplexMirrors);
			app.onEvent(e);
		}
		{
			invGeneratorsChangedEvent e(&gens.invGens);
			app.onEvent(e);
		}
		{
			SimplexDualPointsChangedEvent e("simplexDualPoints", &gens.simplexDualPoints);
			app.onEvent(e);
		}
		{
			HalfCubeDualPointChangedEvent e(&gens.halfCubeDualPoints);
			app.onEvent(e);
		}
		{
			HalfCubeWidthChangedEvent e(&this->hCWK);
			app.onEvent(e);
		}
		{
			CutChangedEvent e("cut1", reinterpret_cast<int*>(&cut1));
			app.onEvent(e);
		}
		{
			CutChangedEvent e("cut4", reinterpret_cast<int*>(&cut4));
			app.onEvent(e);
		}
		{
			RenderShadsChangedEvent e(&this->mRenderShads);
			app.onEvent(e);
		}
		{
			ShadSoftChangedEvent e(&this->mShadowSoftness);
			app.onEvent(e);
		}
		return true;
	}

	void UIOverlay::onEvent(Event& event) {
		EventDispatcher disp(event);

		disp.dispatch<RequestUpdateUniformsFromUIEvent>(
			[this](RequestUpdateUniformsFromUIEvent& e) {
				GeometryChangedEvent response(this->pqr);
				Application::get().onEvent(response);
				return true;
			}
		);

		disp.dispatch<GeometryChangedEvent>(
			[this](GeometryChangedEvent& e){
				return this->updateUniformsFromUI(e);
			}
		);

		disp.dispatch<AppTickEvent>(
			[](AppTickEvent& e) {
				ALIL("{0} recieved by UI", e);
				return true;
			}
		);

		disp.dispatch<AppUpdateEvent>(
			[](AppUpdateEvent& e) {
				ALIL("{0} recieved by UI", e);
				return true;
			}
		);

		disp.dispatch<AppRenderEvent>(
			[](AppRenderEvent& e) {
				ALIL("{0} recieved by UI", e);
				return true;
			}
		);
	}

}; // namespace App

