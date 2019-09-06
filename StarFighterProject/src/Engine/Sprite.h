/**
*	@file	Sprite.h
*	@brief	2D�`��̊Ǘ�
*	@author	tn-mai(�u�`���������)
*/
#pragma once

#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

/**
*	��`�\����
*/
struct Rect {

	glm::vec2 origin;	///< �������_
	glm::vec2 size;		///< �c���̕�
};

/**
*	�X�v���C�g�N���X
*/
class Sprite {
public:

	Sprite() = default;

	/**
	*	Sprite�R���X�g���N�^
	*
	*	@param tex	�X�v���C�g�Ɏg�p����e�N�X�`���I�u�W�F�N�g
	*/
	explicit Sprite(const TexturePtr&);
	virtual ~Sprite() = default;
	Sprite(const Sprite&) = default;
	Sprite& operator=(const Sprite&) = default;

	/**
	*	�X�v���C�g�̍��W�ݒ�
	*
	*	@param p	���W�l
	*/
	void Position(const glm::vec3& p) { position = p; }

	/**
	*	�X�v���C�g�̍��W�擾
	*
	*	@return ���W�l
	*/
	const glm::vec3& Position() const { return position; }

	/**
	*	�X�v���C�g�̉�]�ݒ�
	*	
	*	@param r	��]�l
	*/
	void Rotation(float r) { rotation = r; }

	/**
	*	�X�v���C�g�̉�]���̎擾
	*
	*	@return ��]�l
	*/
	float Rotation() const { return rotation; }

	/**
	*	�X�v���C�g�̃X�P�[�����O�̐ݒ�
	*
	*	@param s	�X�P�[�����O�l
	*/
	void Scale(const glm::vec2& s) { scale = s; }

	/**
	*	�X�v���C�g�̃X�P�[�����O�̎擾
	*
	*	@return	�X�P�[�����O�l
	*/
	const glm::vec2& Scale() const { return scale; }

	/**
	*	�X�v���C�g�̒��_�F�̐ݒ�
	*
	*	@param c	�F���
	*/
	void Color(const glm::vec4& c) { color = c; }

	/**
	*	�X�v���C�g�̒��_�F�̎擾
	*
	*	@return �F���
	*/
	const glm::vec4& Color() const { return color; }

	/**
	*	�`����̐ݒ�
	*
	*	@param r	�`����
	*/
	void Rectangle(const Rect& r) { rect = r; }

	/**
	*	�`���Ԃ̎擾
	*
	*	@return �`����
	*/
	const Rect& Rectangle() const { return rect; }

	/**
	*	�g�p����e�N�X�`�����w�肷��
	*
	*	@param tex	�e�N�X�`���I�u�W�F�N�g
	*/
	void Texture(const TexturePtr& tex);

	/**
	*	�g�p����e�N�X�`�����擾����
	*
	*	@return �e�N�X�`���I�u�W�F�N�g
	*/
	const TexturePtr Texture() const { return texture; }

	/*
	*	�V�F�[�_�̐ݒ�
	*
	*	@param p �V�F�[�_�I�u�W�F�N�g
	*/
	void Program(const Shader::ProgramPtr p) { program = p; }

	/**
	*	�V�F�[�_�̎擾
	*
	*	@return �V�F�[�_�I�u�W�F�N�g
	*/
	Shader::ProgramPtr Program() const { return program; }

private:

	glm::vec3 position = glm::vec3(0);
	glm::f32 rotation = 0;
	glm::vec2 scale = glm::vec2(1);
	glm::vec4 color = glm::vec4(1);
	Rect rect = { glm::vec2(0,0),glm::vec2(1,1) };
	TexturePtr texture;
	Shader::ProgramPtr program;	
};

using SpritePtr = std::shared_ptr<Sprite>;

/**
*	HP�Q�[�W�\���p�X�v���C�g�N���X
*/
class HealthGuage :public Sprite {
public:

	HealthGuage() = default;

	/**
	*	HP�Q�[�W�̏���������
	*
	*	@param tex	�g�p����e�N�X�`��
	*/
	explicit HealthGuage(const TexturePtr& tex);

	virtual ~HealthGuage() = default;
	HealthGuage(const HealthGuage&) = default;
	HealthGuage& operator=(const HealthGuage&) = default;

	/**
	*	�\���䗦�̐ݒ�
	*	@param r	���S�\����1�Ƃ����Ƃ��̔䗦
	*/
	void Ratio(float r);

	/**
	*	�\���䗦�̎擾
	*
	*	@return ���S�\����1�Ƃ����Ƃ��̔䗦
	*/
	float Ratio() const { return ratio; }

private:

	float ratio = 1;
};

/**
*	�X�v���C�g�`��N���X
*/
class SpriteRenderer {
public:

	SpriteRenderer() = default;
	~SpriteRenderer() = default;
	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	/**
	*	�X�v���C�g�`��N���X������������
	*
	*	@param maxSpriteCount	�`��\�ȍő�X�v���C�g��
	*	@param vsPath			���_�V�F�[�_�t�@�C����
	*	@param fsPath			�t���O�����g�V�F�[�_�t�@�C����
	*
	*	@retval true	����������
	*	@retval false	���������s
	*/
	bool Init(size_t maxSpriteCount,Shader::ProgramPtr program);

	/**
	*	���_�f�[�^�̍쐬���J�n����
	*/
	void BeginUpdate();

	/**
	*	���_�f�[�^��ǉ�����
	*
	*	@retval true	�ǉ�����
	*	@retval false	���_�o�b�t�@�����t�Œǉ��ł��Ȃ�
	*/
	bool AddVertices(const Sprite&);

	/**
	*	���_�f�[�^�̍쐬���I������
	*/
	void EndUpdate();
	//void Update();

	/**
	*	�X�v���C�g��`�悷��
	*
	*	@param texture		�`��Ɏg�p����e�N�X�`��
	*	@param screenSize	��ʃT�C�Y
	*/
	void Draw(const glm::vec2&)const;

	/**
	*	�X�v���C�g�`��f�[�^����������
	*/
	void Clear();

private:

	BufferObject vbo;
	BufferObject ibo;
	VertexArrayObject vao;
	Shader::ProgramPtr program;

	struct Vertex {
		glm::vec3 position;	///< ���W
		glm::vec4 color;	///< �F
		glm::vec2 texCoord;	///< �e�N�X�`�����W
	};
	std::vector<Vertex> vertices;	//���_�f�[�^�z��

	struct Primitive {
		size_t count;	///< ���_��
		size_t offset;	///< �f�[�^�擪����̃I�t�Z�b�g
		TexturePtr texture;
		Shader::ProgramPtr program;	///< �v���~�e�B�u��`�悷��V�F�[�_
	};
	std::vector<Primitive> primitives;
};
