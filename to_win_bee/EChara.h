#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�ėp�N���X
//-------------------------------------------------------------------
#include "BChara.h"

class EChara : public BChara
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	EChara(){};
	virtual ~EChara(){};

	//�v���C���[�Ƃ̓����蔻��
	//�����F�v���C���[�����ʂ��ۂ�(bool)
	//�߂�l�F�v���C���[�ɓ����������ۂ�
	bool HitPlayer(bool killPlayer = true);
};