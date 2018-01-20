//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot.h"
#include  "Task_Player.h"

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
		render2D_Priority[1] = 0.8f;

		beforePos = { 0, 0 };
		shotUser = Player; //�b��

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
			break;

		case Enemy:
			HitPlayer();
			break;
		}
		pos += speed;
		ScreenOutShot();
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

	}

	//-------------------------------------------------------------------
	//�v���C���[�Ƃ̓����蔻��
	void Object::HitPlayer()
	{
		auto player = ge->GetTask_One_GN<Player::Object>("�{��", "�v���C���[");
		if (player == nullptr)
			return;

		if (BoxAndLineHit(player->hitBase.OffsetCopy(player->pos)))
			player->Kill();
	}

	//-------------------------------------------------------------------
	//�����Ƌ�`�̓����蔻��
	//�e�̑O���W�ƌ����W�Ƃ̐����Ƌ�`�̕�4�Ƃ̒ʉߔ�����s��
	bool Object::BoxAndLineHit(const ML::Box2D& plyHb)
	{
		POINT posA, posB;
		for (int i = 0; i < 4; ++i)
		{
			switch (i)
			{
			case 0:	//����
				posA = { plyHb.x, plyHb.y };
				posB = { plyHb.x, plyHb.y + plyHb.h };
				break;

			case 1:	//�E��
				posA = { plyHb.x + plyHb.w, plyHb.y };
				posB = { plyHb.x + plyHb.w, plyHb.y + plyHb.h };
				break;

			case 2:	//���
				posA = { plyHb.x, plyHb.y };
				posB = { plyHb.x + plyHb.w, plyHb.y };
				break;

			case 3:	//����
				posA = { plyHb.x, plyHb.y + plyHb.h };
				posB = { plyHb.x + plyHb.w, plyHb.y + plyHb.h };
				break;
			}
			int tA = (posA.x - posB.x) * (pos.y - posA.y) + (posA.x - posB.x) * (posA.y - pos.y);
			int tB = (posB.x - posA.x) * (beforePos.y - posA.y) - (beforePos.x - posA.x) * (posB.y - posA.y);
			int tC = (pos.x - beforePos.x) * (posA.y - pos.y) + (pos.x - beforePos.x) * (pos.y - posA.y);
			int tD = (pos.x - beforePos.x) * (posB.y - pos.y) + (pos.x - beforePos.x) * (pos.y - posB.y);
			if (tA * tB <= 0 && tC * tD <= 0)
				return true;
		}
		return false;
	}

	//-------------------------------------------------------------------
	//��ʊO�ɏo�����������
	void Object::ScreenOutShot()
	{
		if (pos.x < -16.f || float(ge->screen2DWidth) + 16.f < pos.x ||
			pos.y < -16.f || float(ge->screen2DHeight) + 16.f < pos.y)
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