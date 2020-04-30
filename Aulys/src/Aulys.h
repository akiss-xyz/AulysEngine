#pragma once

// This is the main Aulys include, this'll be the public API, facing applications, from Aulys.
// Settings file.
#include "../../AulysConf.h"

#include "Core/Core.h"
#include "Application.h"
#include "Log/Log.h"

#include "Core/Timestep.h"

/* Include the input class, along with the headers containing the keycodes. */
#include "Input/Input.h" 
#include "Input/KeyCodes.h"
#include "Input/MouseButtonCodes.h"

#include "Layer.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/OrthographicCamera.h"

/* Include some useful maths library stuff just by default */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/* And the imgui stuff! */
#include "imgui/imgui.h"
