#pragma once

#include "Aulys.h"
#include "Events/Event.h"

#include "Geometry/Scene.h"

using namespace Aulys;

namespace App 
{
	namespace EventTypes
	{
	// While this might look horrendous to the untrained eye, this allows us to simulate enum
	// class's convenient, leak-proof naming goodness while still retaining the lovely implicit 
	// conversion to int (and hence also bit-field/flag stuffs as well)
	// (We need to be able to convert our enums to int so that Aulys can deal with them as IDs, see
	// the Event.h file that declares the handy-dandy CUSTOM_EVENT_TYPE(...) macro.)
	// Alternatively, you could use enum class and cast to an int, that'd also be doable. However
	// then you'd also have to define (for EventCategory) the operator overloads for | and & and
	// yeah. So this is how we're doing things for now - but remember, Au gives us this freedom!
		enum V {
			None = 1,

			SizeChanged, FOVChanged,

			MaxStepsChanged, MaxDistChanged,

			ScaleChanged,

			AttenuationChanged, RenderShadsChanged, ShadSoftChanged, LightPositionsChanged,
			LightIntensitiesChanged, AmbientChanged,

			TubeRadiusChanged, GeometryChanged, CellPositionChanged, CellSurfaceOffsetChanged,
			CellBoostChanged, invCellBoostChanged, invGeneratorsChanged, CurrentBoostChanged,
			GlobalObjectBoostsChanged, invGlobalObjectBoostsChanged, GlobalObjectRadiiChanged,
			VertexPositionChanged, VertexSurfaceOffsetChanged, UseSimplexChanged,
			SimplexMirrorsChanged, SimplexDualPointsChanged, CutChanged,
			HalfCubeDualPointChanged, HalfCubeWidthChanged,

			TextureChanged,

			NumObjectsChanged,

			TagSwapRequested, RequestUpdateUniforms, UploadObjectsData, MovementSpeedChanged, RotSpeedChanged,

			DebugInfoChanged, DebugVector2Changed, 
		};
	}; // namespace EventType

	namespace EventCategory 
	{
		enum V {
			None = 0,
			UniformChanged = 1 << 0,
				ViewDetailsChanged = 1 << 1,
				RaymarchSettingsChanged = 1 << 2,
				LightingSettingsChanged = 1 << 3,
				SceneChanged = 1 << 4,
					SceneGeometryChanged = 1 << 5,
			Communication = 1 << 6,
			Debug = 1 << 7
		};
	}; // namespace EventCategory

	template<class T>
	class UniformChangedEvent : public Event 
	{
	public:
		UniformChangedEvent(const std::string name, const T* p) : mName(name), mVal(p)
		{
			// A little change: we take in a straight up string, not a pointer or reference to it
			// because I ran into some issues with code like this `SizeChangedEvent e("name", val)`.
			// Here the string being passed in only lasts the lifetime of the construction, and after
			// that is garbage, hence we need to keep a copy of it ourselves to make sure the memory
			// isn't trash.
		};

		virtual std::string toString() const override
		{
			std::stringstream ss; ss << "UniformChangedEvent(" << name()
				<< ")";
				// << ", " << *valptr() << ")";
			return ss.str();
		};

		virtual const std::string& name() const { return mName; } 
		virtual const T* valptr() const { return mVal; }

		EVENT_CUSTOM_CATEGORY(EventCategory::UniformChanged);
	private:
		const std::string mName;
		const T* mVal;
	}; // class AppEvent : public Event;

	class SizeChangedEvent : public UniformChangedEvent<glm::vec2> 
	{
	public:
		SizeChangedEvent(const std::string& name, const glm::vec2* widthAndHeight)
			: UniformChangedEvent(name, widthAndHeight) {};

		SizeChangedEvent(const glm::vec2* widthAndHeight)
			: UniformChangedEvent("screenResolution", widthAndHeight) {};

		EVENT_CUSTOM_TYPE(EventTypes::SizeChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::ViewDetailsChanged, UniformChangedEvent);
	private:
	
	}; // class SizeChangedEvent : public UniformChangedEvent<glm::vec2>

	class FOVChangedEvent : public UniformChangedEvent<float>  
	{
	public:
		FOVChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		FOVChangedEvent(const float* val)
			: UniformChangedEvent("fov", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::FOVChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::ViewDetailsChanged, UniformChangedEvent);
	private:
	
	}; // class FOVChangedEvent : public UniformChangedEvent<float> 

	class ScaleChangedEvent : public Event 
	{
	public:
		ScaleChangedEvent(float val) : mVal(val) {};

		float val() const { return mVal; }

		EVENT_CUSTOM_TYPE(EventTypes::ScaleChanged);
		EVENT_CUSTOM_CATEGORY(EventCategory::ViewDetailsChanged | EventCategory::SceneChanged);
	private:
		float mVal;
	}; // class ScaleChangedEvent : public Event

	class MaxStepsChangedEvent : public UniformChangedEvent<uint32_t>
	{
	public:
		MaxStepsChangedEvent(const std::string& name, const uint32_t* val)
			: UniformChangedEvent(name, val) {};

		MaxStepsChangedEvent(const uint32_t* val)
			: UniformChangedEvent("maxSteps", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::MaxStepsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::RaymarchSettingsChanged, UniformChangedEvent);
	private:
	
	}; // class MaxStepsChangedEvent : public UniformChangedEvent<uint32_t>

	class MaxDistChangedEvent : public UniformChangedEvent<float>
	{
	public:
		MaxDistChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		MaxDistChangedEvent(const float* val)
			: UniformChangedEvent("maxDist", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::MaxDistChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::RaymarchSettingsChanged, UniformChangedEvent);
	private:
	
	}; // class MaxStepsChangedEvent : public UniformChangedEvent<uint32_t>

	class AttenuationChangedEvent : public UniformChangedEvent<int> 
	{
	public:
		AttenuationChangedEvent(const std::string& name, const int* val)
			: UniformChangedEvent(name, val) {};

		AttenuationChangedEvent(const int* val)
			: UniformChangedEvent("attnModel", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::AttenuationChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::LightingSettingsChanged, UniformChangedEvent);
	private:
	
	}; // class AttenuationChangedEvent : public UniformChangedEvent<int>

	class RenderShadsChangedEvent : public UniformChangedEvent<std::array<bool, 2>>  
	{
	public:
		RenderShadsChangedEvent(const std::string& name, const std::array<bool, 2>* val)
			: UniformChangedEvent(name, val) {};

		RenderShadsChangedEvent(const std::array<bool, 2>* val)
			: UniformChangedEvent("renderShadows", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::RenderShadsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::LightingSettingsChanged, UniformChangedEvent);
		
	private:
	
	}; // class RenderShadsChangedEvent : public UniformChangedEvent<bool[2]> 

	class ShadSoftChangedEvent : public UniformChangedEvent<float> 
	{
	public:
		ShadSoftChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		ShadSoftChangedEvent(const float* val)
			: UniformChangedEvent("shadSoft", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::ShadSoftChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::LightingSettingsChanged, UniformChangedEvent);
	private:
	
	}; // class ShadSoftChangedEvent : public UniformChangedEvent<float>

	class TubeRadiusChangedEvent : public UniformChangedEvent<float> 
	{
	public:
		TubeRadiusChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		TubeRadiusChangedEvent(const float* val)
			: UniformChangedEvent("tubeRad", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::TubeRadiusChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class TubeRadiusChangedEvent : public UniformChangedEvent<float>

	class GeometryChangedEvent : public Event
	{
	public:
		GeometryChangedEvent(const std::array<int, 3>& pqr)
			: pqr(pqr), p(pqr[0]), q(pqr[1]), r(pqr[2]) {};

		virtual std::string toString() const override {
			std::stringstream ss;
			ss << "GeometryChangedEvent({" << p << ", " << q << ", " << r << "})";
			return ss.str();
		}

		EVENT_CUSTOM_TYPE(EventTypes::GeometryChanged);
		EVENT_CUSTOM_CATEGORY(EventCategory::SceneGeometryChanged | EventCategory::SceneChanged);

		std::array<int, 3> pqr;
		const int& p, q, r;
	private:
	}; // class GeometryChangedEvent : public Event

	class CellPositionChangedEvent : public UniformChangedEvent<glm::vec4> 
	{
	public:
		CellPositionChangedEvent(const std::string& name, const glm::vec4* val)
			: UniformChangedEvent(name, val) {};

		CellPositionChangedEvent(const glm::vec4* val)
			: UniformChangedEvent("cellPosition", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::CellPositionChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class CellPositionChangedEVent : public UniformChangedEvent<glm::vec4>

	class CellSurfaceOffsetChangedEvent : public UniformChangedEvent<float>
	{
	public:
		CellSurfaceOffsetChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		CellSurfaceOffsetChangedEvent(const float* val)
			: UniformChangedEvent("cellSurfaceOffset", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::CellSurfaceOffsetChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class CellSurfaceOffsetChangedEvent : public UniformChangedEvent<float>

	class VertexPositionChangedEvent : public UniformChangedEvent<glm::vec4>
	{
	public:
		VertexPositionChangedEvent(const std::string& name, const glm::vec4* val)
			: UniformChangedEvent(name, val) {};

		VertexPositionChangedEvent(const glm::vec4* val)
			: UniformChangedEvent("vertexPosition", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::VertexPositionChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class VertexPositionChangedEvent : public UniformChangedEvent<glm::vec4>

	class VertexSurfaceOffsetChangedEvent : public UniformChangedEvent<float>
	{
	public:
		VertexSurfaceOffsetChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		VertexSurfaceOffsetChangedEvent(const float* val)
			: UniformChangedEvent("vertexSurfaceOffset", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::VertexSurfaceOffsetChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class VertexSurfaceOffsetChangedEvent : public UniformChangedEvent<float>

	class UseSimplexChangedEvent : public UniformChangedEvent<bool>
	{
	public:
		UseSimplexChangedEvent(const std::string& name, const bool* val)
			: UniformChangedEvent(name, val) {};

		UseSimplexChangedEvent(const bool* val)
			: UniformChangedEvent("useSimplex", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::UseSimplexChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class UseSimplexChangedEvent : public UniformChangedEvent<bool>

	class SimplexMirrorsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>>
	{
	public:
		SimplexMirrorsChangedEvent(const std::string& name, const std::array<glm::vec4, 4>* val)
			: UniformChangedEvent(name, val) {};

		SimplexMirrorsChangedEvent(const std::array<glm::vec4, 4>* val)
			: UniformChangedEvent("simplexMirrorsKlein", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::SimplexMirrorsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class SimplexMirrorsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>>

	class SimplexDualPointsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>>
	{
	public:
		SimplexDualPointsChangedEvent(const std::string& name, const std::array<glm::vec4, 4>* val)
			: UniformChangedEvent(name, val) {};

		SimplexDualPointsChangedEvent(const std::array<glm::vec4, 4>* val)
			: UniformChangedEvent("simplexDualPoints", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::SimplexDualPointsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class SimplexMirrorsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>>

	class CutChangedEvent : public UniformChangedEvent<int>
	{
	public:
		CutChangedEvent(const std::string& name, const int* val)
			: UniformChangedEvent(name, val) {};

		EVENT_CUSTOM_TYPE(EventTypes::CutChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(
				EventCategory::SceneGeometryChanged | EventCategory::SceneChanged,
				UniformChangedEvent);
	private:
	
	}; // class CutChangedEvent : public UniformChangedEvent<int>

	class TextureChangedEvent : public UniformChangedEvent<Texture2D>
	{
	public:
		TextureChangedEvent(const std::string& name, const Texture2D* val)
			: UniformChangedEvent(name, val) {};

		TextureChangedEvent(const Texture2D* val)
			: UniformChangedEvent("texture", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::TextureChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class TextureChangedEvent : public UniformChangedEvent<>

	class NumObjectsChangedEvent : public UniformChangedEvent<int> 
	{
	public:
		NumObjectsChangedEvent(const std::string& name, const int* val)
			: UniformChangedEvent(name, val) {};

		NumObjectsChangedEvent(const int* val)
			: UniformChangedEvent("NUM_OBJECTS", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::NumObjectsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class NumObjectsChangedEvent : public UniformChangedEvent<int>

	class RequestUpdateUniformsFromUIEvent : public Event
	{
	public:
		RequestUpdateUniformsFromUIEvent() {};

		virtual std::string toString() const override {
			std::stringstream ss;
			ss << "RequestUpdateUniformsFromUIEvent()";
			return ss.str();
		}

		EVENT_CUSTOM_TYPE(EventTypes::RequestUpdateUniforms);
		EVENT_CUSTOM_CATEGORY(EventCategory::Communication);
	private:
	
	}; // class RequestUpdateUniformsFromUIEvent : public Event
	
	class DebugInfoChangedEvent : public UniformChangedEvent<bool> 
	{
	public:
		DebugInfoChangedEvent(const std::string& name, const bool* val)
			: UniformChangedEvent(name, val) {};

		DebugInfoChangedEvent(const bool* val)
			: UniformChangedEvent("debugInfo", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::DebugInfoChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::Communication, UniformChangedEvent)
	private:
	
	}; // class DebugInfoChangedEvent : public Event

	template<int count> 
	class UploadObjectsDataEvent : public UniformChangedEvent< Aulys::Ref<Objects<count>> >
	{
	public:
		UploadObjectsDataEvent(const std::string& name, const Aulys::Ref<Objects<count>>* val)
			: ref(*val), UniformChangedEvent< Aulys::Ref<Objects<count>> >(name, ref) {};

		UploadObjectsDataEvent(Aulys::Ref<Objects<count>> val)
			: ref(*val), UniformChangedEvent< Aulys::Ref<Objects<count>> >(
				"globalObjectBoosts,invGlobalObjectsBoosts,globalObjectRadii,NUM_OBJECTS",
				ref) {};

		UploadObjectsDataEvent()
			: ref(initObjects<count>(Geometry::Hyperbolic)),
			  UniformChangedEvent< Aulys::Ref<Objects<count>> >(
			    "globalObjectBoosts,invGlobalObjectsBoosts,globalObjectRadii,NUM_OBJECTS",
			    &ref
			  ) {};
		
		EVENT_CUSTOM_TYPE(EventTypes::UploadObjectsData);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::Communication,
				UniformChangedEvent< Aulys::Ref<Objects<count>> >);
	private:
		Aulys::Ref<Objects<count>> ref;
	}; // class UploadObjectsDataEvent : public UniformChangedEvent

	class CellBoostChangedEvent : public UniformChangedEvent<glm::mat4> 
	{
	public:
		CellBoostChangedEvent(const std::string& name, const glm::mat4* val)
			: UniformChangedEvent(name, val) {};

		CellBoostChangedEvent(const glm::mat4* val)
			: UniformChangedEvent("cellBoost", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::CellBoostChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class CellBoostChangedEvent : public UniformChangedEventa

	class invCellBoostChangedEvent : public UniformChangedEvent<glm::mat4> 
	{
	public:
		invCellBoostChangedEvent(const std::string& name, const glm::mat4* val)
			: UniformChangedEvent(name, val) {};

		invCellBoostChangedEvent(const glm::mat4* val)
			: UniformChangedEvent("invCellBoost", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::invCellBoostChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class invCellBoostChangedEvent : public UniformChangedEventa

	class invGeneratorsChangedEvent : public UniformChangedEvent<std::array<glm::mat4, 6>>  
	{
	public:
		invGeneratorsChangedEvent(const std::string& name, const std::array<glm::mat4, 6>* val)
			: UniformChangedEvent(name, val) {};

		invGeneratorsChangedEvent(const std::array<glm::mat4, 6>* val)
			: UniformChangedEvent("invGenerators", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::invGeneratorsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class invGeneratorsChangedEvent : public UniformChangedEvent<glm::mat4> 

	class CurrentBoostChangedEvent : public UniformChangedEvent<glm::mat4> 
	{
	public:
		CurrentBoostChangedEvent(const std::string& name, const glm::mat4* val)
			: UniformChangedEvent(name, val) {};

		CurrentBoostChangedEvent(const glm::mat4* val)
			: UniformChangedEvent("currentBoost", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::CurrentBoostChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class CurrentBoostChangedEvent : public UniformChangedEvent<glm::mat4>

	class LightPositionsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>> 
	{
	public:
		LightPositionsChangedEvent(const std::string& name, const std::array<glm::vec4, 4>* val)
			: UniformChangedEvent(name, val) {};

		LightPositionsChangedEvent(const std::array<glm::vec4, 4>* val)
			: UniformChangedEvent("lightPositions", val) {};
		
		EVENT_CUSTOM_TYPE(EventTypes::LightPositionsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::LightingSettingsChanged, UniformChangedEvent);
	private:
	
	}; // class LightPositionsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>>

	class LightIntensitiesChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 5>> 
	{
	public:
		LightIntensitiesChangedEvent(const std::string& name, const std::array<glm::vec4, 5>* val)
			: UniformChangedEvent(name, val) {};

		LightIntensitiesChangedEvent(const std::array<glm::vec4, 5>* val)
			: UniformChangedEvent("lightIntensities", val) {};
		
		EVENT_CUSTOM_TYPE(EventTypes::LightIntensitiesChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::LightingSettingsChanged, UniformChangedEvent);
	private:
	
	}; // class LightPositionsChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 4>>

	class GlobalObjectBoostsChangedEvent : public UniformChangedEvent<std::array<glm::mat4, 4>>  
	{
	public:
		GlobalObjectBoostsChangedEvent(const std::string& name, const std::array<glm::mat4, 4>* val)
			: UniformChangedEvent(name, val) {};

		GlobalObjectBoostsChangedEvent(const std::array<glm::mat4, 4>* val)
			: UniformChangedEvent("globalObjectBoosts", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::GlobalObjectBoostsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class GlobalObjectBoostsChangedEvent : public UniformChangedEvent<std::array<glm::mat4, 4>> 

	class invGlobalObjectBoostsChangedEvent : public UniformChangedEvent<std::array<glm::mat4, 4>>  
	{
	public:
		invGlobalObjectBoostsChangedEvent(const std::string& name, const std::array<glm::mat4, 4>* val)
			: UniformChangedEvent(name, val) {};

		invGlobalObjectBoostsChangedEvent(const std::array<glm::mat4, 4>* val)
			: UniformChangedEvent("invGlobalObjectBoosts", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::invGlobalObjectBoostsChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class invGlobalObjectBoostsChangedEvent : public UniformChangedEvent<std::array<glm::mat4, 4>> 

	class GlobalObjectRadiiChangedEvent : public UniformChangedEvent<std::array<glm::vec3, 4>>  
	{
	public:
		GlobalObjectRadiiChangedEvent(const std::string& name, const std::array<glm::vec3, 4>* val)
			: UniformChangedEvent(name, val) {};

		GlobalObjectRadiiChangedEvent(const std::array<glm::vec3, 4>* val)
			: UniformChangedEvent("globalObjectRadii", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::GlobalObjectRadiiChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneChanged, UniformChangedEvent);
	private:
	
	}; // class GlobalObjectRadiiChangedEvent : public UniformChangedEvent<std::array<glm::vec3, 4>> 

	class HalfCubeDualPointChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 3>>  
	{
	public:
		HalfCubeDualPointChangedEvent(const std::string& name, const std::array<glm::vec4, 3>* val)
			: UniformChangedEvent(name, val) {};

		HalfCubeDualPointChangedEvent(const std::array<glm::vec4, 3>* val)
			: UniformChangedEvent("halfCubeDualPoints", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::HalfCubeDualPointChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneGeometryChanged, UniformChangedEvent);
	private:
	
	}; // class HalfCubeDualPointChangedEvent : public UniformChangedEvent<std::array<glm::vec4, 3>> 

	class HalfCubeWidthChangedEvent : public UniformChangedEvent<float>
	{
	public:
		HalfCubeWidthChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		HalfCubeWidthChangedEvent(const float* val)
			: UniformChangedEvent("halfCubeWidthKlein", val) {};
		
		EVENT_CUSTOM_TYPE(EventTypes::HalfCubeWidthChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::SceneGeometryChanged, UniformChangedEvent);
	private:
	
	}; // class HalfCubeWidthChangedEvent : public UniformChangedEvent<float>

	class DebugVector2ChangedEvent : public UniformChangedEvent<glm::vec2>
	{
	public:
		DebugVector2ChangedEvent(const std::string& name, const glm::vec2* val)
			: UniformChangedEvent(name, val) {};

		DebugVector2ChangedEvent(const glm::vec2* val)
			: UniformChangedEvent("debugVector2", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::DebugVector2Changed);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::Debug, UniformChangedEvent);
	private:

	}; // class DebugVector2ChangedEvent : public UniformChangedEvent<glm::vec2>

	class AmbientChangedEvent : public UniformChangedEvent<float>
	{
	public:
		AmbientChangedEvent(const std::string& name, const float* val)
			: UniformChangedEvent(name, val) {};

		AmbientChangedEvent(const float* val)
			: UniformChangedEvent("ambient", val) {};

		EVENT_CUSTOM_TYPE(EventTypes::AmbientChanged);
		EVENT_CUSTOM_EXTEND_CATEGORY(EventCategory::LightingSettingsChanged, UniformChangedEvent);


	private:
	}; // class AmbientChangedEvent : public UniformChangedEvent<float>

	class MovementSpeedChangedEvent : public Event 
	{
	public:
		MovementSpeedChangedEvent(float val) : mVal(val) {};

		float val() const { return mVal; }

		EVENT_CUSTOM_TYPE(EventTypes::MovementSpeedChanged);
		EVENT_CUSTOM_CATEGORY(EventCategory::Communication);
	private:
		float mVal;
	}; // class MovementSpeedChangedEvent : public Event

	class RotSpeedChangedEvent : public Event 
	{
	public:
		RotSpeedChangedEvent(float val) : mVal(val) {};

		float val() const { return mVal; }

		EVENT_CUSTOM_TYPE(EventTypes::RotSpeedChanged);
		EVENT_CUSTOM_CATEGORY(EventCategory::Communication);
	private:
		float mVal;
	}; // class RotSpeedChangedEvent : public Event

	class TagSwapRequestedEvent : public Event 
	{
	public:
		TagSwapRequestedEvent(const std::string& tag, const std::string& val) : tag(tag), val(val) {};

		const std::string& tag;
		const std::string& val;

		EVENT_CUSTOM_TYPE(EventTypes::TagSwapRequested);
		EVENT_CUSTOM_CATEGORY(EventCategory::Communication);
	private:
	}; // class TagSwapRequestedEvent : public Event
}; // namespace App

