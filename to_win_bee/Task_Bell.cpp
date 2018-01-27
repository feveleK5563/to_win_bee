//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Bell.h"
#include  "Task_Player.h"

namespace  Bell
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		imageName = "Bell";
		DG::Image_Create(imageName, "./data/image/Bell.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		render2D_Priority[1] = 0.75f;
		bellType = Yellow;
		damage = 0;
		hitBase = { -16, -16, 32, 32 };
		speed = { 0.f, -7.f };

		image.ImageCreate(0, 0, 3, 4);
		image.drawPos = { 16, 16 };

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		image.ImageErase();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (speed.y > 0.f)
			speed.y += acclation / 3.f;
		else
			speed.y += acclation;
		pos += speed;
		ScreenOutObj();

		++cntTime;
		if (bellType == Flash)
		{
			image.baseImageNum = flashAnim[(cntTime / 2) % 2];
		}
		image.animCnt = float((cntTime / 15) % 3);

		if (HitPlayer(false))
		{
			EffectGrant();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//�x���̃^�C�v�ύX(�e�q�b�g����)
	void Object::ChangeType()
	{
		++damage;
		if (speed.y > 0.f)
			speed.y += -4.f;

		if (!(damage % 7))
		{
			if (bellType == Flash)
			{
				Kill();
			}
			else
			{
				++bellType;
				if (bellType != Flash)
					image.baseImageNum += 3;
			}
		}
	}

	//-------------------------------------------------------------------
	//�v���C���[�Ɍ��ʕt�^
	void Object::EffectGrant()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("�{��", "�v���C���[");
		if (player == nullptr)
			return;

		switch (bellType)
		{
		case Yellow:	//�X�R�A�{�[�i�X
			break;

		case Blue:		//�X�s�[�h�A�b�v
			player->baseSpeed += 0.5f;
			break;

		case White:		//�e���ύX
			player->ChangeShot();
			break;

		case Red:		//�o���A����
			break;

		case Flash:		//���g
			break;
		}
		Kill();
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}