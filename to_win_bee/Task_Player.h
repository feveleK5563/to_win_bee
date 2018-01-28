#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{��");	//�O���[�v��
	const  string  defName("�v���C���[");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		string imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����

		DI::VGamePad in;	//�R���g���[��
		const float doubleShot[2] = { -4.f, 4.f };
		const ML::Vec2 threeWayShot[3] = {
			{ -7.f, -15.f },
			{  0.f, -15.f },
			{  7.f, -15.f },
		};
		float baseSpeed;	//��{�X�s�[�h
		bool  noStart;		//��ʉ�����u���[�b�Əo�Ă��I�������false�ɂȂ�

		void  PlayerStart();			//��ʉ�����u���[�b�Əo�Ă��鏈��
		float ControllKeyTable();	//�ړ��n�L�[���͂̏���
		void  MovePlayer();			//�v���C���[�̓��쏈��
		void  ChangeShot();			//�e���ς���
		void  CreateShot();			//�e�̏o��
	};
}