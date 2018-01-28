//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot.h"
#include  "Task_Player.h"
#include  "Task_Cloud.h"
#include  "Task_Bell.h"
#include  "Task_Enemy_Itigo.h"
#include  "Task_Enemy_Diecon.h"
#include  "Task_DeathEnemy.h"

namespace  Shot
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		imageName = "Shot";
		DG::Image_Create(imageName, "./data/image/Shot.png");
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
		render2D_Priority[1] = 0.7f;

		beforePos = { 0, 0 };
		hitBase = { -8, -8, 16, 16 };
		shotUser = Player; //(���̌�ʂ̒l�ɕύX���ꂽ�肳��Ȃ�������)

		image.ImageCreate(0, 0, 2, 1, 8, 8);
		image.drawPos = { 4, 4 };

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
		beforePos = pos;
		switch (shotUser)
		{
		case Player:
			HitEnemy();
			HitCloudOrBell();
			break;

		case Enemy:
			HitPlayer();
			break;
		}
		pos += speed;
		ScreenOutObj();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//�G�Ƃ̓����蔻��
	void Object::HitEnemy()
	{
		{	//�C�e�B�S
			auto enemy = ge->GetTask_Group_GN<Itigo::Object>("�G", "�C�e�B�S");
			for (auto it = enemy->begin(); it != enemy->end(); ++it)
			{
				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					auto de = DeathEnemy::Object::Create(true);
					de->pos = (*it)->pos;
					(*it)->Kill();
					Kill();
					return;
				}
			}
		}
		{	//�卪
			auto enemy = ge->GetTask_Group_GN<Diecon::Object>("�G", "�卪");
			for (auto it = enemy->begin(); it != enemy->end(); ++it)
			{
				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					auto de = DeathEnemy::Object::Create(true);
					de->pos = (*it)->pos;
					(*it)->Kill();
					Kill();
					return;
				}
			}
		}
	}

	//-------------------------------------------------------------------
	//�_�A�x���Ƃ̓����蔻��
	void Object::HitCloudOrBell()
	{
		{	//�_
			auto cloud = ge->GetTask_Group_GN<Cloud::Object>("�G", "�_");
			for (auto it = cloud->begin(); it != cloud->end(); ++it)
			{
				if ((*it)->hp <= 0)
					continue;

				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					--(*it)->hp;
					return;
				}
			}
		}
		{	//�x��
			auto bell = ge->GetTask_Group_GN<Bell::Object>("�G", "�x��");
			for (auto it = bell->begin(); it != bell->end(); ++it)
			{
				if (hitBase.OffsetCopy(pos).Hit((*it)->hitBase.OffsetCopy((*it)->pos)))
				{
					(*it)->ChangeType();
					Kill();
					return;
				}
			}
		}
	}

	//-------------------------------------------------------------------
	//�v���C���[�Ƃ̓����蔻��
	void Object::HitPlayer()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("�{��", "�v���C���[");
		if (player == nullptr)
			return;

		if (hitBase.OffsetCopy(pos).Hit(player->hitBase.OffsetCopy(player->pos)))
		{
			player->Kill();
			Kill();
		}
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