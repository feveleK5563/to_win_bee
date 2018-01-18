#pragma once
#include "MyPG.h"

//�摜�̕����Ǘ��ƃA�j���[�V����
class ImageManager
{
public:

	vector<ML::Box2D*>	charaChip;		//�L�����N�^�̑f��
	int					defImageNum;	//�����p�摜�ԍ�
	int					baseImageNum;	//��摜�ԍ�
	POINT				drawPos;		//�`��̊���W
	int					imageWidth;		//�摜�̉���
	int					imageHeight;	//�摜�̏c��
	float				animCnt;		//�A�j���[�V�����J�E���^
	bool				animTurn;		//�A�j���[�V���������]���Ă��邩�ۂ�

	ImageManager() :
		defImageNum(0),
		baseImageNum(0),
		imageWidth(0),
		imageHeight(0),
		animCnt(0),
		animTurn(false)
	{
		drawPos.x = 0;
		drawPos.y = 0;
	};

	~ImageManager() {};

	//--------------------------------------------------------
	//�ȉ��g�p���\�b�h

	//------------------------------------
	//�摜�̕���
	//�����F�J�n�ʒuXY(int, int), �摜�̕�����WH(int, int), �摜�T�C�Y(int, int �f�t�H���g��32x32)
	void ImageCreate(int startX, int startY, int width, int height, int imageWidth = 32, int imageHeight = 32);

	//------------------------------------
	//�摜�̕���(1����)
	//�����F�J�n�ʒuXY(int, int), �I���ʒu(int, int)
	void ImageOneCreate(int startX, int startY, int endX, int endY);

	//------------------------------------
	//�摜�̕�������
	void ImageErase();

	//------------------------------------
	//�摜�̕`��
	//�����F�\�����W(ML::Vec2), �摜��(string), �\���F(ML::Color �f�t�H���g��(1, 1, 1, 1))
	void ImageRender(const ML::Vec2& pos, const string& imageName, const ML::Color& color = { 1, 1, 1, 1 });
};