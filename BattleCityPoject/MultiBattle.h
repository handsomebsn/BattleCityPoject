#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
//��Ϸ��ʼ
void Start_2()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	game_state = GAME;
	memset(&Player_B, 0, sizeof(Player_B));//��սṹ��
	if (start_flag == true) {
		Init_2();
	}
}
//��ʼ��
void Init_2()
{
	MessageBoxA(hwnd, "��ս��ʼ", "", MB_OK);
	stage = 0;
	//BOSSģʽ
	boss_mode = false;
	//���̹��

	if (clientORserver == 1) {
		player_tank = BaseTank(
			player_tank.id,	player_tank.life,player_tank.armor,
			UP,	5 * 64,	10 * 64 + 64,UP,
			player_tank.gun_lock,
			64 / player_tank.speed,	player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id, player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
	}else if(clientORserver==2)
	{
		player_tank = BaseTank(
			player_tank.id,	player_tank.life, player_tank.armor,
			DOWN, 9 * 64, -1 * 64 + 0,	DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed,	player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id,	player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
	}
	//����Ϊ��̬��Դ���ͷ�
	//����ӵ����
	for (list<Bullet*>::iterator i = player_bullet.begin(); i != player_bullet.end(); ++i)
		delete *i;
	player_bullet.clear();
	//���������
	for (list<Fire*>::iterator i = fire.begin(); i != fire.end(); ++i)
		delete *i;
	fire.clear();
	//�����������
	for (list<BaseTank*>::iterator i = enemy_tank.begin(); i != enemy_tank.end(); ++i)
		delete *i;
	enemy_tank.clear();
	//�����ڵ��������
	for (list<list<Bullet*>*>::iterator i = enemy_bullet.begin(); i != enemy_bullet.end(); ++i)
	{
		for (list<Bullet*>::iterator j = (**i).begin(); j != (**i).end(); ++j)
			delete *j;
		(**i).clear();
		delete *i;
	}
	enemy_bullet.clear();
	//�ؿ�
	//wintime = 0;
	//����
	base_time = 0;
	//��һָ�
	player_death = false;
	player_time = 0;
	//��������
	enemy_num = 0;
	enemy_total = 1;
	enemy_rest = enemy_total;
	enemy_time = 1;
	//ש���ˮѭ����ʱ����
	wall_time = 0;
	water_time = 0;
	//��������
	item = NONE;
	invin_time = 0;
	LoadMap();
	//MessageBoxA(NULL, "����", "", MB_OK);
}
//��Ϸ����
void Game_2()
{
	if(clientORserver==1)Receive_Server();
	if(clientORserver==2)Receive_Client();
	//��Ҳ���������Ӧ
	if (player_death == false)
		Key();
	//������Ϸ����
	DrawGame_2();
	//���ػ���
	if (base_time > 0) {
		if (--base_time == 0)
			game_state = GAMEOVER;
	}
	//�������
	if (player_death == true)
	{
		if (--player_time == 0)
		{
			if (player_num == 0)
				game_state = GAMEOVER;
			else {
				//��Ҹ���
				player_death = false;
				player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
			}
			InvalidateRect(hwnd, NULL, false);
		}
	}
	//������Ļ������������ʣ���������������ӵ���
	if (enemy_num < enemy_maxscr && enemy_time == 0 && enemy_rest>0)
	{
		enemy_time = enemy_timemax;
	}
	if (enemy_time > 0)
	{
		--enemy_time;
		if (enemy_time == 0)
		{
			++enemy_num;
			--enemy_rest;
			if(clientORserver==1)
			enemy_tank.push_back(new BaseTank(GOLD, 20, 0, DOWN, 9 * 64, -1 * 64 + 0, DOWN, false, 16, 4, 8, 2, 1, 10, 8));
			else if(clientORserver==2)
			enemy_tank.push_back(new BaseTank(GOLD, 20, 0, UP, 5 * 64, 10 * 64 + 64,UP, false, 16, 4, 8, 2, 1, 10, 8));
			//����ӵ�����
			enemy_bullet.push_back(new list<Bullet*>);
			//ˢ��ʣ������ͼ
			InvalidateRect(hwnd, NULL, false);
		}
	}
}
//��Ϸ��λ����
void DrawGame_2()
{
	//���Ʊ���
	DrawBG();
	//���Ƶ�ͼ��
	DrawBlock();
	//����ͻ��Ƶ���
	//Item();
	//����ͻ��Ƶ���̹��
	EnemyTank_2();
	//����ͻ������̹��
	if (player_death == false)
		player_tank.Draw(writeDC, player_tankDC, false);
	//���Ʋݿ�
	DrawGrass();
	//����ͻ��Ƶ����ڵ�
	//EnemyBullet();
	//����ͻ�������ڵ�
	PlayerBullet();
	//����ͻ��ƿ�������
	player_tank.Draw(writeDC, player_tankDC, true);
	//���ƻ�
	DrawFire();
	//�������Ļ
	Print();
}
void EnemyTank_2()
{
	list<BaseTank*>::iterator iter_tank;
	list<list<Bullet*>*>::iterator iter_bullet;
	//��˳�������ез�̹��
	for (iter_tank = enemy_tank.begin(),
		iter_bullet = enemy_bullet.begin();
		iter_tank != enemy_tank.end();
		++iter_tank,
		++iter_bullet)
	{
		BaseTank& t = **iter_tank;
		//����
		if (t.gun_step == 0 && t.bullet_num < t.bullet_max && rand() % 20 == 0)
		{
			//PLAYA(SHOOT)
			int x = t.x + (64 - 40) / 2;
			int y = t.y + (64 - 40) / 2;
			switch (t.gun_face)
			{
			case DOWN:
				y += 47;
				break;
			case LEFT:
				x -= 47;
				break;
			case UP:
				y -= 47;
				break;
			case RIGHT:
				x += 47;
			}
			(**iter_bullet).push_back(new Bullet(t.bullet_id, 100, t.gun_face, x, y,
				t.bullet_speed, t.bullet_power));
			++t.bullet_num;
			t.Fire();
		}
		int oldface = t.face;
		//��������Ϊ�㲢���ڿ�������ʧʱ��������ƶ�����Ͳ���
		if (t.delay > 0)
		{
			if (++t.move_step > 2)
				t.move_step = 0;
			--t.delay;
		}
		if (t.move == 0 && t.delay == 0)
		{
			t.Change(Player_B.face);//ת������
			t.move = 64 / t.speed;
			//ײ��ש��
			if (TankHitBlock(t) == true)
			{
				t.move = 0;
				t.delay = 5;
			}
			//ײ�����̹��
			else if (player_death == false && TankHitTank(t, player_tank) == true)
			{
				//t.Change(oldface);
				t.move = 0;
				t.delay = 5;
			}
			//���ײ���з�����Ǽ�̹�ˡ���
			else
			{
				for (list<BaseTank*>::iterator i = enemy_tank.begin(); i != enemy_tank.end(); ++i)
					//��������Լ�������ײ���Է�
					if (i != iter_tank && TankHitTank(t, **i) == true)
					{
						//t.Change(oldface);
						t.move = 0;
						break;
					}
			}
			if (t.move > 0)
				t.move = 64 / t.speed_real;
		}
		if (t.move > 0)
		{
			--t.move;
			if (++t.move_step > 2)
				t.move_step = 0;
			t.Move();
		}
		t.Draw(writeDC, player_tankDC, false);
	}
}
#endif