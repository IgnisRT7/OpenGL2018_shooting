/**
*	@file OffscreenBuffer.cpp
*/
#pragma once
#include "OffscreenBuffer.h"
#include <iostream>

/**
*	�I�t�X�N���[���o�b�t�@���쐬����
*
*	@param w �I�t�X�N���[���o�b�t�@�̕�(�s�N�Z���P��)
*	@param h �I�t�X�N���[���o�b�t�@�̍���(�s�N�Z���P��)
*
*	@return �쐬�����I�t�X�N���[���o�b�t�@�ւ̃|�C���^
*/
OffscreenBufferPtr OffscreenBuffer::Create(int w, int h, GLenum iformat) {

	struct Impl : OffscreenBuffer {};
	OffscreenBufferPtr offscreen = std::make_shared<Impl>();

	GLenum format = GL_RGBA;
	switch (iformat) {
	default:
		format = GL_RGBA;
		break;

	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		format = GL_DEPTH_COMPONENT;
		break;
	}

	offscreen->tex = Texture::Create(w, h, iformat, format, nullptr);

	if (format == GL_DEPTH_COMPONENT) {

		//�t���[���o�b�t�@�̍쐬
		glGenFramebuffers(1, &offscreen->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, offscreen->tex->Id(), 0);
		glDrawBuffer(GL_NONE);
	}
	else {

		//�[�x�o�b�t�@�̍쐬
		glGenRenderbuffers(1, &offscreen->depthbuffer);						//�[�x�o�b�t�@�̃������m��
		glBindRenderbuffer(GL_RENDERBUFFER, offscreen->depthbuffer);			//�[�x�o�b�t�@�̕R�Â�
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);	//���̃o�b�t�@�̗p�r�ƃT�C�Y�����߂�
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//�t���[���o�b�t�@�̍쐬
		glGenFramebuffers(1, &offscreen->framebuffer);						//�t���[���o�b�t�@�̃������m��
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->framebuffer);			//�t���[���o�b�t�@�̕R�Â�(�ǂݏ����\)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offscreen->depthbuffer);	//�[�x�o�b�t�@���t���[���o�b�t�@�ɕR�Â�	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offscreen->tex->Id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: OffscreenBuffer�̍쐬�Ɏ��s" << std::endl;
		offscreen.reset();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return offscreen;
}

/**
*	�f�X�g���N�^
*/
OffscreenBuffer::~OffscreenBuffer() {

	if (framebuffer) {
		glDeleteFramebuffers(1, &framebuffer);
	}
	if (depthbuffer) {
		glDeleteRenderbuffers(1, &depthbuffer);
	}
}


///G�o�b�t�@�쐬�e�X�g

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight) {


	auto p = std::make_shared<GBuffer>();

	//
	glGenFramebuffers(1, &p->m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,p->m_fbo);

	//�e�N�X�`���̍쐬(�쐬�� = �}�X�L���O�e�N�X�`���񋓂̐�)
	glGenTextures(GBUFFER_NUM_TEXTURES, p->m_textures);
	glGenTextures(1, &p->m_depthTexture);

	//
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {

		//�e�N�X�`���T�C�Y�̐ݒ�ƃt���[���o�b�t�@�ւ̊��蓖��
		glBindTexture(GL_TEXTURE_2D, p->m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, p->m_textures[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, p->m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p->m_depthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(sizeof(DrawBuffers) / sizeof(GLenum), DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: gbuffer::Draw() error" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;



}