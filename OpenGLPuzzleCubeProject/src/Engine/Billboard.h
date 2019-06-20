/**
*	Billbord.h
*/
#pragma once
#include "BufferObject.h"
#include <glm/glm.hpp>
#include <memory>

namespace Primitive {

	class Buffer;
	using BufferPtr = std::shared_ptr<Buffer>;

	struct Vertex {
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec3 texCoord;
	};

	class Buffer {
	public:

		static BufferPtr Create(int iboSize, int vboSize);


	private:

		Buffer();
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;
		~Buffer() = default;

	private:

		BufferObject ibo, vbo;
		VertexArrayObject vao;
	};




}/// namespace primitive

class Billbord {
public:


private:

};





