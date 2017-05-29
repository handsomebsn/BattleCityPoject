#pragma once
#ifndef __MultiBattle__
#include"HEAD.h"
bool load_enemytank = true;
//��Ϸ��ʼ
void Start_2()
{
	SetTimer(hwnd, GAME_ID, GAME_TIME, NULL);
	player_tank = BaseTank(20 / 10 - 1, 20, 0, UP, 5 * 64, 10 * 64 + 64, UP, false, 16, 4, 8, 2, 1, 10, 8);
	enemy2_tank=BaseTank(GOLD, 20, 0, DOWN, 9 * 64, -64 , DOWN, false, 16, 4, 8, 2, 1, 10, 8);
	game_state = GAME;
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
	//׼����صз�̹��
	load_enemytank = true;
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
		enemy2_tank = BaseTank(
			GOLD, player_tank.life, player_tank.armor,
			DOWN, 9 * 64, -64, DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed, player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id, player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
	}else if(clientORserver==2)
	{
		player_tank = BaseTank(
			player_tank.id,	player_tank.life, player_tank.armor,
			DOWN, 9 * 64, -64 ,	DOWN,
			player_tank.gun_lock,
			64 / player_tank.speed,	player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id,	player_tank.bullet_max,
			player_tank.bullet_power, player_tank.bullet_speed);
		enemy2_tank = BaseTank(
			GOLD, player_tank.life, player_tank.armor,
			UP, 5 * 64, 10 * 64 + 64, UP,
			player_tank.gun_lock,
			64 / player_tank.speed, player_tank.speed,
			player_tank.fire_speed,
			player_tank.bullet_id, player_tank.bullet_max,
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
}
//��Ϸ����
void Game_2()
{
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
	//̹������
	if (enemy2_tank.move == 0)
	{
		//����
		Player_B.face = -1;
		Receive_Server();
		switch (Player_B.face)
		{			
		case DOWN:enemy2_tank.Change(DOWN);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		case LEFT:enemy2_tank.Change(LEFT);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		case UP:enemy2_tank.Change(UP);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		case RIGHT:enemy2_tank.Change(RIGHT);
			enemy2_tank.move = 64 / enemy2_tank.speed; break;
		}
		if (enemy2_tank.move > 0)
		{
			bool hit = TankHitBlock(enemy2_tank);
			enemy2_tank.move = 64 / enemy2_tank.speed_real;
			if (hit == true)
			{
				enemy2_tank.move = 0;
				if (++enemy2_tank.move_step > 2)
					enemy2_tank.move_step = 0;
			}
			if (sound)PLAYB(MOTOR);
		}
	}
	if (enemy2_tank.move > 0)
	{
		--enemy2_tank.move;
		if (++enemy2_tank.move_step > 2)
			enemy2_tank.move_step = 0;
		enemy2_tank.Move();
		if (enemy2_tank.move == 0) {
			enemy2_tank.x = (enemy2_tank.x + 32) / 64 * 64;
			enemy2_tank.y = (enemy2_tank.y + 32) / 64 * 64;
		}
	}
	enemy2_tank.Draw(writeDC, player_tankDC, false);
}
#endif