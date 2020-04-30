#pragma once

#include "glm/glm.hpp"

namespace Aulys 
{
	class OrthographicCamera 
	{
	public:
		// -1 for near, 1 for far, by default
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& getPosition() const { return mPosition; };
		float getRotation() const { return mRotation; };

		void setPosition(const glm::vec3& position) { 
			mPosition = position; 
			recalculateViewMatrix(); 
		};

		void setRotation(float rotation) { 
			mRotation = rotation; 
			recalculateViewMatrix(); 
		};

		const glm::mat4& getProjectionMatrix() const {
			return mProjectionMatrix;
		};
		const glm::mat4& getViewMatrix() const {
			return mViewMatrix;
		};

		const glm::mat4& getViewProjectionMatrix() const {
			return mViewProjectionMatrix;
		};
	private:
		void recalculateViewMatrix();

		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mViewProjectionMatrix;

		glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
		float mRotation = 0.0f; // Rotation relative to z-axis.
	}; // class OrthographicCamera
	
}; // namespace Aulys
