//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Shot.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		imageName = "Player";
		DG::Image_Create(imageName, "./data/image/Player.png");
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
		render2D_Priority[1] = 0.5f;

		state = State1;	//State1 = �ʏ�̃V���b�g
						//State2 = 2������
						//State3 = 3������
		baseSpeed = 3.f;
		pos = { float(ge->screen2DWidth) / 2, float(ge->screen2DHeight) / 3 * 2 };
		hitBase = { -16, -16, 32, 32 };

		image.ImageCreate(0, 0, 1, 1);
		image.baseImageNum = 0;
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
		if (state != Death)
		{
			in = DI::GPad_GetState("P1");

			MovePlayer();
			CreateShot();

			//(��)�e�̎�ޕύX
			if (in.B2.down)
			{
				if (state == State1)
					state = State2;
				else if (state == State2)
					state = State3;
				else if (state == State3)
					state = State1;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		image.ImageRender(pos, res->imageName);
	}

	//-------------------------------------------------------------------
	//�L�[���͂̏���
	float Object::ControllKeyTable()
	{
		float keyDirection[3][3] = {
			{ 225, 270, 315},
			{ 180,  -1,   0},
			{ 135,  90,  45},
		};
		int kdX = 1, kdY = 1;

		if (in.LStick.L.on) { --kdX; }
		if (in.LStick.R.on) { ++kdX; }
		if (in.LStick.U.on) { --kdY; }
		if (in.LStick.D.on) { ++kdY; }

		return keyDirection[kdY][kdX];
	}
	//-------------------------------------------------------------------
	//�v���C���[�̓��쏈��
	void Object::MovePlayer()
	{
		speed = { 0, 0 };
		float angle = ControllKeyTable();
		if (angle != -1)
		{
			speed.x = cos(ML::ToRadian(angle)) * baseSpeed;
			speed.y = sin(ML::ToRadian(angle)) * baseSpeed;
		}
		pos += speed;

		if (pos.x < float(image.drawPos.x))
			pos.x = float(image.drawPos.x);
		if (pos.x > float(ge->screen2DWidth) - image.drawPos.x)
			pos.x = float(ge->screen2DWidth) - image.drawPos.x;
		if (pos.y < float(image.drawPos.y))
			pos.y = float(image.drawPos.y);
		if (pos.y > float(ge->screen2DHeight) - image.drawPos.y)
			pos.y = float(ge->screen2DHeight) - image.drawPos.y;
	}

	//-------------------------------------------------------------------
	//�e�̐���
	void Object::CreateShot()
	{
		if (in.B1.on)
		{
			if (!((cntTime++) % 10))
			{
				switch (state)
				{
				case BChara::State1: //�ʏ�V���b�g
				{
					auto shot = Shot::Object::Create(true);
					shot->pos = pos;
					shot->pos.y -= 8.f;
					shot->speed = { 0.f, -15.f };
				}
					break;

				case BChara::State2: //2�������V���b�g
				{
					for (int i = 0; i < 2; ++i)
					{
						auto shot = Shot::Object::Create(true);
						shot->pos = pos;
						shot->pos.y -= 8.f;
						shot->pos.x += doubleShot[i];
						shot->speed = { 0.f, -15.f };
						shot->image.baseImageNum = 1;
					}
				}
					break;

				case BChara::State3: //3�������V���b�g
				{
					for (int i = 0; i < 3; ++i)
					{
						auto shot = Shot::Object::Create(true);
						shot->pos = pos;
						shot->pos.y -= 8.f;
						shot->speed = threeWayShot[i];
					}
				}
					break;
				}
			}
		}
		else
		{
			cntTime = 0;
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