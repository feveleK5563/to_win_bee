#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�L�����N�^�ėp�N���X
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "ImageManager.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�

	//�ėp��ԊǗ�
	enum State
	{
		Non,		//�`����X�V�����Ȃ�
		State1,		//���1
		State2,		//���2
		State3,		//���3
		Death,		//���S
	};
	State state;

	//�L�����N�^�[�̓���
	ML::Vec2	pos;		//�L�����N�^�[�̍��W
	ML::Vec2	speed;		//�L�����N�^�[�̃X�s�[�h
	ML::Box2D	hitBase;	//�����蔻��͈�

	//�A�j���[�V����
	vector<ML::Box2D*>	charaChip;	//�L�����N�^�̑f��
	float				animCnt;	//�A�j���[�V�����J�E���^

	//���E�㉺�̌���
	enum AngleLRUD { Left, Right, Up, Down };
	AngleLRUD angleLRUD;

	//���̑�
	ImageManager	image;		//�摜�Ǘ�������
	int				cntTime;	//�ėp�^�C�}�[

	//�����o�ϐ��̏�����
	BChara():
		pos(0, 0),
		speed(0, 0),
		hitBase(0, 0, 0, 0),
		animCnt(0),
		angleLRUD(Left),
		cntTime(0){}

	virtual ~BChara(){}

	//--------------------------------------------------------
	//�ȉ��L�����N�^���ʃ��\�b�h
};
